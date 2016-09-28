#ifndef __BLADE_OPENGL_ENUM_MAPPING_H__
#define __BLADE_OPENGL_ENUM_MAPPING_H__

#include <OpenGLBase.h>
#include <RHIEnum.h>

namespace BladeEngine
{
    namespace RHI
    {
        struct OpenGLEnumMapping
        {
            static GLenum GetPixelFormat(ETEX_FORMAT inFormat)
            {
                GLenum result = GL_RGBA;
                ETEX_FORMAT_LAYOUT layout = (ETEX_FORMAT_LAYOUT)(inFormat >> DATA_LAYOUT_SHIFT);
                switch (inFormat)
                {
                case BladeEngine::EDATA_FORMAT_R8G8B8A8_UINT:
                    break;
                case BladeEngine::EDATA_FORMAT_R8G8B8A8_SINT:
                    break;
                case BladeEngine::EDATA_FORMAT_R8G8B8A8_FLOAT:
                    break;
                default:
                    break;
                }
                return result;
            }

            static GLenum GetPixelType(ETEX_FORMAT inFormat)
            {
                GLenum result = GL_NONE;
                ETEX_FORMAT_TYPE layout = (ETEX_FORMAT_TYPE)(inFormat & DATA_TYPE_MASK);
                switch (layout)
                {
                case BladeEngine::ETEX_FORMAT_TYPE_TYPELESS:
                    result = GL_NONE;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_UINT:
                    result = GL_UNSIGNED_INT;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_SINT:
                    result = GL_INT;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_FLOAT:
                    result = GL_FLOAT;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_UNORM:
                    result = GL_UNSIGNED_NORMALIZED;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_SNORM:
                    result = GL_SIGNED_NORMALIZED;
                    break;
                case BladeEngine::ETEX_FORMAT_TYPE_INTERNAL:
                default:
                    break;
                }
                return result;
            }

            static uint32 GetPixelTypeSize(ETEX_FORMAT inFormat)
            {
                uint32 result = 0;
                switch (inFormat)
                {
                case BladeEngine::EDATA_FORMAT_R8G8B8A8_UINT:
                    result = 32;
                    break;
                default:
                    break;
                }
                return result;
            }
        };
    }
}
#endif // !__BLADE_OPENGL_ENUM_MAPPING_H__
