#ifndef __BLADE_CORE_GUID_H__
#define __BLADE_CORE_GUID_H__

#include <TypeDefine.h>
#include <MemUtil.h>

namespace BladeEngine
{
    struct Guid
    {
    public:
        union
        {
            uint32 Value[4];
            uint32 A, B, C, D;
        };

    public:
        Guid(uint32 a, uint32 b, uint32 c, uint32 d) : A(a), B(b), C(c), D(d) 
        {}
        Guid(const void* inValue)
        {
            MemUtil::Memcopy(Value, sizeof(Value), inValue, sizeof(Value));
        }

        Guid(const Guid& rl) : A(rl.A), B(rl.B), C(rl.C), D(rl.D) 
        {}

        Guid& operator = (const Guid& rl)
        {
            MemUtil::Memcopy(Value, sizeof(Value), rl.Value, sizeof(Value));
        }

        bool operator == (const Guid& rl)
        {
            return ::MemMisc::Memcmp(Value, rl.Value, sizeof(Value)) == 0;
        }

        bool operator != (const Guid& rl)
        {
            return ::MemMisc::Memcmp(Value, rl.Value, sizeof(Value)) != 0;
        }
    };
}

#endif // !__BLADE_CORE_GUID_H__
