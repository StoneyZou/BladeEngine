#include <DirectX11Device.h>
#include <stdio.h>
#include <BVector.h>
#include <RHIBufferBase.h>
#include <d3d11.h>
#include <SystemAPI.h>
#include <RHITextureBase.h>
#include <RHIShaderBase.h>

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

            RHI::IRHIDevice* device = module->GetDevice();

            BVector3 vertexs[] =
            {
                { 0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 1.0f, 0.0f },
            };

            RHI::RHIVertexBufferCreateInfo info;
            info.CanCpuWrite = false;
            info.Data = vertexs;
            info.DataSize = sizeof(vertexs);
            info.VertexNum = 3;

            RHI::RHIVertexBufferRef vertexBuffer = device->CreateVertexBuffer(info);
            if(vertexBuffer != NULL)
            {
                ::printf("Create VertexBuffer Success!");
                vertexBuffer->AddVertexDeclaration(RHI::ESHADER_SEMANTIC_POSITION, 0, RHI::EDATA_FORMAT_R32G32B32A32_FLOAT, 0);
            }

            PlatformWindowRef window = SystemAPI::CreatePlatformWindow("Test", 0, 0, 800, 600);
            if (window != NULL)
            {
                ::printf("Create Window Success!");
            }

            RHI::RHISwapChainCreateInfo swapChainCreateInfo;
            swapChainCreateInfo.BufferNum = 2;
            swapChainCreateInfo.RefreshRateDenominator = 60;
            swapChainCreateInfo.RefreshRateNumerator = 1;
            swapChainCreateInfo.SampleCount = 1;
            swapChainCreateInfo.SampleQulity = 0;
            swapChainCreateInfo.Window = window;

            RHI::RHISwapChainRef swapChain = device->CreateSwapChain(swapChainCreateInfo);
            if (swapChain != NULL)
            {
                ::printf("Create SwapChain Success!");
            }

            RHI::RHIImmediateContextRef immediateContext = device->GetImmediateContext();
            if (immediateContext != NULL)
            {
                ::printf("Create immediateContext Success!");
            }

            while (!window->IsClosed())
            {
                immediateContext->SetRenderTarget(swapChain->AsTexture());

                immediateContext->SetViewport(0, 0.0f, 0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 1.0f);

                BColor color(1.0f, 1.0f, 0.0f, 1.0f);
                immediateContext->ClearRenderTarget(color);

                swapChain->Present();

                window->Update();
            }
            int i = 0;
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