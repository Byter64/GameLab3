#version 460

in vec3 vertexColor;
in vec2 texCoord;

layout(binding = 0)uniform sampler2D baseColor;
layout(binding = 1)uniform sampler2D metallicRoughness;
layout(binding = 2)uniform sampler2D normal;
layout(binding = 3)uniform sampler2D occlusion;
layout(binding = 4)uniform sampler2D emissive;

uniform vec4 baseColorFactor;
uniform float metallicFactor;
uniform float roughnessFactor;
uniform float normalScale;
uniform float occlusionStrength;
uniform vec3 emissiveFactor;

layout(location = 0)out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(pow(vertexColor, vec3(1.0/2)), 1.0) * baseColorFactor * texture(baseColor, texCoord);
} 
