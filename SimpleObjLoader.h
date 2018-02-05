//


//Code based off https://www.raywenderlich.com/48293/how-to-export-blender-models-to-opengl-es-part-1
//  SimpleObjLoader.h
//  OpenGLSetupTestWithSTB
//
//  Created by Forrest Miller on 1/25/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#ifndef SimpleObjLoader_h
#define SimpleObjLoader_h

//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>



// Model Structure
typedef struct Model
{
    int vertices;
    int positions;
    int texels;
    int normals;
    int faces;
}
Model;



class SimpleObjLoader {
    
private:
    std::string filePath;
public:
    
    //Variables that are a part of this class
    Model model;
    float ** positions;
    float **texels;
    float ** normals;
    int ** faces;
    
    bool isLoaded;
    
    //SimpleObjLoader() = delete; //Can't create a loader without a destination
    
    SimpleObjLoader() {;}
    
    SimpleObjLoader(std::string fileLocation) {
        this->filePath = fileLocation;
        model = getOBJinfo(filePath);
        
        
//        // Model Data
//        positions[model.positions][3];    // XYZ
//        texels[model.texels][2];          // UV
//        normals[model.normals][3];        // XYZ
//        faces[model.faces][9];              // PTN PTN PTN
        
        //Use dynamic memory
        positions = new float*[model.positions];
        for (int i = 0; i < model.positions; i++) {
            positions[i] = new float[3];
        }
        
        texels = new float*[model.texels];
        for (int i = 0; i < model.texels; i++) {
            texels[i] = new float[2];
        }
       
        normals = new float*[model.normals];
        for (int i = 0; i < model.normals; ++i) {
            normals[i] = new float[3];
        }
        
        faces = new int*[model.faces];
        for (int i = 0; i < model.faces; ++i) {
            faces[i] = new int[9];
        }
        
        
        
        
        
        
        
        
        
        
        
        this->extractOBJdata(filePath, positions, texels, normals, faces);
        
        isLoaded = true;
    }
    
    
    ~SimpleObjLoader() {
        //free dynamic memory
        //Free vertex data:
        for (int i = 0; i < model.positions; i++) {
            delete [] positions[i];
        }
        delete [] positions;
        
        //Delete texel data:
        for (int i = 0; i < model.texels; i++) {
            delete [] texels[i];
        }
        delete [] texels;
        
        //return to the heap memory used for normals
        for (int i = 0; i < model.normals; ++i) {
            delete[] normals[i];
        }
        delete [] normals;
        
        for (int i = 0; i < model.faces; ++i) {
            delete[] faces[i];
        }
        delete [] faces;
        
    }
    
    Model getOBJinfo(std::string fp)
    {
        // 2
        Model model = {0};
        
        // 3
        // Open OBJ file
        std::ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            std::cout << "ERROR OPENING OBJ FILE" << std::endl;
            std::cout << fp << std::endl;
            isLoaded = false;
            exit(EXIT_FAILURE);
        }
        
        // 4
        // Read OBJ file
        while(!inOBJ.eof())
        {
            // 5
            std::string line;
            getline(inOBJ, line);
            std::string type = line.substr(0,2);
            
            // 6
            if(type.compare("v ") == 0)
                model.positions++;
            else if(type.compare("vt") == 0)
                model.texels++;
            else if(type.compare("vn") == 0)
                model.normals++;
            else if(type.compare("f ") == 0)
                model.faces++;
        }
        
        // 7
        model.vertices = model.faces*3;
        
        // 8
        // Close OBJ file
        inOBJ.close();
        
        // 9
        return model;
    }
    
    
    
//    void extractOBJdata(std::string fp, float positions[][3], float texels[][2], float normals[][3], int faces[][9]) {
    void extractOBJdata(std::string fp, float **positions, float ** texels, float ** normals, int **faces) {
        // Counters
        int p = 0;
        int t = 0;
        int n = 0;
        int f = 0;
        
        // Open OBJ file
        std::ifstream inOBJ;
        inOBJ.open(fp);
        if(!inOBJ.good())
        {
            std::cout << "ERROR OPENING OBJ FILE" << std::endl;
            isLoaded = false;
            exit(1);
        }
        
        // Read OBJ file
        while(!inOBJ.eof())
        {
            std::string line;
            getline(inOBJ, line);
            std::string type = line.substr(0,2);
            
            // Positions
            if(type.compare("v ") == 0)
            {
                // 1
                // Copy line for parsing
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                // 2
                // Extract tokens
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    positions[p][i] = atof(strtok(NULL, " "));
                
                // 3
                // Wrap up
                delete[] l;
                p++;
            }
            
            // Texels
            else if(type.compare("vt") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<2; i++)
                    texels[t][i] = atof(strtok(NULL, " "));
                
                delete[] l;
                t++;
            }
            
            // Normals
            else if(type.compare("vn") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                for(int i=0; i<3; i++)
                    normals[n][i] = atof(strtok(NULL, " "));
                
                delete[] l;
                n++;
            }
            
            // Faces
            else if(type.compare("f ") == 0)
            {
                char* l = new char[line.size()+1];
                memcpy(l, line.c_str(), line.size()+1);
                
                strtok(l, " ");
                
                //I added this to prevent my cube test thing from crashing:
                if (this->model.faces == 12) {
                    for (int i = 0; i < 3; i++) {
                        faces[f][i] = atof(strtok(NULL, " /"));
                    }
                }
                else {
                for(int i=0; i<9; i++)
                    faces[f][i] = atof(strtok(NULL, " /"));
                }
                delete[] l;
                f++;
            }
        }
        
        // Close OBJ file
        inOBJ.close();
    }
    
    
};



#endif /* SimpleObjLoader_h */
