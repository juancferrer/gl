#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;

#include "objmodel.h"

ObjModel::ObjModel(string path){
    verticesCount = positionsCount = texelsCount = normalsCount = facesCount = 0;
    ifstream inFile(path.c_str());
    string line;
    string type;
    // Read through it once and get the numbers
    while(getline(inFile, line)){
        type = line.substr(0,2);
        if(type.compare("v ") == 0){
            positionsCount++;
        }
        else if(type.compare("vn") == 0){
            normalsCount++;
        }
        else if(type.compare("vt") == 0){
            texelsCount++;
        }
        else if(type.compare("f ") == 0){
            facesCount++;
        }
    }
    verticesCount = facesCount * 3;

    cout << "Positions: " << positionsCount << endl;
    cout << "Texels: " << texelsCount << endl;
    cout << "Normals: " << normalsCount << endl;
    cout << "Faces: " << facesCount << endl;
    cout << "Vertices: " << verticesCount << endl;

    //Reset the file read ptr to the beginning
    inFile.clear();
    inFile.seekg(ios::beg);

    // Vector of XYZ positions.  3 colums, and 'x' rows
    objPositions.resize(positionsCount, vector<float>(3, 0.0));
    //objPositions = new float[positionsCount * 3];
    objNormals.resize(normalsCount, vector<float>(3, 0.0));
    //objNormals = new float[normalsCount * 3];

    // Vector of UV. 2 columns, and 'texels' rows
    objTexels.resize(texelsCount, vector<float>(2, 0.0));
    //objTexels = new float[texelsCount * 2];

    // Vector of PTN PTN PTN
    objFaces.resize(facesCount, vector<float>(9, 0.0));

    int p, t, n, f;
    p = t = n = f = 0;
    while(getline(inFile, line)){
        type = line.substr(0, 2);
        char *cLine = new char[line.size() + 1];
        memcpy(cLine, line.c_str(), line.size() + 1);

        if(type.compare("v ") == 0){
            strtok(cLine, " "); // Throw away the "v "
            for(int i=0; i<3; i++){
                objPositions[p][i] = atof(strtok(NULL, " "));
            }
            p++;
        }
        else if(type.compare("vn") == 0){
            strtok(cLine, " "); // Throw away the "vn"
            for(int i=0; i<3; i++){
                objNormals[n][i] = atof(strtok(NULL, " "));
            }
            n++;
        }
        else if(type.compare("vt") == 0){
            strtok(cLine, " "); // Throw away the "vt"
            for(int i=0; i<2; i++){
                objTexels[t][i] = atof(strtok(NULL, " "));
            }
            t++;
        }
        else if(type.compare("f ") == 0){
            strtok(cLine, " "); // Throw away the "f "
            for(int i=0; i<9; i++){
                objFaces[f][i] = atof(strtok(NULL, " /"));
            }
            f++;
        }

        delete[] cLine;
    }


    int x = 0;
    positions = new float[verticesCount * 3];
    for(int i=0; i<facesCount; i++){
        int vA = objFaces[i][0] - 1;
        int vB = objFaces[i][3] - 1;
        int vC = objFaces[i][6] - 1;

        positions[x++] = objPositions[vA][0];
        positions[x++] = objPositions[vA][1];
        positions[x++] = objPositions[vA][2];
        positions[x++] = objPositions[vB][0];
        positions[x++] = objPositions[vB][1];
        positions[x++] = objPositions[vB][2];
        positions[x++] = objPositions[vC][0];
        positions[x++] = objPositions[vC][1];
        positions[x++] = objPositions[vC][2];
     }

    x = 0;
    texels = new float[verticesCount * 2];
    for(int i=0; i<facesCount; i++){
        int vtA = objFaces[i][1] - 1;
        int vtB = objFaces[i][4] - 1;
        int vtC = objFaces[i][7] - 1;

        texels[x++] = objTexels[vtA][0];
        texels[x++] = objTexels[vtA][1];
        texels[x++] = objTexels[vtB][0];
        texels[x++] = objTexels[vtB][1];
        texels[x++] = objTexels[vtC][0];
        texels[x++] = objTexels[vtC][1];
     }

    x = 0;
    normals = new float[verticesCount * 3];
    for(int i=0; i<facesCount; i++){
        int vnA = objFaces[i][2] - 1;
        int vnB = objFaces[i][5] - 1;
        int vnC = objFaces[i][8] - 1;

        normals[x++] = objNormals[vnA][0];
        normals[x++] = objNormals[vnA][1];
        normals[x++] = objNormals[vnA][2];
        normals[x++] = objNormals[vnB][0];
        normals[x++] = objNormals[vnB][1];
        normals[x++] = objNormals[vnB][2];
        normals[x++] = objNormals[vnC][0];
        normals[x++] = objNormals[vnC][1];
        normals[x++] = objNormals[vnC][2];
    }

    for(int i=0; i<verticesCount * 3; i+=3){
        cout << positions[i] << " " << positions[i+1] << " " << positions[i+2] << endl;
    }

    cout << "Model Data" << endl;
    cout << "P1: " << objPositions[0][0] << "x " << objPositions[0][1] << "y " << objPositions[0][2] << "z" << endl;
    cout << "T1: " << objTexels[0][0] << "u " << objTexels[0][1] << "v " << endl;
    cout << "N1: " << objNormals[0][0] << "x " << objNormals[0][1] << "y " << objNormals[0][2] << "z" << endl;
    cout << "F1v1: " << objFaces[0][0] << "p " << objFaces[0][1] << "t " << objFaces[0][2] << "n" << endl;
}
