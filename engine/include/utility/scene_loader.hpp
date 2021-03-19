#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/mesh.hpp"
#include "core/material.hpp"
#include "core/scene.hpp"
#include "core/image.hpp"
#include "shading/blinn/blinn_model.hpp"
#include "shading/pbr/pbr_model.hpp"


static void loadTextures(Scene* scene, std::vector<Texture*>& textures, aiMaterial *mat, aiTextureType type)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString file_name;
		mat->GetTexture(type, i, &file_name);
		std::string file_path(scene->directory);
		file_path.append(file_name.C_Str());
		textures.emplace_back(new Texture(file_path, scene->render_quality));
	}
}

static void processMesh(Scene* scene, Mesh* mesh, Material* material, aiMesh *ai_mesh, const aiScene *ai_scene)
{
	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
	{
		// vertices
		Vertex vertex;
		glm::vec3 vector;
		vector.x = ai_mesh->mVertices[i].x;
		vector.y = ai_mesh->mVertices[i].y;
		vector.z = ai_mesh->mVertices[i].z;
		vertex.position = vector;
		if (ai_mesh->mNormals)
		{
			vector.x = ai_mesh->mNormals[i].x;
			vector.y = ai_mesh->mNormals[i].y;
			vector.z = ai_mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		if (ai_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = ai_mesh->mTextureCoords[0][i].x;
			vec.y = ai_mesh->mTextureCoords[0][i].y;
			vertex.texcoord = vec;
		}
		else
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		if (ai_mesh->mTangents)
		{
			vector.x = ai_mesh->mTangents[i].x;
			vector.y = ai_mesh->mTangents[i].y;
			vector.z = ai_mesh->mTangents[i].z;
			vertex.tangent = vector;
		}
		if (ai_mesh->mBitangents)
		{
			vector.x = ai_mesh->mBitangents[i].x;
			vector.y = ai_mesh->mBitangents[i].y;
			vector.z = ai_mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		mesh->vertices.emplace_back(vertex);
	}

	// indices
	mesh->num_faces = ai_mesh->mNumFaces;
	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace face = ai_mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			mesh->indices.emplace_back(face.mIndices[j]);
	}

	aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

	// blinn Materials
	aiColor3D color;
	ai_material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material->ka = glm::vec3(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material->kd = glm::vec3(color.r, color.g, color.b);
	ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material->ks = glm::vec3(color.r, color.g, color.b);
	loadTextures(scene, material->diffuse_textures, ai_material, aiTextureType_DIFFUSE);
	loadTextures(scene, material->specular_textures, ai_material, aiTextureType_SPECULAR);
	loadTextures(scene, material->ambient_textures, ai_material, aiTextureType_AMBIENT);
	loadTextures(scene, material->emissive_textures, ai_material, aiTextureType_EMISSIVE);
	loadTextures(scene, material->height_textures, ai_material, aiTextureType_HEIGHT);
	loadTextures(scene, material->normal_textures, ai_material, aiTextureType_NORMALS);
	loadTextures(scene, material->shininess_textures, ai_material, aiTextureType_SHININESS);
	loadTextures(scene, material->opacity_textures, ai_material, aiTextureType_OPACITY);
	loadTextures(scene, material->displacement_textures, ai_material, aiTextureType_DISPLACEMENT);
	loadTextures(scene, material->lightmap_textures, ai_material, aiTextureType_LIGHTMAP);
	loadTextures(scene, material->reflection_textures, ai_material, aiTextureType_REFLECTION);
	// PBR Materials
	loadTextures(scene, material->base_color_textures, ai_material, aiTextureType_BASE_COLOR);
	loadTextures(scene, material->nomal_camera_textures, ai_material, aiTextureType_NORMAL_CAMERA);
	loadTextures(scene, material->emission_color_textures, ai_material, aiTextureType_EMISSION_COLOR);
	loadTextures(scene, material->metalness_textures, ai_material, aiTextureType_METALNESS);
	loadTextures(scene, material->diffuse_roughness_textures, ai_material, aiTextureType_DIFFUSE_ROUGHNESS);
	loadTextures(scene, material->ambient_occlusion_textures, ai_material, aiTextureType_AMBIENT_OCCLUSION);
}

static void processNode(Scene* scene, aiNode *node, const aiScene *ai_scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		Mesh* mesh = new Mesh();
		Material* material = new Material();
		aiMesh* ai_mesh = ai_scene->mMeshes[node->mMeshes[i]];
		processMesh(scene, mesh, material, ai_mesh, ai_scene);
		Model* model;
		glm::mat4x4 transform(1.0f);
		if(scene->type == SceneType::SCENE_TYPE_BLINN){
			model = new BlinnModel(scene, mesh, material, transform);
		}
		else if(scene->type == SceneType::SCENE_TYPE_PBR){
			
			model = new PBRModel(scene, mesh, material, transform);
		}
		scene->models.emplace_back(model);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(scene, node->mChildren[i], ai_scene);
	}
}

static void LoadScene(Scene* scene, const std::string& filename){
	Assimp::Importer importer;
	const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString();
		return;
	}
	auto dir_seg_pos1 = filename.find_last_of('/');
	auto dir_seg_pos2 = filename.find_last_of('\\') + 1;
	assert(dir_seg_pos1 <= filename.size() || dir_seg_pos2 <= filename.size());
	auto dir_seg_pos = dir_seg_pos1 <= filename.size() ? dir_seg_pos1 : dir_seg_pos2;
	scene->directory = filename.substr(0, dir_seg_pos);

	processNode(scene, ai_scene->mRootNode, ai_scene);
}

#endif //SCENE_LOADER_H
