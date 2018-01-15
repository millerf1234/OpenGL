
#version 400 core
out vec4 color;

uniform float time;
uniform sampler2D tex;

in vec2 pos;
in vec3 COLR;
in vec2 TexCoor;

//A fragment is like what I called my GridElem struct, in that it knows its position and its color and is like a pixel

//position specifies the position of a vertex
//pointSize specifies the size of a point (p.33)


void main() {
    
    //color = texture(tex, TexCoor) + vec4(0.5,0.0,0.0,1.0);
    
    color = vec4(1.0, 0.0, 0.25, 1.0);
    
    //    vec3 col = vec3(sin(0.2*gl_FragCoord.xy+time*0.04*cos(time + (pos.x / pos.y))),
    //                    cos(sin(time*0.1)*16.0*(12.0*(cos(5.0*pos.x)+sin(8.0*pos.y)))));
    //
    //    mat3 rotationMatrix = mat3(-sin(time*0.5), cos(time*0.5), 0.0, cos(time*0.5), sin(time*0.5), 0.0, 0.0, 0.0, 1.0);
    //    //vec3 col = vec3(
    //    vec3 col2 = vec3(sin(0.2*pos.xy+time*0.12), sin(time*0.1)+1.0);
    //
    //    //color = mix(col, col2, 0.5 * sin(1.0/time)); //this one works
    //
    //    //color = mix(col, col2, tst * (col + col2 * cos(time)));
    //    color = mix(col, col2, rotationMatrix * (col + col2));
    //    //color = (0.5*rotationMatrix*col, 0.5, rotationMatrix * col2);
    
}

