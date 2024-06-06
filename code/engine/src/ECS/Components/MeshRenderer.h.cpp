#include "ECS/Components/MeshRenderer.h"

namespace Engine
{
    void MeshRenderer::SetFragmentShader(GLuint fragmentShader)
    {
        for (PrimitiveData &primitive: primitiveData)
            primitive.shader.SetFragmentShader(fragmentShader);
    }

    void MeshRenderer::SetVertexShader(GLuint vertexShader)
    {
        for (PrimitiveData &primitive: primitiveData)
            primitive.shader.SetVertexShader(vertexShader);
    }
}