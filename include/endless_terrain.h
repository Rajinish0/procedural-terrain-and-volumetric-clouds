#ifndef ENDLESS_TERRAIN_H
#define ENDLESS_TERRAIN_H

#include <vector>
#include "g_types.h"
#include "mesh.h"
#include "funcs.h"
#include "perlin.h"
#include "camera.h"
#include <thread>
#include <map>
#include <functional>
#include "debug.h"
#include "engine_consts.h"


const UI SIZE = 241;
const UI chunkSize = SIZE - 1;
const UI LODs = 12;
const UI LOD_INC = 2;
const float SCALE = 10.0f;

// struct ChunkData{
//     FLOAT_VEC heighData;
//     FLOAT_VEC normals;
// };

struct _chunkData{
    VEC4_VEC heights_and_normals;
    // VEC3_VEC  normals;

    _chunkData(VEC4_VEC heights_and_normals)
        :heights_and_normals(heights_and_normals){}

};


_chunkData generateChunkData(UI size, glm::vec2 center = glm::vec2(0.0f)){
    _chunkData data(VEC4_VEC(size * size));
    FLOAT_VEC v( (size+1) * (size + 1)  );
	float tlX = (size - 1)/-2.0f;
	float tlY = (size - 1)/2.0f;
	float x, y;
	float scale = (10.0f/chunkSize)/5.0f;
    Perlin2d perlin(256, 8);
	for (unsigned int i =0; i < size + 1; ++i){
		for (unsigned int j =0; j < size + 1; ++j){
            // if (j == size)
                // x = (center.x + chunkSize + tlX) * scale;
            // else
         	x = (center.x + tlX + (float)j) * scale;
			y = (center.y + tlY - (float)i) * scale;
			float p = perlin.perlin(x, y);
            v[funcs::flatten(i, j, size + 1)] = p;
            // if (i != size && j != size)
     		// v[funcs::flatten(i, j, size)] = p;
		}
	}

    auto vFunc = [size](int i, int j) -> UI{return funcs::flatten(i, j, size + 1);};

    for (UI i =0; i < size; ++i){
        for (UI j =0; j < size; ++j){
            float height = v[ vFunc(i, j) ];
            float height_dx = v[ vFunc(i, j + 1) ];
            float height_dy = v[ vFunc(i + 1, j) ];
            // data.heights[funcs::flatten(i, j, size)] = height;



            glm::vec3 v1 = glm::vec3(((float)j),  height * REngine::MAX_TERRAIN_HEIGHT, (float)i);
            glm::vec3 v2 = glm::vec3(((float)(j+1)) ,  height_dx * REngine::MAX_TERRAIN_HEIGHT, (float)i);
            glm::vec3 v3 = glm::vec3(((float)j), height_dy * REngine::MAX_TERRAIN_HEIGHT, (float)(i+1));

            glm::vec3 dx = v2 - v1;
            glm::vec3 dy = v3 - v1;

            glm::vec3 n = glm::normalize(glm::cross(dy, dx));
            n = (n + 1.0f)/2.0f;
            data.heights_and_normals[funcs::flatten(i, j, size)] = glm::vec4(height, n.x, n.y, n.z);

        } 
    }
    return data;
}


FLOAT_VEC generateHeightData(UI size, glm::vec2 center = glm::vec2(0.0f)){
	Perlin2d perlin;
	FLOAT_VEC v(size * size);

    /*
     TO DO:
        create a padded mesh s.t the edges are only used for normal calculation
        the normals should be passed via a texture to the shader
        FLOAT_VEC normal_vecs(size * size);

        this should help remove the line b/w the chunks
    */

    // FLOAT_VEC normals(size * size);
	float tlX = (size - 1)/-2.0f;
	float tlY = (size - 1)/2.0f;
	float x, y;
	float scale = 10.0f/chunkSize;
    float bsz = 10.0f / (float) (size - 1);
    float startx = -10.0f / 2.0f;
    float starty = 10.0f / 2.0f;
	for (unsigned int i =0; i < size; ++i){
		for (unsigned int j =0; j < size; ++j){
            // float xx = startx + bsz * j;
            // float yy = starty - bsz * i;
            // float cxx = (center.x / chunkSize) * 10.0f;
            // float cyy = (center.y / chunkSize) * 10.0f;
			x = (center.x + tlX + (float)j) * scale;
			y = (center.y + tlY - (float)i) * scale;
			float p = perlin.perlin(x, y) ;
            // glm::vec3 v1 (i, p, j);
            // glm::vec3 v2 (i + 1, )
			// // std::cout << p << std::endl;
			v[funcs::flatten(i, j, size)] = p;
		}
	}

	return v;
}


void requestHeightData(UI size, glm::vec2 center, std::function<void(FLOAT_VEC)> callback){
    /*
    TO DO:
        change generateHeightData to generateChunkData
    */
    std::thread([=](){
        FLOAT_VEC v = generateHeightData(size, center);
        callback(v);
    }
    ).detach();
}

void requestChunkData(UI size, glm::vec2 center, std::function<void(_chunkData)> callback){
    std::thread([=](){
        _chunkData data = generateChunkData(size, center);
        callback(data);
    }
    ).detach();
}


class HeightMapWrapper{

public:
    GLuint texture;
    GLuint normalTexture;
    glm::vec2 center;

    // HeightMapWrapper(UI size, glm::vec2 center)
    //     :size(size), center(center){
    //     glGenTextures(1, &texture);
    //     requestHeightData(size, center, [this](FLOAT_VEC v){onDataRecv(v);});
    // }

    HeightMapWrapper(UI size, glm::vec2 center)
        :size(size), center(center){
            glGenTextures(1, &texture);
            requestChunkData(size, center, [this](_chunkData d){onDataRecv(d);});
        }

    /*
    TO DO:
        recv chunkData instead of vec
    */

    void onDataRecv(_chunkData d){
        // this->v = std::move(d.heights);
        // this->normals = std::move(d.normals);
        this->height_and_normals = std::move(d.heights_and_normals);
        dataReceived = true;
    }

    // void onDataRecv(FLOAT_VEC v){
    //     // print("RECEVD DATA");
    //     // print(v.size());
    //     // print(v[0]);
    //     // print(v[1]);
    //     this->v = std::move(v);
    //     // print(this->v[0]);
    //     // print(this->v[1]);
    //     dataReceived = true;
    // }

    void makeReady(){
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, (size), (size), 0, GL_RED, GL_FLOAT, v.data());
        // glGenerateMipmap(GL_TEXTURE_2D);
        // glBindTexture(GL_TEXTURE_2D, 0);

        // glBindTexture(GL_TEXTURE_2D, normalTexture);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, normals.data());
        // glGenerateMipmap(GL_TEXTURE_2D);
        // glBindTexture(GL_TEXTURE_2D, 0);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size, size, 0, GL_RGBA, GL_FLOAT, height_and_normals.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        isReady = true;
    }

    void update(){

        // print("UPDATED CALLED");
        if (!isReady && dataReceived)
            makeReady();
    }

    bool ready(){
        return isReady;
    }

    void bind(int num = 0){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
    }


private:
    bool isReady = false;
    bool dataReceived = false;
    // FLOAT_VEC v;
    VEC4_VEC height_and_normals;
    UI size;
    float scale;
};

namespace E_T_TYPES{
    using PAIR_MESH_MAP     = std::map<IIPAIR, Mesh*>;
    using PAIR_HEIGHT_MAP   = std::map<IIPAIR, HeightMapWrapper*>;
    using MESH_VEC          = std::vector<Mesh>;
}

class EndlessTerrain{

public:
	glm::mat4 proj;
    Mesh m1 = funcs::genPlane2(SIZE, 1);

    EndlessTerrain(Camera& player, UI size = SIZE, float scale = SCALE)
        :player(player), size(size), chunkSize(size - 1), scale(scale)
    {
        proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
        for (int i = 1; i <= LODs; i += LOD_INC){
            LODMeshes.push_back(
                funcs::genPlane2(size, i)
            );
        }
    }

    void update(){
    }

    void draw(Shader& shader){
        // shader.use();
        // shader.setMatrix("proj", proj);
        // shader.setMatrix("view", player.getView());
        // shader.setMatrix("model", glm::mat4(1.0f));
        // LODMeshes[0].draw(shader);
// Assuming scale is defined and player.position is a glm::vec3 or similar
// float scale = /* your scale value */;
        // size = 
        int gridSize = scale / 2.0f; // Size of each grid cell in terms of player coordinates

        int x = std::floor((player.position.x + gridSize) / scale);
        int y = std::floor((player.position.z + gridSize) / scale);

    //    std::cout << "Px: " << player.position.x << " Py: " 
    //              << player.position.z << " x: " << x 
    //              << " y: " << y << std::endl;;
        for (int di =-1; di <=1; ++di){
            for (int dj =-1; dj <=1; ++dj){
                int ni = y + di,
                    nj = x + dj;

                if (history.count( {ni, nj} ) == 0)
                    history[{ni, nj}] = new HeightMapWrapper(size, glm::vec2((float)nj *chunkSize, (float)ni * chunkSize));

                HeightMapWrapper *chunk = history[{ni, nj}];
                chunk->update();
                if (chunk->ready()){
                    // print(chunk->center.x);
                    // print(chunk->center.y);
                    shader.use();
                    glm::mat4 model(1.0f);
                    model = glm::translate(model, glm::vec3((float)nj * scale, 0.0f, (float)ni * scale ));
                    model = glm::scale(model, glm::vec3(scale/float(chunkSize)));
                    shader.setMatrix("model", model);
                    shader.setInt("heightMap", 1);
                    shader.setInt("normalMap", 2);
                    chunk->bind();
                    LODMeshes[0].draw(shader);
                // m1.draw(shader);
                }
            }
        }
    }

private:
    E_T_TYPES::MESH_VEC LODMeshes;
    Camera& player;
    UI size, chunkSize; 
    // MeshTypes::PAIR_HEIGHT_MAP history;
    /*
    TO DO:
        use LRUCache with capacity ~ 100 for history
    */
    E_T_TYPES::PAIR_HEIGHT_MAP history;
    float scale;
};

#endif
