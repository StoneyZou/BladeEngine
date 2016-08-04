#ifndef __BLADE_FRAMEWORK_BMODULE_H__
#define __BLADE_FRAMEWORK_BMODULE_H__

namespace BladeEngine
{
    namespace FrameWork
    {
        struct IModule
        {
            virtual bool StartUp() = 0;
            virtual void ShutDowm() = 0;
        };
    }
}
#endif // !__BLADE_FRAMEWORK_BMODULE_H__