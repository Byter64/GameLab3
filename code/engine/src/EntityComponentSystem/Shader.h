#pragma once
#include "glad/glad.h"

namespace Engine
{

    struct Shader
    {
        friend class RenderSystem;

    private:
        bool hasChanged = true;

        GLuint id = 0;
        GLuint vertexID = 0;
        GLuint tesselationControlID = 0;
        GLuint tesselationEvaluationID = 0;
        GLuint geometryID = 0;
        GLuint fragmentID = 0;

    public:
        Shader() = default;

        void SetVertexShader(GLuint vertexShader);
        GLuint GetVertexShader();

        void SetTesselationControlShader(GLuint tesselationControlShader);
        GLuint GetTesselationControlShader();

        void SetTesselationEvaluationShader(GLuint tesselationEvaluationShader);
        GLuint GetTesselationEvaluationShader();

        void SetGeometryShader(GLuint geometryShader);
        GLuint GetGeometryShader();

        void SetFragmentShader(GLuint fragmentShader);
        GLuint GetFragmentShader();

        bool operator< (const Shader& other) const;
        bool operator> (const Shader& other) const;
        bool operator== (const Shader& other) const;
        bool operator!= (const Shader& other) const;
    };

} // Engine
