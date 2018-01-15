//
//  ProjectSetup.h
//  OpenGLSetupTest
//
//  Created by Forrest Miller on 1/15/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#ifndef ProjectSetup_h
#define ProjectSetup_h
#pragma once

#include <fstream>
#include <string>
#include <iostream>

// System Headers
#include "glad.h"
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
//#include <btBulletDynamicsCommon.h>
//#define GLEW_STATIC
//#include <GL/glew.h>//
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <GLFW/glfw3.h>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

// Define Some Constants
const int mWidth = 1920;
const int mHeight = 1080;


#endif /* ProjectSetup_h */
