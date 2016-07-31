#ifndef __BLADE_MATH_BCOLOR_H__
#define __BLADE_MATH_BCOLOR_H__

namespace BladeEngine
{
    struct BColor
    {
    public:
        union
        {
            float r, g, b, a;
            float _data[4];
        };

    public:
        BColor(float inR) : r(inR), g(1.0f), b(1.0f), a(1.0f)
        {}

        BColor(float inR, float inG) : r(inR), g(inG), b(1.0f), a(1.0f)
        {}

        BColor(float inR, float inG, float inB) : r(inR), g(inG), b(inB), a(1.0f)
        {}

        BColor(float inR, float inG, float inB, float inA) : r(inR), g(inG), b(inB), a(inA)
        {}

        BColor(const BColor& rh) : r(rh.r), g(rh.g), b(rh.b), a(rh.a)
        {}

    public:
        BColor& operator = (const BColor& rh)
        {
            r = rh.r;
            g = rh.g;
            b = rh.b;
            a = rh.a;
        }

        bool operator == (const BColor& rh)
        {
            return r == rh.r && g == rh.g && b == rh.b && a == rh.a;
        }

        bool operator != (const BColor& rh)
        {
            return r != rh.r || g != rh.g || b != rh.b || a != rh.a;
        }

        BColor operator + (const BColor& rh) const
        {
            float deno = 1.0f / (a + rh.a - a * rh.a);
            float fact1 = a * (1 - rh.a) * deno;
            float fact2 = rh.a * deno;

            return BColor(
                r * fact1 + rh.r * fact2,
                b * fact1 + rh.b * fact2,
                g * fact1 + rh.g * fact2,
                a * fact1 + rh.a * fact2);
        }

        BColor operator += (const BColor& rh)
        {
            float deno = 1.0f / (a + rh.a - a * rh.a);
            float fact1 = a * (1 - rh.a) * deno;
            float fact2 = rh.a * deno;

            r = r * fact1 + rh.r * fact2,
            b = b * fact1 + rh.b * fact2,
            g = g * fact1 + rh.g * fact2,
            a = a * fact1 + rh.a * fact2;
        }

        BColor operator * (const BColor& rh) const
        {
            return BColor(r * rh.r, b * rh.b, g * rh.g, a * rh.a);
        }

        BColor operator *= (const BColor& rh)
        {
            r *= rh.r;
            b *= rh.b;
            g *= rh.g;
            a *= rh.a;
        }
    };
}

#endif // !__BLADE_MATH_BCOLOR_H__