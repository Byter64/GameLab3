#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoord_0;
layout(location = 4) in vec2 texCoord_1;
layout(location = 5) in vec2 texCoord_2;
layout(location = 6) in vec2 texCoord_3;
layout(location = 7) in vec3 color_0;
layout(location = 8) in vec3 color_1;
layout(location = 9) in vec3 color_2;
layout(location = 10)in vec3 color_3;

uniform mat4 model;
uniform mat4 projectionView;

out vec3 vertexColor;
out vec2 texCoord;

void main() 
{
    gl_Position = projectionView * model * vec4(position, 1.0);
    vertexColor = color_0;
    texCoord = texCoord_0;
}