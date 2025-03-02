#ifndef RENGINE_DEBUG_H
#define RENGINE_DEBUG_H

#include <iostream>

#define printVec2(vec) std::cout << vec.x << ' ' << vec.y << std::endl
#define printVec3(vec) std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl
#define printQuat(quat) std::cout << quat.w << quat.x << ' ' << quat.y << ' ' << quat.z << std::endl
#define printMat3(mat) std::cout << mat[0][0] << ' ' << mat[0][1] << ' ' mat[0][2] << '\n' \
                                 << mat[1][0] << ' ' << mat[1][1] << ' ' mat[1][2] << '\n' \
                                 << mat[2][0] << ' ' << mat[2][1] << ' ' mat[2][2] << '\n'


#endif // RENGINE_DEBUG_H