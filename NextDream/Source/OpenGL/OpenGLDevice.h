#pragma once

#include <RHIDevice.h>

namespace BladeEngine
{
    class OpenGLRHIModule : public IRHIModule
    {
    private:


    public:
        OpenGLRHIModule() : m_(NULL), IRHIModule(EMODULE_RHI_DIRECTX11)
        {
        }

    public:
        virtual bool Load(const BString& inFileName);

        virtual bool StartUp();

        virtual void ShutDown();

        virtual void Unload();

        virtual bool InitDevice(uint32 inAdapterIndex);

        //virtual void SwitchAdapterByIndex(uint32 inIndex) const = 0;
    };
}