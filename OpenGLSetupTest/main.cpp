//// Local Headers
#include "ProjectSetup.h"

// System Headers
//#include "glad.h"
//#include <GLFW/glfw3.h>


// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
//#include <iostream>

//#include "objOutputHolder_hpp"

constexpr int triangleCount = 200000;
constexpr int floatsPerTriangle = 6;
constexpr float triangleSize = 0.008;

const GLfloat ROTATIONSPEED = 0.008;

std::string loadSource(char* filename);
void updateVertices(GLfloat* vertices, float t);

int main(int argc, char * argv[]) {
    
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    glfwWindowHint(GL_SAMPLES, 0);
    //glfwWindowHint(GLFW_FSAA_SAMPLES,0); //Doesn't work
    
    
    //auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr); //Open as window
    //auto mWindow = glfwCreateWindow(2650, 1600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Open fullscreen (alt+tab doesn't seem to work)
    
    //Open on secondary monitor:
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        auto mWindow = glfwCreateWindow(1920, 1080, "OpenGL", monitors[1], nullptr );
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
    
    // Load Shaders
    
    // Load shaders
    //std::string vert = loadSource((char*)("../../../basic.vert"));
    //std::string frag = loadSource((char*)("../../../basic.frag"));
    std::string vert = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/basic.vert"));
    std::string frag = loadSource((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/basic.frag"));
    
    //std::printf("source: %s\n", vert.c_str());
    const GLchar* vertexShaderSource = (const GLchar *)vert.c_str();
    const GLchar* fragmentShaderSource = (const GLchar *)frag.c_str();
    
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders //
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "colr"); //I added this line
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    glUseProgram(shaderProgram);
    
    //             //
    
    // Create Vertex Array Object
    GLuint vao;  //A VAO is a storage container for VBOs. See: https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLfloat vertices[6*7*2];
    
    updateVertices(vertices, 0);
    
    
    
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //--------------------------------------------------------------------------
    //Create an element for element array and stuff
    // Create an element array
    GLuint ebo; //element buffer element object
    glGenBuffers(1, &ebo);
    
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
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
    
    //unsigned char* image = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/boki.jpg"), &w, &h, &components, STBI_rgb);
    
    //unsigned char * stbi_data;
    //std::unique_ptr<unsigned char, void(*) (void*)> stbi_data;
    
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/GridBackground.jpg";
    //    char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/GridBackground2.jpg";
    char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/GridBackground3.jpg";
    
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/skewtxt.jpg";
    //char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/Antarctica.jpg";
    // char * img = "/Users/forrestmiller/Desktop/xcode_test_projects/Glitter3/Glitter/Antarctica.jpg";
    
   // std::unique_ptr<unsigned char, void(*)(void*)> stbi_data( stbi_load( img, &w, &h, &components, STBI_rgb ), stbi_image_free); //NOTE: void(*) (void*) is a function pointer to a function that takes a void pointer as parameter. See: https://stackoverflow.com/questions/12830860/what-is-voidvoid
    
    //Without variable for filename:
    //std::unique_ptr<unsigned char, void(*)(void*)> stbi_data( stbi_load( (char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/antarctica.png"), &w, &h, &components, STBI_rgb ), stbi_image_free); //NOTE: void(*) (void*) is a function pointer to a function that takes a void pointer as parameter. See: https://stackoverflow.com/questions/12830860/what-is-voidvoid
    
    //stbi_data = stbi_load((char*)("/Users/forrestmiller/Desktop/xcode_test_projects/Glitter/boki.jpg"), &w, &h, &components, STBI_rgb);
    
    //if(image == nullptr)
    //if (stbi_data == nullptr) {
        //throw new IOException(STBImage.stbi_failure_reason()); //See: http://forum.lwjgl.org/index.php?topic=5991.0
   //     throw(std::string("Failed to load texture"));
    //}
    //int dataSize = w * h * (0 < STBI_rgb ? STBI_rgb : components); //From that online link
    
    //std::vector<unsigned char> image_data;
    //image_data.resize(dataSize + w * (0 < STBI_rgb ? STBI_rgb : components)); //I changed to this while trying to correct texture
    //image_data.resize(dataSize + w * 540);
    //image_data.resize(dataSize + w *10);
    //image_data.resize(dataSize);
    //std::memcpy(image_data.data(), stbi_data.get(), dataSize);
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
    
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    
    
    
    //                    WRAPPING
    //Texture coordinates are addressed from 0 to 1 starting from bottom left corner
    // NOTE: Instead of i,j,k or x,y,z, textures use s,t,r
    // so we Need to tell OpenGL how to handle texture coordinates that at < 0 or > 1
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    
    
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
    
    //     Can also use mipmaps (but only after the texture has been loaded) by doing:
    glGenerateMipmap(GL_TEXTURE_2D);
    
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
    //bottom of the texture being modified? Maybe not.. Who knows
    
    //image_data.insert(image_data.begin(), (unsigned char)255);
    //    image_data.insert(image_data.begin(), (unsigned char)255);
    //    image_data.insert(image_data.begin(), (unsigned char)255);
    
    
    //Gonna try to rewrite what I had above, except in a way that makes it work better
    std::vector<unsigned char>::iterator it;
//    for (int i = 1; i < dataSize-1-h; ++i) {
//        if (i % w == 0) {
//            it = image_data.begin() + i;
//            image_data.insert(it, (unsigned char)255);
//            //image_data.insert(it, (unsigned char)255);
//        }
//        //        if (i == w*3) {
//        //            it = image_data.begin() + i;
//        //            image_data.insert(it, (unsigned char)255);
//        //        }
//    }
    
    
//    if(components == 3) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2745/3 , h-1, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data.data());
//        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
//    }
//    else if(components == 4) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
//        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data.data());
//    }
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    
    
    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    GLint colAttrib = glGetAttribLocation(shaderProgram, "colr"); //Added this line for textures
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texCoord");
    
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
    
    
    
    
    GLint texEnum = glGetUniformLocation(shaderProgram, "tex");
    GLint time = glGetUniformLocation(shaderProgram, "time");
    GLfloat theta = glGetUniformLocation(shaderProgram, "theta"); //I added this line
    
    GLfloat thta = 0.0;
    //GLint textureToDraw = 0;
    
    
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
    
    
    updateVertices(vertices, 0.0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    
    //Try this here
    //////////////////////////
    //Instead try
    glDisable(GL_DITHER);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
#define GL_MULTISAMPLE_ARB 0x809D
    glDisable( GL_MULTISAMPLE_ARB) ;
    ////////////////////////////////
    
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
            thta += ROTATIONSPEED;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            thta -= ROTATIONSPEED;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
            thta = 0.0f;
        }
        
        
        // Background Fill Color
        //glClearColor(sin(counter), 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Send "counter" value into shader via as uniform
        glUniform1f(time, counter);
        //glUniform1i(texEnum, *tex);
        glUniform1i(texEnum, tex[0]);
        glUniform1f(theta, thta);
        
        // Draw Stuff
        updateVertices(vertices, counter);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        tex[0] = 0; //A-HA! This fixed it for some reason
        //tex[0] = (tex[0] + 1) % 2;
        
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex[0], 0);
        
        
        
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
        //glDrawArrays(GL_TRIANGLES, 0, floatsPerTriangle*triangleCount);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);  //I added this one
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);  //I added this one
        //glDrawArrays(GL_TRIANGLES, 0, 6*7);
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Changed 3rd parameter to get it to draw finally
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        counter += 0.05;
    }   glfwTerminate();
    //stbi_image_free(image); //No longer need since i got a unique_ptr
    return EXIT_SUCCESS;
}

void updateVertices(GLfloat* vertices, float t)
{
    float temp = 0.3f + 0.25f * (float)sin(t);
    float tempVertices[] = {
        //        //  Position            Color                              Texcoords
        //        -0.8f,  1.0f,       1.0f,   0.0f,   0.0f,               0.0f, 1.0f, // Top-left
        //        0.8f,  1.0f,       0.0f,   1.0f,   0.0f,               0.0f, 0.0f, // Top-right
        //        0.8f, -1.0f,       0.0f,   temp,   1.0f,               1.0f, 0.0f, // Bottom-right
        //
        //        //  -0.5f,  0.5f,       1.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
        //        -0.8f, -1.0f,       1.0f,   1.0f,   1.0f,               1.0f, 1.0f,  // Bottom-left
        //        //   0.5f, -0.5f,       0.0f,   temp,   1.0f,               1.0f, 1.0f // Bottom-right
        //
        
        //  Position            Color                              Texcoords
        //        -0.25f,  0.25f,       1.0f,   0.0f,   0.0f,               0.0f, 1.0f, // Top-left
        //        0.25f,  0.25f,       0.0f,   1.0f,   0.0f,               0.0f, 0.0f, // Top-right
        //        0.25f, -0.25f,       0.0f,   temp,   1.0f,               1.0f, 0.0f, // Bottom-right
        //
        //
        //        -0.25f, -0.25f,       1.0f,   1.0f,   1.0f,               1.0f, 1.0f,  // Bottom-left
        
        
        //        //  Position            Color                              Texcoords
        //        -915.0f/609.0f,  1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
        //        915.0f/609.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-right
        //        915.0f/609.0f, -1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 1.0f, // Bottom-right
        //        -915.0f/609.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 1.0f,  // Bottom-left
        //
        
        
        //        //Make the triangles accomodate the skew?
        //        //  Position            Color                              Texcoords
        //        -1.0f,  0.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Middle-left corner  //0
        //        0.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-Corner   //1
        //        1.0f, 0.0f,       0.0f,   0.0f,   0.0f,               1.0f, 1.0f, // Middle-Right corner  //2
        //
        //        //  -0.5f,  0.5f,       1.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
        //        0.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 1.0f,  // Bottom-Corner  //3
        //        //   0.5f, -0.5f,       0.0f,   temp,   1.0f,               1.0f, 1.0f // Bottom-right
        //
        
        //  Position            Color                              Texcoords
        -1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 0.0f, // Top-left
        1.0f,  1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 0.0f, // Top-right
        1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               1.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f,       0.0f,   0.0f,   0.0f,               0.0f, 1.0f,  // Bottom-left
        
        
    };
    
    for (int i = 0; i < 7 * 6; i++) {
        //        if (i % 7 == 0 || i % 7 == 1) {
        //            vertices[i] = tempVertices[i] * 2;
        //        }
        //        else {
        vertices[i] = tempVertices[i];
        //    }
    }
    for (int i = 7*6; i < 7*6*2 ; i++) {
        vertices[i] = 0.0f; //Might as well set some things to zero past where array starts
    }
    //    for (int i=0; i<triangleCount; i++) {
    //
    //        int idx = i*floatsPerTriangle;
    //
    //
    //        // Specify triangles with 3 pairs of X and Y coordinates
    //        //float p = 1.0*float(i+t*0.005);
    //
    //        //float xCenter = 0.01*sqrt(p)*0.5*cos(p*0.2);
    //
    //        //float yCenter = 0.01*sqrt(p)*0.5*sin(p*0.2);
    //
    //        //My Code
    //        float xCenter;
    //
    //        if (idx % 200 <= 45) {
    //        //xCenter = 0.001 + sin(0.001*idx)*cos(sin(cos(idx+(idx/3))));
    //        //xCenter = 0;
    //            xCenter = (0.001)*idx * sin(idx);
    //         }
    //         else {
    //        //     xCenter = -0.005 - 0.001*idx - 4*sin(0.025 * idx/t + t);
    //            xCenter = (0.001)*idx * cos(idx);
    //         }
    //
    //        float yCenter = -2.0 * 0.2*sin(t * 0.3 * xCenter);
    //        ///if (idx % 5 == 0)
    //        ///  yCenter = (0.001 * idx / t);
    //        //else
    //        //  yCenter = (-0.0001 * idx) / (t * idx);
    //        //End of my code
    //
    //        vertices[idx+0] = xCenter; //X1
    //        vertices[idx+1] = yCenter+5.0*triangleSize; // Y1
    //
    //        vertices[idx+2] = xCenter-5.0*triangleSize; // X2
    //        vertices[idx+3] = yCenter; // Y2
    //
    //        vertices[idx+4] = xCenter+5.0*triangleSize; // X3
    //        vertices[idx+5] = yCenter; // Y3
    //    }
}

std::string loadSource(char* filename)
{
    std::ifstream infile{ filename };
    return{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
}



