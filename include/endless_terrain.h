#ifndef ENDLESS_TERRAIN_H
#define ENDLESS_TERRAIN_H

#include <vector>
#include "g_types.h"
#include "mesh.h"
#include "funcs.h"
#include "debug.h"
#include "perlin.h"
#include "camera.h"
#include <thread>
#include <map>
#include <functional>
#include "debug.h"
#include "engine_consts.h"
#include "lrucache.h"
#include <memory>
#include <future>



const int SIZE = 241;
const int chunkSize = SIZE - 1;
const int LODs = 12;
const int LOD_INC = 2;
const float SCALE = 100.0f;

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

_chunkData generateChunkData(int size, glm::vec2 center);

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

class HeightMapWrapper{

public:
    GLuint texture;
    glm::vec2 center;

    // HeightMapWrapper(int size, glm::vec2 center)
    //     :size(size), center(center){
    //     glGenTextures(1, &texture);
    //     requestHeightData(size, center, [this](FLOAT_VEC v){onDataRecv(v);});
    // }

    HeightMapWrapper(int size, glm::vec2 center);
    void onDataRecv(_chunkData d);
    void makeReady();
    void update();

    bool ready();

    void bind(int num = 0);

    ~HeightMapWrapper();


private:
    bool isReady = false;
    bool dataReceived = false;
    // FLOAT_VEC v;
    VEC4_VEC height_and_normals;
    int size;
    float scale;
};

namespace E_T_TYPES{
    using PAIR_MESH_MAP         = std::map<IIPAIR, Mesh*>;
    using PAIR_HEIGHT_MAP       = std::unordered_map<IIPAIR, HeightMapWrapper*, pair_hash>;
    using PAIR_HEIGHT_LRU_CACHE = LRUCache<IIPAIR, std::shared_ptr<HeightMapWrapper>, pair_hash>;
    using MESH_VEC              = std::vector<Mesh>;
}

class EndlessTerrain{

public:
	glm::mat4 proj;
    Mesh m1 = funcs::genPlane2(SIZE, 1);

    EndlessTerrain(Camera& player, int size = SIZE, float scale = SCALE);

    const E_T_TYPES::PAIR_HEIGHT_LRU_CACHE &getHistory();

    // ~EndlessTerrain(){
    //     for (auto& [k, v] : history){
    //         delete v;
    //     }
    //     // history.clear();
    // }

    void update();

    void draw(Shader& shader);

private:
    E_T_TYPES::MESH_VEC LODMeshes;
    Camera& player;
    int size, chunkSize; 
    // MeshTypes::PAIR_HEIGHT_MAP history;
    /*
    TO DO:
        use LRUCache with capacity ~ 100 for history
    */
    // E_T_TYPES::PAIR_HEIGHT_MAP history;
    
    // LRUCache<IIPAIR, std::shared_ptr<HeightMapWrapper>, pair_hash> history;
    E_T_TYPES::PAIR_HEIGHT_LRU_CACHE history;
    float scale;
};

#endif
