#ifndef INCLUDED_MUSES_72320_MATH
#define INCLUDED_MUSES_72320_MATH

namespace Math
{
    template<typename T>
    constexpr auto clamp(const T& v, const T& min, const T& max) -> T
    {
        return v < min ? min : v > max ? max : v;
    }

    template<typename T>
    constexpr auto abs(const T& v) -> T
    {
        return v < 0 ? -v : v;
    }
}

#endif // INCLUDED_MUSES_72320_MATH
