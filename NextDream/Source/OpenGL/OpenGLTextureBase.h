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
            void* m_LockingBuffer;

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
                m_LockingBuffer = Malloc::Alloc(inInitInfo.Width * inInitInfo.Height * OpenGLEnumMapping::GetPixelTypeSize(inInitInfo.BaseFormat));
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

                m_Buffer = 0;
            }

        public:
            virtual void* Lock(RHIContextBase* inContext, ERES_LOCK_TYPE inType, const SIZE_T inIndex)
            {
                if (UseRenderBuffer())
                {
                    glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_Buffer);
                    //glFramebufferRenderbufferEXT(GL_RENDERBUFFER_EXT, )
                }
                else
                {
                    if (UseMultiSample())
                    {
                        //glGetTextureImageEXT()
                    }
                    else
                    {
                        glGetTextureImageEXT(
                            m_Buffer,
                            GL_TEXTURE_2D,
                            inIndex,
                            OpenGLEnumMapping::GetPixelFormat(m_DataFormat),
                            OpenGLEnumMapping::GetPixelType(m_DataFormat),
                            NULL
                        );
                    }
                }

                glTextureRenderbufferEXT()

#if _DEBUG
                if (!CanGpuWrite() && inType == ERES_LOCK_ONLY_WRITE)
                {
                    //log can create texture use EGPU_READ_CPU_WRITE
                }
#endif

                if (mapType == D3D11_MAP_WRITE_DISCARD && (GetUsageMode() & ECPU_WRITE_SUB_USAGE) == 0)
                {
                    //log
                    return NULL;
                }

                if (cantLock)
                {
                    m_ShadowTexture = m_Texture;
                    return contextImpl->LockGpuResource(m_Texture, inIndex, mapType, true);
                }

                if (m_ShadowTexture != NULL)
                {
                    ID3D11Device* device = contextImpl->GetDevice();
                    if (device == NULL)
                    {
                        return NULL;
                    }

                    D3D11_TEXTURE2D_DESC desc = { 0 };
                    desc.Width = m_Width;
                    desc.Height = m_Height;
                    desc.Usage = D3D11_USAGE_STAGING;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
                    desc.SampleDesc.Count = m_SampleCount;
                    desc.SampleDesc.Quality = m_SampleQulity;
                    desc.ArraySize = 1;
                    desc.MipLevels = 0;
                    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
                    desc.MiscFlags = 0;
                    desc.Format = DirectXEnumMapping::GetPixelFormat(m_DataFormat);

                    HRESULT hr = device->CreateTexture2D(&desc, NULL, &m_ShadowTexture);
                    if (FAILED(hr))
                    {
                        //log
                    }
                }

                contextImpl->CopyGpuResource(m_Texture, m_ShadowTexture);

                m_ShadowTexture = m_ShadowTexture;
                return contextImpl->LockGpuResource(m_ShadowTexture, 0, mapType, true);
            }

            virtual void Unlock(RHIContextBase* inContext, const SIZE_T inIndex)
            {
                DirectX11ContextBaseImpl* contextImpl = static_cast<DirectX11ContextBaseImpl*>(inContext->GetImpl());
                contextImpl->UnlockGpuResource(m_ShadowTexture, 0);
            }
        };
    }
}

#endif // !__BLADE_OPENGL_TEXTURE_BASE_H__
