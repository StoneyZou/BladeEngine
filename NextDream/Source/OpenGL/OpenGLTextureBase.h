#ifndef __BLADE_OPENGL_TEXTURE_BASE_H__
#define __BLADE_OPENGL_TEXTURE_BASE_H__

#include <OpenGLBase.h>
#include <RHITextureBase.h>
#include <OpenGLEnumMapping.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct OpenGLTextureInitInfo : RHITextureInitInfo
        {
            bool m_IsRenderBuffer;
            GLint m_Buffer;
        };

        class OpenGLTexture2D : public RHITexture2D
        {
        private:
            GLuint m_Buffer;
            
            GLsizei m_LockingBufferSize;
            void* m_LockingBuffer;
            ERES_LOCK_TYPE m_LockingType;

        private:
            bool UseRenderBuffer() const { return GetUsageMode() == ESUIT_GPU_WRITE ; }
            bool UseMultiSample() const { return m_SampleCount > 1; }

        public:
            static OpenGLTexture2D* Create(IRHIDevice* inDevice, const RHITextureCreateInfo& inCreateInfo)
            {
                bool multisample = (inCreateInfo.SampleCount > 1);

                GLuint textureOpenGL = 0;
                GLuint renderBufferOpenGL = 0;
                if (inCreateInfo.UsageMode != ESUIT_GPU_WRITE)
                {
                    glGenTextures(1, &textureOpenGL);
                    if (inCreateInfo.Data == NULL)
                    {
                        // 只开存储空间
                        if (!multisample)
                        {
                            glBindTextureEXT(GL_TEXTURE_2D, textureOpenGL);
                            glTextureStorage2DEXT(
                                textureOpenGL,
                                GL_TEXTURE_2D,
                                0,
                                OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                                inCreateInfo.Width,
                                inCreateInfo.Height);
                            glBindTextureEXT(GL_TEXTURE_2D, 0);
                        }
                        else
                        {
                            glBindTextureEXT(GL_TEXTURE_2D_MULTISAMPLE, textureOpenGL);
                            glTextureStorage2DMultisampleEXT(
                                textureOpenGL,
                                GL_TEXTURE_2D_MULTISAMPLE,
                                inCreateInfo.SampleCount,
                                OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                                inCreateInfo.Width,
                                inCreateInfo.Height,
                                false);
                            glBindTextureEXT(GL_TEXTURE_2D_MULTISAMPLE, 0);
                        }
                    }
                    else
                    {
                        //BladeAssert(multisample);
                        glBindTextureEXT(GL_TEXTURE_2D, textureOpenGL);
                        glTextureImage2DEXT(
                            textureOpenGL,
                            GL_TEXTURE_2D,
                            0,
                            OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                            inCreateInfo.Width,
                            inCreateInfo.Height,
                            0,
                            OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                            OpenGLEnumMapping::GetPixelType(inCreateInfo.BaseFormat),
                            inCreateInfo.Data
                        );
                        glBindTextureEXT(GL_TEXTURE_2D, 0);
                    }
                }
                else
                {
                    // 仅GPU写，直接使用RenderBuffer
                    glGenRenderbuffersEXT(1, &renderBufferOpenGL);
                    if (!multisample)
                    {
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferOpenGL);
                        glRenderbufferStorageEXT(
                            GL_RENDERBUFFER_EXT,
                            OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                            inCreateInfo.Width,
                            inCreateInfo.Height);
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
                    }
                    else
                    {
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBufferOpenGL);
                        glRenderbufferStorageMultisampleEXT(
                            GL_RENDERBUFFER_EXT,
                            inCreateInfo.SampleCount,
                            OpenGLEnumMapping::GetPixelFormat(inCreateInfo.BaseFormat),
                            inCreateInfo.Width,
                            inCreateInfo.Height);
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
                    }
                }

                OpenGLTextureInitInfo initInfo;
                initInfo.UsageMode = inCreateInfo.UsageMode;
                initInfo.BaseFormat = inCreateInfo.BaseFormat;
                initInfo.Depth = 0;
                initInfo.Height = inCreateInfo.Height;
                initInfo.m_IsRenderBuffer = renderBufferOpenGL != 0;
                initInfo.m_Buffer = renderBufferOpenGL != 0 ? renderBufferOpenGL : textureOpenGL;
                initInfo.SampleCount = inCreateInfo.SampleCount;
                initInfo.Width = inCreateInfo.Width;

                return new OpenGLTexture2D(inDevice, initInfo);
            }

        public:
            OpenGLTexture2D(IRHIDevice* inDevice, const OpenGLTextureInitInfo& inInitInfo)
                : RHITexture2D(inDevice, inInitInfo),
                m_LockingBuffer(NULL),
                m_Buffer(inInitInfo.m_Buffer)
            {
                m_LockingBufferSize = inInitInfo.Width * inInitInfo.Height * OpenGLEnumMapping::GetPixelTypeSize(inInitInfo.BaseFormat);
                m_LockingBuffer = Malloc::Alloc(m_LockingBufferSize);
            }

            ~OpenGLTexture2D()
            {
                if (!UseRenderBuffer())
                {
                    glDeleteTextures(1, &m_Buffer);
                }
                else
                {
                    glDeleteRenderbuffersEXT(1, &m_Buffer);
                }

                if (m_LockingBuffer != NULL) { Malloc::Free(m_LockingBuffer); }

                m_LockingBuffer = NULL;
                m_LockingBufferSize = 0;
                m_Buffer = 0;
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inLevel)
            {
                m_LockingType = inType;
                if ((inType & ERES_LOCK_READ) == 0)
                {
                    return m_LockingBuffer;
                }

                if (UseRenderBuffer())
                {
                    glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_Buffer);
                    //glFramebufferRenderbufferEXT(GL_RENDERBUFFER_EXT, )
                }
                else
                {
                    GLenum target = UseMultiSample() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
                    glBindTextureEXT(target, m_Buffer);
                    glGetnTexImage(
                        target,
                        inLevel,
                        OpenGLEnumMapping::GetPixelFormat(m_DataFormat),
                        OpenGLEnumMapping::GetPixelType(m_DataFormat),
                        m_LockingBufferSize,
                        m_LockingBuffer
                    );
                    glBindTextureEXT(target, 0);
                }

                return m_LockingBuffer;
            }

            virtual void Unlock(RHIContextBase* inContext, const SIZE_T inLevel)
            {
                if ((m_LockingType & ERES_LOCK_ONLY_WRITE) != 0)
                {
                    GLenum target = UseMultiSample() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
                    glBindTextureEXT(target, m_Buffer);
                    glTexSubImage2D(
                        target,
                        inLevel,
                        0, 0,
                        m_Width, m_Height,
                        OpenGLEnumMapping::GetPixelFormat(m_DataFormat),
                        OpenGLEnumMapping::GetPixelType(m_DataFormat),
                        m_LockingBuffer
                    );
                    glBindTextureEXT(target, 0);
                }
            }
        };
    }
}

#endif // !__BLADE_OPENGL_TEXTURE_BASE_H__
