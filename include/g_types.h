#ifndef G_TYPES_H
#define G_TYPES_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class HeightMapWrapper;

namespace Types{
    using STRING_VEC = std::vector<std::string>;
    using UI         = unsigned int;
    using UI_VEC     = std::vector<UI>;
    using INT_VEC    = std::vector<int>;
    using FLOAT_VEC  = std::vector<float>;
    using CHAR_VEC   = std::vector<char>;
    using VEC3_VEC   = std::vector<glm::vec3>;
    using VEC4_VEC   = std::vector<glm::vec4>;
    // using MESH_VEC   = std::vector<Mesh>;
    using IIPAIR     = std::pair<int, int>;
}

//should probably remove this later
using namespace Types;

#endif
