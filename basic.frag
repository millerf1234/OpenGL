
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
  
    //vec4 col1 = vec4(abs(sin(pos.x - pos.y + time) + gl_FragCoord.x), abs( 0.5 + 0.15*sin(time)), abs(cos(time)- pos.x-pos.y + tan(pos.x-pos.y / 3.0f)), 1.0 );
    //color = texture(tex, TexCoor) + col1;
    
    //color = vec4(1.0, 0.0, 0.25, 1.0);
    
    vec3 col = vec3(sin(0.2*gl_FragCoord.xy+time*0.04*cos(time + (pos.x / pos.y))),
                   cos(sin(time*0.1)*16.0*(12.0*(cos(5.0*pos.x)+sin(8.0*pos.y)))));
    vec3 col2 = vec3(sin(0.2*pos.xy+time*0.12), sin(time*0.1)+1.0-(gl_FragCoord.x - abs(gl_FragCoord.y)));
    color = texture(tex, TexCoor) + vec4(mix(col, col2, (col-col2) / 2.0), 1.0);
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
}

