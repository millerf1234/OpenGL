//
//  ShaderWrapper.cpp
//  OpenGLSetupTestWithSTB
//
//  Created by Forrest Miller on 1/29/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#include "ShaderWrapper1.h"

//Constructors:
ShaderWrapper::ShaderWrapper() {
    this->isReady = false;
    this->hasVert = this->hasGeom = this->hasFrag = this->hasTessl = false;
    this->vertSuccess = this->geomSuccess = this->fragSuccess = this->tesslSuccess = false;
    this->vertLayoutSet = false;
    
    //Set all the names to just empty null-terminated c-strings
    this->vertexAttribName[0] = '\0';
    this->colorAttribName[0] = '\0';
    this->textureAttribName[0] = '\0';
    this->normalAttribName[0] = '\0';
    
    this->vertexShader = nullptr;
    this->geometryShader = nullptr;
    this->fragmentShader = nullptr;
    this->tesselationShader = nullptr;
    this->programID = nullptr;
    
    this->posAttrib = nullptr;
    this->colAttrib = nullptr;
    this->texAttrib = nullptr;
    this->normAttrib = nullptr;
    
}

ShaderWrapper::~ShaderWrapper() {
    //Delete dynamic memory claimed by this object
    
    if (this->vertexShader != nullptr) {
        // glDeleteShader(*vertexShader); //I decided to delete the compiled shaders
        //                                  right after I finish linking, just to maintain
        //                                  consistancy.
        delete this->vertexShader;
        this->vertexShader = nullptr;
    }
    
    if (this->geometryShader != nullptr) {
        //glDeleteShader(*geometryShader);
        delete this->geometryShader;
        this->geometryShader = nullptr;
    }
    
    if (this->fragmentShader != nullptr) {
       // glDeleteShader(*fragmentShader);
        delete this->fragmentShader;
        this->fragmentShader = nullptr;
    }
    
    if (this->tesselationShader != nullptr) {
        //glDeleteShader(*tesselationShader);
        delete this->tesselationShader;
        this->tesselationShader = nullptr;
    }

    if (this->programID != nullptr) {
        delete this->programID;
        this->programID = nullptr;
    }
    
    if (this->VAO != nullptr) { //Delete the int representing this shader's VAO
        delete this->VAO ;
        this->VAO = nullptr;
    }
    
    if (this->posAttrib != nullptr) {
        delete this->posAttrib;
        this->posAttrib = nullptr;
    }
    
    if (this->colAttrib != nullptr) {
        delete this->colAttrib;
        this->colAttrib = nullptr;
    }
    
    if (this->texAttrib != nullptr) {
        delete this->texAttrib;
        this->texAttrib = nullptr;
    }
    
    if (this->normAttrib != nullptr) {
        delete this->normAttrib;
        this->normAttrib = nullptr;
    }
    
    
    //Template for deleting dynamic memory allocations I might add in the future
//    if (this-> != nullptr) {
//        delete this-> ;
//        this-> = nullptr;
//    }
}

bool ShaderWrapper::attachVert(char * vertSource) {
    if (hasVert) { //Make sure a vert shader is not already attached
        std::cout << "\nError attaching " << vertSource;
        std::cout << "\nThis ShaderWrapper already has a vertSource attached!";
        return false;
    }
    
    //Load the source file text as a c++ string
    this->vert = loadSourceFile((char *)vertSource);
    //Need to convert C++ string to a GLchar C string to be compiled onto the GPU
    const GLchar* vertexShaderSource = (const GLchar *)vert.c_str();
    //Tell OpenGL to compile the shader onto the GPU. VertexShader gets assigned a number to identify and track the shader.
    vertexShader = new GLuint;
    *vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(*vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(*vertexShader);
    
    //Check for compilation success
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, &success);
    //If not successful, let user know and clean self up
    if (!success) {
        glGetShaderInfoLog(*vertexShader, 512, NULL, infoLog);
        
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertSource
        << std::endl << infoLog << std::endl;
        
        delete vertexShader;
        vertexShader = nullptr;
        return false;
    }
    this->hasVert = true;
    this->vertSuccess = true;
    
    return true;
}

bool ShaderWrapper::attachGeom(char * geomSource) {
    if (hasGeom) { //Make sure a geom shader is not already attached
        std::cout << "\nError attaching " << geomSource;
        std::cout << "\nThis ShaderWrapper already has a geomSource attached!";
        return false;
    }
    return false;
}

bool ShaderWrapper::attachFrag(char * fragSource){
    if (hasFrag) { //Make sure a Frag shader is not already attached
        std::cout << "\nError attaching " << fragSource;
        std::cout << "\nThis ShaderWrapper already has a fragSource attached!";
        return false;
    }
    //Load the source file text as a c++ string
    this->frag = loadSourceFile((char *)fragSource);
    //Need to convert C++ string to a GLchar C string to be compiled onto the GPU
    const GLchar* fragmentShaderSource = (const GLchar *)frag.c_str();
    //Tell OpenGL to compile the shader onto the GPU. FragmentShader gets assigned a number to identify and track the shader.
    fragmentShader = new GLuint;
    *fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(*fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(*fragmentShader);
    
    //Check for compilation success
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(*fragmentShader, GL_COMPILE_STATUS, &success);
    //If not successful, let user know and clean self up
    if (!success) {
        glGetShaderInfoLog(*fragmentShader, 512, NULL, infoLog);
        
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragSource
        << std::endl << infoLog << std::endl;
        
        delete fragmentShader;
        fragmentShader = nullptr;
        return false;
    }
    this->hasFrag = true;
    this->fragSuccess = true;
    
    return true;
}

bool ShaderWrapper::attachTessl(char * tesslSource){
    if (hasTessl) { //Make sure a Frag shader is not already attached
        std::cout << "\nError attaching " << tesslSource;
        std::cout << "\nThis ShaderWrapper already has a fragSource attached!";
        return false;
    }
    return false;
}



//Note that a shader program must always have at least a vert and a frag. The
//Tesselation and Geometry shaders are optional
bool ShaderWrapper::link() {
    //Make sure there are enough shaders in place to create the shaderProgram set
    if (!readyToLink() || this->programID != nullptr) {
        if (this->programID != nullptr) {
            std::cout << "ERROR LINKING! This shader program has already been linked!"
            << std::endl;
        }
        return false;
    }
    
    //Create the shader program
    this->programID = new GLuint;
    *programID = glCreateProgram();
    
    //Attach the vertex shader to this object
    glAttachShader(*programID, *vertexShader);
    
    //Attach Geometry shader if there is one
    if (this->hasGeom) {
        //DO the work to attach the geom shader
    }
    
    //Attach Tesselation shader if there is one
    if (this->hasTessl) {
        //DO the work to attach the tesselation shader
    }
    
    //Attach Frag shader
    glAttachShader(*programID, *fragmentShader);
    
    
    //Now that all the shaders have been attached, link the program
    glLinkProgram(*programID);
    
    //Check for errors and print out reason if there are errors:
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(*programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    //So for all code beyond this point, we know that the shader program has
    //successfully been created
    
    //Delete shaders once they are linked
    if (vertexShader != nullptr) { //These nullptr checks aren't necessary but might as well, just to guarentee no issues down the road
        glDeleteShader(*vertexShader);
    }
    if (this->hasGeom) {
        if (this->geometryShader != nullptr) {
            glDeleteShader(*geometryShader);
        }
    }
    if (this->hasTessl) {
        if (this->tesselationShader != nullptr) {
            glDeleteShader(*tesselationShader);
        }
    }
    if (fragmentShader != nullptr) {
        glDeleteShader(*fragmentShader);
    }
    
    
    isLinked = true;
    return isLinked;
}

bool ShaderWrapper::specifyVertexLayout(vertLayoutFormat vlf) {
    if (!this->isLinked) {
        std::cout << "\nError! Need to finish the link step before specifing vertex layout format!\n";
        return false;
    }
    
    this->VAO = new GLuint;
    glGenVertexArrays(1, this->VAO);
    glBindVertexArray(*(this->VAO));
    
    
    if (vlf == VERT2) {
        this->posAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        glVertexAttribPointer(*(this->posAttrib), 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    }
    
    else if (vlf == VERT3) {
        this->posAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        std::cout << "\nposAttrib is " << *(this->posAttrib) << std::endl;
        glVertexAttribPointer(*(this->posAttrib), 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    }
    
    else if (vlf == VERT3TEXEL2) {
        this->posAttrib = new GLint;
        this->texAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        *(this->texAttrib) = glGetAttribLocation(*programID, this->textureAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        std::cout << "\nposAttrib is " << *(this->posAttrib) << std::endl;
         glVertexAttribPointer(*(this->posAttrib), 5, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
        glEnableVertexAttribArray(*(this->texAttrib));
        std::cout << "\ntexAttrib is " << *(this->posAttrib) << std::endl;
        glVertexAttribPointer(*(this->texAttrib), 5, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
    }
    
    else {
        std::cout << "\nOops! Due to Programmer lazyness, the vertex layout format you specified has not yet been implemented." << std::endl;
    }
    
    // glEnableVertexAttribArray(posAttrib3DLine);
   
    
    this->vertLayoutSet = true;
    this->isReady = true;
    return true;
}

bool ShaderWrapper::specifyVertexLayout(vertLayoutFormat vlf, GLuint vertData, GLuint elemData) {
    if (!this->isLinked) {
        std::cout << "\nError! Need to finish the link step before specifing vertex layout format!\n";
        return false;
    }
    
    this->VAO = new GLuint;
    glGenVertexArrays(1, this->VAO);
    glBindVertexArray(*(this->VAO));
    
    glBindBuffer(GL_ARRAY_BUFFER, vertData);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemData);
    
    if (vlf == VERT2) {
        this->posAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        glVertexAttribPointer(*(this->posAttrib), 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    }
    
    else if (vlf == VERT3) {
        this->posAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        std::cout << "\nposAttrib is " << *(this->posAttrib) << std::endl;
        glVertexAttribPointer(*(this->posAttrib), 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    }
    
    else if (vlf == VERT3TEXEL2) {
        this->posAttrib = new GLint;
        this->texAttrib = new GLint;
        *(this->posAttrib) = glGetAttribLocation(*programID, this->vertexAttribName);
        *(this->texAttrib) = glGetAttribLocation(*programID, this->textureAttribName);
        glEnableVertexAttribArray(*(this->posAttrib));
        std::cout << "\nposAttrib is " << *(this->posAttrib) << std::endl;
        glVertexAttribPointer(*(this->posAttrib), 5, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
        glEnableVertexAttribArray(*(this->texAttrib));
        std::cout << "\ntexAttrib is " << *(this->posAttrib) << std::endl;
        glVertexAttribPointer(*(this->texAttrib), 5, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
    }
    
    else {
        std::cout << "\nOops! Due to Programmer lazyness, the vertex layout format you specified has not yet been implemented." << std::endl;
    }
    
    // glEnableVertexAttribArray(posAttrib3DLine);
    
    
    this->vertLayoutSet = true;
    this->isReady = true;
    return true;
}


void ShaderWrapper::turnOffVertexLayout(vertLayoutFormat vlf) {
    std::cout << "Warning! Don't call this function because it should never be ";
    std::cout << "needed if you\nare using the VAO correctly." << std::endl;
    if (vlf == VERT3) {
        glDisableVertexAttribArray(*(this->posAttrib));
    }
    else if (vlf == VERT3TEXEL2) {
        glDisableVertexAttribArray(*(this->posAttrib));
        glDisableVertexAttribArray(*(this->texAttrib));
    }
    else {
        std::cout << "Extra Warning! You are attempting to turn off a vertex layout\n";
        std::cout << "that due to programmer laziness has not yet been implemented!" << std::endl;
    }
}



/*
 Initialisation:
 Code :
 glGenVertexArrays(1, &asteroid_vao);
 glBindVertexArray(asteroid_vao);
 glBindBuffer(GL_ARRAY_BUFFER, asteroid_buffer_object);
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
 
 glGenVertexArrays(1, &ship_vao);
 glBindVertexArray(ship_vao);
 glBindBuffer(GL_ARRAY_BUFFER, ship_buffer_object);
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
 
 glBindVertexArray(0);
 
 Rendering:
 Code :
 glUseProgram(theProgram);
 glBindVertexArray(asteroid_vao);
 glDrawArrays(GL_LINE_LOOP, 0, num_asteroid_vertices);
 glBindVertexArray(ship_vao);
 glDrawArrays(GL_LINE_LOOP, 0, num_ship_vertices);
 glBindVertexArray(0);
 glUseProgram(0);
 */
