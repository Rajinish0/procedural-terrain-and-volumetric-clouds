#include "endless_terrain.h"
#include "BS_thread_pool.h"

_chunkData generateChunkData(int size, glm::vec2 center = glm::vec2(0.0f)){
    _chunkData data(VEC4_VEC(size * size));
    int v_size = size + 2;
    FLOAT_VEC v( ( v_size ) * ( v_size )  );
	float tlX = (size - 1)/-2.0f;
	float tlY = (size - 1)/2.0f;
	float x, y;
	float scale = (SCALE/chunkSize)/200.0f;
    Perlin2d perlin(256, 8);
	for (int i =-1; i < size + 1; ++i){
		for (int j =-1; j < size + 1; ++j){
            // if (j == size)
                // x = (center.x + chunkSize + tlX) * scale;
            // else
         	x = (center.x + tlX + (float)j) * scale;
			y = (center.y + tlY - (float)i) * scale;
			float p = perlin.perlin(x, y);
            v[funcs::flatten(i + 1, j + 1, v_size )] = p;
            // if (i != size && j != size)
     		// v[funcs::flatten(i, j, size)] = p;
		}
	}

    // std::cout << v[0] << std::endl;
    auto vFunc = [v_size](int i, int j) -> int{return funcs::flatten(i + 1, j + 1, v_size);};

    const std::vector< std::pair<std::pair<int, int>, std::pair<int, int>>> dpairs 
                                                            { { {1, -1}, {1, 1}  }, 
                                                               { {1, 1}, {-1, 1},  },
                                                               { {-1, 1} , {-1, -1} },
                                                               { {-1, -1} , {1, -1} },
                                                            };

    for (int i =0; i < size; ++i){
        for (int j =0; j < size; ++j){
            float height = v[ vFunc(i, j) ];
            glm::vec3 norm(0.0f);
            for (auto [p1, p2] : dpairs){
                auto [dy1, dx1] = p1;
                auto [dy2, dx2] = p2;
                float height2 = v[ vFunc(i+dy1, j+dx1) ];
                float height3 = v[ vFunc(i+dy2, j+dx2) ];
                // glm::vec3 norm(0.0f);
                glm::vec3 v1 = glm::vec3(((float)j),  height * REngine::MAX_TERRAIN_HEIGHT, (float)i);
                glm::vec3 v2 = glm::vec3(((float)j+dx1),  height2 * REngine::MAX_TERRAIN_HEIGHT, (float)(i+dy1));
                glm::vec3 v3 = glm::vec3(((float)j+dx2),  height3 * REngine::MAX_TERRAIN_HEIGHT, (float)(i+dy2));

                glm::vec3 dy = v2 - v1;
                glm::vec3 dx = v3 - v1;
                norm += glm::cross(dy, dx);
                // break;
            }
            // float height2 = v[ vFunc(i+1, j+1) ];
            // float height3 = v[ vFunc(i+1, j-1) ];
            // // glm::vec3 norm(0.0f);
            // glm::vec3 v1 = glm::vec3(((float)j),  height * REngine::MAX_TERRAIN_HEIGHT, (float)i);
            // glm::vec3 v2 = glm::vec3(((float)j+1),  height2 * REngine::MAX_TERRAIN_HEIGHT, (float)i+1);
            // glm::vec3 v3 = glm::vec3(((float)j-1),  height3 * REngine::MAX_TERRAIN_HEIGHT, (float)i+1);

            // glm::vec3 dy = v2 - v1;
            // glm::vec3 dx = v3 - v1;
            // glm::vec3 norm = glm::cross(dx, dy);

            // for (int dy : dys ){
            //     float height_dy = v[ vFunc(i + dy, j) ];
            //     glm::vec3 v3 = glm::vec3(((float)(j)),  height_dy * REngine::MAX_TERRAIN_HEIGHT, (float)(i+dy));
            //     for (int dx : dxs){
            //         float height_dx = v[ vFunc(i + dy, j + dx) ];
            //         // data.heights[funcs::flatten(i, j, size)] = height;


            //         glm::vec3 v2 = glm::vec3(((float)(j+dx)) , height_dx * REngine::MAX_TERRAIN_HEIGHT, (float)(i+dy));

            //         glm::vec3 dxx = v2 - v1;
            //         glm::vec3 dyy = v3 - v1;

            //         norm += glm::cross(dyy, dxx);
            //         // n = (n + 1.0f)/2.0f;
            //     }
            // }
            norm = glm::normalize(norm);
            norm = (norm + 1.0f)/2.0f;
            data.heights_and_normals[funcs::flatten(i, j, size)] = glm::vec4(height, norm.x, norm.y, norm.z);
        } 
    }
    return data;
}


FLOAT_VEC generateHeightData(int size, glm::vec2 center = glm::vec2(0.0f)){
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

BS::thread_pool pool;

void requestChunkData(int size, glm::vec2 center, std::function<void(_chunkData)> callback){
    // std::thread([=](){
    //     _chunkData data = generateChunkData(size, center);
    //     callback(data);
    // }
    // ).detach();

    pool.submit_task([=](){
        _chunkData data = generateChunkData(size, center);
        callback(data);
    });
}

HeightMapWrapper::HeightMapWrapper(int size, glm::vec2 center)
:size(size), center(center){
    glGenTextures(1, &texture);
    requestChunkData(size, center, [this](_chunkData d){onDataRecv(d);} );
}

void HeightMapWrapper::onDataRecv(_chunkData d){
    // this->v = std::move(d.heights);
    // this->normals = std::move(d.normals);
    this->height_and_normals = std::move(d.heights_and_normals);
    dataReceived = true;
}

void HeightMapWrapper::makeReady(){
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size, size, 0, GL_RGBA, GL_FLOAT, height_and_normals.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    isReady = true;

    // height_and_normals.clear();
    // height_and_normals.shrink_to_fit();
    std::vector<glm::vec4>().swap(height_and_normals);
}

void HeightMapWrapper::update(){
    if (!isReady && dataReceived)
        makeReady();
}

bool HeightMapWrapper::ready(){
    return isReady;
}

void HeightMapWrapper::bind(int num){
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, normalTexture);
}

HeightMapWrapper::~HeightMapWrapper(){
    glDeleteTextures(1, &texture);
}

EndlessTerrain::EndlessTerrain(Camera& player, int size, float scale)
        :player(player), size(size), chunkSize(size - 1), scale(scale), history(500)
    {
        proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
        for (int i = 1; i <= LODs; i += LOD_INC){
            LODMeshes.push_back(
                funcs::genPlane2(size, i)
            );
        }
    }

void EndlessTerrain::update(){}


void EndlessTerrain::draw(Shader& shader){

    int gridSize = scale / 2.0f; // Size of each grid cell in terms of player coordinates

    int x = std::floor((player.position.x + gridSize) / scale);
    int y = std::floor((player.position.z + gridSize) / scale);

    glm::vec3 dir = player.direction;
    // printVec3(dir);

    for (int di =-10; di <=10; ++di){
        for (int dj =-10; dj <=10; ++dj){
            int ni = y + di,
                nj = x + dj;
            
            if (dir.x * dj + (dir.z)*(di) < 0.0f) continue;
            if (history.count( {ni, nj} ) == 0){
                // std::cout << "INSERTING " << std::endl;
                history.insert({ni, nj}, std::make_shared<HeightMapWrapper>(size, glm::vec2((float)nj *chunkSize, (float)ni * chunkSize)));
                // std::cout << "INSERTED " << std::endl;
            }

            std::shared_ptr<HeightMapWrapper> chunk = history[{ni, nj}];
            chunk->update();
            if (chunk->ready()){
                shader.use();
                glm::mat4 model(1.0f);
                model = glm::translate(model, glm::vec3((float)nj * scale, 0.0f, (float)ni * scale ));
                model = glm::scale(model, glm::vec3(scale/float(chunkSize), 1.0f, scale/float(chunkSize)));
                shader.setMatrix("model", model);
                shader.setInt("heightMap", 1);
                shader.setInt("normalMap", 2);
                chunk->bind();
                LODMeshes[(std::max(std::abs(di), std::abs(dj)))/3.0f].draw(shader);
            }
        }
    }
}