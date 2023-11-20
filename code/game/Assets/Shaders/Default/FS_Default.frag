#version 460

in vec3 vertexColor;
in vec2 texCoord;
in vec4 gl_FragCoord;

layout(binding = 0)uniform sampler2D baseColor;
uniform vec4 baseColorFactor;

layout(location = 0)out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(vertexColor, 1.0) * texture(baseColor, texCoord) * baseColorFactor;
}
