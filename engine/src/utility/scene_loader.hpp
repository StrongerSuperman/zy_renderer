#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../core/mesh.hpp"
#include "../core/scene.hpp"
#include "../core/image.hpp"
#include "../shading/blinn/blinn_model.hpp"


static std::vector<Texture*> loadTextures(Scene* scene, aiMaterial *mat, aiTextureType type)
{
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        textures.emplace_back(new Texture(scene->directory + path, scene->dr_mode));
	}
	return textures;
}

static void processMesh(Scene* scene, Mesh* mesh, aiMesh *ai_mesh, const aiScene *ai_scene)
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

	// texture
	aiMaterial* material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
	// legacy Api Materials
	mesh->textures.emplace("diffuse", loadTextures(scene, material, aiTextureType_DIFFUSE));
	mesh->textures.emplace("specular", loadTextures(scene, material, aiTextureType_SPECULAR));
	mesh->textures.emplace("ambient", loadTextures(scene, material, aiTextureType_AMBIENT));
	mesh->textures.emplace("emissive", loadTextures(scene, material, aiTextureType_EMISSIVE));
	mesh->textures.emplace("height", loadTextures(scene, material, aiTextureType_HEIGHT));
	mesh->textures.emplace("normal", loadTextures(scene, material, aiTextureType_NORMALS));
	mesh->textures.emplace("shininess", loadTextures(scene, material, aiTextureType_SHININESS));
	mesh->textures.emplace("opacity", loadTextures(scene, material, aiTextureType_OPACITY));
	mesh->textures.emplace("displacement", loadTextures(scene, material, aiTextureType_DISPLACEMENT));
	mesh->textures.emplace("lightmap", loadTextures(scene, material, aiTextureType_LIGHTMAP));
	mesh->textures.emplace("reflection", loadTextures(scene, material, aiTextureType_REFLECTION));
    // PBR Materials
	mesh->textures.emplace("base_color", loadTextures(scene, material, aiTextureType_BASE_COLOR));
	mesh->textures.emplace("nomal_camera", loadTextures(scene, material, aiTextureType_NORMAL_CAMERA));
	mesh->textures.emplace("emission_color", loadTextures(scene, material, aiTextureType_EMISSION_COLOR));
	mesh->textures.emplace("metalness", loadTextures(scene, material, aiTextureType_METALNESS));
	mesh->textures.emplace("diffuse_roughness", loadTextures(scene, material, aiTextureType_DIFFUSE_ROUGHNESS));
	mesh->textures.emplace("ambient_occlusion", loadTextures(scene, material, aiTextureType_AMBIENT_OCCLUSION));
}

static void processNode(Scene* scene, aiNode *node, const aiScene *ai_scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
        Mesh* mesh = new Mesh();
		aiMesh* ai_mesh = ai_scene->mMeshes[node->mMeshes[i]];
        processMesh(scene, mesh, ai_mesh, ai_scene);
        if(scene->type == SceneType::SCENE_TYPE_BLINN){
            glm::mat4x4 transform(1.0f);
            Model* model = new BlinnModel(mesh, transform);
            scene->models.emplace_back(model);
        }
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(scene, node->mChildren[i], ai_scene);
	}
}

static void LoadScene(Scene* scene, std::string& filename){
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
