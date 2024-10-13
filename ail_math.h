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

#define AIL_TYPES_IMPL
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
    AIL_Vec2u8 cols;
} AIL_Mat2u8;
