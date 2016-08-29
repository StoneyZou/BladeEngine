#ifndef __BLADE_SHADER_COMPILE_MODULE_H__
#define __BLADE_SHADER_COMPILE_MODULE_H__

#include <BModule.h>
#include <EnumDefine.h>

namespace BladeEngine
{
    enum ESHADER_COMPILE_SUBTYPE
    {
        ESHADER_COMPILE_DIRECT3D,
    };

    class ShaderCompileModule : public Framework::IModule
    {
    public:
        ShaderCompileModule(ESHADER_COMPILE_SUBTYPE inSubType) : IModule(EMODULE_SHADER_COMPILE, inSubType)
        {}

    public:
        virtual bool Compile(const ANSICHAR* inFilename, ESHADER_MODEL inShaderModel) = 0;
    };
}

#endif // !__BLADE_SHADER_COMPILE_MODULE_H__