#ifndef FLOATCOMPARISON_H
#define FLOATCOMPARISON_H

#include <stdint.h> // For int32_t, etc.
#include <cmath>

union Float_t
{
    Float_t(double num = 0.0) : f(num) {}
    // Portable extraction of components.
    bool Negative() const { return i < 0; }
    int32_t RawMantissa() const { return i & ((1 << 23) - 1); }
    int32_t RawExponent() const { return (i >> 23) & 0xFF; }

    int32_t i;
    float f;
#ifdef _DEBUG
    struct
    {   // Bitfields for exploration. Do not use in production code.
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
#endif
};

bool almostEqualUlps(float A, float B, int maxUlpsDiff=2);
#endif
