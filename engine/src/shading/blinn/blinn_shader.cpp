#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shading/blinn/blinn_shader.hpp"
#include "shading/blinn/blinn_material.hpp"
#include "utility/math_tool.hpp"


glm::vec4 BlinnShader::ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) {
	auto _vs_in = static_cast<BlinnVSIn*>(vs_in);
	auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
	auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

	auto position = _vs_in->position;
	auto texcoord = _vs_in->texcoord;
	auto normal = _vs_in->normal;

	auto model_mat = _uniforms->model_mat;
	auto normal_mat = _uniforms->normal_mat;
	auto light_view_mat = _uniforms->light_view_mat;
	auto light_proj_mat = _uniforms->light_proj_mat;
	auto camera_view_mat = _uniforms->camera_view_mat;
	auto camera_proj_mat = _uniforms->camera_proj_mat;

	auto world_position = model_mat*glm::vec4(position, 1);
	auto depth_position = light_proj_mat*light_view_mat*world_position;

	if (_uniforms->shadow_pass) {
		_fs_in->texcoord = texcoord;
		return depth_position;
	} 
	else {
		_fs_in->normal = glm::normalize(normal_mat*normal);
		_fs_in->world_position = glm::vec3(world_position);
		_fs_in->depth_position = vec3_div(glm::vec3(depth_position), depth_position.w);
		_fs_in->texcoord = texcoord;
		auto clip_position = camera_proj_mat*camera_view_mat*world_position;
		return clip_position;
	}
}

glm::vec4 BlinnShader::ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) {
	auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
	auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

	auto depth_position = _fs_in->depth_position;
	auto world_position = _fs_in->world_position;
	auto normal = _fs_in->normal;

	auto shadow_map = _uniforms->shadow_map;
	auto camera_pos = _uniforms->camera_pos;
	auto light_pos = _uniforms->light_pos;
	auto light_dir = glm::normalize(light_pos - world_position);
	auto shadow_pass = _uniforms->shadow_pass;

	BlinnMaterial material(fs_in, uniforms, backface);
	auto alpha = material.has_opacity ? material.opacity[0] : 1.0f;

	if (alpha <= 0.0f) {
		*discard = 1;
		return glm::vec4(0.0f, 0.0f, 0.0f, alpha);
	} 

	if (shadow_pass) {
		return glm::vec4(0.0f, 0.0f, 0.0f, alpha);
	}
	else {
		auto _ambient = material.has_ambient ? material.ambient : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _emission = material.has_emissive ? material.emissive : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _diffuse = material.has_diffuse ? material.diffuse : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _normal = glm::normalize(material.has_normal ? material.normal : (material.has_height ? material.height : normal));
		auto _specular = material.has_specular ? material.specular : glm::vec3(0.0f, 0.0f, 0.0f);
		auto _shininess = material.has_shininess ? material.shininess[0] : 32.0f;
		auto _ka = material.ka;
		auto _kd = material.kd;
		auto _ks = material.ks;

		float n_dot_l = glm::dot(_normal, light_dir);
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

		auto color = _emission;
		color += _ambient*_ka;
		if (n_dot_l > 0 && !is_in_shadow) {
			if (!(_specular.x == 0 &&
					_specular.y == 0 &&
					_specular.z == 0)) {
				auto view_dir = glm::normalize(camera_pos-world_position);
				auto half_dir = glm::normalize(light_dir+view_dir);
				float n_dot_h = glm::dot(_normal, half_dir);
				if (n_dot_h > 0) {
					float strength = pow(n_dot_h, _shininess);
					_specular *= strength;
				}
			}
			auto diffuse = _diffuse*n_dot_l;
			color += diffuse*_kd + _specular*_ks;
		}
		return glm::vec4(color, alpha) / 255.0f;
	}
}
