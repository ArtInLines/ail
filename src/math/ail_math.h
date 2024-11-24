/*
*** Common Math functions ***
* @TODO: Move code to different files
*
* Heavily inspired by the following libraries (in no particular order):
  * la              (https://github.com/tsoding/la)
  * mathc           (https://github.com/felselva/mathc)
  * Raylib          (https://github.com/raysan5/raylib/blob/master/src/raymath.h)
  * fastmod         (https://github.com/lemire/fastmod)
  * HandmadeMath    (https://github.com/HandmadeMath/HandmadeMath)
  * tiny-fixedpoint (https://github.com/kokke/tiny-fixedpoint-c)
*/

#ifndef _AIL_MATH_H_
#define _AIL_MATH_H_

#include "ail_base.h"


///////////////////////////////////
//                               //
//      Function Overloding      //
//                               //
///////////////////////////////////

// @Important: These size definitions for C's standard types might map to bigger int types than actually true for this platform, but if you want proper type inference, you should just use properly sized types (i.e. u8, i16, f32, etc.) anyways
#define _AIL_MATH_GENERIC_INTEGRAL_NAME_1(pre, post, var) _Generic((var), \
    unsigned char:          _AIL_CONCAT_3(pre, u8,  post), \
    signed char:            _AIL_CONCAT_3(pre, i8,  post), \
    unsigned short:         _AIL_CONCAT_3(pre, u16, post), \
    signed short:           _AIL_CONCAT_3(pre, i16, post), \
    unsigned int:           _AIL_CONCAT_3(pre, u32, post), \
    signed int:             _AIL_CONCAT_3(pre, i32, post), \
    unsigned long int:      _AIL_CONCAT_3(pre, u64, post), \
    signed long int:        _AIL_CONCAT_3(pre, i64, post), \
    unsigned long long int: _AIL_CONCAT_3(pre, u64, post), \
    signed long long int:   _AIL_CONCAT_3(pre, i64, post), \
    float:                  _AIL_CONCAT_3(pre, f32, post), \
    double:                 _AIL_CONCAT_3(pre, f64, post)  \
)

#define AIL_MATH_GENERIC_INTEGRAL_NAME(pre, post, arg0) _Generic((arg0), \
    u8:  _AIL_CONCAT_3(pre, u8,  post), \
    i8:  _AIL_CONCAT_3(pre, i8,  post), \
    u16: _AIL_CONCAT_3(pre, u16, post), \
    i16: _AIL_CONCAT_3(pre, i16, post), \
    u32: _AIL_CONCAT_3(pre, u32, post), \
    i32: _AIL_CONCAT_3(pre, i32, post), \
    u64: _AIL_CONCAT_3(pre, u64, post), \
    i64: _AIL_CONCAT_3(pre, i64, post), \
    f32: _AIL_CONCAT_3(pre, f32, post), \
    f64: _AIL_CONCAT_3(pre, f64, post), \
    default: _AIL_MATH_GENERIC_INTEGRAL_NAME_1(pre, post, arg0) \
)
#define AIL_MATH_GENERIC_INTEGRAL(pre, post, arg0, ...) AIL_MATH_GENERIC_INTEGRAL_NAME(pre, post, arg0)((arg0), __VA_ARGS__)

#define AIL_MATH_GENERIC_LINALG(pre, post, arg0, ...) AIL_MATH_GENERIC_INTEGRAL_NAME(pre, post, (arg0).els[0])((arg0), __VA_ARGS__)

#define _AIL_MATH_GENERIC_BINOP_LA_4(pre, post, arg0, arg1) AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1)
#define _AIL_MATH_GENERIC_BINOP_LA_5(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_4(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_6(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_5(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_7(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_6(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_8(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_7(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_9(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_8(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_10(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_9(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_11(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_10(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_12(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_11(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_13(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_12(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_14(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_13(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_15(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_14(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define _AIL_MATH_GENERIC_BINOP_LA_16(pre, post, arg0, arg1, ...) _AIL_MATH_GENERIC_BINOP_LA_15(pre, post, AIL_MATH_GENERIC_LINALG(pre, post, arg0, arg1), __VA_ARGS__)
#define AIL_MATH_GENERIC_BINOP_LA(pre, post, arg0, ...) AIL_VFUNC(_AIL_MATH_GENERIC_BINOP_LA_, pre, post, arg0, __VA_ARGS__)


//////////////////////
//  Linear Algebra  //
//////////////////////

#define ail_vec2(x, y) AIL_MATH_GENERIC_INTEGRAL(ail_vec2, , x, y)
#define ail_vec3(x, y, z) AIL_MATH_GENERIC_INTEGRAL(ail_vec3, , x, y, z)
#define ail_vec4(x, y, z, w) AIL_MATH_GENERIC_INTEGRAL(ail_vec4, , x, y, z, w)

#define ail_vec2_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec2, _add, a, __VA_ARGS__)
#define ail_vec3_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec3, _add, a, __VA_ARGS__)
#define ail_vec4_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec4, _add, a, __VA_ARGS__)

#define ail_vec2_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec2, _sub, a, __VA_ARGS__)
#define ail_vec3_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec3, _sub, a, __VA_ARGS__)
#define ail_vec4_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec4, _sub, a, __VA_ARGS__)

#define ail_vec2_eq(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec2, _eq, a, __VA_ARGS__)
#define ail_vec3_eq(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec3, _eq, a, __VA_ARGS__)
#define ail_vec4_eq(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec4, _eq, a, __VA_ARGS__)

#define ail_vec2_dot(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec2, _dot, a, __VA_ARGS__)
#define ail_vec3_dot(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec3, _dot, a, __VA_ARGS__)
#define ail_vec4_dot(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_vec4, _dot, a, __VA_ARGS__)

#define ail_mat2_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat2, _add, a, __VA_ARGS__)
#define ail_mat3_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat3, _add, a, __VA_ARGS__)
#define ail_mat4_add(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat4, _add, a, __VA_ARGS__)

#define ail_mat2_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat2, _sub, a, __VA_ARGS__)
#define ail_mat3_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat3, _sub, a, __VA_ARGS__)
#define ail_mat4_sub(a, ...) AIL_MATH_GENERIC_BINOP_LA(ail_mat4, _sub, a, __VA_ARGS__)



////////////////////////////////
//                            //
//      Type Definitions      //
//                            //
////////////////////////////////


//////////////////////
//  Linear Algebra  //
//////////////////////

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
    struct { AIL_Vec2u8 rg; u8 _2; };
    struct { u8 _3; AIL_Vec2u8 gb; };
    struct { AIL_Vec2u8 uv; u8 _4; };
    struct { u8 _5; AIL_Vec2u8 vw; };
} AIL_Vec3u8;

typedef union AIL_Vec3i8 {
    i8 els[3];
    struct { i8 x, y, z; };
    struct { i8 r, g, b; };
    struct { i8 u, v, w; };
    struct { AIL_Vec2i8 xy; i8 _0; };
    struct { i8 _1; AIL_Vec2i8 yz; };
    struct { AIL_Vec2i8 rg; i8 _2; };
    struct { i8 _3; AIL_Vec2i8 gb; };
    struct { AIL_Vec2i8 uv; i8 _4; };
    struct { i8 _5; AIL_Vec2i8 vw; };
} AIL_Vec3i8;

typedef union AIL_Vec3u16 {
    u16 els[3];
    struct { u16 x, y, z; };
    struct { u16 r, g, b; };
    struct { u16 u, v, w; };
    struct { AIL_Vec2u16 xy; u16 _0; };
    struct { u16 _1; AIL_Vec2u16 yz; };
    struct { AIL_Vec2u16 rg; u16 _2; };
    struct { u16 _3; AIL_Vec2u16 gb; };
    struct { AIL_Vec2u16 uv; u16 _4; };
    struct { u16 _5; AIL_Vec2u16 vw; };
} AIL_Vec3u16;

typedef union AIL_Vec3i16 {
    i16 els[3];
    struct { i16 x, y, z; };
    struct { i16 r, g, b; };
    struct { i16 u, v, w; };
    struct { AIL_Vec2i16 xy; i16 _0; };
    struct { i16 _1; AIL_Vec2i16 yz; };
    struct { AIL_Vec2i16 rg; i16 _2; };
    struct { i16 _3; AIL_Vec2i16 gb; };
    struct { AIL_Vec2i16 uv; i16 _4; };
    struct { i16 _5; AIL_Vec2i16 vw; };
} AIL_Vec3i16;

typedef union AIL_Vec3u32 {
    u32 els[3];
    struct { u32 x, y, z; };
    struct { u32 r, g, b; };
    struct { u32 u, v, w; };
    struct { AIL_Vec2u32 xy; u32 _0; };
    struct { u32 _1; AIL_Vec2u32 yz; };
    struct { AIL_Vec2u32 rg; u32 _2; };
    struct { u32 _3; AIL_Vec2u32 gb; };
    struct { AIL_Vec2u32 uv; u32 _4; };
    struct { u32 _5; AIL_Vec2u32 vw; };
} AIL_Vec3u32;

typedef union AIL_Vec3i32 {
    i32 els[3];
    struct { i32 x, y, z; };
    struct { i32 r, g, b; };
    struct { i32 u, v, w; };
    struct { AIL_Vec2i32 xy; i32 _0; };
    struct { i32 _1; AIL_Vec2i32 yz; };
    struct { AIL_Vec2i32 rg; i32 _2; };
    struct { i32 _3; AIL_Vec2i32 gb; };
    struct { AIL_Vec2i32 uv; i32 _4; };
    struct { i32 _5; AIL_Vec2i32 vw; };
} AIL_Vec3i32;

typedef union AIL_Vec3u64 {
    u64 els[3];
    struct { u64 x, y, z; };
    struct { u64 r, g, b; };
    struct { u64 u, v, w; };
    struct { AIL_Vec2u64 xy; u64 _0; };
    struct { u64 _1; AIL_Vec2u64 yz; };
    struct { AIL_Vec2u64 rg; u64 _2; };
    struct { u64 _3; AIL_Vec2u64 gb; };
    struct { AIL_Vec2u64 uv; u64 _4; };
    struct { u64 _5; AIL_Vec2u64 vw; };
} AIL_Vec3u64;

typedef union AIL_Vec3i64 {
    i64 els[3];
    struct { i64 x, y, z; };
    struct { i64 r, g, b; };
    struct { i64 u, v, w; };
    struct { AIL_Vec2i64 xy; i64 _0; };
    struct { i64 _1; AIL_Vec2i64 yz; };
    struct { AIL_Vec2i64 rg; i64 _2; };
    struct { i64 _3; AIL_Vec2i64 gb; };
    struct { AIL_Vec2i64 uv; i64 _4; };
    struct { i64 _5; AIL_Vec2i64 vw; };
} AIL_Vec3i64;

typedef union AIL_Vec3f32 {
    f32 els[3];
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { f32 u, v, w; };
    struct { AIL_Vec2f32 xy; f32 _0; };
    struct { f32 _1; AIL_Vec2f32 yz; };
    struct { AIL_Vec2f32 rg; f32 _2; };
    struct { f32 _3; AIL_Vec2f32 gb; };
    struct { AIL_Vec2f32 uv; f32 _4; };
    struct { f32 _5; AIL_Vec2f32 vw; };
} AIL_Vec3f32;

typedef union AIL_Vec3f64 {
    f64 els[3];
    struct { f64 x, y, z; };
    struct { f64 r, g, b; };
    struct { f64 u, v, w; };
    struct { AIL_Vec2f64 xy; f64 _0; };
    struct { f64 _1; AIL_Vec2f64 yz; };
    struct { AIL_Vec2f64 rg; f64 _2; };
    struct { f64 _3; AIL_Vec2f64 gb; };
    struct { AIL_Vec2f64 uv; f64 _4; };
    struct { f64 _5; AIL_Vec2f64 vw; };
} AIL_Vec3f64;

typedef union AIL_Vec4u8 {
    u8 els[4];
    struct { u8 x, y, z, w; };
    struct { u8 r, g, b, a; };
    struct { AIL_Vec3u8 xyz; u8 _0; };
    struct { u8 _1; AIL_Vec3u8 yzw; };
    struct { AIL_Vec2u8 xy; u8 _2[2]; };
    struct { u8 _3; AIL_Vec2u8 yz; u8 _4; };
    struct { u8 _5[2]; AIL_Vec2u8 zw; };
    struct { AIL_Vec3u8 rgb; u8 _6; };
    struct { u8 _7; AIL_Vec3u8 gba; };
    struct { AIL_Vec2u8 rg; u8 _8[2]; };
    struct { u8 _9; AIL_Vec2u8 gb; u8 _10; };
    struct { u8 _11[2]; AIL_Vec2u8 ba; };
} AIL_Vec4u8;

typedef union AIL_Vec4i8 {
    i8 els[4];
    struct { i8 x, y, z, w; };
    struct { i8 r, g, b, a; };
    struct { AIL_Vec3i8 xyz; i8 _0; };
    struct { i8 _1; AIL_Vec3i8 yzw; };
    struct { AIL_Vec2i8 xy; i8 _2[2]; };
    struct { i8 _3; AIL_Vec2i8 yz; i8 _4; };
    struct { i8 _5[2]; AIL_Vec2i8 zw; };
    struct { AIL_Vec3i8 rgb; i8 _6; };
    struct { i8 _7; AIL_Vec3i8 gba; };
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
    struct { u8 x11, x21, x12, x22; };
} AIL_Mat2u8;

typedef union AIL_Mat2i8 {
    i8 els[2][2];
    AIL_Vec2i8 cols[2];
    struct { i8 x11, x21, x12, x22; };
} AIL_Mat2i8;

typedef union AIL_Mat2u16 {
    u16 els[2][2];
    AIL_Vec2u16 cols[2];
    struct { u16 x11, x21, x12, x22; };
} AIL_Mat2u16;

typedef union AIL_Mat2i16 {
    i16 els[2][2];
    AIL_Vec2i16 cols[2];
    struct { i16 x11, x21, x12, x22; };
} AIL_Mat2i16;

typedef union AIL_Mat2u32 {
    u32 els[2][2];
    AIL_Vec2u32 cols[2];
    struct { u32 x11, x21, x12, x22; };
} AIL_Mat2u32;

typedef union AIL_Mat2i32 {
    i32 els[2][2];
    AIL_Vec2i32 cols[2];
    struct { i32 x11, x21, x12, x22; };
} AIL_Mat2i32;

typedef union AIL_Mat2u64 {
    u64 els[2][2];
    AIL_Vec2u64 cols[2];
    struct { u64 x11, x21, x12, x22; };
} AIL_Mat2u64;

typedef union AIL_Mat2i64 {
    i64 els[2][2];
    AIL_Vec2i64 cols[2];
    struct { i64 x11, x21, x12, x22; };
} AIL_Mat2i64;

typedef union AIL_Mat2f32 {
    f32 els[2][2];
    AIL_Vec2f32 cols[2];
    struct { f32 x11, x21, x12, x22; };
} AIL_Mat2f32;

typedef union AIL_Mat2f64 {
    f64 els[2][2];
    AIL_Vec2f64 cols[2];
    struct { f64 x11, x21, x12, x22; };
} AIL_Mat2f64;

typedef union AIL_Mat3u8 {
    u8 els[3][3];
    AIL_Vec3u8 cols[3];
    struct { u8 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u8;

typedef union AIL_Mat3i8 {
    i8 els[3][3];
    AIL_Vec3i8 cols[3];
    struct { i8 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i8;

typedef union AIL_Mat3u16 {
    u16 els[3][3];
    AIL_Vec3u16 cols[3];
    struct { u16 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u16;

typedef union AIL_Mat3i16 {
    i16 els[3][3];
    AIL_Vec3i16 cols[3];
    struct { i16 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i16;

typedef union AIL_Mat3u32 {
    u32 els[3][3];
    AIL_Vec3u32 cols[3];
    struct { u32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u32;

typedef union AIL_Mat3i32 {
    i32 els[3][3];
    AIL_Vec3i32 cols[3];
    struct { i32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i32;

typedef union AIL_Mat3u64 {
    u64 els[3][3];
    AIL_Vec3u64 cols[3];
    struct { u64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3u64;

typedef union AIL_Mat3i64 {
    i64 els[3][3];
    AIL_Vec3i64 cols[3];
    struct { i64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3i64;

typedef union AIL_Mat3f32 {
    f32 els[3][3];
    AIL_Vec3f32 cols[3];
    struct { f32 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3f32;

typedef union AIL_Mat3f64 {
    f64 els[3][3];
    AIL_Vec3f64 cols[3];
    struct { f64 x11, x21, x31, x12, x22, x32, x13, x23, x33; };
} AIL_Mat3f64;

typedef union AIL_Mat4u8 {
    u8 els[4][4];
    AIL_Vec4u8 cols[4];
    struct { u8 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u8;

typedef union AIL_Mat4i8 {
    i8 els[4][4];
    AIL_Vec4i8 cols[4];
    struct { i8 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i8;

typedef union AIL_Mat4u16 {
    u16 els[4][4];
    AIL_Vec4u16 cols[4];
    struct { u16 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u16;

typedef union AIL_Mat4i16 {
    i16 els[4][4];
    AIL_Vec4i16 cols[4];
    struct { i16 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i16;

typedef union AIL_Mat4u32 {
    u32 els[4][4];
    AIL_Vec4u32 cols[4];
    struct { u32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u32;

typedef union AIL_Mat4i32 {
    i32 els[4][4];
    AIL_Vec4i32 cols[4];
    struct { i32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i32;

typedef union AIL_Mat4u64 {
    u64 els[4][4];
    AIL_Vec4u64 cols[4];
    struct { u64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4u64;

typedef union AIL_Mat4i64 {
    i64 els[4][4];
    AIL_Vec4i64 cols[4];
    struct { i64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4i64;

typedef union AIL_Mat4f32 {
    f32 els[4][4];
    AIL_Vec4f32 cols[4];
    struct { f32 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4f32;

typedef union AIL_Mat4f64 {
    f64 els[4][4];
    AIL_Vec4f64 cols[4];
    struct { f64 x11, x21, x31, x41, x12, x22, x32, x42, x13, x23, x33, x43, x14, x24, x34, x44; };
} AIL_Mat4f64;



/////////////////////////////////////
//                                 //
//      Function Declarations      //
//                                 //
/////////////////////////////////////


//////////////////////
//  Linear Algebra  //
//////////////////////

AIL_Vec2u8 ail_vec2u8(u8 x, u8 y);
AIL_Vec2i8 ail_vec2i8(i8 x, i8 y);
AIL_Vec2u16 ail_vec2u16(u16 x, u16 y);
AIL_Vec2i16 ail_vec2i16(i16 x, i16 y);
AIL_Vec2u32 ail_vec2u32(u32 x, u32 y);
AIL_Vec2i32 ail_vec2i32(i32 x, i32 y);
AIL_Vec2u64 ail_vec2u64(u64 x, u64 y);
AIL_Vec2i64 ail_vec2i64(i64 x, i64 y);
AIL_Vec2f32 ail_vec2f32(f32 x, f32 y);
AIL_Vec2f64 ail_vec2f64(f64 x, f64 y);
AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z);
AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z);
AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z);
AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z);
AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z);
AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z);
AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z);
AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z);
AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z);
AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z);
AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w);
AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w);
AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w);
AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w);
AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w);
AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w);
AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w);
AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w);
AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w);
AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w);

AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b);

AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b);
AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b);
AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b);
AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b);
AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b);
AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b);
AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b);
AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b);
AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b);
AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b);
AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b);
AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b);
AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b);
AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b);
AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b);
AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b);
AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b);
AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b);
AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b);
AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b);
AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b);
AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b);
AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b);
AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b);
AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b);
AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b);
AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b);
AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b);
AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b);
AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b);

b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b);
b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b);
b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b);
b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b);
b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b);
b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b);
b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b);
b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b);
b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b);
b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b);
b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b);
b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b);
b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b);
b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b);
b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b);
b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b);
b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b);
b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b);
b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b);
b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b);
b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b);
b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b);
b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b);
b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b);
b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b);
b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b);
b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b);
b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b);
b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b);
b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b);

u8 ail_vec2u8_dot(AIL_Vec2u8 a, AIL_Vec2u8 b);
i8 ail_vec2i8_dot(AIL_Vec2i8 a, AIL_Vec2i8 b);
u16 ail_vec2u16_dot(AIL_Vec2u16 a, AIL_Vec2u16 b);
i16 ail_vec2i16_dot(AIL_Vec2i16 a, AIL_Vec2i16 b);
u32 ail_vec2u32_dot(AIL_Vec2u32 a, AIL_Vec2u32 b);
i32 ail_vec2i32_dot(AIL_Vec2i32 a, AIL_Vec2i32 b);
u64 ail_vec2u64_dot(AIL_Vec2u64 a, AIL_Vec2u64 b);
i64 ail_vec2i64_dot(AIL_Vec2i64 a, AIL_Vec2i64 b);
f32 ail_vec2f32_dot(AIL_Vec2f32 a, AIL_Vec2f32 b);
f64 ail_vec2f64_dot(AIL_Vec2f64 a, AIL_Vec2f64 b);
u8 ail_vec3u8_dot(AIL_Vec3u8 a, AIL_Vec3u8 b);
i8 ail_vec3i8_dot(AIL_Vec3i8 a, AIL_Vec3i8 b);
u16 ail_vec3u16_dot(AIL_Vec3u16 a, AIL_Vec3u16 b);
i16 ail_vec3i16_dot(AIL_Vec3i16 a, AIL_Vec3i16 b);
u32 ail_vec3u32_dot(AIL_Vec3u32 a, AIL_Vec3u32 b);
i32 ail_vec3i32_dot(AIL_Vec3i32 a, AIL_Vec3i32 b);
u64 ail_vec3u64_dot(AIL_Vec3u64 a, AIL_Vec3u64 b);
i64 ail_vec3i64_dot(AIL_Vec3i64 a, AIL_Vec3i64 b);
f32 ail_vec3f32_dot(AIL_Vec3f32 a, AIL_Vec3f32 b);
f64 ail_vec3f64_dot(AIL_Vec3f64 a, AIL_Vec3f64 b);
u8 ail_vec4u8_dot(AIL_Vec4u8 a, AIL_Vec4u8 b);
i8 ail_vec4i8_dot(AIL_Vec4i8 a, AIL_Vec4i8 b);
u16 ail_vec4u16_dot(AIL_Vec4u16 a, AIL_Vec4u16 b);
i16 ail_vec4i16_dot(AIL_Vec4i16 a, AIL_Vec4i16 b);
u32 ail_vec4u32_dot(AIL_Vec4u32 a, AIL_Vec4u32 b);
i32 ail_vec4i32_dot(AIL_Vec4i32 a, AIL_Vec4i32 b);
u64 ail_vec4u64_dot(AIL_Vec4u64 a, AIL_Vec4u64 b);
i64 ail_vec4i64_dot(AIL_Vec4i64 a, AIL_Vec4i64 b);
f32 ail_vec4f32_dot(AIL_Vec4f32 a, AIL_Vec4f32 b);
f64 ail_vec4f64_dot(AIL_Vec4f64 a, AIL_Vec4f64 b);

AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b);

AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b);
AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b);
AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b);
AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b);
AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b);
AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b);
AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b);
AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b);
AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b);
AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b);
AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b);
AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b);
AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b);
AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b);
AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b);
AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b);
AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b);
AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b);
AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b);
AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b);
AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b);
AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b);
AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b);
AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b);
AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b);
AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b);
AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b);
AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b);
AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b);
AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b);

#endif // _AIL_MATH_H_




//////////////////////////////
//                          //
//      Implementation      //
//                          //
//////////////////////////////
#if !defined(AIL_NO_MATH_IMPL) && !defined(AIL_NO_BASE_IMPL) && !defined(AIL_NO_IMPL)
#ifndef _AIL_MATH_IMPL_GUARD_
#define _AIL_MATH_IMPL_GUARD_


//////////////////////
//  Linear Algebra  //
//////////////////////

AIL_Vec2u8 ail_vec2u8(u8 x, u8 y)
{
    return (AIL_Vec2u8) { .x = x, .y = y };
}

AIL_Vec2i8 ail_vec2i8(i8 x, i8 y)
{
    return (AIL_Vec2i8) { .x = x, .y = y };
}

AIL_Vec2u16 ail_vec2u16(u16 x, u16 y)
{
    return (AIL_Vec2u16) { .x = x, .y = y };
}

AIL_Vec2i16 ail_vec2i16(i16 x, i16 y)
{
    return (AIL_Vec2i16) { .x = x, .y = y };
}

AIL_Vec2u32 ail_vec2u32(u32 x, u32 y)
{
    return (AIL_Vec2u32) { .x = x, .y = y };
}

AIL_Vec2i32 ail_vec2i32(i32 x, i32 y)
{
    return (AIL_Vec2i32) { .x = x, .y = y };
}

AIL_Vec2u64 ail_vec2u64(u64 x, u64 y)
{
    return (AIL_Vec2u64) { .x = x, .y = y };
}

AIL_Vec2i64 ail_vec2i64(i64 x, i64 y)
{
    return (AIL_Vec2i64) { .x = x, .y = y };
}

AIL_Vec2f32 ail_vec2f32(f32 x, f32 y)
{
    return (AIL_Vec2f32) { .x = x, .y = y };
}

AIL_Vec2f64 ail_vec2f64(f64 x, f64 y)
{
    return (AIL_Vec2f64) { .x = x, .y = y };
}

AIL_Vec3u8 ail_vec3u8(u8 x, u8 y, u8 z)
{
    return (AIL_Vec3u8) { .x = x, .y = y, .z = z };
}

AIL_Vec3i8 ail_vec3i8(i8 x, i8 y, i8 z)
{
    return (AIL_Vec3i8) { .x = x, .y = y, .z = z };
}

AIL_Vec3u16 ail_vec3u16(u16 x, u16 y, u16 z)
{
    return (AIL_Vec3u16) { .x = x, .y = y, .z = z };
}

AIL_Vec3i16 ail_vec3i16(i16 x, i16 y, i16 z)
{
    return (AIL_Vec3i16) { .x = x, .y = y, .z = z };
}

AIL_Vec3u32 ail_vec3u32(u32 x, u32 y, u32 z)
{
    return (AIL_Vec3u32) { .x = x, .y = y, .z = z };
}

AIL_Vec3i32 ail_vec3i32(i32 x, i32 y, i32 z)
{
    return (AIL_Vec3i32) { .x = x, .y = y, .z = z };
}

AIL_Vec3u64 ail_vec3u64(u64 x, u64 y, u64 z)
{
    return (AIL_Vec3u64) { .x = x, .y = y, .z = z };
}

AIL_Vec3i64 ail_vec3i64(i64 x, i64 y, i64 z)
{
    return (AIL_Vec3i64) { .x = x, .y = y, .z = z };
}

AIL_Vec3f32 ail_vec3f32(f32 x, f32 y, f32 z)
{
    return (AIL_Vec3f32) { .x = x, .y = y, .z = z };
}

AIL_Vec3f64 ail_vec3f64(f64 x, f64 y, f64 z)
{
    return (AIL_Vec3f64) { .x = x, .y = y, .z = z };
}

AIL_Vec4u8 ail_vec4u8(u8 x, u8 y, u8 z, u8 w)
{
    return (AIL_Vec4u8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i8 ail_vec4i8(i8 x, i8 y, i8 z, i8 w)
{
    return (AIL_Vec4i8) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u16 ail_vec4u16(u16 x, u16 y, u16 z, u16 w)
{
    return (AIL_Vec4u16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i16 ail_vec4i16(i16 x, i16 y, i16 z, i16 w)
{
    return (AIL_Vec4i16) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u32 ail_vec4u32(u32 x, u32 y, u32 z, u32 w)
{
    return (AIL_Vec4u32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i32 ail_vec4i32(i32 x, i32 y, i32 z, i32 w)
{
    return (AIL_Vec4i32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4u64 ail_vec4u64(u64 x, u64 y, u64 z, u64 w)
{
    return (AIL_Vec4u64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4i64 ail_vec4i64(i64 x, i64 y, i64 z, i64 w)
{
    return (AIL_Vec4i64) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f32 ail_vec4f32(f32 x, f32 y, f32 z, f32 w)
{
    return (AIL_Vec4f32) { .x = x, .y = y, .z = z, .w = w };
}

AIL_Vec4f64 ail_vec4f64(f64 x, f64 y, f64 z, f64 w)
{
    return (AIL_Vec4f64) { .x = x, .y = y, .z = z, .w = w };
}


AIL_Vec2u8 ail_vec2u8_add(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i8 ail_vec2i8_add(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u16 ail_vec2u16_add(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i16 ail_vec2i16_add(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u32 ail_vec2u32_add(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i32 ail_vec2i32_add(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2u64 ail_vec2u64_add(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2i64 ail_vec2i64_add(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2f32 ail_vec2f32_add(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec2f64 ail_vec2f64_add(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .x = a.x + b.x, .y = a.y + b.y };
}

AIL_Vec3u8 ail_vec3u8_add(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i8 ail_vec3i8_add(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u16 ail_vec3u16_add(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i16 ail_vec3i16_add(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u32 ail_vec3u32_add(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i32 ail_vec3i32_add(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3u64 ail_vec3u64_add(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3i64 ail_vec3i64_add(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3f32 ail_vec3f32_add(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec3f64 ail_vec3f64_add(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

AIL_Vec4u8 ail_vec4u8_add(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i8 ail_vec4i8_add(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u16 ail_vec4u16_add(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i16 ail_vec4i16_add(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u32 ail_vec4u32_add(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i32 ail_vec4i32_add(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4u64 ail_vec4u64_add(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4i64 ail_vec4i64_add(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4f32 ail_vec4f32_add(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

AIL_Vec4f64 ail_vec4f64_add(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}


AIL_Vec2u8 ail_vec2u8_sub(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return (AIL_Vec2u8) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i8 ail_vec2i8_sub(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return (AIL_Vec2i8) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u16 ail_vec2u16_sub(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return (AIL_Vec2u16) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i16 ail_vec2i16_sub(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return (AIL_Vec2i16) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u32 ail_vec2u32_sub(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return (AIL_Vec2u32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i32 ail_vec2i32_sub(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return (AIL_Vec2i32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2u64 ail_vec2u64_sub(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return (AIL_Vec2u64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2i64 ail_vec2i64_sub(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return (AIL_Vec2i64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2f32 ail_vec2f32_sub(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return (AIL_Vec2f32) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec2f64 ail_vec2f64_sub(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return (AIL_Vec2f64) { .x = a.x - b.x, .y = a.y - b.y };
}

AIL_Vec3u8 ail_vec3u8_sub(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return (AIL_Vec3u8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i8 ail_vec3i8_sub(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return (AIL_Vec3i8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u16 ail_vec3u16_sub(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return (AIL_Vec3u16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i16 ail_vec3i16_sub(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return (AIL_Vec3i16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u32 ail_vec3u32_sub(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return (AIL_Vec3u32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i32 ail_vec3i32_sub(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return (AIL_Vec3i32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3u64 ail_vec3u64_sub(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return (AIL_Vec3u64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3i64 ail_vec3i64_sub(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return (AIL_Vec3i64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3f32 ail_vec3f32_sub(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return (AIL_Vec3f32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec3f64 ail_vec3f64_sub(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return (AIL_Vec3f64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

AIL_Vec4u8 ail_vec4u8_sub(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return (AIL_Vec4u8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i8 ail_vec4i8_sub(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return (AIL_Vec4i8) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u16 ail_vec4u16_sub(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return (AIL_Vec4u16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i16 ail_vec4i16_sub(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return (AIL_Vec4i16) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u32 ail_vec4u32_sub(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return (AIL_Vec4u32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i32 ail_vec4i32_sub(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return (AIL_Vec4i32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4u64 ail_vec4u64_sub(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return (AIL_Vec4u64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4i64 ail_vec4i64_sub(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return (AIL_Vec4i64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4f32 ail_vec4f32_sub(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return (AIL_Vec4f32) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

AIL_Vec4f64 ail_vec4f64_sub(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return (AIL_Vec4f64) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}


b32 ail_vec2u8_eq(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i8_eq(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u16_eq(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i16_eq(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u32_eq(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i32_eq(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2u64_eq(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2i64_eq(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2f32_eq(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec2f64_eq(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return  a.x == b.x && a.y == b.y;
}

b32 ail_vec3u8_eq(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i8_eq(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u16_eq(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i16_eq(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u32_eq(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i32_eq(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3u64_eq(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3i64_eq(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3f32_eq(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec3f64_eq(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z;
}

b32 ail_vec4u8_eq(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i8_eq(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u16_eq(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i16_eq(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u32_eq(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i32_eq(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4u64_eq(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4i64_eq(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4f32_eq(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

b32 ail_vec4f64_eq(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return  a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}


u8 ail_vec2u8_dot(AIL_Vec2u8 a, AIL_Vec2u8 b)
{
    return  a.x * b.x + a.y * b.y;
}

i8 ail_vec2i8_dot(AIL_Vec2i8 a, AIL_Vec2i8 b)
{
    return  a.x * b.x + a.y * b.y;
}

u16 ail_vec2u16_dot(AIL_Vec2u16 a, AIL_Vec2u16 b)
{
    return  a.x * b.x + a.y * b.y;
}

i16 ail_vec2i16_dot(AIL_Vec2i16 a, AIL_Vec2i16 b)
{
    return  a.x * b.x + a.y * b.y;
}

u32 ail_vec2u32_dot(AIL_Vec2u32 a, AIL_Vec2u32 b)
{
    return  a.x * b.x + a.y * b.y;
}

i32 ail_vec2i32_dot(AIL_Vec2i32 a, AIL_Vec2i32 b)
{
    return  a.x * b.x + a.y * b.y;
}

u64 ail_vec2u64_dot(AIL_Vec2u64 a, AIL_Vec2u64 b)
{
    return  a.x * b.x + a.y * b.y;
}

i64 ail_vec2i64_dot(AIL_Vec2i64 a, AIL_Vec2i64 b)
{
    return  a.x * b.x + a.y * b.y;
}

f32 ail_vec2f32_dot(AIL_Vec2f32 a, AIL_Vec2f32 b)
{
    return  a.x * b.x + a.y * b.y;
}

f64 ail_vec2f64_dot(AIL_Vec2f64 a, AIL_Vec2f64 b)
{
    return  a.x * b.x + a.y * b.y;
}

u8 ail_vec3u8_dot(AIL_Vec3u8 a, AIL_Vec3u8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i8 ail_vec3i8_dot(AIL_Vec3i8 a, AIL_Vec3i8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u16 ail_vec3u16_dot(AIL_Vec3u16 a, AIL_Vec3u16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i16 ail_vec3i16_dot(AIL_Vec3i16 a, AIL_Vec3i16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u32 ail_vec3u32_dot(AIL_Vec3u32 a, AIL_Vec3u32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i32 ail_vec3i32_dot(AIL_Vec3i32 a, AIL_Vec3i32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u64 ail_vec3u64_dot(AIL_Vec3u64 a, AIL_Vec3u64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

i64 ail_vec3i64_dot(AIL_Vec3i64 a, AIL_Vec3i64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 ail_vec3f32_dot(AIL_Vec3f32 a, AIL_Vec3f32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

f64 ail_vec3f64_dot(AIL_Vec3f64 a, AIL_Vec3f64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

u8 ail_vec4u8_dot(AIL_Vec4u8 a, AIL_Vec4u8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i8 ail_vec4i8_dot(AIL_Vec4i8 a, AIL_Vec4i8 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u16 ail_vec4u16_dot(AIL_Vec4u16 a, AIL_Vec4u16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i16 ail_vec4i16_dot(AIL_Vec4i16 a, AIL_Vec4i16 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u32 ail_vec4u32_dot(AIL_Vec4u32 a, AIL_Vec4u32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i32 ail_vec4i32_dot(AIL_Vec4i32 a, AIL_Vec4i32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

u64 ail_vec4u64_dot(AIL_Vec4u64 a, AIL_Vec4u64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

i64 ail_vec4i64_dot(AIL_Vec4i64 a, AIL_Vec4i64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f32 ail_vec4f32_dot(AIL_Vec4f32 a, AIL_Vec4f32 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f64 ail_vec4f64_dot(AIL_Vec4f64 a, AIL_Vec4f64 b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}


AIL_Mat2u8 ail_mat2u8_add(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i8 ail_mat2i8_add(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u16 ail_mat2u16_add(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i16 ail_mat2i16_add(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u32 ail_mat2u32_add(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i32 ail_mat2i32_add(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2u64 ail_mat2u64_add(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2i64 ail_mat2i64_add(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2f32 ail_mat2f32_add(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat2f64 ail_mat2f64_add(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1]},
    }};
}

AIL_Mat3u8 ail_mat3u8_add(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i8 ail_mat3i8_add(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u16 ail_mat3u16_add(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i16 ail_mat3i16_add(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u32 ail_mat3u32_add(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i32 ail_mat3i32_add(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3u64 ail_mat3u64_add(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3i64 ail_mat3i64_add(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3f32 ail_mat3f32_add(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat3f64 ail_mat3f64_add(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2]},
    }};
}

AIL_Mat4u8 ail_mat4u8_add(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i8 ail_mat4i8_add(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u16 ail_mat4u16_add(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i16 ail_mat4i16_add(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u32 ail_mat4u32_add(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i32 ail_mat4i32_add(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4u64 ail_mat4u64_add(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4i64 ail_mat4i64_add(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4f32 ail_mat4f32_add(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}

AIL_Mat4f64 ail_mat4f64_add(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] + b.els[0][0], a.els[0][1] + b.els[0][1], a.els[0][2] + b.els[0][2], a.els[0][3] + b.els[0][3]},
        { a.els[1][0] + b.els[1][0], a.els[1][1] + b.els[1][1], a.els[1][2] + b.els[1][2], a.els[1][3] + b.els[1][3]},
        { a.els[2][0] + b.els[2][0], a.els[2][1] + b.els[2][1], a.els[2][2] + b.els[2][2], a.els[2][3] + b.els[2][3]},
        { a.els[3][0] + b.els[3][0], a.els[3][1] + b.els[3][1], a.els[3][2] + b.els[3][2], a.els[3][3] + b.els[3][3]},
    }};
}


AIL_Mat2u8 ail_mat2u8_sub(AIL_Mat2u8 a, AIL_Mat2u8 b)
{
    return (AIL_Mat2u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i8 ail_mat2i8_sub(AIL_Mat2i8 a, AIL_Mat2i8 b)
{
    return (AIL_Mat2i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u16 ail_mat2u16_sub(AIL_Mat2u16 a, AIL_Mat2u16 b)
{
    return (AIL_Mat2u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i16 ail_mat2i16_sub(AIL_Mat2i16 a, AIL_Mat2i16 b)
{
    return (AIL_Mat2i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u32 ail_mat2u32_sub(AIL_Mat2u32 a, AIL_Mat2u32 b)
{
    return (AIL_Mat2u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i32 ail_mat2i32_sub(AIL_Mat2i32 a, AIL_Mat2i32 b)
{
    return (AIL_Mat2i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2u64 ail_mat2u64_sub(AIL_Mat2u64 a, AIL_Mat2u64 b)
{
    return (AIL_Mat2u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2i64 ail_mat2i64_sub(AIL_Mat2i64 a, AIL_Mat2i64 b)
{
    return (AIL_Mat2i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2f32 ail_mat2f32_sub(AIL_Mat2f32 a, AIL_Mat2f32 b)
{
    return (AIL_Mat2f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat2f64 ail_mat2f64_sub(AIL_Mat2f64 a, AIL_Mat2f64 b)
{
    return (AIL_Mat2f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1]},
    }};
}

AIL_Mat3u8 ail_mat3u8_sub(AIL_Mat3u8 a, AIL_Mat3u8 b)
{
    return (AIL_Mat3u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i8 ail_mat3i8_sub(AIL_Mat3i8 a, AIL_Mat3i8 b)
{
    return (AIL_Mat3i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u16 ail_mat3u16_sub(AIL_Mat3u16 a, AIL_Mat3u16 b)
{
    return (AIL_Mat3u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i16 ail_mat3i16_sub(AIL_Mat3i16 a, AIL_Mat3i16 b)
{
    return (AIL_Mat3i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u32 ail_mat3u32_sub(AIL_Mat3u32 a, AIL_Mat3u32 b)
{
    return (AIL_Mat3u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i32 ail_mat3i32_sub(AIL_Mat3i32 a, AIL_Mat3i32 b)
{
    return (AIL_Mat3i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3u64 ail_mat3u64_sub(AIL_Mat3u64 a, AIL_Mat3u64 b)
{
    return (AIL_Mat3u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3i64 ail_mat3i64_sub(AIL_Mat3i64 a, AIL_Mat3i64 b)
{
    return (AIL_Mat3i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3f32 ail_mat3f32_sub(AIL_Mat3f32 a, AIL_Mat3f32 b)
{
    return (AIL_Mat3f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat3f64 ail_mat3f64_sub(AIL_Mat3f64 a, AIL_Mat3f64 b)
{
    return (AIL_Mat3f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2]},
    }};
}

AIL_Mat4u8 ail_mat4u8_sub(AIL_Mat4u8 a, AIL_Mat4u8 b)
{
    return (AIL_Mat4u8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i8 ail_mat4i8_sub(AIL_Mat4i8 a, AIL_Mat4i8 b)
{
    return (AIL_Mat4i8) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u16 ail_mat4u16_sub(AIL_Mat4u16 a, AIL_Mat4u16 b)
{
    return (AIL_Mat4u16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i16 ail_mat4i16_sub(AIL_Mat4i16 a, AIL_Mat4i16 b)
{
    return (AIL_Mat4i16) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u32 ail_mat4u32_sub(AIL_Mat4u32 a, AIL_Mat4u32 b)
{
    return (AIL_Mat4u32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i32 ail_mat4i32_sub(AIL_Mat4i32 a, AIL_Mat4i32 b)
{
    return (AIL_Mat4i32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4u64 ail_mat4u64_sub(AIL_Mat4u64 a, AIL_Mat4u64 b)
{
    return (AIL_Mat4u64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4i64 ail_mat4i64_sub(AIL_Mat4i64 a, AIL_Mat4i64 b)
{
    return (AIL_Mat4i64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4f32 ail_mat4f32_sub(AIL_Mat4f32 a, AIL_Mat4f32 b)
{
    return (AIL_Mat4f32) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}

AIL_Mat4f64 ail_mat4f64_sub(AIL_Mat4f64 a, AIL_Mat4f64 b)
{
    return (AIL_Mat4f64) { .els = {
        { a.els[0][0] - b.els[0][0], a.els[0][1] - b.els[0][1], a.els[0][2] - b.els[0][2], a.els[0][3] - b.els[0][3]},
        { a.els[1][0] - b.els[1][0], a.els[1][1] - b.els[1][1], a.els[1][2] - b.els[1][2], a.els[1][3] - b.els[1][3]},
        { a.els[2][0] - b.els[2][0], a.els[2][1] - b.els[2][1], a.els[2][2] - b.els[2][2], a.els[2][3] - b.els[2][3]},
        { a.els[3][0] - b.els[3][0], a.els[3][1] - b.els[3][1], a.els[3][2] - b.els[3][2], a.els[3][3] - b.els[3][3]},
    }};
}




#endif // _AIL_MATH_IMPL_GUARD_
#endif // AIL_NO_MATH_IMPL
