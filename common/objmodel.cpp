#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

#include "objmodel.h"

ObjModel::ObjModel(string path){
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    ifstream inFile(path.c_str());
    string line;
    string type;

    float x,y,z;
    while(getline(inFile, line)){
        type = line.substr(0, 2);
        char *cLine = new char[line.size() + 1];
        memcpy(cLine, line.c_str(), line.size() + 1);

        if(type.compare("v ") == 0){
            strtok(cLine, " "); // Throw away the "v "
            x = atof(strtok(NULL, " "));
            y = atof(strtok(NULL, " "));
            z = atof(strtok(NULL, " "));
            objVertices.push_back(glm::vec3(x, y, z));
        }
        else if(type.compare("vn") == 0){
            strtok(cLine, " "); // Throw away the "vn"
            x = atof(strtok(NULL, " "));
            y = atof(strtok(NULL, " "));
            z = atof(strtok(NULL, " "));
            objNormals.push_back(glm::vec3(x, y, z));
        }
        else if(type.compare("vt") == 0){
            strtok(cLine, " "); // Throw away the "vt"
            x = atof(strtok(NULL, " "));
            y = atof(strtok(NULL, " "));
            objUvs.push_back(glm::vec2(x, y));
        }
        else if(type.compare("f ") == 0){
            strtok(cLine, " "); // Throw away the "f "
            for(int i=0; i<3; i++){
                vertexIndices.push_back(atof(strtok(NULL, " /")));
                uvIndices.push_back(atof(strtok(NULL, " /")));
                normalIndices.push_back(atof(strtok(NULL, " /")));
            }
        }
        delete[] cLine;
    }


    for(int i=0; i<vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        vertices.push_back(objVertices[vertexIndex-1]);
        uvs.push_back(objUvs[uvIndex-1]);
        normals.push_back(objNormals[normalIndex-1]);
    }

    /*
    for(int i=0; i<vertices.size(); i++){
        cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << endl;
    }
    */

    /*
    cout << "OBJ Vertices: " << objVertices.size() << endl;
    cout << "OBJ Texels: " << objUvs.size() << endl;
    cout << "OBJ Normals: " << objNormals.size() << endl;
    cout << "Faces: " << vertices.size() / 3 << endl;
    cout << "Vertices: " << vertices.size() << endl;

    cout << "Model Data" << endl;
    cout << "P1: " << objVertices[0].x << "x " << objVertices[0].y << "y " << objVertices[0].z << "z" << endl;
    cout << "T1: " << objUvs[0].x << "u " << objUvs[0].y << "v " << endl;
    cout << "N1: " << objNormals[0].x << "x " << objNormals[0].y << "y " << objNormals[0].z << "z" << endl;
    //cout << "F1v1: " << objFaces[0].x << "p " << objFaces[0].y << "t " << objFaces[0].z << "n" << endl;
    */
}
