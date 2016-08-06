#ifndef __BLADE_FRAMEWORK_BMODULE_H__
#define __BLADE_FRAMEWORK_BMODULE_H__

namespace BladeEngine
{
    namespace Framework
    {
        struct IModule
        {
            virtual bool Load() = 0;
            virtual bool StartUp() = 0;
            virtual void ShutDowm() = 0;
            virtual void Unload() = 0;
        };
    }
}
#endif // !__BLADE_FRAMEWORK_BMODULE_H__