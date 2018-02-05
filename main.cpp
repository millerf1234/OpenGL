//// Local Headers
#include "ProjectSetup.h"
#include "Quaternion.hpp"
#include "teapotVerts.h"

// System Headers
//#include "glad.h"
//#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

//#include "SpaceShipVertices.h"
#include "SimpleObjLoader.h"
#include "ShaderWrapper1.h"

constexpr int ENGINE_FLAME_TRANSLATION_DELAY_FRAMES = 150;
constexpr float ZOOMSPEED = 0.015f;
constexpr float MOVEMENTX = 0.035;
constexpr float MOVEMENTY = 0.035f;
const GLfloat ROTATIONSPEED = 0.024;

std::string loadSource(char* filename);
void updateVertices(GLfloat* vertices, float t);
//void makeEngineFlameOnSpaceShip(GLfloat * engineVerts);

//Global variables:
Quaternion *xRot, *yRot, *zRot;
ShaderWrapper* mainEngineEffect; //First attempt as using my shader-compilation class
ShaderWrapper* sideEngineEffect; //Second attempt at using my working first attempt of shader-compilation class

float xChange = 0.0f; //Used for translating the vertices before they reach the GPU
float yChange = 0.0f;

aiVector3D rear(0.0f, 0.0f, -2.94231f);

float deleteMeLater__MIN = 0.0f;

aiVector3D * translationHistory; //Used for engine flames

//SimpleObjLoader ssLoader("space_ship2.obj");
SimpleObjLoader ssLoader("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Objects/space_ship2.obj");

int main(int argc, char * argv[]) {
    
    bool movementOccuredThisIteration = false;
    double tildeKeyPressCounter = 0.0;
    mainEngineEffect = new ShaderWrapper;
    sideEngineEffect = new ShaderWrapper;
    
    translationHistory = new aiVector3D[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES];
    //Initialize all vectors in translationHistory to be the 0 vector
    for (int i = 0; i < ENGINE_FLAME_TRANSLATION_DELAY_FRAMES; ++i) {
        translationHistory[i] = aiVector3D(0.0f, 0.0f, -0.5f);
    }
    
    //Make the rotation quaternions
    xRot = new Quaternion(1.0f, 0.0f, 0.0f, 0.0f); //rotation about x axis
    yRot = new Quaternion(0.0f, 1.0f, 0.0f, 0.0f); //rotation about y axis
    zRot = new Quaternion(0.0f, 0.0f, 1.0f, 0.0f); //rotation about z axis
    
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  //version 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //version x.1  (these 2 lines mean version 4.1)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE); //NOT SURE IF THIS IS NEEDED
    glfwWindowHint(GL_SAMPLES, 32);
    //glfwWindowHint(GLFW_FSAA_SAMPLES, 4); //Switched to GL_SAMPLES in GLFW3 and newer
    //For VSYNC, read: http://www.glfw.org/docs/3.0/group__context.html#ga6d4e0cdf151b5e579bd67f13202994ed
    //Function for vsync is:  void glfwSwapInterval    (    int     interval    )
    //However, should read about VSYNC before implementing to make sure it is done correctly
    
    std::cout << glfwGetVersionString();
    
    //Open not fullscreen (aka 'windowed'):
    // auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr); //Open as window
    
    //Open fullscreen on monitor 2 if a second monitor is connected, or on this monitor
    //if no second monitor is detected.
    int displayCount;
    GLFWmonitor** monitors = glfwGetMonitors(&displayCount);
    GLFWwindow * mWindow;
    std::cout << "\nNumber of connected displays detected: " << displayCount << std::endl;
    if (displayCount == 2) {
        mWindow = glfwCreateWindow(1920, 1080, "OpenGL", monitors[1], nullptr );
    }
    else {
        mWindow = glfwCreateWindow(2650, 1600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Open fullscreen (alt+tab doesn't seem to work)
    }
    
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
    
    // Load shaders
    std::string vert = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/engineEffect.vert"));
    std::string vert3D = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic3D.vert"));
    std::string frag = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/engineEffect.frag"));
    //    std::string frag3D = loadSource((char*) ("/Users/forrestmiller/Desktop/xcode_test_projects/OpenGLSetupTestWithSTBWorking/Shaders/basic3D.frag"));
    std::string frag3D = loadSource((char*) ("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/basic3D.frag"));
    std::string line3D = loadSource((char*) "/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/line3D.frag" );
    
    const GLchar* vertexShaderSource = (const GLchar *)vert.c_str();
    const GLchar* fragmentShaderSource = (const GLchar *)frag.c_str();
    
    const GLchar* vertex3DShaderSource = (const GLchar *)vert3D.c_str();
    const GLchar* fragment3DShaderSource = (const GLchar *)frag3D.c_str();
    
    const GLchar* fragment3DLineShaderSource = (const GLchar *)line3D.c_str();
    
    // Build and compile our shader program
    // Vertex shaders
    GLuint vertexShader3D = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader3D, 1, &vertex3DShaderSource, NULL);
    
    glCompileShader(vertexShader3D);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    //Check 3D shader as well for errors:
    glGetShaderiv(vertexShader3D, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX3D::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment shaders
    GLuint fragmentShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint lineShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3D, 1, &fragment3DShaderSource, NULL);
    glShaderSource(lineShader3D, 1, &fragment3DLineShaderSource, NULL);
    glCompileShader(fragmentShader3D);
    glCompileShader(lineShader3D);
    
    // Check for compile time errors
    glGetShaderiv(fragmentShader3D, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT3D::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Check the 3D line shader
    glGetShaderiv(lineShader3D, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(lineShader3D, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::3DLINEFRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Link shaders //
    GLuint shaderProgram3D = glCreateProgram();
    GLuint shaderProgram3DLine = glCreateProgram();
    
    glAttachShader(shaderProgram3D, vertexShader3D);
    glAttachShader(shaderProgram3D, fragmentShader3D);
    
    glAttachShader(shaderProgram3DLine, vertexShader3D);
    glAttachShader(shaderProgram3DLine, lineShader3D);
    
    //Check for 3D shader program link errors
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
    
    glDeleteShader(vertexShader3D);
    glDeleteShader(fragmentShader3D);
    glDeleteShader(lineShader3D);
    
    glUseProgram(shaderProgram3D); //Start with using the 2D shader, will switch to 3D later on
    
    /* from: https://www.opengl.org/discussion_boards/showthread.php/185119-Understanding-VAO-s-VBO-s-and-drawing-two-objects
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
     glUseProgram(0); */
    
    
    
    //Ye OLDE WAY:
    //    // Create Vertex Array Object
    //    GLuint vao;  //A VAO is a storage container for VBOs. See: https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
    //    glGenVertexArrays(1, &vao);
    //    glBindVertexArray(vao);
    
    //The new way:
    GLuint spaceshipVAO; //Have a VAO for the spaceship
    glGenVertexArrays(1, &spaceshipVAO);
    glBindVertexArray(spaceshipVAO);
    
    GLfloat vertices[6*7*2 + teapot_count * 3];
    GLfloat vertsPlain[173 * 3];
    
    updateVertices(vertices, 0);
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //--------------------------------------------------------------------------
    //Create an element buffer for element array and stuff
    // Create an element array
    GLuint ebo; //element buffer element object
    glGenBuffers(1, &ebo);
    
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
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
    
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    // Attempt at using stb to load an image (jpg):
    // See: https://books.google.com/books?id=F0IwDwAAQBAJ&pg=PA521&lpg=PA521&dq=stbi_load+return&source=bl&ots=r0lFnzFQ1p&sig=ZbB9LW2xJXXiAhqIP4HXpZwKLsM&hl=en&sa=X&ved=0ahUKEwj4o4q6u87YAhURwmMKHS3EBOUQ6AEIVzAH#v=onepage&q=stbi_load%20return&f=false
    
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
    image_data.resize(dataSize);
    std::memcpy(image_data.data(), stbi_data.get(), dataSize);
    //--------------------------------------------------------------------------
    
    
    
    //--------------------------------------------------------------------------
    // Texture attempt code, from: https://open.gl/textures
    // Create tex object?
    GLuint tex [1];
    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    
    //                    WRAPPING
    //Texture coordinates are addressed from 0 to 1 starting from bottom left corner
    // NOTE: Instead of i,j,k or x,y,z, textures use s,t,r
    // so we Need to tell OpenGL how to handle texture coordinates that at < 0 or > 1
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    //                   FILTERING
    // Note that GL_TEXTURE_MIN_FILTER is for scaling the texture down and GL_TEXTURE_MAG_FILTER is for scaling up
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    //     Can also use mipmaps (but only after the texture has been loaded) by doing:
    glGenerateMipmap(GL_TEXTURE_2D);
    
    //GL_TEXTURE_MAX_ANISOTROPY_EXT = GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT; //see: https://www.khronos.org/opengl/wiki/Sampler_Object#Sampling_parameters
    
    //    //Swap two colors in texture (SWIZZLE):
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_GREEN); //Swaps Green in for red
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED); //Swaps red in for green
    
    
    //                  LOADING TEXTURE
    std::vector<unsigned char>::iterator it;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Here is the code that fixes a .jpg that is 915x609 pixels
    if (w == 915 && h == 609) {
        int lineFixerCounter = 3;
        for (int i = 0 ; i < w*h * 3 ; ++i) {
            if ((i+1) == (w-1)*3) {
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\xfe');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');
                image_data.insert(it, *(it+3));
                it = image_data.begin() + i+7;
                //image_data.insert(it, '\0');  //??
                image_data.insert(it, *(it+3));
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
        }
    }
    //Give the texture to the GPU
    if(components == 3) {
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2745/3 - 3 , h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h-1, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
        //NOTE!!! Doing w-1 shouldn't cause the RGB values to shift...
        //glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, w, h-1, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
    }
    else if(components == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
    }
    //
    
    //stbi_image_free(image);
    
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    // NOTE on glTexImage2D: The first parameter after the texture target is the level-of-detail, where 0 is the base image. This parameter can be used to load your own mipmap images. The second parameter specifies the internal pixel format, the format in which pixels should be stored on the graphics card. Many different formats are available, including compressed formats, so it's certainly worth taking a look at all of the options. The third and fourth parameters specify the width and height of the image. The fifth parameter should always have a value of 0 per the specification. The next two parameter describe the format of the pixels in the array that will be loaded and the final parameter specifies the array itself. The function begins loading the image at coordinate (0,0), so pay attention to this.
    
    
    
    //--------------------------------------------------------------------------
    
    
    
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    //
    GLint posAttrib3D = glGetAttribLocation(shaderProgram3D, "position3D");
    GLint texAttrib3D = glGetAttribLocation(shaderProgram3D, "texCoord3D");
    
    GLint posAttrib3DLine = glGetAttribLocation(shaderProgram3DLine, "position3D");
    GLint texAttrib3DLine = glGetAttribLocation(shaderProgram3DLine, "texCoord3D");
    
    
    glEnableVertexAttribArray(posAttrib3D);
    glEnableVertexAttribArray(texAttrib3D);
    glVertexAttribPointer(posAttrib3D, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //To this line (see above)
    glVertexAttribPointer(texAttrib3D, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    
    
    glEnableVertexAttribArray(posAttrib3DLine);
    glVertexAttribPointer(posAttrib3DLine, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(texAttrib3DLine);
    glVertexAttribPointer(texAttrib3DLine, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    
    
    glBindVertexArray(0);
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    //   //Try  out my new class for shader creation:
    mainEngineEffect->attachVert("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/engineEffect.vert");
    mainEngineEffect->attachFrag("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/engineEffect.frag");
    
    mainEngineEffect->link();
    mainEngineEffect->setVertexAttribName("enginePos");
    //mainEngineEffect->setTextureAttribName("engineTxtr");
    //mainEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3TEXEL2); //Can I bring in class enums like a namespace?
    
    mainEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3, vbo, ebo); //vbo doesn't have to be the same vbo as used for other draw calls
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
    
    GLint engineZoom = glGetUniformLocation(mainEngineEffect->getID(), "zoom");
    GLint engineTime = glGetUniformLocation(mainEngineEffect->getID(), "time");
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    // SIDE ENGINE SHADER!
    sideEngineEffect->attachVert("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/engineEffect.vert");
    sideEngineEffect->attachFrag("/Users/forrestmiller/Desktop/xcode_test_projects/SpaceShipGame3D/Shaders/sideEngineEffect.frag");
    
    sideEngineEffect->link();
    sideEngineEffect->setVertexAttribName("enginePos");
    //sideEngineEffect->setTextureAttribName("engineTxtr");
    
    
    sideEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3, vbo, ebo); //vbo doesn't have to be the same vbo as used for other draw calls
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
    
    //    GLint engineZoom = glGetUniformLocation(mainEngineEffect->getID(), "zoom");
    //    GLint engineTime = glGetUniformLocation(mainEngineEffect->getID(), "time");
    //GLint engineZoom;
    // GLint engineTime;
    
    
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    glBindVertexArray(0);
    
    //UNIFORMS!!!!!
    GLint texEnum3D = glGetUniformLocation(shaderProgram3D, "tex3D");
    GLint time3D = glGetUniformLocation(shaderProgram3D, "time3D");
    GLint time3DLine = glGetUniformLocation(shaderProgram3DLine, "time3D");
    GLfloat xCoor3D = glGetUniformLocation(shaderProgram3D, "xCoor3D");
    GLfloat yCoor3D = glGetUniformLocation(shaderProgram3D, "yCoor3D");
    
    //glad_glVertex3f mainEngPos = glGetUniformLoation(shaderProgram3D, "mainEnginePos");
    //Turns out i am not sure how to pass in a vector as a uniform, so here:
    GLfloat mainEngX = glGetUniformLocation(shaderProgram3D, "mainEngPosX");
    GLfloat mainEngY = glGetUniformLocation(shaderProgram3D, "mainEngPosY");
    GLfloat mainEngZ = glGetUniformLocation(shaderProgram3D, "mainEngPosZ");
    
    GLfloat xCoord3DLine = glGetUniformLocation(shaderProgram3DLine, "xCoor3D");
    GLfloat yCoord3DLine = glGetUniformLocation(shaderProgram3DLine, "yCoor3D");
    
    GLfloat zoom3D = glGetUniformLocation(shaderProgram3D, "zoom3D"); //For w-axis
    GLfloat zoom3DLine = glGetUniformLocation(shaderProgram3DLine, "zoom3D");
    
    GLfloat thta = 0.0f;
    GLfloat zoomFactor = 5.0f;
    GLfloat xPos = 0.0f;
    GLfloat yPos = 0.0f;
    
    bool drawLines = true;
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        movementOccuredThisIteration = false; //For engine effect translation history thing
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
        
        if (glfwGetKey(mWindow, 'Z') == GLFW_PRESS) {
            zoomFactor += ZOOMSPEED;
        }
        if (glfwGetKey(mWindow, 'X') == GLFW_PRESS) {
            zoomFactor -= ZOOMSPEED;
        }
        if (glfwGetKey(mWindow, 'W') == GLFW_PRESS) {
            yPos -= MOVEMENTY;
            yChange -= MOVEMENTY;
            movementOccuredThisIteration = true;
            //translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].y -= MOVEMENTY;
            translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].y = yChange;
        }
        if (glfwGetKey(mWindow, 'S') == GLFW_PRESS) {
            yPos += MOVEMENTY;
            yChange += MOVEMENTY;
            movementOccuredThisIteration = true;
            //translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].y += MOVEMENTY;
            translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].y = yChange;
        }
        if (glfwGetKey(mWindow, 'A') == GLFW_PRESS) {
            xPos += MOVEMENTX;
            xChange += MOVEMENTX;
            movementOccuredThisIteration = true;
            //translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].x -= MOVEMENTX;
            translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].x = xChange;
        }
        if (glfwGetKey(mWindow, 'D') == GLFW_PRESS) {
            xPos -= MOVEMENTX;
            xChange -= MOVEMENTX;
            movementOccuredThisIteration = true;
            //translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].x += MOVEMENTX;
            translationHistory[ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1].x = xChange;
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
        
        if ((glfwGetKey(mWindow, '`') == GLFW_PRESS)) {
            if (tildeKeyPressCounter < counter - (0.025 * 15)) { //Only switch after a small delay (delay based off counter)
                tildeKeyPressCounter = counter;
                drawLines = !drawLines;
            }
        }
        if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
            thta = 0.0f;
            zoomFactor = 5.0f;
            xPos = 0.0f;
            yPos = 0.0f;
            xRot->changeTheta(0.0f);
            yRot->changeTheta(0.0f);
            zRot->changeTheta(0.0f);
            xChange = 0.0f;
            yChange = 0.0f;
            //Reset translationHistory array
            for (int i = 0; i < ENGINE_FLAME_TRANSLATION_DELAY_FRAMES; ++i) {
                translationHistory[i].x = 0.0f;
                translationHistory[i].y = 0.0f;
                translationHistory[i].z = -0.5f;
            }
            movementOccuredThisIteration = false; //even if it did, everything is getting reset so pretend it never happened
        }
        
        
        // Background Fill Color
        //glClearColor(sin(counter), 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Send "counter" value into shader via as uniform
        //  glUniform1f(time, counter);
        //glUniform1i(texEnum, *tex);
        //     glUniform1i(texEnum, tex[0]);
        // glUniform1i(texEnum3D, tex[1]);
        //     glUniform1f(theta, thta);
        //        glUniform1f(zoom, zoomFactor);
        //glUniform1f(zoom3D, zoomFactor);
        //   glUniform1f(xCoor, xPos);
        //glUniform1f(xCoor3D, xPos);
        //        glUniform1f(xCoord3DLine, xPos);
        //   glUniform1f(yCoor, yPos);
        //glUniform1f(yCoor3D, yPos);
        //glUniform1f(yCoord3DLine, yPos);
        
        //Depth test code (I also need to reset it each frame below the draw call):
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        
        
        // Draw Stuff
        glUseProgram(shaderProgram3D); //Use the 3D shader program
        updateVertices(vertices, counter);
        glBindVertexArray(spaceshipVAO);
        //glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        //        glEnableVertexAttribArray(posAttrib3D);
        //        glEnableVertexAttribArray(texAttrib3D);
        //        glVertexAttribPointer(posAttrib3D, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //To this line (see above)
        //        glVertexAttribPointer(texAttrib3D, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        
        //Handle engine translation by shifting all array contents one to the right
        for (int i = 0; i < ENGINE_FLAME_TRANSLATION_DELAY_FRAMES - 1; ++i) {
            translationHistory[i] = translationHistory[i+1];
            //std::cout << "\nPOS " << i+1 << " Vector is: " << translationHistory[i+1].x << " " << translationHistory[i+1].y << " " << translationHistory[i+1].z;
        } //Shouldn't need to update last slot if movement never occured because its value will be the same
        //std::cout << std::endl;
        
        tex[0] = 0; //A-HA! This fixed it for some reason
        //tex[0] = (tex[0] + 1) % 2; //What? DOn;t rememebr why I wrote this
        
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex[0], 0);
        
        //StreamDraw means going to update once, use once, and discard
        //DynamicDraw means going to set once, then use a bunch with potential modifications
        
        //Reset elements
        for (int i = 0; i < ssLoader.model.faces * 3; ++i) {
            elements[i] = elementsCopyOrignl[i];
        }
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //Set the element array buffer to elements
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);  //I added this one
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);  //I added this one
        
        //Need to redo Uniforms upon switching programs (or at least I think this needs to happen) (try uncommenting to see)
        glUniform1f(time3D, counter);
        glUniform1f(zoom3D, zoomFactor);
        glUniform1f(xCoor3D, xPos);
        glUniform1f(yCoor3D, yPos);
        
        //        GLfloat mainEngX = glGetUniformLocation(shaderProgram3D, "mainEngPosX");
        //        GLfloat mainEngY = glGetUniformLocation(shaderProgram3D, "mainEngPosY");
        //        GLfloat mainEngZ = glGetUniformLocation(shaderProgram3D, "mainEngPosZ");
        glUniform1f(mainEngX, rear.x);
        glUniform1f(mainEngY, rear.y);
        glUniform1f(mainEngZ, rear.z);
        
        glDrawElements(GL_TRIANGLES, ssLoader.model.faces*3*2, GL_UNSIGNED_INT, 0);
        
        // glDisableVertexAttribArray(posAttrib3D);
        // glDisableVertexAttribArray(texAttrib3D);
        
        
        // So at this point, pass 1 has been completed and written to the framebuffer
        // So we switch the active program, set the uniforms required for it, and then drawElements for pass 2
        glUseProgram(shaderProgram3DLine); //Use the line shader program
        
        //Turns out I might not need this, since I made the lines draw thicker
        //        float increaseFactor = 1.000f; //1.005f
        //        //SpaceShip lines fix:
        //        for (int i = 0; i < ssLoader.model.positions * 5; ++i) {
        //            if (i % 5 < 3) {
        //                vertices[i] *= increaseFactor;
        //            }
        //        }
        //Set uniforms
        glUniform1f(zoom3DLine, zoomFactor);
        glUniform1f(xCoord3DLine, xPos);
        glUniform1f(yCoord3DLine, yPos);
        glUniform1f(time3DLine, counter);
        
        
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
        //int test;
        for (int i = 0; i < ssLoader.model.faces*3*2; i++) {
            elements[i] = tempElementsCopy[i];
            //            test = elements[i];
            //            std::cout << "\n   " << test;
        }
        
        //For 3D line shader: (turning this back on seems to make the lines draw?)
        //        if (drawLines) {
        //            glEnableVertexAttribArray(posAttrib3DLine);
        //            glVertexAttribPointer(posAttrib3DLine, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); //To this line (see above)
        //            glEnableVertexAttribArray(texAttrib3DLine);
        //            glVertexAttribPointer(texAttrib3DLine, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        //        }
        
        //Since both the element array and the vertex array have been changed since last draw call, need to let openGL know to get the new buffer data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        
        glad_glEnable(GL_DEPTH_CLAMP); //Gets read of near/far plane culling (but not top/bottom plane, if this makes sense)
        glad_glEnable(GL_DITHER); //Does something
        glad_glEnable(GL_LINE_SMOOTH); //Makes the lines smooth
        glad_glEnable(GL_MULTISAMPLE); //Turns on additional anti-aliasing
        if (drawLines) {
            glDrawElements(GL_LINES, ssLoader.model.faces*3*2, GL_UNSIGNED_INT, 0); //Actual draw call to draw lines
        }
        //        glad_glDisable(GL_DEPTH_CLAMP); //Turn off all the glEnable's I just turned on
        //        glad_glDisable(GL_DITHER);
        //        glad_glDisable(GL_LINE_SMOOTH);
        //        glad_glDisable(GL_MULTISAMPLE);
        
        
        glBindVertexArray(0);
        // glDisableVertexAttribArray(posAttrib3DLine);
        // glDisableVertexAttribArray(texAttrib3DLine);
        
        //(try to) Use engine shader
        
        // mainEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3);
        //        mainEngineEffect->specifyVertexLayout(ShaderWrapper::VERT3TEXEL2, vbo);
        
        
        //Draw the main engine flame
        mainEngineEffect->use();
        //glUseProgram(mainEngineEffect->getID());
        engineZoom = glGetUniformLocation(mainEngineEffect->getID(), "zoom");
        engineTime = glGetUniformLocation(mainEngineEffect->getID(), "time");
        glUniform1f(engineZoom, zoomFactor);
        glUniform1f(engineTime, counter);
        
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_DYNAMIC_DRAW);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        
        //Copy over vertices into a new array that I can change values in
        int vertCounter = 0;
        for (int i = 0; i < ssLoader.model.vertices; ++i) {
            vertsPlain[i] = vertices[vertCounter];
            ++vertCounter;
            if (i % 3 == 2) {
                //                if (vertsPlain[i] <= deleteMeLater__MIN) {
                //                  deleteMeLater__MIN = vertsPlain[i];
                //                }
                //                if (vertsPlain[i] <= deleteMeLater__MIN + 0.25f && vertsPlain[i-2] < -0.7f ) {
                //                   std::cout << "\nMin Z is: " << deleteMeLater__MIN << " and 'i' is " << i;
                //                   std::cout << "\nAlso, x at i=" << i << " is " << vertsPlain[i-2] << std::endl;
                //                }
                vertCounter += 2; //Skip over the texture coordinates in the data in vertices
            }
        }
        rear.x = 0.0f; //Back of engine is -2.34201
        rear.y = 0.0f;
        rear.z = -2.94231f;
        rear += translationHistory[0];
        rear = xRot->computeRotation(rear);
        rear = yRot->computeRotation(rear);
        rear = zRot->computeRotation(rear);
        
        vertsPlain[0] = rear.x;
        vertsPlain[1] = rear.y;
        vertsPlain[2] = rear.z;
        
        
        //Layout of main engine vertices:
        //             138   136   52
        //               _----------_
        //        140 _-˜            ˜-_ 54
        //           /                  \
        //      142 /                    \ 56
        //         |                      |
        //         |                      | 57
        //      143 \                    /
        //           \                  /
        //        141 ˜-_            _-˜  55
        //               ˜----------˜
        //           139      137    53
        //
        
        
        
        elements[0] = 0;
        elements[1] = 52;
        elements[2] = 54;
        elements[3] = 56;
        elements[4] = 57;
        elements[5] = 55;
        elements[6] = 53;
        
        elements[7] = 137;   //
        elements[8] = 139;
        elements[9] = 141;
        elements[10] = 143;
        elements[11] = 142;
        elements[12] = 140;
        elements[13] = 138;
        elements[14] = 136;
        elements[15] = 52;
        /*
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertsPlain), vertsPlain, GL_STREAM_DRAW);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
         
         
         glDrawElements(GL_LINES, ssLoader.model.faces*3, GL_UNSIGNED_INT, 0);
         */
        //        for (unsigned int i = 0u; i < 100u; ++i) {
        //            elements[i] = i;
        //            vertsPlain[i] = -2.34201f;
        //        }
        
        
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertsPlain), vertsPlain, GL_STREAM_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertsPlain), vertsPlain, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
        
        //        glad_glEnable(GL_DEPTH_CLAMP);
        //        glad_glEnable(GL_DITHER); //Does something
        //        glad_glEnable(GL_LINE_SMOOTH); //Makes the lines smooth
        //        glad_glEnable(GL_MULTISAMPLE); //Turns on additional anti-aliasing
        glDrawElements(GL_TRIANGLE_FAN, 16, GL_UNSIGNED_INT, 0);
        //        glad_glDisable(GL_DITHER); //Does something
        //        glad_glDisable(GL_LINE_SMOOTH); //Makes the lines smooth
        //        glad_glDisable(GL_MULTISAMPLE); //Turns on additional anti-aliasing
        //        glad_glDisable(GL_DEPTH_CLAMP);
        
        
        
        //Side ENGINE EFFECT
        sideEngineEffect->use();
        engineZoom = glGetUniformLocation(sideEngineEffect->getID(), "zoom");
        engineTime = glGetUniformLocation(sideEngineEffect->getID(), "time");
        
        glUniform1f(engineZoom, zoomFactor);
        glUniform1f(engineTime, counter);
        
        
        //!!!    This is for the right engine     !!!!
        //reuse rear
        rear.x = 2.00213f; //From model data
        rear.y = 0.0f;
        rear.z = -2.21f; //-2.19111 is the most read side engine z component
        rear += translationHistory[0];
        rear = xRot->computeRotation(rear);
        rear = yRot->computeRotation(rear);
        rear = zRot->computeRotation(rear);
        //set center of cone to rotated values of rear
        vertsPlain[0] = rear.x;
        vertsPlain[1] = rear.y;
        vertsPlain[2] = rear.z;
        
        //  Right engine element location
        //
        //               50
        //               /\_
        //              /   \_
        //          51 /  0   \ 49
        //             \     _/
        //              \  _/
        //               \/
        //               48
        
        elements[0] = 0;
        elements[1] = 51;
        elements[2] = 50;
        elements[3] = 49;
        elements[4] = 48;
        elements[5] = 51;
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertsPlain), vertsPlain, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
        
        glDrawElements(GL_TRIANGLE_FAN, 5+1, GL_UNSIGNED_INT, 0);
        
        
        //!!!    This is for the left engine     !!!!
        //reuse rear
        rear.x = -2.00213f; //I got coordinates from model data
        rear.y = 0.0f;
        rear.z = -2.21f; //-2.19111 is the most read side engine z component
        rear += translationHistory[0];
        rear = xRot->computeRotation(rear);
        rear = yRot->computeRotation(rear);
        rear = zRot->computeRotation(rear);
        //set center of cone to rotated values of rear
        vertsPlain[0] = rear.x;
        vertsPlain[1] = rear.y;
        vertsPlain[2] = rear.z;
        
        // Left side element locations ()...
        //
        //              132
        //     131           129
        //              130
        //
        //GLuint temp = (GLuint) floor(counter * 3.0f);
        
        elements[0] = 0;
        elements[1] = 130u;
        elements[2] = 131u;
        elements[3] = 132u;
        elements[4] = 133u;
        elements[5] = 130u;
        //elements[3] = 131u;
        //elements[4] = 132;
        //elements[5] = 133;
        //elements[6] = 134;
        
        
        //std::cout << "\ntemp is " << temp << " and temp+1 is " << temp+1u;
        //std::cout << std::endl;
        
        
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertsPlain), vertsPlain, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);
        
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
        glad_glDisable(GL_DITHER); //Does something
        glad_glDisable(GL_LINE_SMOOTH); //Makes the lines smooth
        glad_glDisable(GL_MULTISAMPLE); //Turns on additional anti-aliasing (though I don't really see a difference)
        glad_glDisable(GL_DEPTH_CLAMP);
        
        glBindVertexArray(0); //Vertex attribute array
        glUseProgram(0);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Reset depth/color buffer bits
        counter += 0.05;
        
    }   glfwTerminate();
    
    //Free Memory allocated for Rotation Quaternions:
    delete xRot; xRot = nullptr;
    delete yRot; yRot = nullptr;
    delete zRot; zRot = nullptr;
    //Free other dynamic memory
    delete mainEngineEffect; mainEngineEffect = nullptr;
    delete sideEngineEffect; sideEngineEffect = nullptr;
    delete [] translationHistory; translationHistory = nullptr;
    
    //End process
    return EXIT_SUCCESS;
}

//void makeEngineFlameOnSpaceShip(GLfloat * engineVerts) {
//    //Assuming that first 100 values have been set to -2.34201f
//
//}

void updateVertices(GLfloat* vertices, float t) {
    aiVector3D * vects = new aiVector3D[ssLoader.model.positions]; //model contains a count of types of components, so note that model.positions is a single variable representing the number of positions, while positions is an array of vertex data
    
    //Model loads with ship facing -Z direction, so forward is -z
    aiVector3D forward(0.0f, 0.0f, -1.0f);
    
    forward = xRot->computeRotation(forward);
    forward = yRot->computeRotation(forward);
    forward = zRot->computeRotation(forward);
    
    for (int i = 0; i < ssLoader.model.positions; i++) {
        //
        vects[i].x = ssLoader.positions[i][0]; //"Positions" is the vertex data from the model
        vects[i].y = ssLoader.positions[i][1];
        vects[i].z = ssLoader.positions[i][2];
        
        vects[i] = xRot->computeRotation(vects[i]);
        vects[i] = yRot->computeRotation(vects[i]);
        vects[i] = zRot->computeRotation(vects[i]);
        
        vects[i].x += xChange;
        vects[i].y += yChange;
    }
    
    int vertexNum = 0;
    for (int i = 0; i < ssLoader.model.positions * 5; i+=5) {
        vertices[i] = vects[vertexNum].x;
        vertices[i+1] = vects[vertexNum].y;
        vertices[i+2] = vects[vertexNum].z;
        //Loading the cube caused a crash, so here I say "if faces == '# of faces that the cube has', then don't crash by accessing ssLoader.texels[0][arrayIndexThatDoesNotExist]"
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
    //Note that I now just shrink the entire model within the vert shader
    //    for (int i = 0; i < ssLoader.model.positions * 5; i++) {
    //        if (i % 5 < 3) { //Shrink only xyz coords, not texture coords
    //            vertices[i] *= 1.0f/5.0f;
    //        }
    //    }
    delete [] vects;
}

std::string loadSource(char* filename) {
    std::ifstream infile{ filename };
    return{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
}



