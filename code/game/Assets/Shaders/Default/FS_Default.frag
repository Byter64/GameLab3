#version 460

in vec3 vertexColor;
in vec2 texCoord;

layout(binding = 0)uniform sampler2D baseColor;
uniform vec4 baseColorFactor;

layout(location = 0)out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(baseColor, texCoord) * baseColorFactor;
} 
