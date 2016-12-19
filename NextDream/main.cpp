#include <DirectX11Device.h>
#include <stdio.h>
#include <BVector.h>
#include <RHIBufferBase.h>
#include <PlatformAPI.h>
#include <RHITextureBase.h>
#include <RHIShaderBase.h>
#include <BArchive.h>
#include <EnumDefine.h>

const TCHAR* s_ShaderFileName = "../Media/Shader/VertexShader.shader";

using namespace BladeEngine;
void main()
{
    Framework::GlobalConfig::GetInstance().DirectX11DllPath = TEXT("C:\\Windows\\System32\\d3d11.dll");

    IRHIModule* module = new DirectX11RHIModule();
    
    bool result = module->Load(Framework::GlobalConfig::GetInstance().DirectX11DllPath);
    if (!result)
    {
        return;
    }

    result = module->StartUp();
    if (result)
    {
        for(uint32 i = 0; i < module->GetAdapterNum(); ++i)
        {
            ::printf(module->GetAdapterName(i));
        }
        uint32 adapterIndex = 0;
        ::scanf_s("%d", &adapterIndex);

        if (module->InitDevice(adapterIndex))
        {
            ::printf("InitDevice Success!");

            IRHIDevice* device = module->GetDevice();

            BVector3 vertexs[] =
            {
				{ 0.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
            };

            RHIVertexBufferCreateInfo info;
            info.CanCpuWrite = false;
            info.Data = vertexs;
            info.DataSize = sizeof(vertexs);
            info.VertexNum = 3;

            RHIVertexBufferRef vertexBuffer = device->CreateVertexBuffer(info);
            if(vertexBuffer != NULL)
            {
                ::printf("Create VertexBuffer Success!");
                vertexBuffer->AddVertexDeclaration(ESHADER_SEMANTIC_POSITION, 0, EDATA_FORMAT_FLOAT3, 0);
            }

            PlatformWindowRef window = PlatformAPI::CreatePlatformWindow("Test", 0, 0, 800, 600);
            if (window != NULL)
            {
                ::printf("Create Window Success!");
            }

            RHISwapChainCreateInfo swapChainCreateInfo;
            swapChainCreateInfo.BufferNum = 2;
            swapChainCreateInfo.RefreshRateDenominator = 60;
            swapChainCreateInfo.RefreshRateNumerator = 1;
            swapChainCreateInfo.SampleCount = 1;
            swapChainCreateInfo.SampleQulity = 0;
            swapChainCreateInfo.Window = window;

            RHISwapChainRef swapChain = device->CreateSwapChain(swapChainCreateInfo);
            if (swapChain != NULL)
            {
                ::printf("Create SwapChain Success!");
            }

            RHIImmediateContextRef immediateContext = device->GetImmediateContext();
            if (immediateContext != NULL)
            {
                ::printf("Create immediateContext Success!");
            }

            RHIShaderInputTable inputTable;
            RHIShaderResourceTable resoureceTable;
            RHIVertexShaderRef vertexShader = NULL;
            RHIPixelShaderRef pixelShader = NULL;

            HFile fileHandle = PlatformAPI::OpenFile(s_ShaderFileName, EFILE_READ, EFILE_SHARE_READ_WRITE, EFILE_OPEN_EXISTING);
            if (PlatformAPI::CheckHFileValid(fileHandle))
            {
                FileReader fileReader(fileHandle);
                const uint8* content = (const uint8*)fileReader.ReadAll();
                if (content != NULL)
                {
                    fileReader >> inputTable >> resoureceTable;
                    if(fileReader)
                    {
                        RHIShaderCreateInfo createInfo;
                        fileReader >> createInfo.DataSize;
                        createInfo.Data = content + fileReader.Tell();
                        fileReader.Seek(ESEEK_POS_CUR, createInfo.DataSize);

                        vertexShader = device->CreateVextexShader(createInfo);
                        vertexShader->SetInputTable(&inputTable);
                        vertexShader->SetResourceTable(&resoureceTable);

                        fileReader >> createInfo.DataSize;
                        createInfo.Data = content + fileReader.Tell();
                        fileReader.Seek(ESEEK_POS_CUR, createInfo.DataSize);

                        pixelShader = device->CreatePixelShader(createInfo);
                        pixelShader->SetResourceTable(&resoureceTable);
                    }
                }
            }
            PlatformAPI::CloseFile(fileHandle);

            while (!window->IsClosed())
            {
                BColor color(0.0f, 0.0f, 0.0f, 1.0f);
                immediateContext->ClearRenderTarget(color);

                immediateContext->SetRenderTarget(swapChain->AsTexture());

                immediateContext->SetViewport(0, 0.0f, 0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f, 1.0f);

                immediateContext->SetVertexShader(vertexShader);
                
                immediateContext->SetPixelShader(pixelShader);

                immediateContext->SetVertexBuufer(vertexBuffer, sizeof(BVector3), 0);

                immediateContext->DrawAuto();

                swapChain->Present();

                window->Update();
            }
        }
        else
        {
            ::printf("InitDevice Failed!");
        }
        module->ShutDown();
    }

    module->Unload();
    return;
}