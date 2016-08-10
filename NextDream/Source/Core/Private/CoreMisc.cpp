#include <Memory.h>
#include <Utility.h>

namespace BladeEngine
{
    IMalloc* Malloc::s_RegisterMalloc = NULL;

    void _BladeAssert_Impl(bool inCondition, const char* inMessage, const char* inFile, int inLine)
    {
    }
}