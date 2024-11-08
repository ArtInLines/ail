// Common Math functions
//
// Heavily inspired by the following libraries:
// - HandmadeMath (https://github.com/HandmadeMath/HandmadeMath)
// - la (https://github.com/tsoding/la)
// - Raylib (https://github.com/raysan5/raylib/blob/master/src/raymath.h)
// - mathc (https://github.com/felselva/mathc)
// - fastmod (https://github.com/lemire/fastmod)
// - tiny-fixedpoint (https://github.com/kokke/tiny-fixedpoint-c)
// -
//
// LICENSE
/*
Copyright (c) 2024 Lily Val Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef AIL_MATH_H

#include "ail.h"

#ifndef AIL_MATH_DEF
#ifdef  AIL_DEF
#define AIL_MATH_DEF AIL_DEF
#else
#define AIL_MATH_DEF
#endif // AIL_DEF
#endif // AIL_MATH_DEF
#ifndef AIL_MATH_DEF_INLINE
#ifdef  AIL_DEF_INLINE
#define AIL_MATH_DEF_INLINE AIL_DEF_INLINE
#else
#define AIL_MATH_DEF_INLINE static inline
#endif // AIL_DEF_INLINE
#endif // AIL_MATH_DEF_INLINE


////////////////////
// Linear Algebra //
////////////////////

typedef union AIL_Vec2u8 {
    u8 els[2];
    struct { u8 x, y; };
    struct { u8 r, g; };
    struct { u8 u, v; };
    struct { u8 width, height; };
} AIL_Vec2u8;

typedef union AIL_Vec2i8 {
    i8 els[2];
    struct { i8 x, y; };
    struct { i8 r, g; };
    struct { i8 u, v; };
    struct { i8 width, height; };
} AIL_Vec2i8;

typedef union AIL_Vec2u16 {
    u16 els[2];
    struct { u16 x, y; };
    struct { u16 r, g; };
    struct { u16 u, v; };
    struct { u16 width, height; };
} AIL_Vec2u16;

typedef union AIL_Vec2i16 {
    i16 els[2];
    struct { i16 x, y; };
    struct { i16 r, g; };
    struct { i16 u, v; };
    struct { i16 width, height; };
} AIL_Vec2i16;

typedef union AIL_Vec2u32 {
    u32 els[2];
    struct { u32 x, y; };
    struct { u32 r, g; };
    struct { u32 u, v; };
    struct { u32 width, height; };
} AIL_Vec2u32;

typedef union AIL_Vec2i32 {
    i32 els[2];
    struct { i32 x, y; };
    struct { i32 r, g; };
    struct { i32 u, v; };
    struct { i32 width, height; };
} AIL_Vec2i32;

typedef union AIL_Vec2u64 {
    u64 els[2];
    struct { u64 x, y; };
    struct { u64 r, g; };
    struct { u64 u, v; };
    struct { u64 width, height; };
} AIL_Vec2u64;

typedef union AIL_Vec2i64 {
    i64 els[2];
    struct { i64 x, y; };
    struct { i64 r, g; };
    struct { i64 u, v; };
    struct { i64 width, height; };
} AIL_Vec2i64;

typedef union AIL_Vec2f32 {
    f32 els[2];
    struct { f32 x, y; };
    struct { f32 r, g; };
    struct { f32 u, v; };
    struct { f32 width, height; };
} AIL_Vec2f32;

typedef union AIL_Vec2f64 {
    f64 els[2];
    struct { f64 x, y; };
    struct { f64 r, g; };
    struct { f64 u, v; };
    struct { f64 width, height; };
} AIL_Vec2f64;

typedef union AIL_Vec3u8 {
    u8 els[3];
    struct { u8 x, y, z; };
    struct { u8 r, g, b; };
    struct { u8 u, v, w; };
    struct { AIL_Vec2u8 xy; u8 _0; };
    struct { u8 _1; AIL_Vec2u8 yz; };
    struct { AIL_Vec2u8 uv; u8 _2; };
    struct { u8 _3; AIL_Vec2u8 vw; };
} AIL_Vec3u8;

typedef union AIL_Vec3i8 {
    i8 els[3];
    struct { i8 x, y, z; };
    struct { i8 r, g, b; };
    struct { i8 u, v, w; };
    struct { AIL_Vec2i8 xy; i8 _0; };
    struct { i8 _1; AIL_Vec2i8 yz; };
    struct { AIL_Vec2i8 uv; i8 _2; };
    struct { i8 _3; AIL_Vec2i8 vw; };
} AIL_Vec3i8;

typedef union AIL_Vec3u16 {
    u16 els[3];
    struct { u16 x, y, z; };
    struct { u16 r, g, b; };
    struct { u16 u, v, w; };
    struct { AIL_Vec2u16 xy; u16 _0; };
    struct { u16 _1; AIL_Vec2u16 yz; };
    struct { AIL_Vec2u16 uv; u16 _2; };
    struct { u16 _3; AIL_Vec2u16 vw; };
} AIL_Vec3u16;

typedef union AIL_Vec3i16 {
    i16 els[3];
    struct { i16 x, y, z; };
    struct { i16 r, g, b; };
    struct { i16 u, v, w; };
    struct { AIL_Vec2i16 xy; i16 _0; };
    struct { i16 _1; AIL_Vec2i16 yz; };
    struct { AIL_Vec2i16 uv; i16 _2; };
    struct { i16 _3; AIL_Vec2i16 vw; };
} AIL_Vec3i16;

typedef union AIL_Vec3u32 {
    u32 els[3];
    struct { u32 x, y, z; };
    struct { u32 r, g, b; };
    struct { u32 u, v, w; };
    struct { AIL_Vec2u32 xy; u32 _0; };
    struct { u32 _1; AIL_Vec2u32 yz; };
    struct { AIL_Vec2u32 uv; u32 _2; };
    struct { u32 _3; AIL_Vec2u32 vw; };
} AIL_Vec3u32;

typedef union AIL_Vec3i32 {
    i32 els[3];
    struct { i32 x, y, z; };
    struct { i32 r, g, b; };
    struct { i32 u, v, w; };
    struct { AIL_Vec2i32 xy; i32 _0; };
    struct { i32 _1; AIL_Vec2i32 yz; };
    struct { AIL_Vec2i32 uv; i32 _2; };
    struct { i32 _3; AIL_Vec2i32 vw; };
} AIL_Vec3i32;

typedef union AIL_Vec3u64 {
    u64 els[3];
    struct { u64 x, y, z; };
    struct { u64 r, g, b; };
    struct { u64 u, v, w; };
    struct { AIL_Vec2u64 xy; u64 _0; };
    struct { u64 _1; AIL_Vec2u64 yz; };
    struct { AIL_Vec2u64 uv; u64 _2; };
    struct { u64 _3; AIL_Vec2u64 vw; };
} AIL_Vec3u64;

typedef union AIL_Vec3i64 {
    i64 els[3];
    struct { i64 x, y, z; };
    struct { i64 r, g, b; };
    struct { i64 u, v, w; };
    struct { AIL_Vec2i64 xy; i64 _0; };
    struct { i64 _1; AIL_Vec2i64 yz; };
    struct { AIL_Vec2i64 uv; i64 _2; };
    struct { i64 _3; AIL_Vec2i64 vw; };
} AIL_Vec3i64;

typedef union AIL_Vec3f32 {
    f32 els[3];
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { f32 u, v, w; };
    struct { AIL_Vec2f32 xy; f32 _0; };
    struct { f32 _1; AIL_Vec2f32 yz; };
    struct { AIL_Vec2f32 uv; f32 _2; };
    struct { f32 _3; AIL_Vec2f32 vw; };
} AIL_Vec3f32;

typedef union AIL_Vec3f64 {
    f64 els[3];
    struct { f64 x, y, z; };
    struct { f64 r, g, b; };
    struct { f64 u, v, w; };
    struct { AIL_Vec2f64 xy; f64 _0; };
    struct { f64 _1; AIL_Vec2f64 yz; };
    struct { AIL_Vec2f64 uv; f64 _2; };
    struct { f64 _3; AIL_Vec2f64 vw; };
} AIL_Vec3f64;

typedef union AIL_Vec4u8 {
    u8 els[4];
    struct { u8 x, y, z, w; };
    struct { u8 r, g, b, a; };
    struct {AIL_Vec3u8 xyz; u8 _0; };
    struct {u8 _1; AIL_Vec3u8 yzw; };
    struct { AIL_Vec2u8 xy; u8 _2[2]; };
    struct { u8 _3; AIL_Vec2u8 yz; u8 _4; };
    struct { u8 _5[2]; AIL_Vec2u8 zw; };
    struct {AIL_Vec3u8 rgb; u8 _6; };
    struct {u8 _7; AIL_Vec3u8 gba; };
    struct { AIL_Vec2u8 rg; u8 _8[2]; };
    struct { u8 _9; AIL_Vec2u8 gb; u8 _10; };
    struct { u8 _11[2]; AIL_Vec2u8 ba; };
} AIL_Vec4u8;

typedef union AIL_Vec4i8 {
    i8 els[4];
    struct { i8 x, y, z, w; };
    struct { i8 r, g, b, a; };
    struct {AIL_Vec3i8 xyz; i8 _0; };
    struct {i8 _1; AIL_Vec3i8 yzw; };
    struct { AIL_Vec2i8 xy; i8 _2[2]; };
    struct { i8 _3; AIL_Vec2i8 yz; i8 _4; };
    struct { i8 _5[2]; AIL_Vec2i8 zw; };
    struct {AIL_Vec3i8 rgb; i8 _6; };
    struct {i8 _7; AIL_Vec3i8 gba; };
    struct { AIL_Vec2i8 rg; i8 _8[2]; };
    struct { i8 _9; AIL_Vec2i8 gb; i8 _10; };
    struct { i8 _11[2]; AIL_Vec2i8 ba; };
} AIL_Vec4i8;

typedef union AIL_Vec4u16 {
    u16 els[4];
    struct { u16 x, y, z, w; };
    struct { u16 r, g, b, a; };
    struct { AIL_Vec3u16 xyz; u16 _0; };
    struct { u16 _1; AIL_Vec3u16 yzw; };
    struct { AIL_Vec2u16 xy; u16 _2[2]; };
    struct { u16 _3; AIL_Vec2u16 yz; u16 _4; };
    struct { u16 _5[2]; AIL_Vec2u16 zw; };
    struct { AIL_Vec3u16 rgb; u16 _6; };
    struct { u16 _7; AIL_Vec3u16 gba; };
    struct { AIL_Vec2u16 rg; u16 _8[2]; };
    struct { u16 _9; AIL_Vec2u16 gb; u16 _10; };
    struct { u16 _11[2]; AIL_Vec2u16 ba; };
} AIL_Vec4u16;

typedef union AIL_Vec4i16 {
    i16 els[4];
    struct { i16 x, y, z, w; };
    struct { i16 r, g, b, a; };
    struct { AIL_Vec3i16 xyz; i16 _0; };
    struct { i16 _1; AIL_Vec3i16 yzw; };
    struct { AIL_Vec2i16 xy; i16 _2[2]; };
    struct { i16 _3; AIL_Vec2i16 yz; i16 _4; };
    struct { i16 _5[2]; AIL_Vec2i16 zw; };
    struct { AIL_Vec3i16 rgb; i16 _6; };
    struct { i16 _7; AIL_Vec3i16 gba; };
    struct { AIL_Vec2i16 rg; i16 _8[2]; };
    struct { i16 _9; AIL_Vec2i16 gb; i16 _10; };
    struct { i16 _11[2]; AIL_Vec2i16 ba; };
} AIL_Vec4i16;

typedef union AIL_Vec4u32 {
    u32 els[4];
    struct { u32 x, y, z, w; };
    struct { u32 r, g, b, a; };
    struct { AIL_Vec3u32 xyz; u32 _0; };
    struct { u32 _1; AIL_Vec3u32 yzw; };
    struct { AIL_Vec2u32 xy; u32 _2[2]; };
    struct { u32 _3; AIL_Vec2u32 yz; u32 _4; };
    struct { u32 _5[2]; AIL_Vec2u32 zw; };
    struct { AIL_Vec3u32 rgb; u32 _6; };
    struct { u32 _7; AIL_Vec3u32 gba; };
    struct { AIL_Vec2u32 rg; u32 _8[2]; };
    struct { u32 _9; AIL_Vec2u32 gb; u32 _10; };
    struct { u32 _11[2]; AIL_Vec2u32 ba; };
} AIL_Vec4u32;

typedef union AIL_Vec4i32 {
    i32 els[4];
    struct { i32 x, y, z, w; };
    struct { i32 r, g, b, a; };
    struct { AIL_Vec3i32 xyz; i32 _0; };
    struct { i32 _1; AIL_Vec3i32 yzw; };
    struct { AIL_Vec2i32 xy; i32 _2[2]; };
    struct { i32 _3; AIL_Vec2i32 yz; i32 _4; };
    struct { i32 _5[2]; AIL_Vec2i32 zw; };
    struct { AIL_Vec3i32 rgb; i32 _6; };
    struct { i32 _7; AIL_Vec3i32 gba; };
    struct { AIL_Vec2i32 rg; i32 _8[2]; };
    struct { i32 _9; AIL_Vec2i32 gb; i32 _10; };
    struct { i32 _11[2]; AIL_Vec2i32 ba; };
} AIL_Vec4i32;

typedef union AIL_Vec4u64 {
    u64 els[4];
    struct { u64 x, y, z, w; };
    struct { u64 r, g, b, a; };
    struct { AIL_Vec3u64 xyz; u64 _0; };
    struct { u64 _1; AIL_Vec3u64 yzw; };
    struct { AIL_Vec2u64 xy; u64 _2[2]; };
    struct { u64 _3; AIL_Vec2u64 yz; u64 _4; };
    struct { u64 _5[2]; AIL_Vec2u64 zw; };
    struct { AIL_Vec3u64 rgb; u64 _6; };
    struct { u64 _7; AIL_Vec3u64 gba; };
    struct { AIL_Vec2u64 rg; u64 _8[2]; };
    struct { u64 _9; AIL_Vec2u64 gb; u64 _10; };
    struct { u64 _11[2]; AIL_Vec2u64 ba; };
} AIL_Vec4u64;

typedef union AIL_Vec4i64 {
    i64 els[4];
    struct { i64 x, y, z, w; };
    struct { i64 r, g, b, a; };
    struct { AIL_Vec3i64 xyz; i64 _0; };
    struct { i64 _1; AIL_Vec3i64 yzw; };
    struct { AIL_Vec2i64 xy; i64 _2[2]; };
    struct { i64 _3; AIL_Vec2i64 yz; i64 _4; };
    struct { i64 _5[2]; AIL_Vec2i64 zw; };
    struct { AIL_Vec3i64 rgb; i64 _6; };
    struct { i64 _7; AIL_Vec3i64 gba; };
    struct { AIL_Vec2i64 rg; i64 _8[2]; };
    struct { i64 _9; AIL_Vec2i64 gb; i64 _10; };
    struct { i64 _11[2]; AIL_Vec2i64 ba; };
} AIL_Vec4i64;

typedef union AIL_Vec4f32 {
    f32 els[4];
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { AIL_Vec3f32 xyz; f32 _0; };
    struct { f32 _1; AIL_Vec3f32 yzw; };
    struct { AIL_Vec2f32 xy; f32 _2[2]; };
    struct { f32 _3; AIL_Vec2f32 yz; f32 _4; };
    struct { f32 _5[2]; AIL_Vec2f32 zw; };
    struct { AIL_Vec3f32 rgb; f32 _6; };
    struct { f32 _7; AIL_Vec3f32 gba; };
    struct { AIL_Vec2f32 rg; f32 _8[2]; };
    struct { f32 _9; AIL_Vec2f32 gb; f32 _10; };
    struct { f32 _11[2]; AIL_Vec2f32 ba; };
} AIL_Vec4f32;

typedef union AIL_Vec4f64 {
    f64 els[4];
    struct { f64 x, y, z, w; };
    struct { f64 r, g, b, a; };
    struct { AIL_Vec3f64 xyz; f64 _0; };
    struct { f64 _1; AIL_Vec3f64 yzw; };
    struct { AIL_Vec2f64 xy; f64 _2[2]; };
    struct { f64 _3; AIL_Vec2f64 yz; f64 _4; };
    struct { f64 _5[2]; AIL_Vec2f64 zw; };
    struct { AIL_Vec3f64 rgb; f64 _6; };
    struct { f64 _7; AIL_Vec3f64 gba; };
    struct { AIL_Vec2f64 rg; f64 _8[2]; };
    struct { f64 _9; AIL_Vec2f64 gb; f64 _10; };
    struct { f64 _11[2]; AIL_Vec2f64 ba; };
} AIL_Vec4f64;

typedef union AIL_Mat2u8 {
    u8 els[2][2];
    AIL_Vec2u8 cols[2];
} AIL_Mat2u8;

typedef union AIL_Mat2i8 {
    i8 els[2][2];
    AIL_Vec2i8 cols[2];
} AIL_Mat2i8;

typedef union AIL_Mat2u16 {
    u16 els[2][2];
    AIL_Vec2u16 cols[2];
} AIL_Mat2u16;

typedef union AIL_Mat2i16 {
    i16 els[2][2];
    AIL_Vec2i16 cols[2];
} AIL_Mat2i16;

typedef union AIL_Mat2u32 {
    u32 els[2][2];
    AIL_Vec2u32 cols[2];
} AIL_Mat2u32;

typedef union AIL_Mat2i32 {
    i32 els[2][2];
    AIL_Vec2i32 cols[2];
} AIL_Mat2i32;

typedef union AIL_Mat2u64 {
    u64 els[2][2];
    AIL_Vec2u64 cols[2];
} AIL_Mat2u64;

typedef union AIL_Mat2i64 {
    i64 els[2][2];
    AIL_Vec2i64 cols[2];
} AIL_Mat2i64;

typedef union AIL_Mat2f32 {
    f32 els[2][2];
    AIL_Vec2f32 cols[2];
} AIL_Mat2f32;

typedef union AIL_Mat2f64 {
    f64 els[2][2];
    AIL_Vec2f64 cols[2];
} AIL_Mat2f64;

typedef union AIL_Mat3u8 {
    u8 els[3][3];
    AIL_Vec3u8 cols[3];
} AIL_Mat3u8;

typedef union AIL_Mat3i8 {
    i8 els[3][3];
    AIL_Vec3i8 cols[3];
} AIL_Mat3i8;

typedef union AIL_Mat3u16 {
    u16 els[3][3];
    AIL_Vec3u16 cols[3];
} AIL_Mat3u16;

typedef union AIL_Mat3i16 {
    i16 els[3][3];
    AIL_Vec3i16 cols[3];
} AIL_Mat3i16;

typedef union AIL_Mat3u32 {
    u32 els[3][3];
    AIL_Vec3u32 cols[3];
} AIL_Mat3u32;

typedef union AIL_Mat3i32 {
    i32 els[3][3];
    AIL_Vec3i32 cols[3];
} AIL_Mat3i32;

typedef union AIL_Mat3u64 {
    u64 els[3][3];
    AIL_Vec3u64 cols[3];
} AIL_Mat3u64;

typedef union AIL_Mat3i64 {
    i64 els[3][3];
    AIL_Vec3i64 cols[3];
} AIL_Mat3i64;

typedef union AIL_Mat3f32 {
    f32 els[3][3];
    AIL_Vec3f32 cols[3];
} AIL_Mat3f32;

typedef union AIL_Mat3f64 {
    f64 els[3][3];
    AIL_Vec3f64 cols[3];
} AIL_Mat3f64;

typedef union AIL_Mat4u8 {
    u8 els[4][4];
    AIL_Vec4u8 cols[4];
} AIL_Mat4u8;

typedef union AIL_Mat4i8 {
    i8 els[4][4];
    AIL_Vec4i8 cols[4];
} AIL_Mat4i8;

typedef union AIL_Mat4u16 {
    u16 els[4][4];
    AIL_Vec4u16 cols[4];
} AIL_Mat4u16;

typedef union AIL_Mat4i16 {
    i16 els[4][4];
    AIL_Vec4i16 cols[4];
} AIL_Mat4i16;

typedef union AIL_Mat4u32 {
    u32 els[4][4];
    AIL_Vec4u32 cols[4];
} AIL_Mat4u32;

typedef union AIL_Mat4i32 {
    i32 els[4][4];
    AIL_Vec4i32 cols[4];
} AIL_Mat4i32;

typedef union AIL_Mat4u64 {
    u64 els[4][4];
    AIL_Vec4u64 cols[4];
} AIL_Mat4u64;

typedef union AIL_Mat4i64 {
    i64 els[4][4];
    AIL_Vec4i64 cols[4];
} AIL_Mat4i64;

typedef union AIL_Mat4f32 {
    f32 els[4][4];
    AIL_Vec4f32 cols[4];
} AIL_Mat4f32;

typedef union AIL_Mat4f64 {
    f64 els[4][4];
    AIL_Vec4f64 cols[4];
} AIL_Mat4f64;


///////////////////////////
// Macros for Overloding //
///////////////////////////

// @Important: These size definitions are not necessarily correct, but if you want proper type inference, you should just use properly sized types anyways
#define _AIL_MATH_GENERIC_1(pre, post, arg0, ...) _Generic((arg0), \
    unsigned char:          AIL_CONCAT(pre, u8, post),  \
    signed char:            AIL_CONCAT(pre, i8, post),  \
    unsigned short:         AIL_CONCAT(pre, u16, post), \
    signed short:           AIL_CONCAT(pre, i16, post), \
    unsigned int:           AIL_CONCAT(pre, u32, post), \
    signed int:             AIL_CONCAT(pre, i32, post), \
    unsigned long int:      AIL_CONCAT(pre, u64, post), \
    signed long int:        AIL_CONCAT(pre, i64, post), \
    unsigned long long int: AIL_CONCAT(pre, u64, post), \
    signed long long int:   AIL_CONCAT(pre, i64, post)  \
)

#define AIL_MATH_GENERIC(pre, post, arg0, ...) _Generic((arg0), \
    u8: AIL_CONCAT(pre, u8, post),   \
    i8: AIL_CONCAT(pre, i8, post),   \
    u16: AIL_CONCAT(pre, u16, post), \
    i16: AIL_CONCAT(pre, i16, post), \
    u32: AIL_CONCAT(pre, u32, post), \
    i32: AIL_CONCAT(pre, i32, post), \
    u64: AIL_CONCAT(pre, u64, post), \
    i64: AIL_CONCAT(pre, i64, post), \
    f32: AIL_CONCAT(pre, f32, post), \
    f64: AIL_CONCAT(pre, f64, post), \
    default: _AIL_MATH_GENERIC_1(pre, post, arg0, __VA_ARGS__) \
) ((arg0), __VA_ARGS__)


/////////////////////////
// Overloded Functions //
/////////////////////////

#define ail_vec2(x, y) AIL_MATH_GENERIC(ail_vec2, , x, y)
#define ail_vec3(x, y, z) AIL_MATH_GENERIC(ail_vec3, , x, y, z)
#define ail_vec4(x, y, z, w) AIL_MATH_GENERIC(ail_vec4, , x, y, z, w)
#define ail_mat2(x11, x12, x21, x22) AIL_MATH_GENERIC(ail_mat2, , x11, x12, x21, x22)
#define ail_mat3(x11, x12, x13, x21, x22, x23, x31, x32, x33) AIL_MATH_GENERIC(ail_mat3, , x11, x12, x13, x21, x22, x23, x31, x32, x33)
#define ail_mat4(x11, x12, x13, x14, x21, x22, x23, x24, x31, x32, x33, x34, x41, x42, x43, x44) AIL_MATH_GENERIC(ail_mat4, , x11, x12, x13, x14, x21, x22, x23, x24, x31, x32, x33, x34, x41, x42, x43, x44)

#define ail_vec2_eq(a, b) AIL_MATH_GENERIC(ail_vec2, _eq, a, b)
#define ail_vec3_eq(a, b) AIL_MATH_GENERIC(ail_vec3, _eq, a, b)
#define ail_vec4_eq(a, b) AIL_MATH_GENERIC(ail_vec4, _eq, a, b)
#define ail_mat2_eq(a, b) AIL_MATH_GENERIC(ail_mat2, _eq, a, b)
#define ail_mat3_eq(a, b) AIL_MATH_GENERIC(ail_mat3, _eq, a, b)
#define ail_mat4_eq(a, b) AIL_MATH_GENERIC(ail_mat4, _eq, a, b)

#define ail_vec2_add(a, b) AIL_MATH_GENERIC(ail_vec2, _add, a, b)
#define ail_vec3_add(a, b) AIL_MATH_GENERIC(ail_vec3, _add, a, b)
#define ail_vec4_add(a, b) AIL_MATH_GENERIC(ail_vec4, _add, a, b)
#define ail_mat2_add(a, b) AIL_MATH_GENERIC(ail_mat2, _add, a, b)
#define ail_mat3_add(a, b) AIL_MATH_GENERIC(ail_mat3, _add, a, b)
#define ail_mat4_add(a, b) AIL_MATH_GENERIC(ail_mat4, _add, a, b)

#define ail_vec2_sub(a, b) AIL_MATH_GENERIC(ail_vec2, _sub, a, b)
#define ail_vec3_sub(a, b) AIL_MATH_GENERIC(ail_vec3, _sub, a, b)
#define ail_vec4_sub(a, b) AIL_MATH_GENERIC(ail_vec4, _sub, a, b)
#define ail_mat2_sub(a, b) AIL_MATH_GENERIC(ail_mat2, _sub, a, b)
#define ail_mat3_sub(a, b) AIL_MATH_GENERIC(ail_mat3, _sub, a, b)
#define ail_mat4_sub(a, b) AIL_MATH_GENERIC(ail_mat4, _sub, a, b)


///////////////////////////
// Function Declarations //
///////////////////////////

AIL_MATH_DEF_INLINE AIL_Vec2u8 ail_vec2u8(u8 x, u8 y);
AIL_MATH_DEF_INLINE AIL_Vec2i8 ail_vec2i8(i8 x, i8 y);
AIL_MATH_DEF_INLINE AIL_Vec2u16 ail_vec2u16(u16 x, u16 y);
AIL_MATH_DEF_INLINE AIL_Vec2i16 ail_vec2i16(i16 x, i16 y);
AIL_MATH_DEF_INLINE AIL_Vec2u32 ail_vec2u32(u32 x, u32 y);
AIL_MATH_DEF_INLINE AIL_Vec2i32 ail_vec2i32(i32 x, i32 y);
AIL_MATH_DEF_INLINE AIL_Vec2u64 ail_vec2u64(u64 x, u64 y);
AIL_MATH_DEF_INLINE AIL_Vec2i64 ail_vec2i64(i64 x, i64 y);
AIL_MATH_DEF_INLINE AIL_Vec2f32 ail_vec2f32(f32 x, f32 y);
AIL_MATH_DEF_INLINE AIL_Vec2f64 ail_vec2f64(f64 x, f64 y);
AIL_MATH_DEF_INLINE AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z);
AIL_MATH_DEF_INLINE AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z);
AIL_MATH_DEF_INLINE AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z);
AIL_MATH_DEF_INLINE AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z);
AIL_MATH_DEF_INLINE AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z);
AIL_MATH_DEF_INLINE AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z);
AIL_MATH_DEF_INLINE AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z);
AIL_MATH_DEF_INLINE AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z);
AIL_MATH_DEF_INLINE AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z);
AIL_MATH_DEF_INLINE AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z);
AIL_MATH_DEF_INLINE AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w);
AIL_MATH_DEF_INLINE AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w);
AIL_MATH_DEF_INLINE AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w);
AIL_MATH_DEF_INLINE AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w);
AIL_MATH_DEF_INLINE AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w);
AIL_MATH_DEF_INLINE AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w);
AIL_MATH_DEF_INLINE AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w);
AIL_MATH_DEF_INLINE AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w);
AIL_MATH_DEF_INLINE AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w);
AIL_MATH_DEF_INLINE AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w);
AIL_MATH_DEF_INLINE AIL_Mat2u8 ail_mat2u8(u8 x11, u8 x12, u8 x21, u8 x22);
AIL_MATH_DEF_INLINE AIL_Mat2i8 ail_mat2i8(i8 x11, i8 x12, i8 x21, i8 x22);
AIL_MATH_DEF_INLINE AIL_Mat2u16 ail_mat2u16(u16 x11, u16 x12, u16 x21, u16 x22);
AIL_MATH_DEF_INLINE AIL_Mat2i16 ail_mat2i16(i16 x11, i16 x12, i16 x21, i16 x22);
AIL_MATH_DEF_INLINE AIL_Mat2u32 ail_mat2u32(u32 x11, u32 x12, u32 x21, u32 x22);
AIL_MATH_DEF_INLINE AIL_Mat2i32 ail_mat2i32(i32 x11, i32 x12, i32 x21, i32 x22);
AIL_MATH_DEF_INLINE AIL_Mat2u64 ail_mat2u64(u64 x11, u64 x12, u64 x21, u64 x22);
AIL_MATH_DEF_INLINE AIL_Mat2i64 ail_mat2i64(i64 x11, i64 x12, i64 x21, i64 x22);
AIL_MATH_DEF_INLINE AIL_Mat2f32 ail_mat2f32(f32 x11, f32 x12, f32 x21, f32 x22);
AIL_MATH_DEF_INLINE AIL_Mat2f64 ail_mat2f64(f64 x11, f64 x12, f64 x21, f64 x22);
AIL_MATH_DEF_INLINE AIL_Mat3u8 ail_mat3u8(u8 x11, u8 x12, u8 x13, u8 x21, u8 x22, u8 x23, u8 x31, u8 x32, u8 x33);
AIL_MATH_DEF_INLINE AIL_Mat3i8 ail_mat3i8(i8 x11, i8 x12, i8 x13, i8 x21, i8 x22, i8 x23, i8 x31, i8 x32, i8 x33);
AIL_MATH_DEF_INLINE AIL_Mat3u16 ail_mat3u16(u16 x11, u16 x12, u16 x13, u16 x21, u16 x22, u16 x23, u16 x31, u16 x32, u16 x33);
AIL_MATH_DEF_INLINE AIL_Mat3i16 ail_mat3i16(i16 x11, i16 x12, i16 x13, i16 x21, i16 x22, i16 x23, i16 x31, i16 x32, i16 x33);
AIL_MATH_DEF_INLINE AIL_Mat3u32 ail_mat3u32(u32 x11, u32 x12, u32 x13, u32 x21, u32 x22, u32 x23, u32 x31, u32 x32, u32 x33);
AIL_MATH_DEF_INLINE AIL_Mat3i32 ail_mat3i32(i32 x11, i32 x12, i32 x13, i32 x21, i32 x22, i32 x23, i32 x31, i32 x32, i32 x33);
AIL_MATH_DEF_INLINE AIL_Mat3u64 ail_mat3u64(u64 x11, u64 x12, u64 x13, u64 x21, u64 x22, u64 x23, u64 x31, u64 x32, u64 x33);
AIL_MATH_DEF_INLINE AIL_Mat3i64 ail_mat3i64(i64 x11, i64 x12, i64 x13, i64 x21, i64 x22, i64 x23, i64 x31, i64 x32, i64 x33);
AIL_MATH_DEF_INLINE AIL_Mat3f32 ail_mat3f32(f32 x11, f32 x12, f32 x13, f32 x21, f32 x22, f32 x23, f32 x31, f32 x32, f32 x33);
AIL_MATH_DEF_INLINE AIL_Mat3f64 ail_mat3f64(f64 x11, f64 x12, f64 x13, f64 x21, f64 x22, f64 x23, f64 x31, f64 x32, f64 x33);
AIL_MATH_DEF_INLINE AIL_Mat4u8 ail_mat4u8(u8 x11, u8 x12, u8 x13, u8 x14, u8 x21, u8 x22, u8 x23, u8 x24, u8 x31, u8 x32, u8 x33, u8 x34, u8 x41, u8 x42, u8 x43, u8 x44);
AIL_MATH_DEF_INLINE AIL_Mat4i8 ail_mat4i8(i8 x11, i8 x12, i8 x13, i8 x14, i8 x21, i8 x22, i8 x23, i8 x24, i8 x31, i8 x32, i8 x33, i8 x34, i8 x41, i8 x42, i8 x43, i8 x44);
AIL_MATH_DEF_INLINE AIL_Mat4u16 ail_mat4u16(u16 x11, u16 x12, u16 x13, u16 x14, u16 x21, u16 x22, u16 x23, u16 x24, u16 x31, u16 x32, u16 x33, u16 x34, u16 x41, u16 x42, u16 x43, u16 x44);
AIL_MATH_DEF_INLINE AIL_Mat4i16 ail_mat4i16(i16 x11, i16 x12, i16 x13, i16 x14, i16 x21, i16 x22, i16 x23, i16 x24, i16 x31, i16 x32, i16 x33, i16 x34, i16 x41, i16 x42, i16 x43, i16 x44);
AIL_MATH_DEF_INLINE AIL_Mat4u32 ail_mat4u32(u32 x11, u32 x12, u32 x13, u32 x14, u32 x21, u32 x22, u32 x23, u32 x24, u32 x31, u32 x32, u32 x33, u32 x34, u32 x41, u32 x42, u32 x43, u32 x44);
AIL_MATH_DEF_INLINE AIL_Mat4i32 ail_mat4i32(i32 x11, i32 x12, i32 x13, i32 x14, i32 x21, i32 x22, i32 x23, i32 x24, i32 x31, i32 x32, i32 x33, i32 x34, i32 x41, i32 x42, i32 x43, i32 x44);
AIL_MATH_DEF_INLINE AIL_Mat4u64 ail_mat4u64(u64 x11, u64 x12, u64 x13, u64 x14, u64 x21, u64 x22, u64 x23, u64 x24, u64 x31, u64 x32, u64 x33, u64 x34, u64 x41, u64 x42, u64 x43, u64 x44);
AIL_MATH_DEF_INLINE AIL_Mat4i64 ail_mat4i64(i64 x11, i64 x12, i64 x13, i64 x14, i64 x21, i64 x22, i64 x23, i64 x24, i64 x31, i64 x32, i64 x33, i64 x34, i64 x41, i64 x42, i64 x43, i64 x44);
AIL_MATH_DEF_INLINE AIL_Mat4f32 ail_mat4f32(f32 x11, f32 x12, f32 x13, f32 x14, f32 x21, f32 x22, f32 x23, f32 x24, f32 x31, f32 x32, f32 x33, f32 x34, f32 x41, f32 x42, f32 x43, f32 x44);
AIL_MATH_DEF_INLINE AIL_Mat4f64 ail_mat4f64(f64 x11, f64 x12, f64 x13, f64 x14, f64 x21, f64 x22, f64 x23, f64 x24, f64 x31, f64 x32, f64 x33, f64 x34, f64 x41, f64 x42, f64 x43, f64 x44);

AIL_MATH_DEF_INLINE b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_MATH_DEF_INLINE b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_MATH_DEF_INLINE b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_MATH_DEF_INLINE b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_MATH_DEF_INLINE b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_MATH_DEF_INLINE b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_MATH_DEF_INLINE b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_MATH_DEF_INLINE b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_MATH_DEF_INLINE b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_MATH_DEF_INLINE b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_MATH_DEF_INLINE b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_MATH_DEF_INLINE b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_MATH_DEF_INLINE b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_MATH_DEF_INLINE b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_MATH_DEF_INLINE b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_MATH_DEF_INLINE b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_MATH_DEF_INLINE b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_MATH_DEF_INLINE b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_MATH_DEF_INLINE b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_MATH_DEF_INLINE b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_MATH_DEF_INLINE b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_MATH_DEF_INLINE b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_MATH_DEF_INLINE b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_MATH_DEF_INLINE b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_MATH_DEF_INLINE b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_MATH_DEF_INLINE b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_MATH_DEF_INLINE b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_MATH_DEF_INLINE b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_MATH_DEF_INLINE b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_MATH_DEF_INLINE b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b);
AIL_MATH_DEF_INLINE b32 ail_mat2u8_eq(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_MATH_DEF_INLINE b32 ail_mat2i8_eq(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_MATH_DEF_INLINE b32 ail_mat2u16_eq(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_MATH_DEF_INLINE b32 ail_mat2i16_eq(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_MATH_DEF_INLINE b32 ail_mat2u32_eq(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_MATH_DEF_INLINE b32 ail_mat2i32_eq(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_MATH_DEF_INLINE b32 ail_mat2u64_eq(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_MATH_DEF_INLINE b32 ail_mat2i64_eq(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_MATH_DEF_INLINE b32 ail_mat2f32_eq(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_MATH_DEF_INLINE b32 ail_mat2f64_eq(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_MATH_DEF_INLINE b32 ail_mat3u8_eq(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_MATH_DEF_INLINE b32 ail_mat3i8_eq(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_MATH_DEF_INLINE b32 ail_mat3u16_eq(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_MATH_DEF_INLINE b32 ail_mat3i16_eq(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_MATH_DEF_INLINE b32 ail_mat3u32_eq(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_MATH_DEF_INLINE b32 ail_mat3i32_eq(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_MATH_DEF_INLINE b32 ail_mat3u64_eq(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_MATH_DEF_INLINE b32 ail_mat3i64_eq(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_MATH_DEF_INLINE b32 ail_mat3f32_eq(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_MATH_DEF_INLINE b32 ail_mat3f64_eq(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_MATH_DEF_INLINE b32 ail_mat4u8_eq(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_MATH_DEF_INLINE b32 ail_mat4i8_eq(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_MATH_DEF_INLINE b32 ail_mat4u16_eq(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_MATH_DEF_INLINE b32 ail_mat4i16_eq(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_MATH_DEF_INLINE b32 ail_mat4u32_eq(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_MATH_DEF_INLINE b32 ail_mat4i32_eq(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_MATH_DEF_INLINE b32 ail_mat4u64_eq(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_MATH_DEF_INLINE b32 ail_mat4i64_eq(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_MATH_DEF_INLINE b32 ail_mat4f32_eq(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_MATH_DEF_INLINE b32 ail_mat4f64_eq(AIL_Mat4f64 a, AIL_Mat4f64 b);

AIL_MATH_DEF_INLINE AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_MATH_DEF_INLINE AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_MATH_DEF_INLINE AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_MATH_DEF_INLINE AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_MATH_DEF_INLINE AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_MATH_DEF_INLINE AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_MATH_DEF_INLINE AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_MATH_DEF_INLINE AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_MATH_DEF_INLINE AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_MATH_DEF_INLINE AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_MATH_DEF_INLINE AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_MATH_DEF_INLINE AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_MATH_DEF_INLINE AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_MATH_DEF_INLINE AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_MATH_DEF_INLINE AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_MATH_DEF_INLINE AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_MATH_DEF_INLINE AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_MATH_DEF_INLINE AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_MATH_DEF_INLINE AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_MATH_DEF_INLINE AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_MATH_DEF_INLINE AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_MATH_DEF_INLINE AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_MATH_DEF_INLINE AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_MATH_DEF_INLINE AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_MATH_DEF_INLINE AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_MATH_DEF_INLINE AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_MATH_DEF_INLINE AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_MATH_DEF_INLINE AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_MATH_DEF_INLINE AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_MATH_DEF_INLINE AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b);
AIL_MATH_DEF_INLINE AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_MATH_DEF_INLINE AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_MATH_DEF_INLINE AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_MATH_DEF_INLINE AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_MATH_DEF_INLINE AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_MATH_DEF_INLINE AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_MATH_DEF_INLINE AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_MATH_DEF_INLINE AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_MATH_DEF_INLINE AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_MATH_DEF_INLINE AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_MATH_DEF_INLINE AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_MATH_DEF_INLINE AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_MATH_DEF_INLINE AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_MATH_DEF_INLINE AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_MATH_DEF_INLINE AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_MATH_DEF_INLINE AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_MATH_DEF_INLINE AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_MATH_DEF_INLINE AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_MATH_DEF_INLINE AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_MATH_DEF_INLINE AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_MATH_DEF_INLINE AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_MATH_DEF_INLINE AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_MATH_DEF_INLINE AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_MATH_DEF_INLINE AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_MATH_DEF_INLINE AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_MATH_DEF_INLINE AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_MATH_DEF_INLINE AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_MATH_DEF_INLINE AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_MATH_DEF_INLINE AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_MATH_DEF_INLINE AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b);

AIL_MATH_DEF_INLINE AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_MATH_DEF_INLINE AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_MATH_DEF_INLINE AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_MATH_DEF_INLINE AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_MATH_DEF_INLINE AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_MATH_DEF_INLINE AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_MATH_DEF_INLINE AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_MATH_DEF_INLINE AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_MATH_DEF_INLINE AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_MATH_DEF_INLINE AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_MATH_DEF_INLINE AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_MATH_DEF_INLINE AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_MATH_DEF_INLINE AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_MATH_DEF_INLINE AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_MATH_DEF_INLINE AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_MATH_DEF_INLINE AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_MATH_DEF_INLINE AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_MATH_DEF_INLINE AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_MATH_DEF_INLINE AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_MATH_DEF_INLINE AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_MATH_DEF_INLINE AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_MATH_DEF_INLINE AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_MATH_DEF_INLINE AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_MATH_DEF_INLINE AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_MATH_DEF_INLINE AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_MATH_DEF_INLINE AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_MATH_DEF_INLINE AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_MATH_DEF_INLINE AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_MATH_DEF_INLINE AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_MATH_DEF_INLINE AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b);
AIL_MATH_DEF_INLINE AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_MATH_DEF_INLINE AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_MATH_DEF_INLINE AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_MATH_DEF_INLINE AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_MATH_DEF_INLINE AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_MATH_DEF_INLINE AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_MATH_DEF_INLINE AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_MATH_DEF_INLINE AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_MATH_DEF_INLINE AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_MATH_DEF_INLINE AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_MATH_DEF_INLINE AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_MATH_DEF_INLINE AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_MATH_DEF_INLINE AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_MATH_DEF_INLINE AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_MATH_DEF_INLINE AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_MATH_DEF_INLINE AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_MATH_DEF_INLINE AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_MATH_DEF_INLINE AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_MATH_DEF_INLINE AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_MATH_DEF_INLINE AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_MATH_DEF_INLINE AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_MATH_DEF_INLINE AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_MATH_DEF_INLINE AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_MATH_DEF_INLINE AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_MATH_DEF_INLINE AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_MATH_DEF_INLINE AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_MATH_DEF_INLINE AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_MATH_DEF_INLINE AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_MATH_DEF_INLINE AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_MATH_DEF_INLINE AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b);


#endif // AIL_MATH_H_


////////////////////
// IMPLEMENTATION //
////////////////////

#ifdef AIL_MATH_IMPL
#ifndef _AIL_MATH_IMPL_GUARD_
#define _AIL_MATH_IMPL_GUARD_


////////////////////
// Linear Algebra //
////////////////////

AIL_Vec2u8 ail_vec2u8(u8 x, u8 y) {
    return (AIL_Vec2u8) { .x = x, .y = y };
}

AIL_Vec2i8 ail_vec2i8(i8 x, i8 y) {
    return (AIL_Vec2i8) { .x = x, .y = y };
}

AIL_Vec2u16 ail_vec2u16(u16 x, u16 y) {
    return (AIL_Vec2u16) { .x = x, .y = y };
}

AIL_Vec2i16 ail_vec2i16(i16 x, i16 y) {
    return (AIL_Vec2i16) { .x = x, .y = y };
}

AIL_Vec2u32 ail_vec2u32(u32 x, u32 y) {
    return (AIL_Vec2u32) { .x = x, .y = y };
}

AIL_Vec2i32 ail_vec2i32(i32 x, i32 y) {
    return (AIL_Vec2i32) { .x = x, .y = y };
}

AIL_Vec2u64 ail_vec2u64(u64 x, u64 y) {
    return (AIL_Vec2u64) { .x = x, .y = y };
}

AIL_Vec2i64 ail_vec2i64(i64 x, i64 y) {
    return (AIL_Vec2i64) { .x = x, .y = y };
}

AIL_Vec2f32 ail_vec2f32(f32 x, f32 y) {
    return (AIL_Vec2f32) { .x = x, .y = y };
}

AIL_Vec2f64 ail_vec2f64(f64 x, f64 y) {
    return (AIL_Vec2f64) { .x = x, .y = y };
}

AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z) {
    return (AIL_Vec3u8) { .x = x, .y = y, .z = z };
}

AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z) {
    return (AIL_Vec3i8) { .x = x, .y = y, .z = z };
}

AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z) {
    return (AIL_Vec3u16) { .x = x, .y = y, .z = z };
}

AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z) {
    return (AIL_Vec3i16) { .x = x, .y = y, .z = z };
}

AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z) {
    return (AIL_Vec3u32) { .x = x, .y = y, .z = z };
}

AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z) {
    return (AIL_Vec3i32) { .x = x, .y = y, .z = z };
}

AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z) {
    return (AIL_Vec3u64) { .x = x, .y = y, .z = z };
}

AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z) {
    return (AIL_Vec3i64) { .x = x, .y = y, .z = z };
}

AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z) {
    return (AIL_Vec3f32) { .x = x, .y = y, .z = z };
}

AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z) {
    return (AIL_Vec3f64) { .x = x, .y = y, .z = z };
}

AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w) {
    return (AIL_Vec4u8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w) {
    return (AIL_Vec4i8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w) {
    return (AIL_Vec4u16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w) {
    return (AIL_Vec4i16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w) {
    return (AIL_Vec4u32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w) {
    return (AIL_Vec4i32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w) {
    return (AIL_Vec4u64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w) {
    return (AIL_Vec4i64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w) {
    return (AIL_Vec4f32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w) {
    return (AIL_Vec4f64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Mat2u8 ail_mat2u8(u8 x11, u8 x12, u8 x21, u8 x22)
{
    return (AIL_Mat2u8) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2i8 ail_mat2i8(i8 x11, i8 x12, i8 x21, i8 x22)
{
    return (AIL_Mat2i8) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2u16 ail_mat2u16(u16 x11, u16 x12, u16 x21, u16 x22)
{
    return (AIL_Mat2u16) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2i16 ail_mat2i16(i16 x11, i16 x12, i16 x21, i16 x22)
{
    return (AIL_Mat2i16) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2u32 ail_mat2u32(u32 x11, u32 x12, u32 x21, u32 x22)
{
    return (AIL_Mat2u32) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2i32 ail_mat2i32(i32 x11, i32 x12, i32 x21, i32 x22)
{
    return (AIL_Mat2i32) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2u64 ail_mat2u64(u64 x11, u64 x12, u64 x21, u64 x22)
{
    return (AIL_Mat2u64) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2i64 ail_mat2i64(i64 x11, i64 x12, i64 x21, i64 x22)
{
    return (AIL_Mat2i64) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2f32 ail_mat2f32(f32 x11, f32 x12, f32 x21, f32 x22)
{
    return (AIL_Mat2f32) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat2f64 ail_mat2f64(f64 x11, f64 x12, f64 x21, f64 x22)
{
    return (AIL_Mat2f64) { .els = {
        { x11, x12 },
        { x21, x22 },
    }};
}

AIL_Mat3u8 ail_mat3u8(u8 x11, u8 x12, u8 x13, u8 x21, u8 x22, u8 x23, u8 x31, u8 x32, u8 x33)
{
    return (AIL_Mat3u8) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3i8 ail_mat3i8(i8 x11, i8 x12, i8 x13, i8 x21, i8 x22, i8 x23, i8 x31, i8 x32, i8 x33)
{
    return (AIL_Mat3i8) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3u16 ail_mat3u16(u16 x11, u16 x12, u16 x13, u16 x21, u16 x22, u16 x23, u16 x31, u16 x32, u16 x33)
{
    return (AIL_Mat3u16) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3i16 ail_mat3i16(i16 x11, i16 x12, i16 x13, i16 x21, i16 x22, i16 x23, i16 x31, i16 x32, i16 x33)
{
    return (AIL_Mat3i16) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3u32 ail_mat3u32(u32 x11, u32 x12, u32 x13, u32 x21, u32 x22, u32 x23, u32 x31, u32 x32, u32 x33)
{
    return (AIL_Mat3u32) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3i32 ail_mat3i32(i32 x11, i32 x12, i32 x13, i32 x21, i32 x22, i32 x23, i32 x31, i32 x32, i32 x33)
{
    return (AIL_Mat3i32) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3u64 ail_mat3u64(u64 x11, u64 x12, u64 x13, u64 x21, u64 x22, u64 x23, u64 x31, u64 x32, u64 x33)
{
    return (AIL_Mat3u64) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3i64 ail_mat3i64(i64 x11, i64 x12, i64 x13, i64 x21, i64 x22, i64 x23, i64 x31, i64 x32, i64 x33)
{
    return (AIL_Mat3i64) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3f32 ail_mat3f32(f32 x11, f32 x12, f32 x13, f32 x21, f32 x22, f32 x23, f32 x31, f32 x32, f32 x33)
{
    return (AIL_Mat3f32) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat3f64 ail_mat3f64(f64 x11, f64 x12, f64 x13, f64 x21, f64 x22, f64 x23, f64 x31, f64 x32, f64 x33)
{
    return (AIL_Mat3f64) { .els = {
        { x11, x12, x13 },
        { x21, x22, x23 },
        { x31, x32, x33 },
    }};
}

AIL_Mat4u8 ail_mat4u8(u8 x11, u8 x12, u8 x13, u8 x14, u8 x21, u8 x22, u8 x23, u8 x24, u8 x31, u8 x32, u8 x33, u8 x34, u8 x41, u8 x42, u8 x43, u8 x44)
{
    return (AIL_Mat4u8) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4i8 ail_mat4i8(i8 x11, i8 x12, i8 x13, i8 x14, i8 x21, i8 x22, i8 x23, i8 x24, i8 x31, i8 x32, i8 x33, i8 x34, i8 x41, i8 x42, i8 x43, i8 x44)
{
    return (AIL_Mat4i8) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4u16 ail_mat4u16(u16 x11, u16 x12, u16 x13, u16 x14, u16 x21, u16 x22, u16 x23, u16 x24, u16 x31, u16 x32, u16 x33, u16 x34, u16 x41, u16 x42, u16 x43, u16 x44)
{
    return (AIL_Mat4u16) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4i16 ail_mat4i16(i16 x11, i16 x12, i16 x13, i16 x14, i16 x21, i16 x22, i16 x23, i16 x24, i16 x31, i16 x32, i16 x33, i16 x34, i16 x41, i16 x42, i16 x43, i16 x44)
{
    return (AIL_Mat4i16) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4u32 ail_mat4u32(u32 x11, u32 x12, u32 x13, u32 x14, u32 x21, u32 x22, u32 x23, u32 x24, u32 x31, u32 x32, u32 x33, u32 x34, u32 x41, u32 x42, u32 x43, u32 x44)
{
    return (AIL_Mat4u32) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4i32 ail_mat4i32(i32 x11, i32 x12, i32 x13, i32 x14, i32 x21, i32 x22, i32 x23, i32 x24, i32 x31, i32 x32, i32 x33, i32 x34, i32 x41, i32 x42, i32 x43, i32 x44)
{
    return (AIL_Mat4i32) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4u64 ail_mat4u64(u64 x11, u64 x12, u64 x13, u64 x14, u64 x21, u64 x22, u64 x23, u64 x24, u64 x31, u64 x32, u64 x33, u64 x34, u64 x41, u64 x42, u64 x43, u64 x44)
{
    return (AIL_Mat4u64) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4i64 ail_mat4i64(i64 x11, i64 x12, i64 x13, i64 x14, i64 x21, i64 x22, i64 x23, i64 x24, i64 x31, i64 x32, i64 x33, i64 x34, i64 x41, i64 x42, i64 x43, i64 x44)
{
    return (AIL_Mat4i64) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4f32 ail_mat4f32(f32 x11, f32 x12, f32 x13, f32 x14, f32 x21, f32 x22, f32 x23, f32 x24, f32 x31, f32 x32, f32 x33, f32 x34, f32 x41, f32 x42, f32 x43, f32 x44)
{
    return (AIL_Mat4f32) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

AIL_Mat4f64 ail_mat4f64(f64 x11, f64 x12, f64 x13, f64 x14, f64 x21, f64 x22, f64 x23, f64 x24, f64 x31, f64 x32, f64 x33, f64 x34, f64 x41, f64 x42, f64 x43, f64 x44)
{
    return (AIL_Mat4f64) { .els = {
        { x11, x12, x13, x14 },
        { x21, x22, x23, x24 },
        { x31, x32, x33, x34 },
        { x41, x42, x43, x44 },
    }};
}

b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1];
}

b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2];
}

b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return a.els[0] == b.els[0] && a.els[1] == b.els[1] && a.els[2] == b.els[2] && a.els[3] == b.els[3];
}

b32 ail_mat2u8_eq(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2i8_eq(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2u16_eq(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2i16_eq(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2u32_eq(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2i32_eq(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2u64_eq(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2i64_eq(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2f32_eq(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat2f64_eq(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1];
}

b32 ail_mat3u8_eq(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3i8_eq(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3u16_eq(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3i16_eq(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3u32_eq(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3i32_eq(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3u64_eq(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3i64_eq(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3f32_eq(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat3f64_eq(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2];
}

b32 ail_mat4u8_eq(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4i8_eq(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4u16_eq(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4i16_eq(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4u32_eq(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4i32_eq(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4u64_eq(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4i64_eq(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4f32_eq(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

b32 ail_mat4f64_eq(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return a.els[0][0] == b.els[0][0] && a.els[0][1] == b.els[0][1] && a.els[0][2] == b.els[0][2] && a.els[0][3] == b.els[0][3] &&
           a.els[1][0] == b.els[1][0] && a.els[1][1] == b.els[1][1] && a.els[1][2] == b.els[1][2] && a.els[1][3] == b.els[1][3] &&
           a.els[2][0] == b.els[2][0] && a.els[2][1] == b.els[2][1] && a.els[2][2] == b.els[2][2] && a.els[2][3] == b.els[2][3] &&
           a.els[3][0] == b.els[3][0] && a.els[3][1] == b.els[3][1] && a.els[3][2] == b.els[3][2] && a.els[3][3] == b.els[3][3];
}

AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1] }};
}

AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2] }};
}

AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .els = { a.els[0] + b.els[0], a.els[1] + b.els[1], a.els[2] + b.els[2], a.els[3] + b.els[3] }};
}

AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1] },
    }};
}

AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2] },
    }};
}

AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3] },
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3] },
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3] },
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3] },
    }};
}

AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1] }};
}

AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2] }};
}

AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .els = { a.els[0] - b.els[0], a.els[1] - b.els[1], a.els[2] - b.els[2], a.els[3] - b.els[3] }};
}

AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1] },
    }};
}

AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2] },
    }};
}

AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}

AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3] },
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3] },
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3] },
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3] },
    }};
}




////////////////////
// Floating Point //
////////////////////




#endif // _AIL_MATH_IMPL_GUARD_
#endif // AIL_MATH_IMPL
