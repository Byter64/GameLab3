#version 460

in vec3 vertexColour;
in vec2 texCoord;
in vec4 gl_FragCoord;

layout(binding = 0)uniform sampler2D baseColour;
uniform vec4 baseColourFactor;

layout(location = 0)out vec4 fragmentColour;

void main()
{
    fragmentColour = vec4(vertexColour, 1.0) * texture(baseColour, texCoord) * baseColourFactor;
}
