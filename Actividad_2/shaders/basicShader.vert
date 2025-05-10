#version 330 core

in vec3 vertex;

uniform vec3 col;  // nova variável para cor
uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3 fcolor;

void main() {
    fcolor = col;  // usa o uniform
    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
