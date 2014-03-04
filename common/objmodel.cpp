#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "objmodel.h"

ObjModel::ObjModel(string objPath, string texturePath)
    :objPath(objPath), texturePath(texturePath){
    initBuffers();
    initVAO();
    initTexture();
    initModelVBO();
    initTextureVBO();
    initNormalVBO();
}

void ObjModel::initBuffers(){
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    ifstream inFile(objPath.c_str());
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

void ObjModel::initVAO(){
    // Setup the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}


void ObjModel::initModelVBO(){
    glGenBuffers(1, &modelVbo);
    glBindBuffer(GL_ARRAY_BUFFER, modelVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0, vertices
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
}

void ObjModel::initTextureVBO(){
    // Create the texels VBO and bind texture data to it
    glGenBuffers(1, &textureVbo);
    glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2) , &uvs[0], GL_STATIC_DRAW);
    // Texture attrib
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void ObjModel::initNormalVBO(){
    glGenBuffers(1, &normalVbo) ;
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void ObjModel::initTexture(){
    // Setup the texture
    glGenTextures(1, &texturePtr);
    glBindTexture(GL_TEXTURE_2D, texturePtr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cv::Mat textureData = cv::imread(texturePath, CV_LOAD_IMAGE_COLOR);
    cv::flip(textureData, textureData, 0); // Flip the image upside down for OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.cols, textureData.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData.data);
}
