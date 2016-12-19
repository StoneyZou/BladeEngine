#ifndef __BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__
#define __BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__

#include <d3dcompiler.h>
#include <ShaderCompileModule.h>
#include <PlatformAPI.h>
#include <BArchive.h>
#include <d3d11.h>
#include <RHIShaderBase.h>
#include <DirectXEnumMapping.h>
#include <BPath.h>

namespace BladeEngine
{
    class _ComPtrGuard
    {
    private:
        IUnknown* m_Ptr;

    public:
        _ComPtrGuard(IUnknown* inPtr) : m_Ptr(m_Ptr)
        {
        }

        ~_ComPtrGuard()
        {
            if (m_Ptr != NULL)
            {
                m_Ptr->Release();
            }
        }
    };

    #define ComPtrGuard(ptr) _ComPtrGuard ptr##Guard(ptr)

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
        HModule m_ModuleHandle;
        pD3DCompile m_D3DCompileFunc;
        pD3DReflect m_D3DReflectFunc;

        ID3DBlob* m_VSData;
        ID3DBlob* m_DSData;
        ID3DBlob* m_HSData;
        ID3DBlob* m_GSData;
        ID3DBlob* m_PSData;

        RHIShaderInputTable m_InputTable;
        RHIShaderResourceTable m_ResourceTable;

    public:
        Direct3DShaderCompileModule() : ShaderCompileModule(ESHADER_COMPILE_DIRECT3D), 
            m_D3DCompileFunc(NULL)
        {}

    public:
        virtual bool Load(const BString& inFileName)
        {
            m_ModuleHandle = PlatformAPI::LoadBaseModule(inFileName);
            if (!PlatformAPI::CheckHModuleValid(m_ModuleHandle))
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
            HFile fileHandle = PlatformAPI::OpenFile(inFilename, EFILE_READ, EFILE_SHARE_READ_WRITE, EFILE_OPEN_EXISTING);
            if (PlatformAPI::CheckHFileValid(fileHandle))
            {
                FileReader fileReader(fileHandle);

                const void* content = fileReader.ReadAll();
                if (content != NULL)
                {
                    if (!CompileVS(inShaderModel, content, fileReader.GetFileSize(), inFilename))
                        return false;
                    if (!CompilePS(inShaderModel, content, fileReader.GetFileSize(), inFilename))
                        return false;
                    if (!ReflectDescriptionTable())
                        return false;
                    if (!OutputToFile(inFilename))
                        return false;
                }
            }
            PlatformAPI::CloseFile(fileHandle);
            return true;
        }

    private:
        bool CompileVS(ESHADER_MODEL inShaderModel, const void* inContext, SIZE_T inContextSize, const ANSICHAR* inFileName)
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

            ID3DBlob* error = NULL;
            HRESULT hr = m_D3DCompileFunc(inContext, inContextSize, inFileName, NULL, NULL, VS_EntryPoint, vs_target,
                D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_ENABLE_STRICTNESS
                , 0, &m_VSData, &error);
            ComPtrGuard(m_VSData);
            ComPtrGuard(error);

            if (FAILED(hr))
            {
                ::printf("DirectX Compile Vertex Shader Error.");
                return false;
            }

            m_VSData->AddRef();
            return true;
        }

        bool CompilePS(ESHADER_MODEL inShaderModel, const void* inContext, SIZE_T inContextSize, const ANSICHAR* inFileName)
        {
            const char* ps_target = "";
            switch (inShaderModel)
            {
            case BladeEngine::ESHADER_MODEL_5_0:
                ps_target = PS_Target_5_0;
                break;
            case BladeEngine::ESHADER_MODEL_4_0:
                ps_target = PS_Target_4_0;
                break;
            case BladeEngine::ESHADER_MODEL_3_0:
                ps_target = PS_Target_3_0;
                break;
            case BladeEngine::ESHADER_MODEL_2_0:
                ps_target = PS_Target_2_0;
                break;
            default:
                //log
                break;
            }

            ID3DBlob* error = NULL;
            HRESULT hr = m_D3DCompileFunc(inContext, inContextSize, inFileName, NULL, NULL, PS_EntryPoint, ps_target,
                D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_ENABLE_STRICTNESS
                , 0, &m_PSData, &error);
            ComPtrGuard(m_PSData);
            ComPtrGuard(error);

            if (FAILED(hr))
            {
                ::printf("DirectX Compile Vertex Shader Error.");
                ::printf((char*)error->GetBufferPointer());
                return false;
            }

            m_PSData->AddRef();
            return true;
        }

        bool ReflectDescriptionTable()
        {
            ID3D11ShaderReflection* reflection = NULL;
            HRESULT hr = m_D3DReflectFunc(m_VSData->GetBufferPointer(), m_VSData->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);
            ComPtrGuard(reflection);

            if (FAILED(hr))
            {
                ::printf("DirectX Reflect Vertex Shader Error.");
                return false;
            }

            uint32 inputParamIndex = 0;
            D3D11_SIGNATURE_PARAMETER_DESC inputParamDesc = { 0 };

            while (reflection->GetInputParameterDesc(inputParamIndex++, &inputParamDesc) == S_OK)
            {
                m_InputTable.AddInputBindDesc(
                    DirectXEnumMapping::GetSematicType(inputParamDesc.SemanticName),
                    inputParamDesc.SemanticIndex,
                    inputParamDesc.Register);
            }

            uint32 resourceIndex = 0;
            D3D11_SHADER_INPUT_BIND_DESC resourceDesc = { 0 };

            struct _cbuffer_index
            {
                ID3D11ShaderReflectionConstantBuffer* cbuffer;
                SIZE_T index;
            };
            TArray<_cbuffer_index> cbuffer_indexs;

            ID3D11ShaderReflectionConstantBuffer* cbuffer = NULL;
            while (reflection->GetResourceBindingDesc(resourceIndex++, &resourceDesc) == S_OK)
            {
                switch (resourceDesc.Type)
                {
                case D3D_SIT_CBUFFER:
                case D3D_SIT_TBUFFER:
                    cbuffer = reflection->GetConstantBufferByName(resourceDesc.Name);
                    if (cbuffer != NULL)
                    {
                        D3D11_SHADER_BUFFER_DESC cbufferDesc = { 0 };
                        if (cbuffer->GetDesc(&cbufferDesc) == S_OK)
                        {
                            SIZE_T uniformBufferIndex = m_ResourceTable.AddUniformBufferDesc(resourceDesc.Name, resourceDesc.BindPoint, cbufferDesc.Size);
                            _cbuffer_index cbuffer_index = { cbuffer,uniformBufferIndex };

                            cbuffer_indexs.Add(cbuffer_index);
                        }
                    }
                    break;
                case D3D_SIT_TEXTURE:
                    m_ResourceTable.AddResourceBindDesc(resourceDesc.Name, resourceDesc.BindPoint, resourceDesc.BindCount, ESHADER_RESOURCE_TEXTURE);
                    break;
                case D3D_SIT_SAMPLER:
                    m_ResourceTable.AddResourceBindDesc(resourceDesc.Name, resourceDesc.BindPoint, resourceDesc.BindCount, ESHADER_RESOURCE_SAMPLER);
                    break;
                default:
                    break;
                }
            }

            m_ResourceTable.BuildUniformBuffers();
            for (uint32 i = 0; i < cbuffer_indexs.Size(); ++i)
            {
                _cbuffer_index cbuffer_index = cbuffer_indexs[i];

                uint32 varIndex = 0;
                ID3D11ShaderReflectionVariable* variable = NULL;

                while ((variable = cbuffer_index.cbuffer->GetVariableByIndex(varIndex++)) != NULL)
                {
                    D3D11_SHADER_VARIABLE_DESC variableDesc = { 0 };
                    D3D11_SHADER_TYPE_DESC typeDesc;
                    ID3D11ShaderReflectionType* type = variable->GetType();

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
                            PlatformAPI::PrintToConsole("Not Support Types");
                        }

                        m_ResourceTable.AddAttributionDesc(
                            variableDesc.Name,
                            cbuffer_index.index,
                            variableDesc.StartOffset,
                            variableDesc.Size,
                            DirectXEnumMapping::GetAttrType(typeDesc.Type),
                            typeDesc.Rows,
                            typeDesc.Columns,
                            variableDesc.DefaultValue);
                    }
                }
            }
            return true;
        }

        bool OutputToFile(const ANSICHAR* inFilename)
        {
            BString outFilename = BPath::GetFilenameWithoutExt(inFilename) + TEXT(".shader");
            HFile outFileHandle = PlatformAPI::OpenFileA(outFilename, EFILE_WRITE, EFILE_SHARE_WRITE, EFIlE_CREATE);

            if (!PlatformAPI::CheckHFileValid(outFileHandle))
            {
                return false;
            }

            FileWriter fileWriter(outFileHandle);
            fileWriter << m_InputTable;
            fileWriter << m_ResourceTable;
            fileWriter << (SIZE_T)m_VSData->GetBufferSize();
            fileWriter.Write((byte*)m_VSData->GetBufferPointer(), m_VSData->GetBufferSize());
            fileWriter << (SIZE_T)m_PSData->GetBufferSize();
            fileWriter.Write((byte*)m_PSData->GetBufferPointer(), m_PSData->GetBufferSize());
            fileWriter.Flush();

            PlatformAPI::CloseFile(outFileHandle);
            return true;
        }

        bool ClearData()
        {
            if (m_VSData != NULL) { m_VSData->Release(); m_VSData = NULL; }
            if (m_PSData != NULL) { m_PSData->Release(); m_PSData = NULL; }
        }
    };
}

#endif // !__BLADE_DIRECT3D_SHADER_COMPILE_MODULE_H__
