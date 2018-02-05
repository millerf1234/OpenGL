

#version 400 core
out vec4 color;

uniform float time3D;
//uniform sampler3D tex3D;
uniform sampler2D tex3D;

//uniform vec3 mainEnginePos;
uniform float mainEngPosX;
uniform float mainEngPosY;
uniform float mainEngPosZ; 

in vec3 pos3D;
//in vec3 COLR;
in vec2 TexCoor3D;

//A fragment is like what I called my GridElem struct, in that it knows its position and its color and is like a pixel

//position specifies the position of a vertex
//pointSize specifies the size of a point (p.33)


void main() {
    
    //vec4 col1 = vec4(abs(sin(pos.x - pos.y + time) + gl_FragCoord.x), abs( 0.5 + 0.15*sin(time)), abs(cos(time)- pos.x-pos.y + tan(pos.x-pos.y / 3.0f)), 1.0 );
    //color = texture(tex, TexCoor) + col1;
    
    //color = vec4(1.0, 0.0, 0.25, 1.0);
    
    // vec3 col = vec3(sin(0.2*gl_FragCoord.xy+time*0.04*cos(time + (pos.x / pos.y))),
    //                cos(sin(time*0.1)*16.0*(12.0*(cos(5.0*pos.x)+sin(8.0*pos.y)))));
    //vec3 col2 = vec3(sin(0.2*pos.xy+time*0.12), sin(time*0.1)+1.0-(gl_FragCoord.x - abs(gl_FragCoord.y)));
    //color = texture(tex, TexCoor) + vec4(mix(col, col2, pos.z), 1.0);
    //
    //vec4 color1 = texture(tex, TexCoor) + vec4( mix(col, col2, 0.5 * sin(1.0/time)), 1.0); //this one works
    //vec4 color2 = vec4(-color1.x, -color1.y, -abs(0.25*cos(color1.z)), 0.0f);
    //color = color1 + color2 + vec4(abs(0.35f + 0.45f * cos(time + gl_FragCoord.y)), abs(0.35 * sin(gl_FragCoord.y + time * tan(time + pos.x))), 0.0f, 0.0f) ;
    //
    //    //color = mix(col, col2, tst * (col + col2 * cos(time)));
    //    color = mix(col, col2, rotationMatrix * (col + col2));
    //    //color = (0.5*rotationMatrix*col, 0.5, rotationMatrix * col2);
    
    
    //color = texture(tex, TexCoor) + vec4(0.4f * abs(sin(gl_FragCoord.x + time)), 0.3f * abs(gl_FragCoord.y), 0.75f - 0.20f * tan(time + sin(gl_FragCoord.x * gl_FragCoord.y)), 1.0);
    
    //color = texture(tex, TexCoor) + vec4(sin(0.75 + gl_FragCoord.x + time / 100.0f), 0.6 + tan(sqrt((abs(pos.x + pos.y)))), 0.6 * abs(sin(pos.x-pos.y)), 0.9);
    
    //color = texture(tex3D, TexCoor) + vec4(abs(cos(time/16)), 0.5, 0.5, 1.0 );
    
    //color = vec4(0.8, 0.8, 0.2, 1.0) + texture(tex3D, TexCoor3D);
    
    
    
    //Orangeish/yellowish/redish:
//
//
//        //Fake sorta Phong shading maybe
//        //Pretend these are all uniforms:
//        vec3 ambientCol = vec3(0.4f, 0.4f, 0.3f);
//
//        // http://www.vendian.org/mncharity/dir3/starcolor/ ///What color are the stars? asks/answers this website
//        vec3 lightCol = vec3(0.95f, 0.55f, 0.35f);  //ffc877
//
//        //vec3 lightPos = vec3(0.5f + 0.25f * cos(time3D), 0.5f + 0.5f * cos(time3D), 0.5f + 0.5 * sin(time3D));
//        //vec3 lightPos = vec3(2.95f, 2.95f, 3.0f );
//        //vec3 lightPos = mainEnginePos;
//        vec3 lightPos = vec3(mainEngPosX, mainEngPosY, mainEngPosZ);
//        vec3 eyePos = vec3(0.0f, 0.0f /* + 0.25f * sin(time3D) */, 2.0f);
//        vec3 Ka = vec3(0.3f, 0.3f, 0.3f); //Ka is the ambient color of the material
//        //vec3 Kd = vec3(abs(sin(time3D)), abs(sin(time3D + 0.25f)), abs(sin(time3D-0.25f)));
//        vec3 Kd = vec3(0.25f, 0.26f, 0.27f);
//        vec3 Ks = vec3(0.45f, 0.45f, 0.25f);
//        //float shiny = abs(0.65f + 0.45f * cos(time3D * 3.0)) ;
//        float shiny = 0.5f;
//
//        vec3 P = pos3D.xyz;  //Position?
//        //vec3 N = cross(pos3D, eyePos);//vec3 N = normal;  //Normal?
//        vec3 N = vec3(0.0, pos3D.y, pos3D.x);
//        //Modify normal to be more normal
//        //N = cross(pos3D, N);
//        vec3 V = normalize(eyePos - P);  //Direction of camera minus surface
//        vec3 H = normalize(vec3(0.55 /*+ 0.1 * tan(time3D * 0.05f)*/, 0.65 + 0.35 * cos(time3D) , 0.0) + V);
//        //vec3 H = normalize(L + V); //Light-Surface-Eye direction?
//
//        vec3 ambient = Ka * ambientCol;
//
//        vec3 L = normalize(lightPos - P);
//        float diffLight = max(dot(L,N), 0);
//        vec3 diffuse = Kd * lightCol * diffLight;
//
//        float specLight = pow(max(dot(H,N),0), shiny);
//        vec3 specular = Ks * lightCol * specLight;
//
//        color = texture(tex3D, TexCoor3D.xy) + vec4(ambient + diffuse + specular, 1.0f);
//
    
    
    
    
    //Blue:
    
    
    //Fake sorta Phong shading maybe
    //Pretend these are all uniforms:
    vec3 ambientCol = vec3(0.4f, 0.4f, 0.8f);
    
    // http://www.vendian.org/mncharity/dir3/starcolor/ ///What color are the stars? asks/answers this website
    vec3 lightCol = vec3(0.35f, 0.35f, 0.85f);  //ffc877
    
    //vec3 lightPos = vec3(0.5f + 0.25f * cos(time3D), 0.5f + 0.5f * cos(time3D), 0.5f + 0.5 * sin(time3D));
    //vec3 lightPos = vec3(2.95f, 2.95f, 3.0f );
    //vec3 lightPos = mainEnginePos;
    vec3 lightPos = vec3(mainEngPosX, mainEngPosY, mainEngPosZ);
    vec3 eyePos = vec3(0.0f, 0.0f /* + 0.25f * sin(time3D) */, 2.0f);
    vec3 Ka = vec3(0.3f, 0.3f, 0.3f); //Ka is the ambient color of the material
    //vec3 Kd = vec3(abs(sin(time3D)), abs(sin(time3D + 0.25f)), abs(sin(time3D-0.25f)));
    vec3 Kd = vec3(0.25f, 0.26f, 0.87f);
    vec3 Ks = vec3(0.45f, 0.45f, 0.25f);
    //float shiny = abs(0.65f + 0.45f * cos(time3D * 3.0)) ;
    float shiny = 0.5f;
    
    vec3 P = pos3D.xyz;  //Position?
    //vec3 N = cross(pos3D, eyePos);//vec3 N = normal;  //Normal?
    vec3 N = vec3(0.0, pos3D.y, pos3D.x);
    //Modify normal to be more normal
    //N = cross(pos3D, N);
    vec3 V = normalize(eyePos - P);  //Direction of camera minus surface
    vec3 H = normalize(vec3(0.55 /*+ 0.1 * tan(time3D * 0.05f)*/, 0.65 + 0.35 * cos(time3D) , 0.0) + V);
    //vec3 H = normalize(L + V); //Light-Surface-Eye direction?
    
    vec3 ambient = Ka * ambientCol;
    
    vec3 L = normalize(lightPos - P);
    float diffLight = max(dot(L,N), 0);
    vec3 diffuse = Kd * lightCol * diffLight;
    
    float specLight = pow(max(dot(H,N),0), shiny);
    vec3 specular = Ks * lightCol * specLight;
    
    color = texture(tex3D, TexCoor3D.xy) + vec4(ambient + diffuse + specular, 1.0f);
    
}
        
        
