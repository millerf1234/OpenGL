//// Local Headers
#include "ProjectSetup.h"
#include "Quaternion.hpp"
#include "teapotVerts.h"
//#include "AlternativeAssimpObjLoader.h"
//#include "PlayerShip.h"
// System Headers
//#include "glad.h"
//#include <GLFW/glfw3.h>


// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
//#include <iostream>

#include "SpaceShipVertices.h"
#include "SimpleObjLoader.h"
//#include "Quaternion.hpp"
#include "ShaderWrapper.h"


//#include "objOutputHolder_hpp"

constexpr int triangleCount = 200000;
constexpr int floatsPerTriangle = 6;
constexpr float triangleSize = 0.008;

constexpr float ZOOMSPEED = 0.015f;
constexpr float MOVEMENTX = 0.035;
constexpr float MOVEMENTY = 0.035f;
const GLfloat ROTATIONSPEED = 0.024;

std::string loadSource(char* filename);
void updateVertices(GLfloat* vertices, float t);

//Global variables:
//Quaternion * xRot = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
//Quaternion * yRot = new Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
//Quaternion * zRot = new Quaternion(0.0f, 0.0f, 1.0f, 0.0f);
Quaternion *xRot, *yRot, *zRot;
//PlayerShip *pShip;

ShaderWrapper * mainEngineEffect; //First attempt as using my shader-compilation class

float xChange = 0.0f; //Used for translating the vertices before they reach the GPU
float yChange = 0.0f;

//Global vector of spaceship indices
std::vector<float> ssVerts;

//SimpleObjLoader ssLoader("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship.obj");

//SimpleObjLoader ssLoader("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship2.obj");

SimpleObjLoader ssLoader("space_ship2.obj");

//SimpleObjLoader ssLoader;

//SimpleObjLoader ssLoader("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/testCube.obj");

//SimpleObjLoader ssLoader("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/testCubeSimple.obj");

int main(int argc, char * argv[]) {
    
//    std::cout << "Please enter your file's location: \n";
//    std::string fileLocation;
//    std::cin.get();
//    getline(std::cin, fileLocation);
//
//    SimpleObjLoader * ssLoaderTemp = new SimpleObjLoader(fileLocation.c_str());
//
//    ssLoader = *ssLoaderTemp;
//
//    delete ssLoaderTemp;
    
    extractVertices1(&ssVerts); 
    
    mainEngineEffect = new ShaderWrapper;
    
    //Make the rotation quaternions
    xRot = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
    yRot = new Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
    zRot = new Quaternion(0.0f, 0.0f, 1.0f, 0.0f);
    
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE); //NOT SURE IF THIS IS NEEDED
    glfwWindowHint(GL_SAMPLES, 16);
    //glfwWindowHint(GLFW_FSAA_SAMPLES,0); //Doesn't work
    
    std::cout << glfwGetVersionString();
    
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr); //Open as window
   //auto mWindow = glfwCreateWindow(2650, 1600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Open fullscreen (alt+tab doesn't seem to work)
    
//    //Open on secondary monitor:
//        int count;
//        GLFWmonitor** monitors = glfwGetMonitors(&count);
//    GLFWwindow * mWindow;
//    if (count == 2) {
//        std::cout << "\nThe monitor count is: " << count << std::endl;
//        //auto mWindow = glfwCreateWindow(1920, 1080, "OpenGL", monitors[1], nullptr );
//        mWindow = glfwCreateWindow(1920, 1080, "OpenGL", monitors[1], nullptr );
//    }
//    else {
//        mWindow = glfwCreateWindow(2650, 1600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Open fullscreen (alt+tab doesn't seem to work)
////        auto mWindow = glfwCreateWindow(2650, 1600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Open fullscreen (alt+tab doesn't seem to work)
//    }
    
    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }
    
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    float counter = 0.0;
    
    //Load object file data
    //load player ship
//    PlayerShip * pShip = new PlayerShip("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship.obj");
    
//    pShip = new PlayerShip("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship.obj");
    
  
    //Need to load mesh into vertices and elements...
    
    //std::ifstream infile{ "/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship.obj" };
   // ObjImporterWrapper("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Objects/space_ship.obj");
    
    
    // Load Shaders
    
    // Load shaders
    std::string vert = loadSource((char*)("../../../../../../../../Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic.vert"));
    std::string vert3D = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic3D.vert"));
    std::string frag = loadSource((char*)("../../../../../../../../Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic.frag"));
    std::string frag3D = loadSource((char*) ("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic3D.frag"));
    std::string line3D = loadSource((char*) "/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/line3D.frag" );
    //std::string vert = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/basic.vert"));
    //std::string frag = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/basic.frag"));
    
    //std::printf("source: %s\n", vert.c_str());
    const GLchar* vertexShaderSource = (const GLchar *)vert.c_str();
    const GLchar* fragmentShaderSource = (const GLchar *)frag.c_str();
    
    const GLchar* vertex3DShaderSource = (const GLchar *)vert3D.c_str();
    const GLchar* fragment3DShaderSource = (const GLchar *)frag3D.c_str();
    
    const GLchar* fragment3DLineShaderSource = (const GLchar *)line3D.c_str();
    

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint vertexShader3D = glCreateShader(GL_VERTEX_SHADER);
    
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(vertexShader3D, 1, &vertex3DShaderSource, NULL);
    
    glCompileShader(vertexShader);
    glCompileShader(vertexShader3D);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Check 3D shader as well for errors:
    glGetShaderiv(vertexShader3D, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX3D::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shaders
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint lineShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShader3D, 1, &fragment3DShaderSource, NULL);
    glShaderSource(lineShader3D, 1, &fragment3DLineShaderSource, NULL);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShader3D);
    glCompileShader(lineShader3D);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Check the 3D fragment shader for errors as well
    glGetShaderiv(fragmentShader3D, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT3D::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Check the 3D line shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::3DLINEFRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    // Link shaders //
    GLuint shaderProgram = glCreateProgram();
    GLuint shaderProgram3D = glCreateProgram();
    GLuint shaderProgram3DLine = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glAttachShader(shaderProgram3D, vertexShader3D);
    glAttachShader(shaderProgram3D, fragmentShader3D);
    
    glAttachShader(shaderProgram3DLine, vertexShader3D);
    glAttachShader(shaderProgram3DLine, lineShader3D);

    //glBindFragDataLocation(shaderProgram, 0, "colr"); //I added this line
    glLinkProgram(shaderProgram);
    
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //Check for 3D shader program
    glLinkProgram(shaderProgram3D);
    glGetProgramiv(shaderProgram3D, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER3D::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    //Check 3D line shader for linking errors:
    glLinkProgram(shaderProgram3DLine);
    glGetProgramiv(shaderProgram3DLine, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER3DLINE::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader3D);
    glDeleteShader(fragmentShader3D);
    glDeleteShader(lineShader3D);
    
    glUseProgram(shaderProgram); //Start with using the 2D shader, will switch to 3D later on
    
    //             //
    
    // Create Vertex Array Object
    GLuint vao;  //A VAO is a storage container for VBOs. See: https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLfloat vertices[6*7*2 + teapot_count * 3];
    
    updateVertices(vertices, 0);
    
    //put pShip into vertices
    //int shipVerts = pShip->putShipIntoVertices(vertices, 0);
    
    
    
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    //--------------------------------------------------------------------------
    //Create an element for element array and stuff
    // Create an element array
    GLuint ebo; //element buffer element object
    glGenBuffers(1, &ebo);
    
////    GLuint elements[] = {
////        0, 1, 2,
////        2, 3, 0
////    };
//    //GLuint elements[teapot_count / 3];
//    GLuint elements[teapot_count];
//    for (GLuint i = 0; i < teapot_count; i++) {
//        elements[i] = i;
//    }
    
    
    int faceCounter = 0;
    //This is the version of elements that actually works properly
    GLuint elements[ssLoader.model.faces*9];
    for (GLuint i = 0; i < ssLoader.model.faces*3; i+=3) {
        //    GLuint elements[ssLoader.model.faces*9];
        //    for (GLuint i = 0; i < ssLoader.model.faces*9; i+=9) {
        
        elements[i] = ssLoader.faces[faceCounter][0] - 1; //Gotta shift index so values start at 0, not 1
        elements[i+1] = ssLoader.faces[faceCounter][3] - 1;
        elements[i+2] = ssLoader.faces[faceCounter][6] - 1;
        //elements[i+3] = ssLoader.faces[faceCounter][1] - 1;
       // elements[i+4] = ssLoader.faces[faceCounter][4] - 1;
        
       faceCounter++;
    }
    
    //Create a copy of the elements matrix so we can reset it later in case it gets modified (hint: it gets modified later)
    GLuint elementsCopyOrignl[ssLoader.model.faces * 3];
    for (int i = 0; i < ssLoader.model.faces * 3; ++i) {
        elementsCopyOrignl[i] = elements[i];
    }
    
    
//    GLuint elements[ssLoader.model.faces*5];
//    for (GLuint i = 0; i < ssLoader.model.faces*5; i+=5) {
////    GLuint elements[ssLoader.model.faces*9];
////    for (GLuint i = 0; i < ssLoader.model.faces*9; i+=9) {
//
//        elements[i] = ssLoader.faces[faceCounter][0] - 1;
//        elements[i+1] = ssLoader.faces[faceCounter][3] - 1;
//        elements[i+2] = ssLoader.faces[faceCounter][6] - 1;
//        elements[i+3] = ssLoader.faces[faceCounter][1] - 1;
//        elements[i+4] = ssLoader.faces[faceCounter][4] - 1;
//
//        //Weird... this seems right
//        std::cout << "\nFaces are: " << ssLoader.faces[faceCounter][0];
//        std::cout << "  " <<  ssLoader.faces[faceCounter][3];
//        std::cout << "  " << ssLoader.faces[faceCounter][6];
//
//
//
//
////        elements[i] = ssLoader.faces[faceCounter][0];
////        elements[i+1] = ssLoader.faces[faceCounter][1];
////        elements[i+2] = ssLoader.faces[faceCounter][2];
////        elements[i+3] = ssLoader.faces[faceCounter][3];
////        elements[i+4] = ssLoader.faces[faceCounter][4];
////        elements[i+5] = ssLoader.faces[faceCounter][5];
////        elements[i+6] = ssLoader.faces[faceCounter][6];
////        elements[i+7] = ssLoader.faces[faceCounter][7];
////        elements[i+8] = ssLoader.faces[faceCounter][8];
//        faceCounter++;
 //   }


 
    //GLuint elements[] = {
      /* 0, 1, 2,
       0, 3, 2, */
       /*
        2, 4, 1,
        8, 6, 5,
        5, 2, 1,
        6, 3, 2,
        3, 8, 4,
        1, 8, 5,
        2, 3, 4,
        8, 7, 6,
        5, 6, 2,
        6, 7, 3,
        3, 7, 8,
        1, 4, 8
        
    };
    
    for (int i = 0; i < 3*12; ++i) {
        elements[i] -= 1;
    } */
    
    //From when spaceship was drawn with quads
//    GLuint elements[] = {
//        //For the spaceship drawn with quads:
//        1, 2, 3, //row 1
//        2, 3, 4,
//        2, 5, 4, //row 2
//        5, 4, 6,
//        3, 7, 8, //row 3
//        7, 8, 1,
//        5, 9, 4, //row 4
//        9, 4, 6,
//        1, 3, 10, //row 5
//        3, 10, 11,
//        12, 13, 14, //row 6
//        13, 14, 15,
//        16, 14, 17, //row 7
//        14, 17, 15,
//        14, 16, 5, //row 8
//        16, 05, 2,
//        13, 2, 8, //row 9
//        2, 8, 17,
//        13, 18, 19, //row 10
//        18, 19, 17,
//        18, 2, 20, //row 11
//        2,  20, 21,
//        22, 23, 24, //row 12
//        23, 24, 25,
//        26, 21, 24, //row 13
//        21, 24, 23,
//        27, 20, 11, //row 14
//        20, 11, 28,
//
//    };
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
    //--------------------------------------------------------------------------
    
    
    
    //--------------------------------------------------------------------------
    // Attempt at using stb to load an image (jpg):
    //
    //
    // See: https://books.google.com/books?id=F0IwDwAAQBAJ&pg=PA521&lpg=PA521&dq=stbi_load+return&source=bl&ots=r0lFnzFQ1p&sig=ZbB9LW2xJXXiAhqIP4HXpZwKLsM&hl=en&sa=X&ved=0ahUKEwj4o4q6u87YAhURwmMKHS3EBOUQ6AEIVzAH#v=onepage&q=stbi_load%20return&f=false
    //
    //
    //Set attributes (NOT SURE WHERE TO DO THIS? OR IF NEEDED?):
    //    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    //    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    //    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    //    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    //    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    
    //std::string filename = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/Antarctica.jpg"));
    
    int w;
    int h;
    int components;
    //unsigned char* image = stbi_load(filename.c_str(), &w, &h, &components, STBI_rgb_alpha);
    
    //unsigned char* image = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/Antarctica.png"), &w, &h, &components, STBI_rgb_alpha);
    //unsigned char* image = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/Antarctica.jpg"), &w, &h, &components, STBI_rgb_alpha);
    
    // unsigned char* image = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/sample.png"), &w, &h, &components, STBI_rgb);
    
    //uint8_t * image = (uint8_t *) stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/boki.jpg"), &w, &h, &components, STBI_rgb); //Didn't work
    
    // char* image = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/boki.jpg"), &w, &h, &components, STBI_rgb);
    
    //unsigned char * stbi_data;
    //std::unique_ptr<unsigned char, void(*) (void*)> stbi_data;
    
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/GridBackground.jpg";
    //    char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/GridBackground2.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/GridBackground3.jpg";
    
    
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/RandomTextFromInternet.png";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/skewtxt.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/Antarctica.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/Antarctica.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/boki.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/sample.png";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/Antarctica.png";
    char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Cool_Picture_of_the_moon_915_609.jpg";
    std::unique_ptr<unsigned char, void(*)(void*)> stbi_data( stbi_load( img, &w, &h, &components, STBI_rgb ), stbi_image_free); //NOTE: void(*) (void*) is a function pointer to a function that takes a void pointer as parameter. See: https://stackoverflow.com/questions/12830860/what-is-voidvoid
    
    //Without variable for filename:
    //std::unique_ptr<unsigned char, void(*)(void*)> stbi_data( stbi_load( (char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/antarctica.png"), &w, &h, &components, STBI_rgb ), stbi_image_free); //NOTE: void(*) (void*) is a function pointer to a function that takes a void pointer as parameter. See: https://stackoverflow.com/questions/12830860/what-is-voidvoid
    
    //stbi_data = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/boki.jpg"), &w, &h, &components, STBI_rgb);
    
    //if(image == nullptr)
    if (stbi_data == nullptr) {
        //throw new IOException(STBImage.stbi_failure_reason()); //See: http://forum.lwjgl.org/index.php?topic=5991.0
        throw(std::string("Failed to load texture"));
    }
    int dataSize = w * h * (0 < STBI_rgb ? STBI_rgb : components); //From that online link
    
    std::vector<unsigned char> image_data;
    //image_data.resize(dataSize + w * (0 < STBI_rgb ? STBI_rgb : components)); //I changed to this while trying to correct texture
    //image_data.resize(dataSize + w * 540);
    //image_data.resize(dataSize + w *10);
    image_data.resize(dataSize);
    std::memcpy(image_data.data(), stbi_data.get(), dataSize);
    //stbi_image_free(stbi_data); //No longer need? Because of unique ptr
    ////    if(comp == 3)
    ////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    ////    else if(comp == 4)
    ////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    //
    //    stbi_image_free(image);
    //--------------------------------------------------------------------------
    
    
    
    //--------------------------------------------------------------------------
    // Texture attempt code, from: https://open.gl/textures
    // Create tex object?
    GLuint tex [1];
    glGenTextures(1, tex);
    //GLuint tex [2];
    //glGenTextures(2, tex); //glGenTextures(1, tex); //first parameter is num textures to generate?
    
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    //glBindTexture(GL_TEXTURE_3D, tex[1]);
    
    
    
    
    //                    WRAPPING
    //Texture coordinates are addressed from 0 to 1 starting from bottom left corner
    // NOTE: Instead of i,j,k or x,y,z, textures use s,t,r
    // so we Need to tell OpenGL how to handle texture coordinates that at < 0 or > 1
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    //NOTE!! The 'i' in glTexParameteri specifies the type of value to be used here (integer). For example, if we wanted to
    //       change the color at the border of the texture, we could use glTexParameterfv (float vector) like so:
    //
    // float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    
    // This changes the border color to red
    
    
    //                   FILTERING
    
    // Note that GL_TEXTURE_MIN_FILTER is for scaling the texture down and GL_TEXTURE_MAG_FILTER is for scaling up
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    //     Can also use mipmaps (but only after the texture has been loaded) by doing:
    glGenerateMipmap(GL_TEXTURE_2D);
    //glGenerateMipmap(GL_TEXTURE_3D);

    
    //GL_TEXTURE_MAX_ANISOTROPY_EXT = GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT; //see: https://www.khronos.org/opengl/wiki/Sampler_Object#Sampling_parameters
    
    //    //Swap two colors in texture (SWIZZLE):
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_GREEN); //Swaps Green in for red
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED); //Swaps red in for green
    
    
    
    
    
    //                  LOADING TEXTURE
    
    //Let's just create a simple pseudo-texture
    //    // Black/white checkerboard
    //    float pixels[] = {
    //        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    //        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    //    };
    
    
    //Try to fix the image_data:
    //for (int i = 0; i < h; i++) {
    //    image_data.push_back(0.0f);
    //}
    
    
    
    //This is what made it work but now it is not working:
    //    std::vector<unsigned char>::iterator it;
    //    int loopCounter = 0;
    //    for (it=image_data.begin(); it<image_data.begin() + dataSize - 10 * w; ++it) {
    //        if (3 == components) {
    //            if (++loopCounter % w == 0) {
    //                image_data.insert(it, (unsigned char) 1.0);
    //                //image_data.insert(it, (unsigned char) 1.0);
    //                //image_data.insert(it, (unsigned char) 1.0);
    //            }
    //            else if (loopCounter % w == 1) {
    //                //image_data.insert(it, (unsigned char)1.0);
    //            }
    //        }
    //    }
    //    for (int i = 0; i < 1830; i++) { //Aha!
    //        image_data[i] = (unsigned char) 255;
    //    }
    
    //Gotta erase the bogus data  (This is the other thing I was doing that made it sorta work)
    //image_data.erase (image_data.begin(), image_data.begin()+1832*1); //915 is image width x2
    
    //    //Before this point, it is almost there...
    //    for (it=image_data.begin(); it<image_data.end(); ++it) {
    //        if (3 == components) {
    //            if (loopCounter % w == 62) { //gotta try to find where first shift happens
    //                image_data.insert(it, (unsigned char)255);
    //            }
    //        }
    //    }
    
    
    //YO SOMETHING TO LOOK INTO: Doesn't the GPU take rgb values backwards? Is the
    //bottom of the texture being modified? Maybe not.. Who knows  (Answer: Probably not)
    
    //image_data.insert(image_data.begin(), (unsigned char)255);
    //    image_data.insert(image_data.begin(), (unsigned char)255);
    //    image_data.insert(image_data.begin(), (unsigned char)255);
    
    std::vector<unsigned char>::iterator it;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Here is the code that fixes a .jpg that is 915x609 pixels
    if (w == 915 && h == 609) {
        //This is just some debug code:
        //unsigned char test[w * h]; //This array allowed me to view
        int lineFixerCounter = 3;
        //for (int i = 3*w - 6 ; i < 9*w+6 ; ++i) {
        //    for (int i = 0 ; i < 27*w+6 ; ++i) {
        for (int i = 0 ; i < w*h * 3 ; ++i) {
            //test[i] = image_data[i];
            //std::cout << (unsigned int)test[i] << " ";
            //if (i % 3 == 2) {std::cout << std::endl << "pixel " << (i+1)/3 << ": ";}
            if ((i+1) == (w-1)*3) {
                //            image_data[i+1] = (unsigned char) 255;
                //            image_data[i+2] = (unsigned char) 255;
                //            image_data[i+3] = (unsigned char) 255;
                //            image_data[i+4] = (unsigned char) 0;
                //            image_data[i+5] = (unsigned char) 0;
                //            image_data[i+6] = (unsigned char) 255;
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\xfe');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');  //??
                image_data.insert(it, *(it+3));
                
                //image_data[i+7] = (unsigned char) 255;
                //image_data[i+8] = (unsigned char) 255;
                //image_data[i+9] = (unsigned char) 255;
            }
            if ((i+1) == (w-1)*6) {
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\xfe');  //??
                image_data.insert(it, *(it+3));
            }
            //Aha! I have discovered a pattern that almost almost works
            if ((i+1) == (914-1) * 3 * lineFixerCounter) {
                if (lineFixerCounter < 10) {
                    lineFixerCounter++;
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');
                    image_data.insert(it, *(it+3));
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');
                    image_data.insert(it, *(it+3));
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');  //??
                    image_data.insert(it, *(it+3));
                    //Red in top right corner:
                    // lineFixerCounter++;
                    // it = image_data.begin() + i+7;
                    // image_data.insert(it, '\0');
                    // it = image_data.begin() + i+7;
                    // image_data.insert(it, '\0');
                    // it = image_data.begin() + i+7;
                    // image_data.insert(it, '\xfe');  //??
                }
                else {
                    lineFixerCounter++;
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');
                    image_data.insert(it, *(it+3));
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');
                    image_data.insert(it, *(it+3));
                    it = image_data.begin() + i+7;
                    //image_data.insert(it, '\xfe');
                    image_data.insert(it, *(it+3));
                }
            }
            // if ((i+1)/3 == 1829) {
            /*image_data[i+1] = (unsigned char) 4;
             image_data[i+2] = (unsigned char) 110;
             image_data[i+3] = (unsigned char) 244;
             image_data[i+4] = (unsigned char) 211;
             image_data[i+5] = (unsigned char) 211;
             image_data[i+6] = (unsigned char) 211;
             
             
             it = image_data.begin() + i+3;
             image_data.insert(it, '\0');
             it = image_data.begin() + i+4;
             image_data.insert(it, '\0'); */
            //  it = image_data.begin() + i+2;
            //  image_data.insert(it, '\0');
            //  it = image_data.begin() + i+3;
            //  image_data.insert(it, '\0');
            //  it = image_data.begin() + i+4;
            //  image_data.insert(it, '\0');
            // }
            // if (i < 1155*3) {
            //      image_data[i] = '\xfe';
            // }
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
//    for (int i = 0; i < w * h * 3; i++) {
//
//    }
    
    //Gonna try to rewrite what I had above, except in a way that makes it work better
    
    //for (int i = 1; i < dataSize-1-h; ++i) {
//        if (i % (2*w) == 0) {
//            it = image_data.begin() + i;
//            image_data.insert(it, '\0');
//            it = image_data.begin() + i;
//            image_data.insert(it, '\0');
//            it = image_data.begin() + i;
//            image_data.insert(it, '\0');
//        }
//        if (i < 2744*3) {
//            image_data[i] = '\xfe';
//        }
      /*  if (i % w == 0) {
            it = image_data.begin() + i;
            image_data.insert(it, (unsigned char)255);
            //image_data.insert(it, (unsigned char)255);
        }
        if (i  == 305*3) {
            it = image_data.begin() + i;
            //image_data.insert(it, (unsigned char)255);
            image_data.erase(it);
        } */
        //        if (i == w*3) {
        //            it = image_data.begin() + i;
        //            image_data.insert(it, (unsigned char)255);
        //        }
    //s}
    
    //NEW NOTE FROM SCREENSHOT!!!  FIRST RED LINE APPEARS AT PIXEL 230
    
    
    if(components == 3) {
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2745/3 - 3 , h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h-1, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
        //NOTE!!! Doing w-1 shouldn't cause the RGB values to shift...
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, w, h-1, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
    }
    else if(components == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h-1, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
    }
//
    
    //stbi_image_free(image);
    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    // NOTE on glTexImage2D: The first parameter after the texture target is the level-of-detail, where 0 is the base image. This parameter can be used to load your own mipmap images. The second parameter specifies the internal pixel format, the format in which pixels should be stored on the graphics card. Many different formats are available, including compressed formats, so it's certainly worth taking a look at all of the options. The third and fourth parameters specify the width and height of the image. The fifth parameter should always have a value of 0 per the specification. The next two parameter describe the format of the pixels in the array that will be loaded and the final parameter specifies the array itself. The function begins loading the image at coordinate (0,0), so pay attention to this.
    
    
    
    //--------------------------------------------------------------------------
    
    
    //I am moving these next 3 lines up above to see if that changes anything
    //    // Create a Vertex Buffer Object and copy the vertex data to it
    //    GLuint vbo;
    //    glGenBuffers(1, &vbo);
    
    
    //GLfloat vertices[floatsPerTriangle*triangleCount];
    // GLfloat vertices[6*7*2]; //Moving to up early in code
    
    
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    GLint colAttrib = glGetAttribLocation(shaderProgram, "colr"); //Added this line for textures
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
    
    GLint posAttrib3D = glGetAttribLocation(shaderProgram3D, "position3D");
    GLint texAttrib3D = glGetAttribLocation(shaderProgram3D, "texCoord3D");
    
    GLint posAttrib3DLine = glGetAttribLocation(shaderProgram3DLine, "position3D");
    GLint texAttrib3DLine = glGetAttribLocation(shaderProgram3DLine, "texCoord3D");
    
    /*
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(colAttrib); //Added this line for textures
    //glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0); //Changed this line to (see below)
    //--------------------------------------------------------------------------
    // More Texture stuff:
    //GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0); //To this line (see above)
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));
    
    
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
    */ //turn off 2D shader for testing
    //For 3D shader:
    glEnableVertexAttribArray(posAttrib3D);
    glEnableVertexAttribArray(texAttrib3D);
    glVertexAttribPointer(posAttrib3D, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //To this line (see above)
     glVertexAttribPointer(texAttrib3D, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    
    
    //For 3D line shader:
    glEnableVertexAttribArray(posAttrib3DLine);
    glEnableVertexAttribArray(texAttrib3DLine);
    glVertexAttribPointer(posAttrib3DLine, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //To this line (see above)
    glVertexAttribPointer(texAttrib3DLine, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    
    //For 3D line shader (don't need?):
//    glEnableVertexAttribArray(texAttrib3DLine);
//    glVertexAttribPointer(posAttrib3DLine)
    
    
    
    //or
    //    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    //    glEnableVertexAttribArray(posAttrib);
    //    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0); //To this line (see above)
    //
    //    GLint colAttrib = glGetAttribLocation(shaderProgram, "colr");
    //    glEnableVertexAttribArray(colAttrib);
    //    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    //
    //    GLint texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
    //    glEnableVertexAttribArray(texAttrib);
    //    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    //
    //    //    //--------------------------------------------------------------------------
    
    
    
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //   //Try  out my new class for shader creation:
    mainEngineEffect->attachVert("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/engineEffect.vert");
    mainEngineEffect->attachFrag("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/engineEffect.frag");
    
    mainEngineEffect->link();
    mainEngineEffect->setVertexAttribName("enginePos");
    mainEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3); //Can I bring in class enums like a namespace?
    
    GLint engineZoom = glGetUniformLocation(mainEngineEffect->getID(), "zoom");
    GLint engineTime = glGetUniformLocation(mainEngineEffect->getID(), "time");
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    
    
    
    
    
    //UNIFORMS!!!!!
    GLint texEnum = glGetUniformLocation(shaderProgram, "tex");
    GLint texEnum3D = glGetUniformLocation(shaderProgram3D, "tex3D");
    GLint time = glGetUniformLocation(shaderProgram, "time");
    GLint time3D = glGetUniformLocation(shaderProgram3D, "time3D");
    GLint time3DLine = glGetUniformLocation(shaderProgram3DLine, "time3D");
    GLfloat theta = glGetUniformLocation(shaderProgram, "theta"); //I added this line
    GLfloat xCoor = glGetUniformLocation(shaderProgram, "xCoor");
    GLfloat yCoor = glGetUniformLocation(shaderProgram, "yCoor");
    GLfloat xCoor3D = glGetUniformLocation(shaderProgram3D, "xCoor3D");
    GLfloat yCoor3D = glGetUniformLocation(shaderProgram3D, "yCoor3D");
    
    GLfloat xCoord3DLine = glGetUniformLocation(shaderProgram3DLine, "xCoor3D");
    GLfloat yCoord3DLine = glGetUniformLocation(shaderProgram3DLine, "yCoor3D");
    
    //GLfloat xCoor3DLine = glGetUniformLocation(shaderProgram3DLine, "xCoor3D");
    //GLfloat yCoor3DLine = glGetUniformLocation(shaderProgram3DLine, "yCoor3D");
    
    GLfloat zoom = glGetUniformLocation(shaderProgram, "zoom"); //For w-axis
    GLfloat zoom3D = glGetUniformLocation(shaderProgram3D, "zoom3D"); //For w-axis
    GLfloat zoom3DLine = glGetUniformLocation(shaderProgram3DLine, "zoom3D");
    
    GLfloat thta = 0.0;
    GLfloat zoomFactor = 1.0;
    GLfloat xPos = 0.0;
    GLfloat yPos = 0.0;
    //GLint textureToDraw = 0;
    
    //Make these global
//    Quaternion * xRot = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
//    Quaternion * yRot = new Quaternion(0.0f, 1.0f, 0.0f, 0.0f);
//    Quaternion * zRot = new Quaternion(0.0f, 0.0f, 1.0f, 0.0f);
    
    //    //Just a test:
    //    for (unsigned char c: image_data) {
    //        std::cout << (float) c << " ";
    //    }
    
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //Here is where I try doing:http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
    //
    //    GLuint FramebufferName = 0;
    //    glGenFramebuffers(1, &FramebufferName);
    //    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    //
    //
    //    // The texture we're going to render to
    //    GLuint renderedTexture;
    //    glGenTextures(1, &renderedTexture);
    //
    //    // "Bind" the newly created texture : all future texture functions will modify this texture
    //    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    //
    //    // Give an empty image to OpenGL ( the last "0" )
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
    //
    //    // Poor filtering. Needed !
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //
    //    // Set "renderedTexture" as our colour attachement #0
    //    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    //
    //    // Set the list of draw buffers.
    //    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    //    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    //
    //
    //    // Always check that our framebuffer is ok
    //    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    //        return EXIT_FAILURE;
    //    }
    //    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    
    //updateVertices(vertices, 0.0); //Update vertices is where rotations and translations are applied
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    
//    //Try this here
//    //////////////////////////
//    //Instead try
//    glDisable(GL_DITHER);
//    glDisable(GL_POINT_SMOOTH);
//    glDisable(GL_LINE_SMOOTH);
//    glDisable(GL_POLYGON_SMOOTH);
//    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
//    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
//#define GL_MULTISAMPLE_ARB 0x809D
//    glDisable( GL_MULTISAMPLE_ARB) ;
    ////////////////////////////////
    
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mWindow, true);
        }
        if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
            //thta += ROTATIONSPEED;
            yRot->changeTheta(yRot->getTheta() + ROTATIONSPEED);
        }
        if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //thta -= ROTATIONSPEED;
            yRot->changeTheta(yRot->getTheta() - ROTATIONSPEED);
            
        }
        if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS) {
            thta += ROTATIONSPEED;
            xRot->changeTheta(xRot->getTheta() + ROTATIONSPEED);
        }
        if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
            thta -= ROTATIONSPEED;
            xRot->changeTheta(xRot->getTheta() - ROTATIONSPEED);
        }
        if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
            thta = 0.0f;
            zoomFactor = 1.0f;
            xPos = 0.0f;
            yPos = 0.0f;
            xRot->changeTheta(0.0f);
            yRot->changeTheta(0.0f);
            zRot->changeTheta(0.0f);
            xChange = 0.0f;
            yChange = 0.0f;
        }
        
        if (glfwGetKey(mWindow, 'Z') == GLFW_PRESS) {
            zoomFactor += ZOOMSPEED;
        }
        if (glfwGetKey(mWindow, 'X') == GLFW_PRESS) {
            zoomFactor -= ZOOMSPEED;
        }
        if (glfwGetKey(mWindow, 'W') == GLFW_PRESS) {
            yPos -= MOVEMENTY;
            yChange -= MOVEMENTY;
        }
        if (glfwGetKey(mWindow, 'S') == GLFW_PRESS) {
            yPos += MOVEMENTY;
            yChange += MOVEMENTY;
        }
        if (glfwGetKey(mWindow, 'A') == GLFW_PRESS) {
            xPos += MOVEMENTX;
            xChange += MOVEMENTX;
        }
        if (glfwGetKey(mWindow, 'D') == GLFW_PRESS) {
            xPos -= MOVEMENTX;
            xChange -= MOVEMENTX;
        }
        if (glfwGetKey(mWindow, 'Q') == GLFW_PRESS) {
            yRot->changeTheta(yRot->getTheta() + ROTATIONSPEED);
        }
        if (glfwGetKey(mWindow, 'E') == GLFW_PRESS) {
            yRot->changeTheta(yRot->getTheta() - ROTATIONSPEED);
        }
        
        if (glfwGetKey(mWindow, '.') == GLFW_PRESS) {
            //thta += ROTATIONSPEED;
            zRot->changeTheta(zRot->getTheta() + ROTATIONSPEED);
        }
        if (glfwGetKey(mWindow, ',') == GLFW_PRESS) {
            //thta -= ROTATIONSPEED;
            zRot->changeTheta(zRot->getTheta() - ROTATIONSPEED);
        }
        
        // Background Fill Color
        //glClearColor(sin(counter), 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Send "counter" value into shader via as uniform
        glUniform1f(time, counter);
        //glUniform1i(texEnum, *tex);
        glUniform1i(texEnum, tex[0]);
       // glUniform1i(texEnum3D, tex[1]);
        glUniform1f(theta, thta);
        glUniform1f(zoom, zoomFactor);
        //glUniform1f(zoom3D, zoomFactor);
        glUniform1f(xCoor, xPos);
        //glUniform1f(xCoor3D, xPos);
//        glUniform1f(xCoord3DLine, xPos);
        glUniform1f(yCoor, yPos);
        //glUniform1f(yCoor3D, yPos);
        //glUniform1f(yCoord3DLine, yPos);
        
        //Depth test code (I also need to reset it each frame below the draw call):
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        
        
        // Draw Stuff
        updateVertices(vertices, counter);
        //int shipVertCount = pShip->putShipIntoVertices(vertices, 0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        tex[0] = 0; //A-HA! This fixed it for some reason
        //tex[0] = (tex[0] + 1) % 2;
        
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex[0], 0);
        
        //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex[1], 0);
        
        //StreamDraw means going to update once, use once, and discard
        //DynamicDraw means going to set once, then use a bunch with potential modifications
        
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //glDrawArrays(GL_TRIANGLES, 0, floatsPerTriangle*triangleCount);
        
        //Reset elements:
        for (int i = 0; i < ssLoader.model.faces * 3; ++i) {
            elements[i] = elementsCopyOrignl[i];
        }
        
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);  //I added this one
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);  //I added this one
        //glDrawArrays(GL_TRIANGLES, 0, 6*7);
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Changed 3rd parameter to get it to draw finally
        
//        if (counter != 0.0) { //Don't turn the first program on during the first iteration because it will already be on
//            glUseProgram(shaderProgram);
//            glDrawArrays(GL_LINE_LOOP, (2*teapot_count+18), 6);
//            //glDrawElements(GL_TRIANGLE_FAN, 5000, GL_UNSIGNED_INT, 0);
//        }
        
        
        
        
        //glEnable(GL_TEXTURE_3D); //Turn on 3D texturing
       // tex[0] = 1; //Use the 3D texture? //No I don't think so
        glUseProgram(shaderProgram3D); //Use the 3D shader program
        
        //Need to redo shaders upon switching programs (or at least I think this needs to happen) (try uncommenting to see)
        glUniform1f(time3D, counter);
        glUniform1f(zoom3D, zoomFactor);
        glUniform1f(xCoor3D, xCoor);
        glUniform1f(yCoor3D, yCoor);
        //glDrawArrays(GL_LINES, 0, teapot_count / 3);
        //glDrawArrays(GL_TRIANGLES, 0, 90);
        //
        //Sytanx for glDrawElements is
        // glDrawElements(    GLenum mode, GLsizei count, GLenum type, const GLvoid * indices); //Note that 'type' is
        //                   the type of the array 'elements', not 'vertices' (so probably should be some type of integer).
        //glDrawElements(GL_TRIANGLES, teapot_count / 3, GL_UNSIGNED_INT, 0); //Not sure why last param is 0.
        
        //!!!!!!!!! THIS NEXT LINE BELOW WAS MY DRAW CALL
        //glDrawElements(GL_TRIANGLES, teapot_count / 3, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, ssLoader.model.faces, GL_UNSIGNED_INT, 0);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
      
        
        
        glDrawElements(GL_TRIANGLES, ssLoader.model.faces*3, GL_UNSIGNED_INT, 0);
        
        // So at this point, pass 1 has been completed and written to the framebuffer
        // So we switch the active program, set the uniforms required for it, and then drawElements for pass 2
        glUseProgram(shaderProgram3DLine); //Use the line shader program
        
         //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
        
        //Need to make lines appear slightly outside the teapot to avoid clipping issue
        //float yFixShift = 0.50f;
        float increaseFactor = 1.005f;
        
        //If drawing the teapot, do:
        /*
        for (int i = 0; i < teapot_count * (5) / 3; ++i) {
            //Need to handle y case seperatly because teapot isn't centered on y axis, it is all
            //above the y axis
            if (i % 5 == 1) { //If vert data is in format (x,y,z,tex_X,tex_Y) then y is at position 1
                vertices[i] = (yFixShift*increaseFactor) + (increaseFactor * (vertices[i] - yFixShift));
            }
            else {
                vertices[i] = vertices[i] * increaseFactor;
            }
        }  */
        
        //SpaceShip lines fix:
        for (int i = 0; i < ssLoader.model.positions * 5; ++i) {
            if (i % 5 < 3) {
                vertices[i] *= increaseFactor;
            }
        }
        
        //Reset buffer data?
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
         //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        
       // glUniform1f(time3D, counter);
        glUniform1f(zoom3DLine, zoomFactor);
        glUniform1f(xCoord3DLine, xCoor);
        glUniform1f(yCoord3DLine, yCoor);
        glUniform1f(time3DLine, counter);
        
        //!!!!!!!!! THIS NEXT LINE BELOW WAS MY DRAW CALL
        //glDrawElements(GL_LINES, teapot_count / 3, GL_UNSIGNED_INT, 0);
        //glDisable(GL_TEXTURE_3D); //Turn off 3d texturing
        //glDrawElements(GL_LINES, 774/3, GL_UNSIGNED_INT, 0);
       // glDrawElements(GL_LINES, ssLoader.model.faces*3*2, GL_UNSIGNED_INT, 0);
        
        
        GLuint tempElementsCopy[ssLoader.model.faces*9];
        int elemtsPos = 0;
        for (int i = 0; i < ssLoader.model.faces*3*2; i+=6) {
            //if elements is:
            //   1   2    3
            //   4   5    6
            //we want the new elements to be
            //  1 2    2 3   3 1
            //  4 5    5 6   6 4
            tempElementsCopy[i] = elements[elemtsPos];
            tempElementsCopy[i+1] = elements[elemtsPos+1];
            tempElementsCopy[i+2] = elements[elemtsPos+1];
            tempElementsCopy[i+3] = elements[elemtsPos+2];
            tempElementsCopy[i+4] = elements[elemtsPos+2];
            tempElementsCopy[i+5] = elements[elemtsPos];
            elemtsPos += 3;
        }
        //Copy temp back into elements
        for (int i = 0; i < ssLoader.model.faces*3*2; i++) {
            elements[i] = tempElementsCopy[i];
        }
        
        //Since both the element array and the vertex array have been changed since last draw call, need to let openGL know to get the new buffer data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        
        
        glad_glEnable(GL_DEPTH_CLAMP); //Gets read of near/far plane culling (but not top/bottom plane, if this makes sense)
        glad_glEnable(GL_DITHER); //Does something
        glad_glEnable(GL_LINE_SMOOTH); //Makes the lines smooth
        glad_glEnable(GL_MULTISAMPLE); //Turns on additional anti-aliasing
        glDrawElements(GL_LINES, ssLoader.model.faces*3*2, GL_UNSIGNED_INT, 0); //Actual draw call to draw lines
        glad_glDisable(GL_DEPTH_CLAMP); //Turn off all the glEnable's I just turned on 
        glad_glDisable(GL_DITHER);
        glad_glDisable(GL_LINE_SMOOTH);
        glad_glDisable(GL_MULTISAMPLE);
        
        //Use engine shader
        mainEngineEffect->use();
        glUseProgram(mainEngineEffect->getID());
        glUniform1f(engineZoom, zoomFactor);
        glUniform1f(engineTime, counter);
        
        //Modify elements to just the vertex positions of the engines
        int elemCounter = 0;
        for (unsigned int i = 741u; i < 753u; i+=3u) {
            elements[elemCounter] = i;
            elements[elemCounter+1] = i+1u;
            elements[elemCounter+2] = 1+2u;
            elemCounter += 3;
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        
//        //Reset elements to how it was when the render loop first began
//        //Copy temp back into elements
//        for (int i = 0; i < ssLoader.model.faces*3*2; i++) {
//            elements[i] = tempElementsCopy[i];
//        }
        
        
        //After both passes have completed, flip buffer to the screen
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Reset depth/color buffer bits
        counter += 0.05;
    }   glfwTerminate();
    //stbi_image_free(image); //No longer need since i got a unique_ptr
    
    //Free Memory allocated for Rotation Quaternions:
    delete xRot; xRot = nullptr;
    delete yRot; yRot = nullptr;
    delete zRot; zRot = nullptr;
    
    delete mainEngineEffect; mainEngineEffect = nullptr;
    
    //End process
    return EXIT_SUCCESS;
}

void updateVertices(GLfloat* vertices, float t) {
    aiVector3D * vects = new aiVector3D[ssLoader.model.positions]; //model contains a count of types of components, so note that model.positions is a single variable representing the number of positions, while positions is an array of vertex data
    
    //Need to figure out the vector for 'forward'
    aiVector3D forward(0.0f, 0.0f, -1.0f);
    
//    aiVector3D right(1.0f, 0.0f, 0.0f);
    forward = xRot->computeRotation(forward);
    forward = yRot->computeRotation(forward);
    forward = zRot->computeRotation(forward);

//    right = xRot->computeRotation(right);
//    right = yRot->computeRotation(right);
//    right = zRot->computeRotation(right);
//
    
    
  //  forward.Normalize(); //Not sure if this returns normalized vector or actually normalizes the vector
    
    
    for (int i = 0; i < ssLoader.model.positions; i++) {
        vects[i].x = ssLoader.positions[i][0]; //"Positions" is the verte data in the model
        vects[i].y = ssLoader.positions[i][1];
        vects[i].z = ssLoader.positions[i][2];
        
//        xRot->changeRotationAxis(xRot->getRotationAxis() + aiVector3D(yChange * forward.x, yChange * forward.y, forward.z));
//        yRot->changeRotationAxis(yRot->getRotationAxis() + aiVector3D(yChange * forward.x, yChange * forward.y, forward.z));
//        zRot->changeRotationAxis(zRot->getRotationAxis() + aiVector3D(yChange * forward.x, yChange * forward.y, forward.z));
//
        vects[i] = xRot->computeRotation(vects[i]);
        vects[i] = yRot->computeRotation(vects[i]);
        vects[i] = zRot->computeRotation(vects[i]);
        
//        vects[i].x += yChange * forward.x;
//        vects[i].y += yChange * forward.y;
        
//        vects[i].x -= xChange * right.x;
//        vects[i].y -= xChange * right.y;
        
        vects[i].x += xChange;
        vects[i].y += yChange;
//
        
    }
    
    int vertexNum = 0;
    for (int i = 0; i < ssLoader.model.positions * 5; i+=5) {
        vertices[i] = vects[vertexNum].x;
        vertices[i+1] = vects[vertexNum].y;
        vertices[i+2] = vects[vertexNum].z;
        
        //Loading the cube caused a crash, so here I say "if faces == faces that the cube has, then don't crash" 
        if (ssLoader.model.faces == 12) {
            vertices[i+3] = 0.0f;
            vertices[i+4] = 0.0f;
        }
        else {
            vertices[i+3] = ssLoader.texels[0][0];
            vertices[i+4] = ssLoader.texels[0][1];
        }
        vertexNum++;
    }
   
    for (int i = 0; i < ssLoader.model.positions * 5; i++) {
        if (i % 5 < 3) { //This is more efficent than if statement below this one
        //if (i % 5 != 3 && i % 5 != 4) { //Shrink only xyz coords, not texture coords
            vertices[i] *= 1.0f/5.0f;
        }
    }
    
    /*
    //Figure out where the smallest (most negative) z values occur (i.e. get the coordinates of the back of the ship)
    int minZ = 100.0f;
    for (int i = 2; i < (ssLoader.model.positions * 5)-3 ; ++i) {
        if (vertices[i] < minZ) {
            minZ = vertices[i];
//            std::cout << "\nSmallest z vertices occur at index: " << i;
//            std::cout << "\nAnd the X Y Z values for this vertice are: ";
//            if (i > 3) {
//                std::cout << vertices[i-2] << ", " << vertices[i-1] << ", ";
//                std::cout << vertices[i] << std::endl;
            //}
        }
    } //Turns out they occur at [752] and [787] (those are positions of the z values)
    //The xyz coordinates are 0.0, -+0.33824, -2.20744
    //also [747] and [742]
    //also [737] and [732]
    //also [727] and [722]
    */
    
    delete [] vects;
}




//Old teapot stuff and what not:
//void updateVertices(GLfloat* vertices, float t)
//{
//    //    //2d:  //Note: This never worked correctly
////    for (int i = 0; i < 6; i++) {
////        int index2d = 2*teapot_count+18;
////        vertices[index2d++] = 0.8f; //1x
////        vertices[index2d++] = 0.8f; //1y
////        vertices[index2d++] = -0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = -0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = 0.8f; //2x?
////        vertices[index2d++] = -0.8f; //2y
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = -0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = -0.8f; //3x?
////        vertices[index2d++] = 0.8f; //3y?
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = -0.8f;
////        vertices[index2d++] = 0.8f;
////        vertices[index2d++] = -0.8f;
////        vertices[index2d++] = 0.8f;
////    }
//
//    //float temp = 0.3f + 0.25f * (float)sin(t);
//    float tempVertices[] = {
//        //        //  Position            Color                              Texcoords
//        //        -0.8f,  1.0f,       1.0f,   0.0f,   0.0f,               0.0f, 1.0f, // Top-left
//        //        0.8f,  1.0f,       0.0f,   1.0f,   0.0f,               0.0f, 0.0f, // Top-right
//        //        0.8f, -1.0f,       0.0f,   temp,   1.0f,               1.0f, 0.0f, // Bottom-right
//        //
//        //        //  -0.5f,  0.5f,       1.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
//        //        -0.8f, -1.0f,       1.0f,   1.0f,   1.0f,               1.0f, 1.0f,  // Bottom-left
//        //        //   0.5f, -0.5f,       0.0f,   temp,   1.0f,               1.0f, 1.0f // Bottom-right
//        //
//
//        //  Position            Color                              Texcoords
//        //        -0.25f,  0.25f,       1.0f,   0.0f,   0.0f,               0.0f, 1.0f, // Top-left
//        //        0.25f,  0.25f,       0.0f,   1.0f,   0.0f,               0.0f, 0.0f, // Top-right
//        //        0.25f, -0.25f,       0.0f,   temp,   1.0f,               1.0f, 0.0f, // Bottom-right
//        //
//        //
//        //        -0.25f, -0.25f,       1.0f,   1.0f,   1.0f,               1.0f, 1.0f,  // Bottom-left
//
//
//        //        //  Position            Color                              Texcoords
//        //        -915.0f/609.0f,  1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
//        //        915.0f/609.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-right
//        //        915.0f/609.0f, -1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 1.0f, // Bottom-right
//        //        -915.0f/609.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 1.0f,  // Bottom-left
//        //
//
//
//        //        //Make the triangles accomodate the skew?
//        //  Position            Color                              Texcoords
//        -1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Middle-left corner  //0
//        1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-Corner   //1
//        1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 1.0f, // Middle-Right corner  //2
//        -1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 1.0f,  // Bottom-Corner  //3
//
//
//
//
//        //Zoomed in on texture
//        //  Position            Color                              Texcoords
//        //        -1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
//        //        1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-right
//        //        1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.05f, // Bottom-right
//        //        -1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.05f,  // Bottom-left
//
//    };
////
////    //Some quaternion testing
//////    Quaternion* qPtr, *qPtr2;
//////    if ((int)t % 180 < 90) {
//////        qPtr = new Quaternion(0.3f, -0.3f, 1.0f, 3.141592f / 2.0f * t*t/10.0f);
//////    }
//////    else {
//////        qPtr = new Quaternion(-0.3f, 0.3f, 1.0f, 3.141592f / 2.0f * (t*t - (t-90.0f)*(t-90.0f))/10.0f);
//////    }
//////    qPtr2 = new Quaternion(0.5f, 0.5f, 0.33f, 3.14159f * t/ 32.0f);
//////    for (int i = 0; i < 7 * 4; i++) {
////////        if (i % 7 == 0 || i % 7 == 1) {
////////            vertices[i] = tempVertices[i] * 2;
////////        }
//////        if (i % 7 == 0) {
//////            aiVector3D tempV(tempVertices[i], tempVertices[i+1], 1.0);
//////            //aiVector3D tempV2(tempVertices[i], tempVertices[i+1], 1.0);
//////
//////            //Next line for debug:
//////            std::cout << "BEFORE QUAT: Tempv is  x: " << tempV.x << "    and y: " << tempV.y << std::endl;
//////            tempV = qPtr->computeRotation(tempV);
//////            //tempV = qPtr2->computeRotation(tempV);
//////            //Or try doing this (should result in same rotation):
//////            //qPtr->changeRotationAxis(0.0f, 0.0f, 1.0f );
//////           // *qPtr = (*qPtr) * (*qPtr2);
//////            //tempV = qPtr->computeRotation(tempV);
//////
//////            //Next line for debug:
//////            std::cout << "AFTER QUAT: Tempv is  x: " << tempV.x << "    and y: " << tempV.y << std::endl;// << std::endl;
//////            std::cout << "And t is: " << t << std::endl << std::endl;
//////            //std::cout << "AFTER QUAT: Tempv2 is  x: " << tempV.x << "    and y: " << tempV2.y << std::endl << std::endl;
//////            vertices[i] = tempV.x;
//////            vertices[i+1] = tempV.y;
//////        }
//////        else if (i % 7 == 1) {
//////            //do nothing
//////        }
//////        else {
//////            vertices[i] = tempVertices[i];
//////        }
//////    }
//////    //Delete the quaternions
//////    delete qPtr; qPtr = nullptr; delete qPtr2; qPtr2 = nullptr;
//////    //Set zeros in
//////    for (int i = 7*4; i < 7*4*2 ; i++) {
//////        vertices[i] = 0.0f; //Might as well set some things to zero past where array starts
//////    }
////
////
////    int j = 0; //J is just an index iterator for looping
////    float xMax, yMax;
////    xMax = yMax = 0.0f;
////    aiVector3D teapotVerts [teapot_count / 3];
////    for (int i = 0; i < teapot_count; i+=3) {
////        //teapotVerts[j] = aiVector3D(teapot[i], teapot[i+1], teapot[i+2]);
////        if (teapot[i] > xMax) {xMax = teapot[i];}
////        if (teapot[i+1] > yMax) {yMax = teapot[i+1];}
////        aiVector3D temp(teapot[i], teapot[i+1], teapot[i+2]);
////        temp = xRot->computeRotation(temp);
////        temp = yRot->computeRotation(temp);
////        temp = zRot->computeRotation(temp);
////        teapotVerts[j] = temp;
////        j+=1;
////    }
////
////    j = 0; //Reuse J, note that it just represents an index iterator for this next loop
////    //This next loop handles direct translation and also texture rotation maybe
////    for (int i = 0; i < teapot_count * 2.0f; i+=5) {
////        float zoomFact = 1.0f/2.50f;
////        //DO zooming and translation
////        vertices[i] = zoomFact * teapotVerts[j].x + xChange; //xChange is xDisplacement from user input
////        vertices[i+1] = -1.0f * zoomFact * teapotVerts[j].y + yChange; //yChange is set from user input
////        vertices[i+2] = zoomFact * teapotVerts[j].z;
////
////        vertices[i+3] =  -((teapotVerts[j].x / xMax) + 1.0f) / 2.0f; //I did some work on paper and this might work
////
////        //Do
////        //vertices[i+4] =  ((teapotVerts[j].y / yMax) + 1.0f) / 2.0f;
////        vertices[i+4] =  (abs((teapotVerts[j].y)) - 1.7f + 0.0f*sin(0.25f*t) ) / 7.0f;
////
//////        aiVector3D textureVector(  -((teapotVerts[j].x / xMax) + 1.0f) / 2.0f,
//////                              (abs((teapotVerts[j].y)) - 1.7f + 0.0f*sin(0.25f*t) ) / 7.0f,
//////                              0.1f );
//////
//////        textureVector = xRot->computeRotation(textureVector);
//////        textureVector = yRot->computeRotation(textureVector);
//////        textureVector = zRot->computeRotation(textureVector);
//////
//////        vertices[i+4] = textureVector.x;
//////        vertices[i+5] = textureVector.y;
////        //Old thing this loop did back when I did translation within the vert shader
//////        vertices[i+3] = zoomFact * teapotVerts[j].x;
//////        vertices[i+4] = 1.0f * zoomFact * teapotVerts[j].y;
////        //vertices[i+5] = zoomFact * teapotVerts[j].z;
////        j += 1;
////    }
////
////    //New and improved spaceship loading:
////
////
////    
//////    //Don't do the teapot but instead do the spaceship. Overwrite the vertices with these:
//////    int k = 0;
//////    for (int i = 0; i < (774 * 5) / 3; i+=5) {
//////        float zoomFact = 1.0f/12.0f;
//////
//////        //More complicated copying over, with rotations included
//////        aiVector3D temp(ssVerts[k], ssVerts[k+1], ssVerts[k+2]);
//////        k+=3;
//////        temp = xRot->computeRotation(temp);
//////        temp = yRot->computeRotation(temp);
//////        temp = zRot->computeRotation(temp);
//////
//////        vertices[i] = zoomFact * temp.x + xChange;
//////        vertices[i+1] = zoomFact * temp.y + yChange;
//////        vertices[i+2] = zoomFact * temp.z;
//////
////////        //Simple copying over of vertices:
////////        vertices[i] = ssVerts[k++] + xChange;
////////        vertices[i+1] = ssVerts[k++] + yChange;
////////        vertices[i+2] = ssVerts[k++];
//////    }
////    
////    //for (int i = 7*4*2; i < 7*4*2+teapot_count; i+=7) {
////        //aiVector3D temp(teapot[j], teapot[j+1], teapot[j+2]);
////        //j+=3;
////        //Quaternion rotation(0.0, 0.0, 1.0, 3.14159f/12.0f * t);
////
////        //temp = rotation.computeRotation(temp);
////
////        
////
//////        vertices[i] = temp.x / 10.0f;
//////        vertices[i+1] = temp.y / 10.0f;
//////        vertices[i+2] = temp.z / 10.0f;
//////        vertices[i+3] = 0.0f;
//////        vertices[i+4] = teapot[j];
//////        vertices[i+5] = teapot[j];
//////        vertices[i+6] = teapot[j-1];
////
////
////
////
////
//////        vertices[i] = teapot[j++];
//////        vertices[i+1] = teapot[j++];
//////        vertices[i+2] = teapot[j++];
//////        vertices[i+3] = 0.0f;
//////        vertices[i+4] = teapot[j];
//////        vertices[i+5] = teapot[j];
//////        vertices[i+6] = teapot[j-1];
//////
////        
//////        vertices[i] = teapot[j++];
//////        vertices[i+1] = teapot[j++];
//////        vertices[i+2] = teapot[j++];
//////        vertices[i+3] =teapot[j++];
//////        vertices[i+4] = teapot[j++];
//////        vertices[i+5] = teapot[j++];
//////        vertices[i+6] = teapot[j++];
//////
////
////   // }
////    //    for (int i=0; i<triangleCount; i++) {
////    //
////    //        int idx = i*floatsPerTriangle;
////    //
////    //
////    //        // Specify triangles with 3 pairs of X and Y coordinates
////    //        //float p = 1.0*float(i+t*0.005);
////    //
////    //        //float xCenter = 0.01*sqrt(p)*0.5*cos(p*0.2);
////    //
////    //        //float yCenter = 0.01*sqrt(p)*0.5*sin(p*0.2);
////    //
////    //        //My Code
////    //        float xCenter;
////    //
////    //        if (idx % 200 <= 45) {
////    //        //xCenter = 0.001 + sin(0.001*idx)*cos(sin(cos(idx+(idx/3))));
////    //        //xCenter = 0;
////    //            xCenter = (0.001)*idx * sin(idx);
////    //         }
////    //         else {
////    //        //     xCenter = -0.005 - 0.001*idx - 4*sin(0.025 * idx/t + t);
////    //            xCenter = (0.001)*idx * cos(idx);
////    //         }
////    //
////    //        float yCenter = -2.0 * 0.2*sin(t * 0.3 * xCenter);
////    //        ///if (idx % 5 == 0)
////    //        ///  yCenter = (0.001 * idx / t);
////    //        //else
////    //        //  yCenter = (-0.0001 * idx) / (t * idx);
////    //        //End of my code
////    //
////    //        vertices[idx+0] = xCenter; //X1
////    //        vertices[idx+1] = yCenter+5.0*triangleSize; // Y1
////    //
////    //        vertices[idx+2] = xCenter-5.0*triangleSize; // X2
////    //        vertices[idx+3] = yCenter; // Y2
////    //
////    //        vertices[idx+4] = xCenter+5.0*triangleSize; // X3
////    //        vertices[idx+5] = yCenter; // Y3
////    //    }
////}

std::string loadSource(char* filename)
{
    std::ifstream infile{ filename };
    return{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
}
