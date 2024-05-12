/**
 * @file    SimplexNoise.cpp
 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D).
 *
 * Copyright (c) 2014-2018 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * This C++ implementation is based on the speed-improved Java version 2012-03-09
 * by Stefan Gustavson (original Java source code in the public domain).
 * http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java:
 * - Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * - Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * - Better rank ordering method by Stefan Gustavson in 2012.
 *
 * This implementation is "Simplex Noise" as presented by
 * Ken Perlin at a relatively obscure and not often cited course
 * session "Real-Time Shading" at Siggraph 2001 (before real
 * time shading actually took on), under the title "hardware noise".
 * The 3D function is numerically equivalent to his Java reference
 * code available in the PDF course notes, although I re-implemented
 * it from scratch to get more readable code. The 1D, 2D and 4D cases
 * were implemented from scratch by me from Ken Perlin's text.
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "SimplexNoise.h"

#include <cstdint>  // int32_t/uint8_t

/**
 * Computes the largest integer value not greater than the float one
 *
 * This method is faster than using (int32_t)std::floor(fp).
 *
 * I measured it to be approximately twice as fast:
 *  float:  ~18.4ns instead of ~39.6ns on an AMD APU),
 *  double: ~20.6ns instead of ~36.6ns on an AMD APU),
 * Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
 *
 * @param[in] fp    float input value
 *
 * @return largest integer value not greater than fp
 */
static inline int32_t fastfloor(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

/**
 * Permutation table. This is just a random jumble of all numbers 0-255.
 *
 * This produce a repeatable pattern of 256, but Ken Perlin stated
 * that it is not a problem for graphic texture as the noise features disappear
 * at a distance far enough to be able to see a repeatable pattern of 256.
 *
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an uint32_t[] instead of a uint8_t[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
static const uint8_t perm[256] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

/**
 * Helper function to hash an integer using the above permutation table
 *
 *  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
 *
 *  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
 * but fast integer Hash functions uses more time and have bad random properties.
 *
 * @param[in] i Integer value to hash
 *
 * @return 8-bits hashed value
 */
static inline uint8_t hash(int32_t i) {
    return perm[static_cast<uint8_t>(i)];
}

/* NOTE Gradient table to test if lookup-table are more efficient than calculs
static const float gradients1D[16] = {
        -8.f, -7.f, -6.f, -5.f, -4.f, -3.f, -2.f, -1.f,
         1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f
};
*/

/**
 * Helper function to compute gradients-dot-residual vectors (1D)
 *
 * @note that these generate gradients of more than unit length. To make
 * a close match with the value range of classic Perlin noise, the final
 * noise values need to be rescaled to fit nicely within [-1,1].
 * (The simplex noise functions as such also have different scaling.)
 * Note also that these noise functions are the most practical and useful
 * signed version of Perlin noise.
 *
 * @param[in] hash  hash value
 * @param[in] x     distance to the corner
 *
 * @return gradient value
 */
static float grad(int32_t hash, float x) {
    const int32_t h = hash & 0x0F;  // Convert low 4 bits of hash code
    float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
    if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
    return (grad * x);              // Multiply the gradient with the distance
}

/**
 * Helper functions to compute gradients-dot-residual vectors (2D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 *
 * @return gradient value
 */
static float grad(int32_t hash, float x, float y) {
    const int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
    const float u = h < 4 ? x : y;  // into 8 simple gradient directions,
    const float v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v); // and compute the dot product with (x,y).
}

/**
 * Helper functions to compute gradients-dot-residual vectors (3D)
 *
 * @param[in] hash  hash value
 * @param[in] x     x coord of the distance to the corner
 * @param[in] y     y coord of the distance to the corner
 * @param[in] z     z coord of the distance to the corner
 *
 * @return gradient value
 */
static float grad(int32_t hash, float x, float y, float z) {
    int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
    float u = h < 8 ? x : y; // gradient directions, and compute dot product.
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

/**
 * 1D Perlin simplex noise
 *
 *  Takes around 74ns on an AMD APU.
 *
 * @param[in] x float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x) {
    float n0, n1;   // Noise contributions from the two "corners"

    // No need to skew the input space in 1D

    // Corners coordinates (nearest integer values):
    int32_t i0 = fastfloor(x);
    int32_t i1 = i0 + 1;
    // Distances to corners (between 0 and 1):
    float x0 = x - i0;
    float x1 = x0 - 1.0f;

    // Calculate the contribution from the first corner
    float t0 = 1.0f - x0*x0;
//  if(t0 < 0.0f) t0 = 0.0f; // not possible
    t0 *= t0;
    n0 = t0 * t0 * grad(hash(i0), x0);

    // Calculate the contribution from the second corner
    float t1 = 1.0f - x1*x1;
//  if(t1 < 0.0f) t1 = 0.0f; // not possible
    t1 *= t1;
    n1 = t1 * t1 * grad(hash(i1), x1);

    // The maximum value of this noise is 8*(3/4)^4 = 2.53125
    // A factor of 0.395 scales to fit exactly within [-1,1]
    return 0.395f * (n0 + n1);
}

/**
 * 2D Perlin simplex noise
 *
 *  Takes around 150ns on an AMD APU.
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x, float y) {
    float n0, n1, n2;   // Noise contributions from the three corners

    // Skewing/Unskewing factors for 2D
    static const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
    static const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

    // Skew the input space to determine which simplex cell we're in
    const float s = (x + y) * F2;  // Hairy factor for 2D
    const float xs = x + s;
    const float ys = y + s;
    const int32_t i = fastfloor(xs);
    const int32_t j = fastfloor(ys);

    // Unskew the cell origin back to (x,y) space
    const float t = static_cast<float>(i + j) * G2;
    const float X0 = i - t;
    const float Y0 = j - t;
    const float x0 = x - X0;  // The x,y distances from the cell origin
    const float y0 = y - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        i1 = 1;
        j1 = 0;
    } else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        i1 = 0;
        j1 = 1;
    }

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    const float x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
    const float y1 = y0 - j1 + G2;
    const float x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords
    const float y2 = y0 - 1.0f + 2.0f * G2;

    // Work out the hashed gradient indices of the three simplex corners
    const int gi0 = hash(i + hash(j));
    const int gi1 = hash(i + i1 + hash(j + j1));
    const int gi2 = hash(i + 1 + hash(j + 1));

    // Calculate the contribution from the first corner
    float t0 = 0.5f - x0*x0 - y0*y0;
    if (t0 < 0.0f) {
        n0 = 0.0f;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0);
    }

    // Calculate the contribution from the second corner
    float t1 = 0.5f - x1*x1 - y1*y1;
    if (t1 < 0.0f) {
        n1 = 0.0f;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1);
    }

    // Calculate the contribution from the third corner
    float t2 = 0.5f - x2*x2 - y2*y2;
    if (t2 < 0.0f) {
        n2 = 0.0f;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 45.23065f * (n0 + n1 + n2);
}


/**
 * 3D Perlin simplex noise
 *
 * @param[in] x float coordinate
 * @param[in] y float coordinate
 * @param[in] z float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x, float y, float z) {
    float n0, n1, n2, n3; // Noise contributions from the four corners

    // Skewing/Unskewing factors for 3D
    static const float F3 = 1.0f / 3.0f;
    static const float G3 = 1.0f / 6.0f;

    // Skew the input space to determine which simplex cell we're in
    float s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
    int i = fastfloor(x + s);
    int j = fastfloor(y + s);
    int k = fastfloor(z + s);
    float t = (i + j + k) * G3;
    float X0 = i - t; // Unskew the cell origin back to (x,y,z) space
    float Y0 = j - t;
    float Z0 = k - t;
    float x0 = x - X0; // The x,y,z distances from the cell origin
    float y0 = y - Y0;
    float z0 = z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // X Y Z order
        } else if (x0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; // X Z Y order
        } else {
            i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; // Z X Y order
        }
    } else { // x0<y0
        if (y0 < z0) {
            i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; // Z Y X order
        } else if (x0 < z0) {
            i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; // Y Z X order
        } else {
            i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; // Y X Z order
        }
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    float y1 = y0 - j1 + G3;
    float z1 = z0 - k1 + G3;
    float x2 = x0 - i2 + 2.0f * G3; // Offsets for third corner in (x,y,z) coords
    float y2 = y0 - j2 + 2.0f * G3;
    float z2 = z0 - k2 + 2.0f * G3;
    float x3 = x0 - 1.0f + 3.0f * G3; // Offsets for last corner in (x,y,z) coords
    float y3 = y0 - 1.0f + 3.0f * G3;
    float z3 = z0 - 1.0f + 3.0f * G3;

    // Work out the hashed gradient indices of the four simplex corners
    int gi0 = hash(i + hash(j + hash(k)));
    int gi1 = hash(i + i1 + hash(j + j1 + hash(k + k1)));
    int gi2 = hash(i + i2 + hash(j + j2 + hash(k + k2)));
    int gi3 = hash(i + 1 + hash(j + 1 + hash(k + 1)));

    // Calculate the contribution from the four corners
    float t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0, z0);
    }
    float t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1, z1);
    }
    float t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2, z2);
    }
    float t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
    if (t3 < 0) {
        n3 = 0.0;
    } else {
        t3 *= t3;
        n3 = t3 * t3 * grad(gi3, x3, y3, z3);
    }
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 32.0f*(n0 + n1 + n2 + n3);
}


/**
 * Fractal/Fractional Brownian Motion (fBm) summation of 1D Perlin Simplex noise
 *
 * @param[in] octaves   number of fraction of noise to sum
 * @param[in] x         float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::fractal(size_t octaves, float x) const {
    float output    = 0.f;
    float denom     = 0.f;
    float frequency = mFrequency;
    float amplitude = mAmplitude;

    for (size_t i = 0; i < octaves; i++) {
        output += (amplitude * noise(x * frequency));
        denom += amplitude;

        frequency *= mLacunarity;
        amplitude *= mPersistence;
    }

    return (output / denom);
}

/**
 * Fractal/Fractional Brownian Motion (fBm) summation of 2D Perlin Simplex noise
 *
 * @param[in] octaves   number of fraction of noise to sum
 * @param[in] x         x float coordinate
 * @param[in] y         y float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::fractal(size_t octaves, float x, float y) const {
    float output = 0.f;
    float denom  = 0.f;
    float frequency = mFrequency;
    float amplitude = mAmplitude;

    for (size_t i = 0; i < octaves; i++) {
        output += (amplitude * noise(x * frequency, y * frequency));
        denom += amplitude;

        frequency *= mLacunarity;
        amplitude *= mPersistence;
    }

    return (output / denom);
}

/**
 * Fractal/Fractional Brownian Motion (fBm) summation of 3D Perlin Simplex noise
 *
 * @param[in] octaves   number of fraction of noise to sum
 * @param[in] x         x float coordinate
 * @param[in] y         y float coordinate
 * @param[in] z         z float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::fractal(size_t octaves, float x, float y, float z) const {
    float output = 0.f;
    float denom  = 0.f;
    float frequency = mFrequency;
    float amplitude = mAmplitude;

    for (size_t i = 0; i < octaves; i++) {
        output += (amplitude * noise(x * frequency, y * frequency, z * frequency));
        denom += amplitude;

        frequency *= mLacunarity;
        amplitude *= mPersistence;
    }

    return (output / denom);
}


#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <cmath>

using namespace glm;

float mod289(float x) {
    return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 permute(vec4 x) {
    return mod289(((x * 34.0f) + 1.0f) * x);
}

vec2 fade(vec2 t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

vec3 fade(vec3 t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159f - 0.85373472095314f * r;
}

float cnoise2D(vec2 P) {
    vec4 Pi = floor(vec4(P.x, P.y, P.x, P.y)) + vec4(0.0f, 0.0f, 1.0f, 1.0f);
    vec4 Pf = fract(vec4(P.x, P.y, P.x, P.y)) - vec4(0.0f, 0.0f, 1.0f, 1.0f);
    Pi = mod289(Pi);
    vec4 ix = vec4(Pi.x, Pi.z, Pi.x, Pi.z);
    vec4 iy = vec4(Pi.y, Pi.y, Pi.w, Pi.w);
    vec4 fx = vec4(Pi.x, Pi.z, Pi.x, Pi.z);
    vec4 fy = vec4(Pi.y, Pi.y, Pi.w, Pi.w);
    vec4 i = permute(permute(ix) + iy);
    vec4 gx = 2.0f * fract(i * 0.0243902439f) - 1.0f;
    vec4 gy = abs(gx) - 0.5f;
    vec4 tx = floor(gx + 0.5f);
    gx = gx - tx;
    vec2 g00 = vec2(gx.x, gy.x);
    vec2 g10 = vec2(gx.y, gy.y);
    vec2 g01 = vec2(gx.z, gy.z);
    vec2 g11 = vec2(gx.w, gy.w);
    vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
    g00 *= norm.x;
    g01 *= norm.y;
    g10 *= norm.z;
    g11 *= norm.w;
    float n00 = dot(g00, vec2(fx.x, fy.x));
    float n10 = dot(g10, vec2(fx.y, fy.y));
    float n01 = dot(g01, vec2(fx.z, fy.z));
    float n11 = dot(g11, vec2(fx.w, fy.w));
    vec2 fade_xy = fade(vec2(Pf.x, Pf.y));
    vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
    float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
    return 2.3f * n_xy;
}

float cnoise3D(vec3 P) {
    vec3 Pi0 = floor(P);
    vec3 Pi1 = Pi0 + vec3(1.0f);
    Pi0 = mod(Pi0, vec3(289.0f));
    Pi1 = mod(Pi1, vec3(289.0f));
    vec3 Pf0 = fract(P);
    vec3 Pf1 = Pf0 - vec3(1.0f);
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.y, Pi1.y, Pi0.y, Pi1.y);
    vec4 iz0 = vec4(Pi0.z);
    vec4 iz1 = vec4(Pi1.z);

    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);

    vec4 gx0 = fract(ixy0 * (1.0f / 41.0f)) * 2.0f - 1.0f;
    vec4 gy0 = abs(gx0) - 0.5f;
    vec4 gz0 = vec4(0.5f) - abs(gx0) - abs(gy0);
    vec4 sz0 = step(gz0, vec4(0.0f));
    gx0 -= sz0 * (step(0.0f, gx0) - 0.5f);
    gy0 -= sz0 * (step(0.0f, gy0) - 0.5f);

    vec4 gx1 = fract(ixy1 * (1.0f / 41.0f)) * 2.0f - 1.0f;
    vec4 gy1 = abs(gx1) - 0.5f;
    vec4 gz1 = vec4(0.5f) - abs(gx1) - abs(gy1);
    vec4 sz1 = step(gz1, vec4(0.0f));
    gx1 -= sz1 * (step(0.0f, gx1) - 0.5f);
    gy1 -= sz1 * (step(0.0f, gy1) - 0.5f);

    vec3 g000 = vec3(gx0.x, gy0.x, gz0.x);
    vec3 g100 = vec3(gx0.y, gy0.y, gz0.y);
    vec3 g010 = vec3(gx0.z, gy0.z, gz0.z);
    vec3 g110 = vec3(gx0.w, gy0.w, gz0.w);
    vec3 g001 = vec3(gx1.x, gy1.x, gz1.x);
    vec3 g101 = vec3(gx1.y, gy1.y, gz1.y);
    vec3 g011 = vec3(gx1.z, gy1.z, gz1.z);
    vec3 g111 = vec3(gx1.w, gy1.w, gz1.w);

    vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
    g000 *= norm0.x;
    g010 *= norm0.y;
    g100 *= norm0.z;
    g110 *= norm0.w;
    vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
    g001 *= norm1.x;
    g011 *= norm1.y;
    g101 *= norm1.z;
    g111 *= norm1.w;

    float n000 = dot(g000, Pf0);
    float n100 = dot(g100, vec3(Pf1.x, Pf0.y, Pf0.z));
    float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
    float n110 = dot(g110, vec3(Pf1.x, Pf1.y, Pf0.z));
    float n001 = dot(g001, vec3(Pf0.x, Pf0.y, Pf1.z));
    float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
    float n011 = dot(g011, vec3(Pf0.x, Pf1.y, Pf1.z));
    float n111 = dot(g111, Pf1);

    vec3 fade_xyz = fade(Pf0);
    vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
    vec2 n_yz = mix(vec2(n_z.x, n_z.y), vec2(n_z.z, n_z.w), fade_xyz.y);
    float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
    return 2.2f * n_xyz;
}

float SimplexNoise::cnoise(vec3 P) {
    //, float frequency, float amplitude, float persistence, float lacunarity
    float total = 0.0f;
    float maxAmplitude = 0.1f;
    float amplitudeF = mAmplitude;
    float frequencyF = mFrequency;
    float octaves = 1.0f;

    for (int i = 0; i < octaves; i++) {
        total += cnoise3D(P * frequencyF) * amplitudeF;
        maxAmplitude += amplitudeF;
        amplitudeF *= mPersistence;
        frequencyF *= mLacunarity;
    }

    return total / maxAmplitude;
}

float SimplexNoise::cnoise(vec2 P, float frequency, float amplitude, float persistence, float lacunarity) {
    float total = 0.0f;
    float maxAmplitude = 0.1f;
    float amplitudeF = amplitude;
    float frequencyF = frequency;

    for (int i = 0; i < 2; i++) {
        total += cnoise2D(P * frequencyF) * amplitudeF;
        maxAmplitude += amplitudeF;
        amplitudeF *= persistence;
        frequencyF *= lacunarity;
    }

    return total / maxAmplitude;
}