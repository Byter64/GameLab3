#include "../../include/Shader.h"

namespace Engine
{
    bool Shader::operator==(const Shader &other) const
    {
        return vertexID == other.vertexID &&
               tesselationControlID == other.tesselationControlID &&
               tesselationEvaluationID == other.tesselationEvaluationID &&
               geometryID == other.geometryID &&
               fragmentID == other.fragmentID;
    }

    bool Shader::operator!= (const Shader& other) const
    {
        return !(*this == other);
    }

    bool Shader::operator<(const Shader &other) const
    {
        return this->id < other.id;
    }

    bool Shader::operator>(const Shader &other) const
    {
        return this->id > other.id;
    }

    void Shader::SetVertexShader(GLuint vertexShader)
    {
        vertexID = vertexShader;
        hasChanged = true;
    }

    GLuint Shader::GetVertexShader()
    {
        return vertexID;
    }

    void Shader::SetTesselationControlShader(GLuint tesselationControlShader)
    {
        tesselationControlID = tesselationControlShader;
        hasChanged = true;
    }

    GLuint Shader::GetTesselationControlShader()
    {
        return tesselationControlID;
    }

    void Shader::SetTesselationEvaluationShader(GLuint tesselationEvaluationShader)
    {
        tesselationEvaluationID = tesselationEvaluationShader;
        hasChanged = true;
    }

    GLuint Shader::GetTesselationEvaluationShader()
    {
        return tesselationEvaluationID;
    }

    void Shader::SetGeometryShader(GLuint geometryShader)
    {
        geometryID = geometryShader;
        hasChanged = true;
    }

    GLuint Shader::GetGeometryShader()
    {
        return geometryID;
    }

    void Shader::SetFragmentShader(GLuint fragmentShader)
    {
        fragmentID = fragmentShader;
        hasChanged = true;
    }

    GLuint Shader::GetFragmentShader()
    {
        return  fragmentID;
    }

} // Engine