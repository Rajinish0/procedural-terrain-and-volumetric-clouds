#include "model.h"

Model::Model(std::string path) {
	loadModel(path);
}

void Model::draw(Shader& shader) {
	for (Mesh& mesh : meshes)
		mesh.draw(shader);
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Assimp error:" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);

	// std::cout << "LOADED MESHES: " << meshes.size() << std::endl;

}


void Model::processNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<Texture> textures;
	Material meshMaterial;

	for (int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		glm::vec3 vector(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		vertex.position = vector;

		glm::vec3 normal(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		vertex.normal = normal;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);

			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i]; // this is gonna be a triangle because of the flag we set at the start
		for (int j = 0; j < face.mNumIndices; ++j) {
			indicies.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// std::cout << "LOADING DIFFUSE ONES " << std::endl;
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// std::cout << "LOADING SPECULAR ONES " << std::endl;
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		if(AI_SUCCESS == material->Get(AI_MATKEY_OPACITY, meshMaterial.opacity)) {
			// std::cout << "GOT THE OPACITY: " << meshMaterial.opacity << std::endl;
		}
	}

	return Mesh(vertices, textures, indicies, meshMaterial);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::Type typeName) {
	std::vector<Texture> textures;

	/*
	* TO DO:
	* USE HASHMAP FOR cachedTextures
	* better to have smth like lru cache for a bigger proj
	*/
	float opacity = 1.0f;
	// aiColor4D diffuse;
	// if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS){
	// 	opacity = diffuse.a;
	// 	std::cout << "GOT THE OPACITY: " << opacity << std::endl;
	// }


	for (int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		bool cached = false;
		// for (int j = 0; j < cachedTextures.size(); ++j) {
		// 	if (std::strcmp(cachedTextures[j].path.data(), str.C_Str()) == 0) {
		// 		cached = true;
		// 		textures.push_back(cachedTextures[j]);
		// 		break;
		// 	}
		// }
		cached = (cachedTextures.count(str.C_Str()) != 0);// {
			// cached = true;
		// }
		if (!cached) {
			Texture texture;
			// std::cout << "LOADING TEXTURE: " << str.C_Str() << std::endl;
			texture.id = funcs::TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			// textures.push_back(texture);
			cachedTextures[texture.path] = texture;
		} 
		textures.push_back(cachedTextures[str.C_Str()]);
	}

	return textures;
}
