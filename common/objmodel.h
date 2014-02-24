#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <iostream>
#include <vector>

class ObjModel
{
public:
    ObjModel(std::string path);
    float *positions;
    float *texels;
    float *normals;
    int verticesCount, positionsCount, texelsCount, normalsCount, facesCount;

private:
    std::vector< std::vector<float> > objPositions;
    std::vector< std::vector<float> > objNormals;
    std::vector< std::vector<float> > objTexels;
    std::vector< std::vector<float> > objFaces;
};

#endif // OBJMODEL_H
