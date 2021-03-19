#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shading/pbr/pbr_shader.hpp"
#include "shading/pbr/pbr_material.hpp"
#include "utility/math_tool.hpp"
#include "core/define.hpp"


glm::vec4 PBRShader::ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) {
	auto _vs_in = static_cast<PBRVSIn*>(vs_in);
	auto _fs_in = static_cast<PBRFSIn*>(fs_in);
	auto _uniforms = static_cast<PBRUniforms*>(uniforms);

	auto position = _vs_in->position;
	auto texcoord = _vs_in->texcoord;
	auto normal = _vs_in->normal;
	auto tangent = _vs_in->tangent;

	auto model_mat = _uniforms->model_mat;
	auto normal_mat = _uniforms->normal_mat;
	auto light_view_mat = _uniforms->light_view_mat;
	auto light_proj_mat = _uniforms->light_proj_mat;
	auto camera_view_mat = _uniforms->camera_view_mat;
	auto camera_proj_mat = _uniforms->camera_proj_mat;

	auto world_position = model_mat*glm::vec4(position, 1);
	auto depth_position = light_proj_mat*light_view_mat*world_position;
	auto clip_position = camera_proj_mat*camera_view_mat*world_position;

	if (_uniforms->shadow_pass) {
		_fs_in->texcoord = texcoord;
		return depth_position;
	}
	else {
		auto world_normal = glm::normalize(normal_mat*normal);
		if (_uniforms->nomal_camera_map) {
			auto world_tangent = glm::normalize(glm::vec3(model_mat*glm::vec4(tangent, 1)));
			auto world_bitangent = glm::cross(world_normal, world_tangent);
			_fs_in->world_tangent = world_tangent;
			_fs_in->world_bitangent = world_bitangent;
		}
		_fs_in->world_normal = world_normal;
		_fs_in->world_position = glm::vec3(world_position);
		_fs_in->depth_position = vec3_div(glm::vec3(depth_position), depth_position.w);
		_fs_in->texcoord = texcoord;
		_fs_in->clip_position = clip_position;
		return clip_position;
	}
}


static float distributionGGX(const glm::vec3& N, glm::vec3& H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = std::max(glm::dot(N, H), 0.0f);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return nom / std::max(denom, 0.0000001f); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

static float geometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r*r) / 8.0f;

	float nom   = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}

static float geometrySmith(const glm::vec3& N, glm::vec3& V, const glm::vec3& L, float roughness)
{
	float NdotV = std::max(glm::dot(N, V), 0.0f);
	float NdotL = std::max(glm::dot(N, L), 0.0f);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

static glm::vec3 fresnelSchlick(float cosTheta, const glm::vec3& F0)
{
	return F0 + float_minus_vec3(1.0f, F0)*pow(std::max(1.0f-cosTheta, 0.0f), 5.0f);
}

glm::vec4 PBRShader::ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) {
	auto _fs_in = static_cast<PBRFSIn*>(fs_in);
	auto _uniforms = static_cast<PBRUniforms*>(uniforms);

	auto world_position = _fs_in->world_position;
	auto depth_position = _fs_in->depth_position;
	auto clip_position = _fs_in->clip_position;
	auto world_normal = _fs_in->world_normal;
	auto world_tangent = _fs_in->world_tangent;
	auto world_bitangent = _fs_in->world_bitangent;
	auto texcoord = _fs_in->texcoord;

	auto shadow_map = _uniforms->shadow_map;
	auto camera_pos = _uniforms->camera_pos;
	auto light_pos = _uniforms->light_pos;
	auto light_dir = glm::normalize(light_pos - world_position);
	auto shadow_pass = _uniforms->shadow_pass;

	PBRMaterial material(fs_in, uniforms, backface);
	auto alpha = 1.0f;

	if (alpha <= 0.0f) {
		*discard = 1;
		return glm::vec4(0.0f, 0.0f, 0.0f, alpha);
	} 

	if (shadow_pass) {
		return glm::vec4(0.0f, 0.0f, 0.0f, alpha);
	}
	else{
		auto _base_color = material.has_base_color ? material.base_color : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _nomal_camera = material.has_nomal_camera ? material.nomal_camera : world_normal;
		auto _emission_color = material.has_emission_color ? material.emission_color : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _metalness = material.has_metalness ? material.metalness.x : 0.0f;
		auto _diffuse_roughness = material.has_diffuse_roughness ? material.diffuse_roughness.x : 0.0f;
		auto _ambient_occlusion = material.has_ambient_occlusion ? material.ambient_occlusion.x : 0.0f;

		auto N = _nomal_camera;
		auto V = glm::normalize(camera_pos-world_position);
		auto L = light_dir;

		float n_dot_l = glm::dot(N, L);
		bool is_in_shadow = false;
		if (shadow_map) {
			float u = (depth_position.x + 1) * 0.5f;
			float v = (depth_position.y + 1) * 0.5f;
			float d = (depth_position.z + 1) * 0.5f;
			float depth_bias = std::max(0.05f * (1 - n_dot_l), 0.005f);
			float current_depth = d - depth_bias;
			auto depth_texcoord = glm::vec2(u, v);
			float closest_depth = shadow_map->Sample(depth_texcoord).x;
			is_in_shadow = current_depth > closest_depth;
		}

		if(!is_in_shadow){
			// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
			// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
			auto F0 = glm::vec3(0.04); 
			F0 = glm::mix(F0, _base_color, _metalness);

			// reflectance equation
			auto Lo = glm::vec3(0.0f);
			// calculate light radiance
			auto H = glm::normalize(V + L);
			float distance = static_cast<float>((light_pos - world_position).length());
			float attenuation = 1.0f / (distance * distance);
			auto light_color = glm::vec3(0.3, 0.5, 0.6);
			auto radiance = light_color * attenuation;
			// Cook-Torrance BRDF
			float NDF = distributionGGX(N, H, _diffuse_roughness);   
			float G = geometrySmith(N, V, L, _diffuse_roughness);      
			auto F = fresnelSchlick(float_clamp(glm::dot(H, V), 0.0f, 1.0f), F0);

			auto nominator = NDF * G * F; 
			float denominator = 4 * std::max(glm::dot(N, V), 0.0f) * std::max(glm::dot(N, L), 0.0f);
			auto specular = nominator / std::max(denominator, 0.001f); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

			// kS is equal to Fresnel
			auto kS = F;
			// for energy conservation, the diffuse and specular light can't
			// be above 1.0 (unless the surface emits light); to preserve this
			// relationship the diffuse component (kD) should equal 1.0 - kS.
			auto kD = glm::vec3(1.0f) - kS;
			// multiply kD by the inverse metalness such that only non-metals 
			// have diffuse lighting, or a linear blend if partly metal (pure metals
			// have no diffuse light).
			kD *= 1.0f - _metalness;
			// scale light by NdotL
			float NdotL = std::max(glm::dot(N, L), 0.0f);
			// add to outgoing radiance Lo
			Lo += (kD * vec3_div(_base_color, PI) + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

			// ambient lighting (note that the next IBL tutorial will replace 
			// this ambient lighting with environment lighting).
			auto ambient = glm::vec3(0.03f) * _base_color * _ambient_occlusion;

			auto color = ambient + Lo + _emission_color;

			// HDR tonemapping
			color = color / (color + glm::vec3(1.0f));
			// gamma correct
			color = pow(color, glm::vec3(1.0f/2.2)); 

			return glm::vec4(color, alpha);
		}
	return glm::vec4(_emission_color, alpha);
	}
}
