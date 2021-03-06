#include <DirectXEnumMapping.h>
#include <Utility.h>

namespace BladeEngine
{
    const ANSICHAR* SemanticStrs[] = {
        "BINORMAL",
        "BLENDINDICES",
        "BLENDWEIGHT",
        "COLOR",
        "NORMAL",
        "POSITION",
        "POSITIONT",
        "PSIZE",
        "TANGENT",
        "TEXCOORD",
    };

    const ESHADER_SEMANTIC_TYPE SemanticTypes[] = {
        ESHADER_SEMANTIC_BINORMAL,
        ESHADER_SEMANTIC_BLENDINDICES,
        ESHADER_SEMANTIC_BLENDWEIGHT,
        ESHADER_SEMANTIC_COLOR,
        ESHADER_SEMANTIC_NORMAL,
        ESHADER_SEMANTIC_POSITION,
        ESHADER_SEMANTIC_POSITIONT,
        ESHADER_SEMANTIC_PSIZE,
        ESHADER_SEMANTIC_TANGENT,
        ESHADER_SEMANTIC_TEXCOORD,
    };

    ESHADER_SEMANTIC_TYPE DirectXEnumMapping::GetSematicType(const ANSICHAR* inSemanticStr)
    {
        for (uint32 i = 0; i < countof(SemanticStrs); ++i)
        {
            SIZE_T len1 = StringUtil::Strlen(inSemanticStr);
            SIZE_T len2 = StringUtil::Strlen(SemanticStrs[i]);

            if (StringUtil::Strcmp(inSemanticStr, SemanticStrs[i], len1 < len2 ? len1 : len2) == 0)
            {
                return SemanticTypes[i];
            }
        }
        return ESHADER_SEMANTIC_UNKNOWN;
    }
}