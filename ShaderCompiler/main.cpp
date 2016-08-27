#include <stdio.h>
#include <d3dcompiler.h>
#include <TypeDefine.h>
#include <StringUtil.h>

const char* DirectXName = "--Direct3D";
const char* OpenGLName = "--OpenGL";

const char* ShaderModel_5_0 = "--SM_5_0";
const char* ShaderModel_4_0 = "--SM_4_0";
const char* ShaderModel_3_0 = "--SM_3_0";
const char* ShaderModel_2_0 = "--SM_2_0";

const char* VS_EntryPoint = "vs_main";
const char* PS_EntryPoint = "ps_main";
const char* DS_EntryPoint = "ds_main";
const char* HS_EntryPoint = "hs_main";
const char* GS_EntryPoint = "gs_main";

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        ::printf("one or more arguments invalid.");
        return -1;
    }

    E3D_GRAPHIC_API_TYPE graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
    if (StringUtil::Strcmp(argv[0], DirectXName) == 0)
    {
        graphicAPI = E3D_GRAPHIC_API_DIRECT3D;
    }
    else if(StringUtil::Strcmp(argv[0], OpenGLName) == 0)
    {
        graphicAPI = E3D_GRAPHIC_API_OPENGL;
    }

    ESHADER_MODEL shaderModel = RHI::ESHADER_MODEL_2_0;
    if (StringUtil::Strcmp(argv[1], ShaderModel_5_0) == 0)
    {
        graphicAPI = ESHADER_MODEL_5_0;
    }
    else if (StringUtil::Strcmp(argv[1], ShaderModel_4_0) == 0)
    {
        graphicAPI = ESHADER_MODEL_4_0;
    }
    else if (StringUtil::Strcmp(argv[1], ShaderModel_3_0) == 0)
    {
        graphicAPI = ESHADER_MODEL_3_0;
    }
    else if (StringUtil::Strcmp(argv[1], ShaderModel_2_0) == 0)
    {
        graphicAPI = ESHADER_MODEL_2_0;
    }

    D3DCompileFromFile(argv[3], NULL, NULL, "vs_main", )
}