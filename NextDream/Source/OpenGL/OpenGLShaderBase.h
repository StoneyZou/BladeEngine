#ifndef __BLADE_OPENGL_SHADER_BASE_H__
#define __BLADE_OPENGL_SHADER_BASE_H__

#include <OpenGLBase.h>
#include <RHIShaderBase.h>

namespace BladeEngine
{
    class OpenGLShaderState : public RHIShaderState
    {
    public:
        static OpenGLShaderState* Create(const RHIShaderStateCreateInfo& inCreateInfo)
        {

        }
    };

    class OpenGLVertexShader : public RHIVertexShader
    {
    private:
        GLuint m_Shader;

    public:
        static OpenGLVertexShader* Create(const RHIShaderCreateInfo& inCreateInfo)
        {
            GLuint shader = glCreateShader(GL_VERTEX_SHADER);

            const GLchar* shaderSrc = (const GLchar*)inCreateInfo.Data;
            const GLint shaderSrcLen = (GLint)inCreateInfo.DataSize;
            glShaderSource(shader, 1, &shaderSrc, &shaderSrcLen);
            glCompileShader(shader);
            
            GLint compileStatus = GL_TRUE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
            if (compileStatus != GL_TRUE)
            {
                char Msg[2048];
                glGetShaderInfoLog(shader, 2048, nullptr, Msg);
            }
        }
    };

}

#endif // !__BLADE_OPENGL_SHADER_BASE_H__
