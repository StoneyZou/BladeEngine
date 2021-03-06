#ifndef __BLADE_CODE_ENUM_DEFINE_H__
#define __BLADE_CODE_ENUM_DEFINE_H__

namespace BladeEngine
{
    enum ESEEK_POS
    {
        ESEEK_POS_BEGIN,
        ESEEK_POS_CUR,
        ESEEK_POS_END,
    };

    enum E3D_GRAPHIC_API_TYPE
    {
        E3D_GRAPHIC_API_DIRECT3D,
        E3D_GRAPHIC_API_OPENGL,
    };

    enum ESHADER_MODEL
    {
        ESHADER_MODEL_5_0,
        ESHADER_MODEL_4_0,
        ESHADER_MODEL_3_0,
        ESHADER_MODEL_2_0,
    };
}

#endif // !__BLADE_CODE_ENUM_DEFINE_H__
