
#version 400 core
out vec4 color;

uniform float time3D;
//uniform sampler3D tex3D;
uniform sampler2D tex3D;

in vec3 pos3D;
//in vec3 COLR;
in vec2 TexCoor3D;

//A fragment is like what I called my GridElem struct, in that it knows its position and its color and is like a pixel

//position specifies the position of a vertex
//pointSize specifies the size of a point (p.33)


void main() {
    //Orangeish/Yellowish/Redish lines
    //color = vec4(1.0f, 0.65f, 0.0f /* + 0.4f*sin(time3D / 5.0f)*/, 1.0f);
    
    //Blue lines:
    color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
    
    //This next line makes the line colors shift ocasionally
    //color = vec4(abs(floor(sin(time3D * 2.0f))), abs(floor(sin(time3D + 3.1415927f/4.0f))), 0.25f *abs(floor(sin(time3D + 3.1415927f/2.0f))), 1.0f);
}
