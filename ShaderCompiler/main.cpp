#include <stdio.h>
#include <d3dcompiler.h>
#include <TypeDefine.h>
#include <StringUtil.h>
#include <EnumDefine.h>
#include <PlatformAPI.h>
#include <PlatformEnum.h>
#include <TArray.h>
#include <Direct3DShaderCompileModule.h>

using namespace BladeEngine;

const char* DirectXName = "--Direct3D";
const char* OpenGLName = "--OpenGL";

const char* ShaderModel_5_0 = "--SM_5_0";
const char* ShaderModel_4_0 = "--SM_4_0";
const char* ShaderModel_3_0 = "--SM_3_0";
const char* ShaderModel_2_0 = "--SM_2_0";

int main(int argc, const char *argv[])
{
    const char* _argv[] = { "", DirectXName, ShaderModel_5_0, "E:/BladeEngine/Media/Shader/VertexShader.hlsl" };
    argv = _argv;
    argc = 4;

    if (argc < 4)
    {
        ::printf("one or more arguments invalid.");
        return -1;
    }

    uint32 arg0_len = StringUtil::Strlen(argv[1]);
    uint32 arg1_len = StringUtil::Strlen(argv[2]);

    E3D_GRAPHIC_API_TYPE graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
    if (StringUtil::Strcmp(argv[1], DirectXName, arg0_len) == 0)
    {
        graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
    }
    else if (StringUtil::Strcmp(argv[1], OpenGLName, arg0_len) == 0)
    {
        graphicAPI = E3D_GRAPHIC_API_OPENGL;
    }

    ESHADER_MODEL shaderModel = ESHADER_MODEL_2_0;
    if (StringUtil::Strcmp(argv[2], ShaderModel_5_0, arg1_len) == 0)
    {
        shaderModel = ESHADER_MODEL_5_0;
    }
    else if (StringUtil::Strcmp(argv[2], ShaderModel_4_0, arg1_len) == 0)
    {
        shaderModel = ESHADER_MODEL_4_0;
    }
    else if (StringUtil::Strcmp(argv[2], ShaderModel_3_0, arg1_len) == 0)
    {
        shaderModel = ESHADER_MODEL_3_0;
    }
    else if (StringUtil::Strcmp(argv[2], ShaderModel_2_0, arg1_len) == 0)
    {
        shaderModel = ESHADER_MODEL_2_0;
    }

    ShaderCompileModule* module = new Direct3DShaderCompileModule();
    if (module->Load("../3rdParty/bin/DirectX/d3dCompiler_47.dll"))
    {
        if (module->StartUp())
        {
            module->Compile(argv[3], shaderModel);
        }
        module->ShutDown();
    }
    module->Unload();

    delete module;
    return 0;
}
