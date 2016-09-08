#ifndef __BLADE_CORE_BPATH_H__
#define __BLADE_CORE_BPATH_H__

#include <TypeDefine.h>
#include <BString.h>

namespace BladeEngine
{
    class BPath
    {
    public:
        static BString GetFilenameWithoutExt(const BString& inFilename)
        {
            SIZE_T index = 0;
            if ((index = inFilename.IndexOf(TEXT('.')) != -1)
            {
                return inFilename.SubStr(0, index);
            }
            return "";
        }

        static BString Combine(const BString& inStr1, const BString& inStr2)
        {
            return inStr1 + "/" + inStr2;
        }
    };
}

#endif  //!__BLADE_CORE_BPATH_H__
