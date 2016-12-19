#ifndef __BLADE_MATH_BVECTOR3_H__
#define __BLADE_MATH_BVECTOR3_H__

namespace BladeEngine
{
    struct BVector3
    {
        float x, y, z;

        BVector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ)
        {
            // do nothing
        }

        BVector3 operator + ( BVector3 rh ) const
        {
            return BVector3(x + rh.x, y + rh.y, z + rh.z);
        }

        static float Dot(BVector3 lh, BVector3 rh)
        {
            return lh.x * rh.x + lh.y * rh.y + lh.z * rh.z;
        }

        static BVector3 Cross(BVector3 lh, BVector3 rh)
        {
            return BVector3(
                lh.y * rh.z - lh.z * rh.y,
                lh.x * rh.z - lh.z * rh.x,
                lh.x * rh.y - lh.y * rh.x
            );
        }
    };
}

#endif // !__BLADE_MATH_BVECTOR3_H__