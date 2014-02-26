#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

class ObjModel
{
public:
    ObjModel(std::string path);
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;

private:
    std::vector< glm::vec3 > objVertices;
    std::vector< glm::vec3 > objNormals;
    std::vector< glm::vec2 > objUvs;
    std::vector< glm::vec3 > objFaces;
};

#endif // OBJMODEL_H
