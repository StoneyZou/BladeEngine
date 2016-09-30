#ifndef __BLADE_RHI_DATA_FORMAT_H__
#define __BLADE_RHI_DATE_FORMAT_H__

#include <TypeDefine.h>
#include <RHIEnum.h>

namespace BladeEngine
{
    template<bool IsCompressed, uint32 _ByteWidth, ETEX_FORMAT_TYPE _Type>
    struct DataFormatBaseDesc
    {
        const bool IsCompressed = IsCompressed;
        const uint32 ByteWidth = _ByteWidth;
        const ETEX_FORMAT_TYPE Type = _Type;
    };

    template<ETEX_FORMAT _Format>
    struct DataFormatDesc : public DataFormatBaseDesc<false, 0, ETEX_FORMAT_TYPE_TYPELESS> {};

    #define AddDataFormatDesc(format, isCompressed, byteWidth, type) \
            template<>  \
            struct DataFormatDesc<format> : public DataFormatBaseDesc<isCompressed, byteWidth, type> {};

    AddDataFormatDesc(ETEX_FORMAT_R8G8B8A8_UINT, false, 32, ETEX_FORMAT_TYPE_UINT);
    AddDataFormatDesc(ETEX_FORMAT_R8G8B8A8_SINT, false, 32, ETEX_FORMAT_TYPE_SINT);
    AddDataFormatDesc(EDATA_FORMAT_R8G8B8A8_FLOAT, false, 32, ETEX_FORMAT_TYPE_FLOAT);
    AddDataFormatDesc(ETEX_FORMAT_R8G8B8A8_TYPELESS, false, 32, ETEX_FORMAT_TYPE_TYPELESS);
}

#endif // !__BLADE_RHI_DATA_FORMAT_H__