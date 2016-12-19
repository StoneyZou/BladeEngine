#ifndef __BLADE_MATH_BVECTOR2_H__
#define __BLADE_MATH_BVECTOR2_H__

namespace BladeEngine
{
    struct BVector2
    {
        union
        {
            struct { float x, y; };
            struct { float u, v; };
        }
    };
}

#endif // !__BLADE_MATH_BVECTOR2_H__
