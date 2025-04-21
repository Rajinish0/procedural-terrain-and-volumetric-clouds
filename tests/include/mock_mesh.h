#ifndef MOCK_MESH_H
#define MOCK_MESH_H

#include "mesh.h"
#include <gtest/gtest.h>


class MockMesh
{
    MockMesh(std::vector<Vertex>, std::vector<Texture>, std::vector<unsigned int>, Material)
    {}
};


#endif 