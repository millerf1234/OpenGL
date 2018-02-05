
#version 400 core

//Currently I am using the same vert shader for both the main and side engines

in vec3 enginePos;
//in vec2 engineTxtr;

uniform float zoom;
uniform float time;

out vec3 enginePosition;
//out vec2 engineTex;

void main()
{
    enginePosition = enginePos;
   // engineTex = engineTxtr;
  
    //        mat3 aspectRatioMatrix = mat3(1.0f,       0.0f     , 0.0f,
    //                                      0.0f, 2650.0f/1600.0f, 0.0f,
    //                                      0.0f,       0.0f     , 1.0f );
    
    mat3 aspectRatioMatrix = mat3(1.0f,       0.0f     , 0.0f,
                                  0.0f, 1920.0f/1080.0f, 0.0f,
                                  0.0f,       0.0f     , 1.0f );
    
    
    enginePosition = aspectRatioMatrix * enginePosition;
    
    //enginePosition.x += 0.005f*cos(time*100.0f);
    
    
    gl_Position = vec4(enginePosition, 1.0 + zoom) ;// vec4(1.0f, 1.0f, 1.0f, 0.0f);
    
}

