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
            static GLenum GetPixelFormat(EDATA_FORMAT inFormat)
            {
                GLenum result = GL_RGBA;
                EDATA_LAYOUT_DESC layout = (EDATA_LAYOUT_DESC)(inFormat >> DATA_LAYOUT_SHIFT);
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

            static GLenum GetPixelType(EDATA_FORMAT inFormat)
            {
                GLenum result = GL_NONE;
                EDATA_ELEMENT_TYPE layout = (EDATA_ELEMENT_TYPE)(inFormat & DATA_TYPE_MASK);
                switch (layout)
                {
                case BladeEngine::EDATA_ELEMENT_TYPELESS:
                    result = GL_NONE;
                    break;
                case BladeEngine::EDATA_ELEMENT_UINT:
                    result = GL_UNSIGNED_INT;
                    break;
                case BladeEngine::EDATA_ELEMENT_SINT:
                    result = GL_INT;
                    break;
                case BladeEngine::EDATA_ELEMENT_FLOAT:
                    result = GL_FLOAT;
                    break;
                case BladeEngine::EDATA_ELEMENT_UNORM:
                    result = GL_UNSIGNED_NORMALIZED;
                    break;
                case BladeEngine::EDATA_ELEMENT_SNORM:
                    result = GL_SIGNED_NORMALIZED;
                    break;
                case BladeEngine::EDATA_ELEMENT_INTERNAL:
                default:
                    break;
                }
                return result;
            }

            static uint32 GetPixelTypeSize(EDATA_FORMAT inFormat)
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
