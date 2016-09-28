#ifndef __BLADE_RHI_UTILITY_H__
#define __BLADE_RHI_UTILITY_H__

#include <RHIEnum.h>

namespace BladeEngine
{
    struct RHIUtility
    {
        static bool CanAsShadowResource(ETEX_FORMAT inFormat, ECPU_GPU_USAGE_MODE inUsageMode)
        {
            return RHIDataFormat::CheckDateUsage(inFormat, ETEX_FORMAT_USAGE_SHADER_RESOURCE) && (
                inUsageMode == ESUIT_GPU_READ || inUsageMode == ESUIT_GPU_READ_WRITE);
        }

        static bool CanAsRenderTarget(ETEX_FORMAT inFormat, ECPU_GPU_USAGE_MODE inUsageMode)
        {
            return RHIDataFormat::CheckDateUsage(inFormat, ETEX_FORMAT_USAGE_RENDER_TARGET) && (
                inUsageMode == ESUIT_GPU_WRITE || inUsageMode == ESUIT_GPU_READ_WRITE);
        }

        static bool CanAsDepthStencil(ETEX_FORMAT inFormat, ECPU_GPU_USAGE_MODE inUsageMode)
        {
            return RHIDataFormat::CheckDateUsage(inFormat, ETEX_FORMAT_USAGE_DEPTH_STENCIL) && (
                    inUsageMode == ESUIT_GPU_WRITE );
        }

        static bool CheckUsageMode(ETEX_FORMAT inFormat, ECPU_GPU_USAGE_MODE inUsageMode)
        {
            return RHIDataFormat::CheckDateUsage(inFormat, ETEX_FORMAT_USAGE_DEPTH_STENCIL) && (
                inUsageMode == ESUIT_GPU_WRITE);
        }
    };
}

#endif // !__BLADE_RHI_UTILITY_H__
