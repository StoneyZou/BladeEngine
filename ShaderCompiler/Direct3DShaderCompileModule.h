#ifndef __BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__
#define __BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__

#include <d3dcompiler.h>
#include <ShaderCompileModule.h>
#include <PlatformAPI.h>
#include <BArchive.h>
#include <d3d11.h>
#include <RHIShaderBase.h>
#include <DirectXEnumMapping.h>

namespace BladeEngine
{
    class Direct3DShaderCompileModule : public ShaderCompileModule
    {
    private:
        typedef HRESULT (WINAPI* pD3DReflect)(LPCVOID pSrcData, SIZE_T SrcDataSize, REFIID pInterface, void** ppReflector);

    private:
        const TCHAR* s_D3DCompileFuncName = TEXT("D3DCompile");
        const TCHAR* s_D3DReflectFuncName = TEXT("D3DReflect");

        const char* VS_EntryPoint = "vs_main";
        const char* PS_EntryPoint = "ps_main";
        const char* DS_EntryPoint = "ds_main";
        const char* HS_EntryPoint = "hs_main";
        const char* GS_EntryPoint = "gs_main";

        const char* VS_Target_5_0 = "vs_5_0";
        const char* VS_Target_4_0 = "vs_4_0";
        const char* VS_Target_3_0 = "vs_3_0";
        const char* VS_Target_2_0 = "vs_2_0";

        const char* PS_Target_5_0 = "ps_5_0";
        const char* PS_Target_4_0 = "ps_4_0";
        const char* PS_Target_3_0 = "ps_3_0";
        const char* PS_Target_2_0 = "ps_2_0";

    private:
        ModuleHandle m_ModuleHandle;
        pD3DCompile m_D3DCompileFunc;
        pD3DReflect m_D3DReflectFunc;

    public:
        Direct3DShaderCompileModule() : ShaderCompileModule(ESHADER_COMPILE_DIRECT3D), 
            m_D3DCompileFunc(NULL)
        {}

    public:
        virtual bool Load(const BString& inFileName)
        {
            m_ModuleHandle = PlatformAPI::LoadBaseModule(inFileName);
            if (!PlatformAPI::CheckModuleHandleValid(m_ModuleHandle))
            {
                return false;
            }

            m_D3DCompileFunc = (pD3DCompile)PlatformAPI::GetProcAddress(m_ModuleHandle, s_D3DCompileFuncName);
            if (m_D3DCompileFunc == NULL)
            {
                return false;
            }

            m_D3DReflectFunc = (pD3DReflect)PlatformAPI::GetProcAddress(m_ModuleHandle, s_D3DReflectFuncName);
            if (m_D3DReflectFunc == NULL)
            {
                return false;
            }

            return true;
        }

        virtual bool StartUp() { return true; }
        virtual void ShutDown() {}

        virtual void Unload()
        {
            PlatformAPI::FreeBaseModule(m_ModuleHandle);
        }

    public:
        virtual bool Compile(const ANSICHAR* inFilename, ESHADER_MODEL inShaderModel)
        {
            FileHandle fileHandle = PlatformAPI::OpenFile(inFilename, EFILE_READ, EFILE_SHARE_READ_WRITE, EFILE_OPEN_EXISTING);
            if (PlatformAPI::CheckFileHandleValid(fileHandle))
            {
                FileReader fileReader(fileHandle, 0);

                const void* content = fileReader.ReadAll();
                if (content != NULL)
                {
                    const char* vs_target = "";
                    switch (inShaderModel)
                    {
                    case BladeEngine::ESHADER_MODEL_5_0:
                        vs_target = VS_Target_5_0;
                        break;
                    case BladeEngine::ESHADER_MODEL_4_0:
                        vs_target = VS_Target_4_0;
                        break;
                    case BladeEngine::ESHADER_MODEL_3_0:
                        vs_target = VS_Target_3_0;
                        break;
                    case BladeEngine::ESHADER_MODEL_2_0:
                        vs_target = VS_Target_2_0;
                        break;
                    default:
                        //log
                        break;
                    }

                    ID3DBlob* data = NULL, *error = NULL;
                    HRESULT hr = m_D3DCompileFunc(content, fileReader.GetFileSize(), inFilename, NULL, NULL, VS_EntryPoint, vs_target, D3DCOMPILE_DEBUG, 0, &data, &error);
                    ComPtrGuard(data);
                    ComPtrGuard(error);

                    if (FAILED(hr))
                    {
                        ::printf("DirectX Compile Vertex Shader Error.");
                        return false;
                    }

                    ID3D11ShaderReflection* reflection = NULL;
                    hr = m_D3DReflectFunc(data->GetBufferPointer(), data->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
                    ComPtrGuard(reflection);

                    if (FAILED(hr))
                    {
                        ::printf("DirectX Reflect Vertex Shader Error.");
                        return false;
                    }

                    uint32 inputParamIndex = 0;
                    D3D11_SIGNATURE_PARAMETER_DESC inputParamDesc = { 0 };
                    RHI::RHIShaderInputTable inputTable;

                    while (reflection->GetInputParameterDesc(inputParamIndex++, &inputParamDesc) == S_OK)
                    {
                        inputTable.AddInputBindDesc(
                            RHI::DirectXEnumMapping::GetSematicType(inputParamDesc.SemanticName),
                            inputParamDesc.SemanticIndex,
                            inputParamDesc.Register);
                    }

                    uint32 resourceIndex = 0;
                    D3D11_SHADER_INPUT_BIND_DESC resourceDesc = { 0 };
                    RHI::RHIShaderResourceTable resourceTable;

                    struct _cbuffer_index
                    {
                        ID3D11ShaderReflectionConstantBuffer* cbuffer;
                        SIZE_T index;
                    };
                    TArray<_cbuffer_index> cbuffer_indexs;

                    while (reflection->GetResourceBindingDesc(resourceIndex++, &resourceDesc) == S_OK)
                    {
                        switch (resourceDesc.Type)
                        {
                        case D3D_SIT_CBUFFER:
                        case D3D_SIT_TBUFFER:
                            ID3D11ShaderReflectionConstantBuffer* cbuffer = reflection->GetConstantBufferByName(resourceDesc.Name);
                            if (cbuffer != NULL)
                            {
                                D3D11_SHADER_BUFFER_DESC cbufferDesc = { 0 };
                                if (cbuffer->GetDesc(&cbufferDesc) == S_OK)
                                {
                                    SIZE_T uniformBufferIndex = resourceTable.AddUniformBufferDesc(resourceDesc.Name, resourceDesc.BindPoint, cbufferDesc.Size);
                                    _cbuffer_index cbuffer_index = { cbuffer,uniformBufferIndex };

                                    cbuffer_indexs.Add(cbuffer_index);
                                }
                            }
                            break;
                        case D3D_SIT_TEXTURE:
                            resourceTable.AddResourceBindDesc(resourceDesc.Name, resourceDesc.BindPoint, resourceDesc.BindCount, RHI::ESHADER_RESOURCE_TEXTURE);
                            break;
                        case D3D_SIT_SAMPLER:
                            resourceTable.AddResourceBindDesc(resourceDesc.Name, resourceDesc.BindPoint, resourceDesc.BindCount, RHI::ESHADER_RESOURCE_SAMPLER);
                            break;
                        default:
                            break;
                        }
                    }

                    resourceTable.BuildUniformBuffers();
                    for (uint32 i = 0; i < cbuffer_indexs.Size(); ++i)
                    {
                        _cbuffer_index cbuffer_index = cbuffer_indexs[i];

                        uint32 varIndex = 0;
                        ID3D11ShaderReflectionVariable* variable = NULL;

                        while ((variable = cbuffer_index.cbuffer->GetVariableByIndex(varIndex++)) != NULL)
                        {
                            D3D11_SHADER_VARIABLE_DESC variableDesc = { 0 };
                            ID3D11ShaderReflectionType* type = variable->GetType();
                            D3D11_SHADER_TYPE_DESC typeDesc;

                            if (variable->GetDesc(&variableDesc) == S_OK && type != NULL && type->GetDesc(&typeDesc) == S_OK)
                            {
                                if (typeDesc.Elements != 0)
                                {
                                    PlatformAPI::PrintToConsole("Not Support Array Variable!!!");
                                    continue;
                                }
                                else if (typeDesc.Members != 0)
                                {
                                    PlatformAPI::PrintToConsole("Not Support Struct Variable");
                                    continue;
                                }

                                if (typeDesc.Type != D3D_SVT_INT &&
                                    typeDesc.Type != D3D_SVT_FLOAT)
                                {
                                    PlatformAPI::PrintToConsole("Not Support Types);
                                }

                                resourceTable.AddAttributionDesc(
                                    variableDesc.Name,
                                    cbuffer_index.index,
                                    variableDesc.StartOffset,
                                    variableDesc.Size,
                                    RHI::DirectXEnumMapping::GetAttrType(typeDesc.Type),
                                    typeDesc.Rows,
                                    typeDesc.Columns,
                                    variableDesc.DefaultValue);
                            }
                        }
                    }
                }
            }

            PlatformAPI::CloseFile(fileHandle);
        }
    };
}

#endif // !__BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__
