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

#define AIL_ALL_IMPL
#define AIL_SV_IMPL
#define AIL_FS_IMPL
#include "../ail.h"
#include "../ail_sv.h"
#include "../ail_fs.h"

#define SV_LIT AIL_SV_FROM_LITERAL
#define SV_LIT_T AIL_SV_FROM_LITERAL_T
#define STR_LIT AIL_STR_FROM_LITERAL
#define STR_LIT_T AIL_STR_FROM_LITERAL_T

AIL_Str num_type_names[] = {
    STR_LIT(AIL_STRINGIFY(u8)),
    STR_LIT(AIL_STRINGIFY(i8)),
    STR_LIT(AIL_STRINGIFY(u16)),
    STR_LIT(AIL_STRINGIFY(i16)),
    STR_LIT(AIL_STRINGIFY(u32)),
    STR_LIT(AIL_STRINGIFY(i32)),
    STR_LIT(AIL_STRINGIFY(u64)),
    STR_LIT(AIL_STRINGIFY(i64)),
    STR_LIT(AIL_STRINGIFY(f32)),
    STR_LIT(AIL_STRINGIFY(f64)),
};

AIL_Str get_container_name(AIL_SV container_type, u32 n, AIL_SV inner_type)
{
    return ail_sv_concat(SV_LIT_T("AIL_Math"), ail_sv_concat(ail_sv_concat(container_type, ail_sv_new_unsigned(n)), inner_type));
}

AIL_Str typedefs_vec(AIL_SB *sb)
{
    AIL_SV xs[] = { SV_LIT("x"), SV_LIT("y"), SV_LIT("z"), SV_LIT("w") };
    AIL_SV cs[] = { SV_LIT("r"), SV_LIT("g"), SV_LIT("b"), SV_LIT("a") };
    for (u32 i = 2; i < 5; i++) {
        AIL_Str xs_sv = ail_sv_join(xs, i, SV_LIT_T(", "));
        AIL_Str cs_sv = ail_sv_join(cs, i, SV_LIT_T(", "));
        for (u32 j = 0; j < AIL_ARRLEN(num_type_names); j++) {
            AIL_SV inner = ail_sv_from_str(num_type_names[j]);
            AIL_Str name = get_container_name(SV_LIT_T("Vec"), i, inner);
            char buf[1024];
            snprintf(buf, AIL_ARRLEN(buf), "typedef union %s {\n    %s els[%d];\n    struct { %s %s; };\n    struct { %s %s; };\n} %s;\n\n", name.str, inner.str, i, inner.str, xs_sv.str, inner.str, cs_sv.str, name.str);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
        }
    }
    return ail_sb_to_str(*sb);
}

AIL_Str typedefs_mat(AIL_SB *sb)
{
    AIL_SV xs[] = { SV_LIT("x"), SV_LIT("y"), SV_LIT("z"), SV_LIT("w") };
    AIL_SV cs[] = { SV_LIT("r"), SV_LIT("g"), SV_LIT("b"), SV_LIT("a") };
    for (u32 i = 2; i < 5; i++) {
        AIL_Str xs_sv = ail_sv_join(xs, i, SV_LIT_T(", "));
        AIL_Str cs_sv = ail_sv_join(cs, i, SV_LIT_T(", "));
        for (u32 j = 0; j < AIL_ARRLEN(num_type_names); j++) {
            AIL_SV inner = ail_sv_from_str(num_type_names[j]);
            AIL_Str name = get_container_name(SV_LIT_T("Vec"), i, inner);
            char buf[1024];
            snprintf(buf, AIL_ARRLEN(buf), "typedef union %s {\n    %s els[%d];\n    struct { %s %s; };\n    struct { %s %s; };\n} %s;\n\n", name.str, inner.str, i, inner.str, xs_sv.str, inner.str, cs_sv.str, name.str);
            ail_sb_push_sv(sb, ail_sv_from_cstr(buf));
        }
    }
    return ail_sb_to_str(*sb);
}

int main(void)
{
    AIL_SB sb = ail_sb_new();
    AIL_Str tmp = SV_LIT("#define AIL_TYPES_IMPL\n#include \"../ail.h\"\n\n");
    ail_sb_push_sv(&sb, tmp);
    typedefs_vec(&sb);
    ail_fs_write_file("./la.h", sb.data, sb.len);
    return 0;
}
