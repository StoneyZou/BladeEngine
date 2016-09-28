#ifndef __BLADE_RHI_UTILITY_H__
#define __BLADE_RHI_UTILITY_H__

#include <RHIEnum.h>

namespace BladeEngine
{
    class RHIUtility
    {
        static bool CanAsRenderTarget(EDATA_FORMAT inFormat, ECPU_GPU_USAGE_MODE inUsageMode)
        {
            return inFormat == EDATA_FORMAT_R8G8B8A8_UINT && (inUsageMode == ESUIT_GPU_WRITE || inUsageMode == ESUIT_GPU_READ_WRITE);
        }
    };
}

#endif // !__BLADE_RHI_UTILITY_H__
