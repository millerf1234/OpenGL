//
//  ShaderWrapper.hpp
//  OpenGLSetupTestWithSTB
//
//  Created by Forrest Miller on 1/29/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#ifndef ShaderWrapper_h
#define ShaderWrapper_h

#include <iostream>
#include <fstream>
#include "glad.h"

//#include "ProjectSetup.h"

class ShaderWrapper {
private:
    
    
    bool isLinked;
    bool isReady;
    bool hasVert, hasGeom, hasFrag, hasTessl;
    bool vertSuccess, geomSuccess, fragSuccess, tesslSuccess;
    
    bool vertLayoutSet;
   // int uniformCount;
    std::string vert;
    std::string geom;
    std::string frag;
    std::string tessl;
    
    
   //Add static to track number of programs in use?
    
    
    std::string loadSourceFile(char* filename)
    {
        std::ifstream infile{ filename };
        return{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    }
    
    
public:
    //Enum for vertexLayoutFormats
    enum vertLayoutFormat{VERT2, VERT3, VERT2TEXEL2, VERT3TEXEL2, VERT3TEXEL3, VERT2NORMALS2TEXEL2, VERT3NORMAL3TEXEL3};
    
    ShaderWrapper();
    ShaderWrapper(const ShaderWrapper&) = delete;
    
    ~ShaderWrapper();
    
    //No copy assignment operator
    ShaderWrapper operator=(const ShaderWrapper&) = delete;
    
    //Add a function that handles uniforms
    
    
    GLuint * vertexShader;
    GLuint * geometryShader;
    GLuint * fragmentShader;
    GLuint * tesselationShader;
    
    GLuint * programID;
    GLuint * VAO;
    
    GLint * posAttrib;
    GLint * colAttrib;
    GLint * texAttrib;
    GLint * normAttrib;
    
    
    GLchar vertexAttribName[512];
    GLchar colorAttribName[512];
    GLchar textureAttribName[512];
    GLchar normalAttribName[512];
    
    
    bool attachVert(char * vertSource);
    bool attachGeom(char * geomSource);
    bool attachFrag(char * fragSource);
    bool attachTessl(char * tesslSource);
    
    
    //These next 4 functions are for letting this object know what the 'in' variables
    //in the vert shader are called
    void setVertexAttribName(const GLchar * name) {
        int i = 0;
        while (name[i] != '\0') {
            vertexAttribName[i] = name[i];
            ++i;
        }
        vertexAttribName[i] = '\0'; //Don't forget to add character signifying end of c-string
    }
    
    void setColorAttribName(const GLchar * name) {
        int i = 0;
        while (name[i] != '\0') {
            colorAttribName[i] = name[i];
            ++i;
        }
        colorAttribName[i] = '\0'; //Add terminating character to c-string
    }
    
    void setTextureAttribName(const GLchar * name) {
        int i = 0;
        while (name[i] != '\0') {
            textureAttribName[i] = name[i];
            ++i;
        }
        textureAttribName[i] = '\0'; //Add the termination to the cstring
    }
    
    void setNormalAttribName(const GLchar * name) {
        int i = 0;
        while (name[i] != '\0') {
            normalAttribName[i] = name[i];
            ++i;
        }
        normalAttribName[i] = '\0';
    }
    
    bool readyToLink() {
        if (this->hasVert && this->hasFrag) {
            return true;
        } //Else:
        return false;
    }
    
    bool link();
    
    bool specifyVertexLayout(vertLayoutFormat);
    bool specifyVertexLayout(vertLayoutFormat, GLuint vertData, GLuint elemData);
    void turnOffVertexLayout(vertLayoutFormat);
    
    void use() {
        if (!this->isReady) {
            std::cout << "OOPS! This program isn't ready!";
            return;
        }
        glBindVertexArray(*(this->VAO));
        glUseProgram(*this->programID);
    }
    
    GLuint getID() {
        if (programID == nullptr) {
            std::cout << "\nError! getID() called on incomplete shader program\n";
            return ( (GLuint) 8742);
        }
        return *programID;
    }
};


#endif /* ShaderWrapper_h */
