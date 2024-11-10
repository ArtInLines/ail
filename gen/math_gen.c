// Generate ail_math.h
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

#include "../ail.h"
#include "../ail_sv.h"
#include "../ail_fs.h"

#define SV_LIT    AIL_SV_FROM_LITERAL
#define SV_LIT_T  AIL_SV_FROM_LITERAL_T
#define STR_LIT   AIL_STR_FROM_LITERAL
#define STR_LIT_T AIL_STR_FROM_LITERAL_T

global AIL_Str LICENSE_COMMENT = STR_LIT(
    "// Common Math functions\n"
    "//\n"
    "// Heavily inspired by the following libraries (in no particular order):\n"
    "// - la              (https://github.com/tsoding/la)\n"
    "// - mathc           (https://github.com/felselva/mathc)\n"
    "// - Raylib          (https://github.com/raysan5/raylib/blob/master/src/raymath.h)\n"
    "// - fastmod         (https://github.com/lemire/fastmod)\n"
    "// - HandmadeMath    (https://github.com/HandmadeMath/HandmadeMath)\n"
    "// - tiny-fixedpoint (https://github.com/kokke/tiny-fixedpoint-c)\n"
    "// -\n"
    "//\n"
    "// LICENSE\n"
    "/*\n"
    "Copyright (c) 2024 Lily Val Richter\n"
    "\n"
    "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
    "of this software and associated documentation files (the \"Software\"), to deal\n"
    "in the Software without restriction, including without limitation the rights\n"
    "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
    "copies of the Software, and to permit persons to whom the Software is\n"
    "furnished to do so, subject to the following conditions:\n"
    "\n"
    "The above copyright notice and this permission notice shall be included in all\n"
    "copies or substantial portions of the Software.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
    "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
    "SOFTWARE.\n"
    "*/\n"
);

global AIL_Str HEADER_BEGIN = STR_LIT(
    "#ifndef AIL_MATH_H_\n"
    "\n"
    "#include \"ail.h\"\n"
    "\n"
    "#ifndef AIL_MATH_DEF\n"
    "#ifdef  AIL_DEF\n"
    "#define AIL_MATH_DEF AIL_DEF\n"
    "#else\n"
    "#define AIL_MATH_DEF\n"
    "#endif // AIL_DEF\n"
    "#endif // AIL_MATH_DEF\n"
    "#ifndef AIL_MATH_DEF_INLINE\n"
    "#ifdef  AIL_DEF_INLINE\n"
    "#define AIL_MATH_DEF_INLINE AIL_DEF_INLINE\n"
    "#else\n"
    "#define AIL_MATH_DEF_INLINE static inline\n"
    "#endif // AIL_DEF_INLINE\n"
    "#endif // AIL_MATH_DEF_INLINE\n"
);
global AIL_Str HEADER_END = STR_LIT("#endif // AIL_MATH_H_\n");
global AIL_Str IMPL_BEGIN = STR_LIT(
    "#if !defined(AIL_NO_MATH_IMPL) && !defined(AIL_NO_IMPL)\n"
    "#ifndef _AIL_MATH_IMPL_GUARD_\n"
    "#define _AIL_MATH_IMPL_GUARD_\n"
);
global AIL_Str IMPL_END = STR_LIT(
    "#endif // _AIL_MATH_IMPL_GUARD_\n"
    "#endif // AIL_NO_MATH_IMPL\n"
);


AIL_SLICE_INIT(AIL_SV);

global AIL_SV vec_attrs_xy[]    = { SV_LIT("x"), SV_LIT("y"), SV_LIT("z"), SV_LIT("w") };
global AIL_SV vec_attrs_color[] = { SV_LIT("r"), SV_LIT("g"), SV_LIT("b"), SV_LIT("a") };
global AIL_SV vec_attrs_uv[]    = { SV_LIT("u"), SV_LIT("v"), SV_LIT("w") };
global AIL_SV vec_attrs_rect[]  = { SV_LIT("width"), SV_LIT("height") };
global AIL_SLICE(AIL_SV) vec_attrs[] = {
    ail_slice_from_arr(vec_attrs_xy),
    ail_slice_from_arr(vec_attrs_color),
    ail_slice_from_arr(vec_attrs_uv),
    ail_slice_from_arr(vec_attrs_rect)
};

global char COMMENT_STRING[] = "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////";
global char NEWLINES_STRING[] = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
global char SPACE_STRING[] = "                                                                                                    ";

#define VEC_MIN_SIZE 2
#define VEC_MAX_SIZE 4
#define MAT_MIN_SIZE 2
#define MAT_MAX_SIZE 4
#define GENERIC_BINOP_LA_NAME "AIL_MATH_GENERIC_BINOP_LA"
#define GENERIC_INTEGRAL_NAME "AIL_MATH_GENERIC_INTEGRAL"
#define GENERIC_LA_NAME       "AIL_MATH_GENERIC_LINALG"

typedef enum NumType {
    NUM_TYPE_U8 = 0,
    NUM_TYPE_I8,
    NUM_TYPE_U16,
    NUM_TYPE_I16,
    NUM_TYPE_U32,
    NUM_TYPE_I32,
    NUM_TYPE_U64,
    NUM_TYPE_I64,
    NUM_TYPE_F32,
    NUM_TYPE_F64,
    NUM_TYPE_COUNT,
} NumType;
global AIL_Str num_type_names[] = {
    [NUM_TYPE_U8]  = STR_LIT(AIL_STRINGIFY(u8)),
    [NUM_TYPE_I8]  = STR_LIT(AIL_STRINGIFY(i8)),
    [NUM_TYPE_U16] = STR_LIT(AIL_STRINGIFY(u16)),
    [NUM_TYPE_I16] = STR_LIT(AIL_STRINGIFY(i16)),
    [NUM_TYPE_U32] = STR_LIT(AIL_STRINGIFY(u32)),
    [NUM_TYPE_I32] = STR_LIT(AIL_STRINGIFY(i32)),
    [NUM_TYPE_U64] = STR_LIT(AIL_STRINGIFY(u64)),
    [NUM_TYPE_I64] = STR_LIT(AIL_STRINGIFY(i64)),
    [NUM_TYPE_F32] = STR_LIT(AIL_STRINGIFY(f32)),
    [NUM_TYPE_F64] = STR_LIT(AIL_STRINGIFY(f64)),
};

internal void math_generic_macro(AIL_SB *sb) {
    ail_sb_push_sv(sb, ail_sv_from_cstr("// @Important: These size definitions for C's standard types might map to bigger int types than actually true for this platform, but if you want proper type inference, you should just use properly sized types (i.e. u8, i16, f32, etc.) anyways\n"
        "#define _" GENERIC_INTEGRAL_NAME "_NAME_1(pre, post, var) _Generic((var), \\\n"
        "    unsigned char:          _AIL_CONCAT_3(pre, u8,  post), \\\n"
        "    signed char:            _AIL_CONCAT_3(pre, i8,  post), \\\n"
        "    unsigned short:         _AIL_CONCAT_3(pre, u16, post), \\\n"
        "    signed short:           _AIL_CONCAT_3(pre, i16, post), \\\n"
        "    unsigned int:           _AIL_CONCAT_3(pre, u32, post), \\\n"
        "    signed int:             _AIL_CONCAT_3(pre, i32, post), \\\n"
        "    unsigned long int:      _AIL_CONCAT_3(pre, u64, post), \\\n"
        "    signed long int:        _AIL_CONCAT_3(pre, i64, post), \\\n"
        "    unsigned long long int: _AIL_CONCAT_3(pre, u64, post), \\\n"
        "    signed long long int:   _AIL_CONCAT_3(pre, i64, post), \\\n"
        "    float:                  _AIL_CONCAT_3(pre, f32, post), \\\n"
        "    double:                 _AIL_CONCAT_3(pre, f64, post)  \\\n"
        ")\n"
        "\n"
        "#define " GENERIC_INTEGRAL_NAME "_NAME(pre, post, arg0) _Generic((arg0), \\\n"
    ));
    u32 max_len = 0;
    for (u32 i = 0; i < NUM_TYPE_COUNT; i++) max_len = AIL_MAX(max_len, num_type_names[i].len);
    for (u32 i = 0; i < NUM_TYPE_COUNT; i++) {
        char *type    = num_type_names[i].str;
        u32 space_len = max_len - num_type_names[i].len;
        ail_sb_print(sb, "    %s:%.*s _AIL_CONCAT_3(pre, %s,%.*s post), \\\n", type, space_len, SPACE_STRING, type, space_len, SPACE_STRING);
    }
    ail_sb_push_sv(sb, ail_sv_from_cstr(
        "    default: _" GENERIC_INTEGRAL_NAME "_NAME_1(pre, post, arg0) \\\n"
        ")\n"
        "#define " GENERIC_INTEGRAL_NAME "(pre, post, arg0, ...) " GENERIC_INTEGRAL_NAME "_NAME(pre, post, arg0)((arg0), __VA_ARGS__)\n"
        "\n"
    ));

    ail_sb_push_sv(sb, ail_sv_from_cstr("#define " GENERIC_LA_NAME "(pre, post, arg0, ...) " GENERIC_INTEGRAL_NAME "_NAME(pre, post, (arg0).els[0])((arg0), __VA_ARGS__)\n\n"));

    ail_sb_push_sv(sb, ail_sv_from_cstr("#define _" GENERIC_BINOP_LA_NAME "_4(pre, post, arg0, arg1) " GENERIC_LA_NAME "(pre, post, arg0, arg1)\n"));
    for (u32 i = 5; i <= 16; i++) {
        ail_sb_print(sb,
            "#define _" GENERIC_BINOP_LA_NAME "_%d(pre, post, arg0, arg1, ...) _" GENERIC_BINOP_LA_NAME "_%d(pre, post, " GENERIC_LA_NAME "(pre, post, arg0, arg1), __VA_ARGS__)\n",
            i, i-1
        );
    }
    ail_sb_print(sb,
        "#define " GENERIC_BINOP_LA_NAME "(pre, post, arg0, ...) AIL_VFUNC(_" GENERIC_BINOP_LA_NAME "_, pre, post, arg0, __VA_ARGS__)\n"
    );
}

AIL_Str get_newlines(u32 n)
{
    char buf[64];
    snprintf(buf, AIL_ARRLEN(buf), "%.*s", n, NEWLINES_STRING);
    return ail_str_new_cstr(buf);
}

AIL_Str get_title_comment(AIL_SV text, u32 newlines_count)
{
    u32 len = text.len + 16;
    char buf[512];
    snprintf(buf, AIL_ARRLEN(buf), "%s%.*s\n//%.*s//\n//      %.*s      //\n//%.*s//\n%.*s%s", get_newlines(newlines_count + 1).str, len, COMMENT_STRING, len - 4, SPACE_STRING, (u32)text.len, text.str, len - 4, SPACE_STRING, len, COMMENT_STRING, get_newlines(newlines_count).str);
    return ail_str_new_cstr(buf);
}

AIL_Str get_subtitle_comment(AIL_SV text, u32 newlines_count)
{
    u32 len = text.len + 8;
    char buf[256];
    snprintf(buf, AIL_ARRLEN(buf), "%s%.*s\n//  %.*s  //\n%.*s\n%s", get_newlines(newlines_count + 1).str, len, COMMENT_STRING, (u32)text.len, text.str, len, COMMENT_STRING, get_newlines(newlines_count).str);
    return ail_str_new_cstr(buf);
}

#define get_container_name(container_type, n, inner_type) get_container_name_full(container_type.str, container_type.len, n, inner_type.str, inner_type.len)
AIL_Str get_container_name_full(char *container_type_str, u64 container_type_len, u32 n, char *inner_type_str, u64 inner_type_len)
{
    return ail_sv_concat(SV_LIT_T("AIL_"), ail_sv_from_parts(container_type_str, container_type_len), ail_sv_new_unsigned(n), ail_sv_from_parts(inner_type_str, inner_type_len));
}


void vec_typedefs(AIL_SB *sb)
{
    char buf[1024];
    for (u32 i = VEC_MIN_SIZE; i <= VEC_MAX_SIZE; i++) {
        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str name = get_container_name(SV_LIT_T("Vec"), i, inner);
            snprintf(buf, AIL_ARRLEN(buf), "typedef union %s {\n    %s els[%d];\n", name.str, inner.str, i);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));

            for (u32 k = 0; k < AIL_ARRLEN(vec_attrs); k++) {
                if (vec_attrs[k].len >= i) {
                    snprintf(buf, AIL_ARRLEN(buf), "    struct { %s %s; };\n", inner.str, ail_sv_join(vec_attrs[k].data, i, SV_LIT_T(", ")).str);
                    ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
                }
            }
            u32 counter = 0;
            for (u32 k = 0; k < AIL_ARRLEN(vec_attrs); k++) {
                if (vec_attrs[k].len >= i) {
                    for (u32 l = vec_attrs[k].len - 1; l >= 2; l--) {
                        if (l >= i) continue;
                        for (u32 m = 0; m + l <= i; m++) {
                            ail_sb_push_sv(sb, ail_sv_from_cstr("    struct { "));
                            if (m > 0) {
                                if (m > 1) snprintf(buf, AIL_ARRLEN(buf), "%s _%d[%d]; ", inner.str, counter++, m);
                                else       snprintf(buf, AIL_ARRLEN(buf), "%s _%d; ",     inner.str, counter++);
                                ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
                            }
                            snprintf(buf, AIL_ARRLEN(buf), "%s %s; ", get_container_name(SV_LIT_T("Vec"), l, inner).str, ail_sv_join(vec_attrs[k].data + m, l, SV_LIT_T("")).str);
                            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
                            i32 count = i - l - m;
                            if (count > 0) {
                                if (count > 1) snprintf(buf, AIL_ARRLEN(buf), "%s _%d[%d]; ", inner.str, counter++, count);
                                else           snprintf(buf, AIL_ARRLEN(buf), "%s _%d; ",     inner.str, counter++);
                                ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
                            }
                            ail_sb_push_sv(sb, ail_sv_from_cstr("};\n"));
                        }
                    }
                }
            }
            snprintf(buf, AIL_ARRLEN(buf), "} %s;\n\n", name.str);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
        }
    }
}

void mat_typedefs(AIL_SB *sb)
{
    char buf[1024];
    for (u32 i = MAT_MIN_SIZE; i <= MAT_MAX_SIZE; i++) {
        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str name = get_container_name(SV_LIT_T("Mat"), i, inner);
            snprintf(buf, AIL_ARRLEN(buf), "typedef union %s {\n    %s els[%d][%d];\n    %s cols[%d];\n    struct { %s ", name.str, inner.str, i, i, get_container_name(SV_LIT_T("Vec"), i, inner).str, i, inner.str);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
            for (u32 col = 0; col < i; col++) {
                for (u32 row = 0; row < i; row++) {
                    if (col || row) ail_sb_push_sv(sb, ail_sv_from_cstr(", "));
                    ail_sb_push_sv(sb, ail_sv_concat(SV_LIT_T("x"), ail_str_new_unsigned(row+1), ail_str_new_unsigned(col+1)));
                }
            }
            snprintf(buf, AIL_ARRLEN(buf), "; };\n} %s;\n\n", name.str);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
        }
    }
}

void vec_constructor(AIL_SB *generic_sb, AIL_SB *decl_sb, AIL_SB *impl_sb)
{
    char buf[1024];
    for (u32 i = VEC_MIN_SIZE; i <= VEC_MAX_SIZE; i++) {
        AIL_SLICE(AIL_SV) attrs = ail_slice_from_parts(vec_attrs_xy, i);
        AIL_Str untyped_input   = ail_sv_join_da(attrs, ail_sv_from_cstr(", "));
        snprintf(buf, AIL_ARRLEN(buf), "#define ail_vec%d(%s) " GENERIC_INTEGRAL_NAME "(ail_vec%d, , %s)\n", i, untyped_input.str, i, untyped_input.str);
        ail_sb_push_sv(generic_sb, ail_sv_from_cstr(buf));

        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str type = get_container_name(SV_LIT_T("Vec"), i, inner);
            AIL_SB op_sb = ail_sb_new();
            for (u32 k = 0; k < i; k++) {
                snprintf(buf, AIL_ARRLEN(buf), "%s.%s = %s", (k ? ", " : ""), vec_attrs_xy[k].str, attrs.data[k].str);
                ail_sb_push_sv(&op_sb, ail_sv_from_cstr(buf));
            }
            snprintf(buf, AIL_ARRLEN(buf), "%s ail_vec%d%s(", type.str, i, inner.str);
            for (u32 k = 0; k < i; k++) {
                snprintf(buf, AIL_ARRLEN(buf), "%s%s%s %s", buf, (k ? ", " : ""), inner.str, attrs.data[k].str);
            }
            snprintf(buf, AIL_ARRLEN(buf), "%s)", buf);
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(buf));
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(";\n"));

            snprintf(buf, AIL_ARRLEN(buf), "%s\n{\n    return (%s) { %s };\n}\n\n", buf, type.str, ail_sb_to_str(op_sb).str);
            ail_sb_push_sv(impl_sb, ail_sv_from_cstr(buf));
        }
    }
    ail_sb_push_sv(generic_sb, get_newlines(1));
    ail_sb_push_sv(decl_sb,    get_newlines(1));
    ail_sb_push_sv(impl_sb,    get_newlines(1));
}

void vec_element_wise_binop(AIL_SB *generic_sb, AIL_SB *decl_sb, AIL_SB *impl_sb, AIL_Str op_name, AIL_Str op)
{
    char buf[1024];
    for (u32 i = VEC_MIN_SIZE; i <= VEC_MAX_SIZE; i++) {
        snprintf(buf, AIL_ARRLEN(buf), "#define ail_vec%d_%s(a, ...) " GENERIC_BINOP_LA_NAME "(ail_vec%d, _%s, a, __VA_ARGS__)\n", i, op_name.str, i, op_name.str);
        ail_sb_push_sv(generic_sb, ail_sv_from_cstr(buf));

        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str type = get_container_name(SV_LIT_T("Vec"), i, inner);
            AIL_SB op_sb = ail_sb_new();
            for (u32 k = 0; k < i; k++) {
                snprintf(buf, AIL_ARRLEN(buf), "%s.%s = a.%s %s b.%s", (k ? ", " : ""), vec_attrs_xy[k].str, vec_attrs_xy[k].str, op.str, vec_attrs_xy[k].str);
                ail_sb_push_sv(&op_sb, ail_sv_from_cstr(buf));
            }
            snprintf(buf, AIL_ARRLEN(buf), "%s ail_vec%d%s_%s(%s a, %s b)", type.str, i, inner.str, op_name.str, type.str, type.str);
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(buf));
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(";\n"));

            snprintf(buf, AIL_ARRLEN(buf), "%s\n{\n    return (%s) { %s };\n}\n\n", buf, type.str, ail_sb_to_str(op_sb).str);
            ail_sb_push_sv(impl_sb, ail_sv_from_cstr(buf));
        }
    }
    ail_sb_push_sv(generic_sb, get_newlines(1));
    ail_sb_push_sv(decl_sb,    get_newlines(1));
    ail_sb_push_sv(impl_sb,    get_newlines(1));
}

void mat_element_wise_binop(AIL_SB *generic_sb, AIL_SB *decl_sb, AIL_SB *impl_sb, AIL_Str op_name, AIL_Str op)
{
    char buf[1024];
    for (u32 i = MAT_MIN_SIZE; i <= MAT_MAX_SIZE; i++) {
        snprintf(buf, AIL_ARRLEN(buf), "#define ail_mat%d_%s(a, ...) " GENERIC_BINOP_LA_NAME "(ail_mat%d, _%s, a, __VA_ARGS__)\n", i, op_name.str, i, op_name.str);
        ail_sb_push_sv(generic_sb, ail_sv_from_cstr(buf));

        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str type = get_container_name(SV_LIT_T("Mat"), i, inner);
            AIL_SB op_sb = ail_sb_new();
            for (u32 col = 0; col < i; col++) {
                ail_sb_push_sv(&op_sb, ail_sv_from_cstr("        {"));
                for (u32 row = 0; row < i; row++) {
                    snprintf(buf, AIL_ARRLEN(buf), "%sa.els[%d][%d] %s b.els[%d][%d]", (row ? ", " : " "), col, row, op.str, col, row);
                    ail_sb_push_sv(&op_sb, ail_sv_from_cstr(buf));
                }
                ail_sb_push_sv(&op_sb, ail_sv_from_cstr("},\n"));
            }
            snprintf(buf, AIL_ARRLEN(buf), "%s ail_mat%d%s_%s(%s a, %s b)", type.str, i, inner.str, op_name.str, type.str, type.str);
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(buf));
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(";\n"));

            snprintf(buf, AIL_ARRLEN(buf), "%s\n{\n    return (%s) { .els = {\n%s    }};\n}\n\n", buf, type.str, ail_sb_to_str(op_sb).str);
            ail_sb_push_sv(impl_sb, ail_sv_from_cstr(buf));
        }
    }
    ail_sb_push_sv(generic_sb, get_newlines(1));
    ail_sb_push_sv(decl_sb,    get_newlines(1));
    ail_sb_push_sv(impl_sb,    get_newlines(1));
}

// @Note: If return_type.len is 0, the vector's inner type will be returned
void vec_element_wise_connected_binop(AIL_SB *generic_sb, AIL_SB *decl_sb, AIL_SB *impl_sb, AIL_Str op_name, AIL_Str op, AIL_Str connector, AIL_Str return_type)
{
    char buf[1024];
    for (u32 i = VEC_MIN_SIZE; i <= VEC_MAX_SIZE; i++) {
        snprintf(buf, AIL_ARRLEN(buf), "#define ail_vec%d_%s(a, ...) " GENERIC_BINOP_LA_NAME "(ail_vec%d, _%s, a, __VA_ARGS__)\n", i, op_name.str, i, op_name.str);
        ail_sb_push_sv(generic_sb, ail_sv_from_cstr(buf));

        for (u32 j = 0; j < NUM_TYPE_COUNT; j++) {
            AIL_Str inner = num_type_names[j];
            AIL_Str type = get_container_name(SV_LIT_T("Vec"), i, inner);
            AIL_SB op_sb = ail_sb_new();
            for (u32 k = 0; k < i; k++) {
                snprintf(buf, AIL_ARRLEN(buf), " %s a.%s %s b.%s", (k ? connector.str : ""), vec_attrs_xy[k].str, op.str, vec_attrs_xy[k].str);
                ail_sb_push_sv(&op_sb, ail_sv_from_cstr(buf));
            }
            snprintf(buf, AIL_ARRLEN(buf), "%s ail_vec%d%s_%s(%s a, %s b)", return_type.len ? return_type.str : inner.str, i, inner.str, op_name.str, type.str, type.str);
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(buf));
            ail_sb_push_sv(decl_sb, ail_sv_from_cstr(";\n"));

            snprintf(buf, AIL_ARRLEN(buf), "%s\n{\n    return%s;\n}\n\n", buf, ail_sb_to_str(op_sb).str);
            ail_sb_push_sv(impl_sb, ail_sv_from_cstr(buf));
        }
    }
    ail_sb_push_sv(generic_sb, get_newlines(1));
    ail_sb_push_sv(decl_sb,    get_newlines(1));
    ail_sb_push_sv(impl_sb,    get_newlines(1));
}

int main(void)
{
    AIL_SB type_sb    = ail_sb_new();
    AIL_SB decl_sb    = ail_sb_new();
    AIL_SB impl_sb    = ail_sb_new();
    AIL_SB generic_sb = ail_sb_new();

    AIL_Str subtitle_linalg     = get_subtitle_comment(SV_LIT_T("Linear Algebra"), 1);
    AIL_Str subtitle_fixedpoint = get_subtitle_comment(SV_LIT_T("Fixed Point"), 1);
    AIL_Str subtitle_trig       = get_subtitle_comment(SV_LIT_T("Trigonometry"), 1);

    ail_sb_push_sv(&generic_sb, subtitle_linalg);
    ail_sb_push_sv(&type_sb, subtitle_linalg);
    ail_sb_push_sv(&decl_sb, subtitle_linalg);
    ail_sb_push_sv(&impl_sb, subtitle_linalg);
    vec_typedefs(&type_sb);
    mat_typedefs(&type_sb);

    vec_constructor(&generic_sb, &decl_sb, &impl_sb);
    vec_element_wise_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("add"), STR_LIT_T("+"));
    vec_element_wise_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("sub"), STR_LIT_T("-"));
    vec_element_wise_connected_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("eq"), STR_LIT_T("=="), STR_LIT_T("&&"), STR_LIT_T("b32"));
    vec_element_wise_connected_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("dot"), STR_LIT_T("*"), STR_LIT_T("+"), (AIL_Str){0});

    mat_element_wise_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("add"), STR_LIT_T("+"));
    mat_element_wise_binop(&generic_sb, &decl_sb, &impl_sb, STR_LIT_T("sub"), STR_LIT_T("-"));

    AIL_SB file_sb = ail_sb_new();
    ail_sb_push_sv(&file_sb, LICENSE_COMMENT);
    ail_sb_push_sv(&file_sb, get_newlines(1));
    ail_sb_push_sv(&file_sb, HEADER_BEGIN);
    ail_sb_push_sv(&file_sb, get_title_comment(SV_LIT_T("Function Overloding"), 1));
    ail_sb_push_sv(&file_sb, get_newlines(1));
    math_generic_macro(&file_sb);
    ail_da_pushn(&file_sb, generic_sb.data, generic_sb.len);
    ail_sb_push_sv(&file_sb, get_title_comment(SV_LIT_T("Type Definitions"), 1));
    ail_da_pushn(&file_sb, type_sb.data, type_sb.len);
    ail_sb_push_sv(&file_sb, get_title_comment(SV_LIT_T("Function Declarations"), 1));
    ail_da_pushn(&file_sb, decl_sb.data, decl_sb.len);
    ail_sb_push_sv(&file_sb, HEADER_END);
    ail_sb_push_sv(&file_sb, get_newlines(2));
    ail_sb_push_sv(&file_sb, get_title_comment(SV_LIT_T("Implementation"), 1));
    ail_sb_push_sv(&file_sb, IMPL_BEGIN);
    ail_da_pushn(&file_sb, impl_sb.data, impl_sb.len);
    ail_sb_push_sv(&file_sb, get_newlines(2));
    ail_sb_push_sv(&file_sb, IMPL_END);
    ail_fs_write_file("../ail_math.h", file_sb.data, file_sb.len);
    return 0;
}
