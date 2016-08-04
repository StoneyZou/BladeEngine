#include <Memory.h>
#include <Utility.h>

namespace BladeEngine
{
    SystemMalloc SystemMalloc::s_Instance;

    void _BladeAssert_Impl(bool inCondition, const char* inMessage, const char* inFile, int inLine)
    {
    }
}