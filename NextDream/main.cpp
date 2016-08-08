#include <DirectX11Device.h>
#include <stdio.h>

using namespace BladeEngine;
void main()
{
    Framework::GlobalConfig::GetInstance().DirectX11DllPath = TEXT("..");

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