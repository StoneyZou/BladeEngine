#ifndef __BLADE_MATH_BMATH_H__
#define __BLADE_MATH_BMATH_H__

namespace BladeEngine
{
    class Math
    {
    public:
        template<typename Type>
        static Type Max(Type lh, Type rh)
        {
            return lh < rh ? rh : lh;
        }

        template<typename Type>
        static Type Min(Type lh, Type rh)
        {
            return lh < rh ? lh : rh;
        }
    };
}

#endif // !__BLADE_MATH_BMATH_H__