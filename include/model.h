#ifndef MODEL_HEADER
#define MODEL_HEADER

#include "mesh.h"
#include "funcs.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <map>
#include <assimp/postprocess.h>
#include <unordered_map>


class Model {
public:
	Model(std::string path);
	void draw(Shader& shader);
	std::vector<Mesh> meshes;

private:
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::unordered_map<std::string, Texture> cachedTextures;
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture::Type typeName);
};



#endif

