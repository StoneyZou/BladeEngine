#include <stdio.h>
#include <d3dcompiler.h>
#include <TypeDefine.h>
#include <StringUtil.h>
#include <EnumDefine.h>
#include <PlatformAPI.h>
#include <PlatformEnum.h>
#include <TArray.h>
#include <Direct3DShaderCompileModule.h>

namespace BladeEngine
{
    const char* DirectXName = "--Direct3D";
    const char* OpenGLName = "--OpenGL";

    const char* ShaderModel_5_0 = "--SM_5_0";
    const char* ShaderModel_4_0 = "--SM_4_0";
    const char* ShaderModel_3_0 = "--SM_3_0";
    const char* ShaderModel_2_0 = "--SM_2_0";

    int main(int argc, char *argv[])
    {
        if (argc < 2)
        {
            ::printf("one or more arguments invalid.");
            return -1;
        }

        uint32 arg0_len = StringUtil::Strlen(argv[0]);
        uint32 arg1_len = StringUtil::Strlen(argv[1]);

        E3D_GRAPHIC_API_TYPE graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
        if (StringUtil::Strcmp(argv[0], DirectXName, arg0_len) == 0)
        {
            graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
        }
        else if (StringUtil::Strcmp(argv[0], OpenGLName, arg0_len) == 0)
        {
            graphicAPI = E3D_GRAPHIC_API_OPENGL;
        }

        ESHADER_MODEL shaderModel = ESHADER_MODEL_2_0;
        if (StringUtil::Strcmp(argv[1], ShaderModel_5_0, arg1_len) == 0)
        {
            shaderModel = ESHADER_MODEL_5_0;
        }
        else if (StringUtil::Strcmp(argv[1], ShaderModel_4_0, arg1_len) == 0)
        {
            shaderModel = ESHADER_MODEL_4_0;
        }
        else if (StringUtil::Strcmp(argv[1], ShaderModel_3_0, arg1_len) == 0)
        {
            shaderModel = ESHADER_MODEL_3_0;
        }
        else if (StringUtil::Strcmp(argv[1], ShaderModel_2_0, arg1_len) == 0)
        {
            shaderModel = ESHADER_MODEL_2_0;
        }

        ShaderCompileModule* module = new Direct3DShaderCompileModule();
        if (module->Load("C:/Windows/System32/D3DCompiler_43.dll"))
        {
            if (module->StartUp())
            {
                module->Compile(argv[2], shaderModel);
            }
            module->ShutDown();
        }
        module->Unload();

        delete module;
        return 0;
    }
}