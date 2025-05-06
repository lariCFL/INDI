#version 330 core

in vec3 vertex;
in vec3 color;
uniform mat4 TG;


uniform bool isTerra;
out float vIsTerra; 

out vec4 vertex_pos;
out vec4 vertex_color;


void main()  {
    gl_Position = TG * vec4 (vertex, 1.0);
    vertex_pos = gl_Position;
    vertex_color = vec4 (color,1.0);
    vIsTerra = isTerra ? 1.0 : 0.0; //bool to int
}
