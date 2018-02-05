
#version 400 core

in vec2 position;
in vec3 colr;
in vec2 texCoord;


//uniform float theta; //I added this


uniform float time;
uniform float tex;
uniform float theta;
uniform float zoom;
uniform float xCoor;
uniform float yCoor;

out vec2 pos;
out vec3 COLR;
out vec2 TexCoor;

void main()
{

pos = position;
COLR = colr;
TexCoor = texCoord;
// gl_Position = vec4(position.x+0.7*sin(exp(position.y)+time*0.05 + 0.05 *cos(time * 4)),
//                (1.0+0.3*sin(10.0*position.y))*position.y+0.6*cos(exp(position.x)+time*0.04), 0.0, 1.0);

//mat3 rotationMatrix = mat3(-sin(time*0.3), cos(time*0.6), 0.0, cos(time*0.3), sin(time*0.6), 0.0, 0.0, 0.0, 1.0);
// mat3 rotationMatrix = mat3(-sin(theta), cos(theta), 0.0, cos(theta), sin(theta), 0.0, 0.0, 0.0, 1.0);


//    mat3 aspectRatioMatrix = mat3(1.0f, 0.0, 0.0, 0.0, 1670.0f/970.0f, 0.0f, 0.0, 0.0, 1.0f);
//
//    mat3 rotationMatrixZ = mat3(cos(thetaZ), -sin(thetaZ), 0.0,
//                                sin(thetaZ), cos(thetaZ),  0.0,
//                                0.0,         0.0,  1.0);
//
//    mat3 rotationMatrixY = mat3(cos(thetaY), 0.0, -sin(thetaY),
//                                0.0    , 1.0,      0.0    ,
//                                sin(thetaY), 0.0,  cos(thetaY));
//
//    mat3 rotationMatrixX = mat3(    1.0  ,      0.0    ,     0.0     ,
//                                0.0  , cos(thetaX) , -sin(thetaX),
//                                0.0  , sin(thetaX) ,  cos(thetaX) );
//
//
//    mat3 dialationMatrix = mat3(1.0 + iStretch,       0.0        ,      0.0       ,   //Column 1
//                                0.0       ,  1.0 + jStretch  ,      0.0       ,   //Column 2
//                                0.0       ,       0.0        , 1.0 + kStretch) ; //Column 3
//
//    vec3 translation = vec3(xPos, yPos, 0.0);
//

//    mat3 aspectRatioMatrix = mat3(1.0f,       0.0f     , 0.0f,
//                                  0.0f, 1670.0f/970.0f , 0.0f,
//                                  0.0f,       0.0f     , 1.0f );
    
//    mat3 aspectRatioMatrix = mat3(1.0f,       0.0f     , 0.0f,
//                                  0.0f, 2650.0f/1600.0f, 0.0f,
//                                  0.0f,       0.0f     , 1.0f );

    mat3 aspectRatioMatrix = mat3(1.0f,       0.0f     , 0.0f,
                                  0.0f, 1920.0f/1080.0f, 0.0f,
                                  0.0f,       0.0f     , 1.0f );

    mat3 rotationMatrix = mat3(cos(theta), -sin(theta), 0.0f,
                               sin(theta), cos(theta),  0.0f,
                                  0.0f   ,     0.0f  ,  1.0f );
//gl_Position = vec4(position.x, position.y, sin(time), 1.0 + 0.2*cos(time));


vec3 temp = vec3( aspectRatioMatrix * rotationMatrix * vec3(position.x, position.y, 0));


gl_Position = vec4(temp + vec3(xCoor, yCoor, 0.0f), 1.0 + zoom);

//gl_Position = vec4(position.x, position.y, 0.0, 1);

//gl_PointSize = 10 + sin(time);
}

