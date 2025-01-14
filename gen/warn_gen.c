#include <stdio.h>

#define _STR(x) #x
#define STR(x) _STR(x)

#define WARNINGS                         \
    X(AIL_WARN_NONE)                     \
    X(AIL_WARN_ALL)                      \
    X(AIL_WARN_EXHAUSTIVE_SWITCH)        \
    X(AIL_WARN_UNSAFE_CONVERSION)        \
    X(AIL_WARN_MISSING_ARGLIST)          \
    X(AIL_WARN_IFDEF_0_MACRO)            \
    X(AIL_WARN_SECTION_NAME_TOO_LONG)    \
    X(AIL_WARN_PRINTF_FORMTAT)           \
    X(AIL_WARN_SIGN_EXTENDED_CONVERSION) \
    X(AIL_WARN_STRING_CAST)              \
    X(AIL_WARN_UNBALANCED_PUSH_POP)      \
    X(AIL_WARN_DEPRECATED)               \
    X(AIL_WARN_UNKNOWN_PRAGMA)           \
    X(AIL_WARN_UNKNOWN_MACRO)            \
    X(AIL_WARN_UNUSED_FUNCTION)          \
    X(AIL_WARN_UNUSED_VARIABLE)          \
    X(AIL_WARN_ZERO_LENGTH_ARRAY)        \
    X(AIL_WARN_CAST_QUAL)                \
    X(AIL_WARN_IMPLICIT_INT)             \
    X(AIL_WARN_UNREACHABLE_CODE)         \
    X(AIL_WARN_COUNT)

typedef enum Warn_Kind {
    #define X(warn) warn,
        WARNINGS
    #undef X
} Warn_Kind;

static char* warn_names_clang[AIL_WARN_COUNT] = {
    [AIL_WARN_UNKNOWN_PRAGMA] = "-Wunknown-pragmas",
    [AIL_WARN_DEPRECATED] = "-Wdeprecated-declarations",
    [AIL_WARN_UNUSED_FUNCTION] = "-Wunused-function",
    [AIL_WARN_UNUSED_VARIABLE] = "-Wunused-variable",
    [AIL_WARN_ZERO_LENGTH_ARRAY] = "-Wzero-length-array",
    [AIL_WARN_CAST_QUAL] = "-Wcast-qual",
};
static char* warn_names_gcc[AIL_WARN_COUNT] = {
    [AIL_WARN_UNKNOWN_PRAGMA] = "-Wunknown-pragmas",
    [AIL_WARN_DEPRECATED] = "-Wdeprecated-declarations",
    [AIL_WARN_UNUSED_FUNCTION] = "-Wunused-function",
    [AIL_WARN_UNUSED_VARIABLE] = "-Wunused-variable",
    [AIL_WARN_ZERO_LENGTH_ARRAY] = "-Wpedantic",
    [AIL_WARN_CAST_QUAL] = "-Wcast-qual",
};
static char* warn_names_msvc[AIL_WARN_COUNT] = {
    [AIL_WARN_EXHAUSTIVE_SWITCH] = "4061 4062 5262",
    [AIL_WARN_UNSAFE_CONVERSION] = "4191 4242 4254 4287 4388 5219",
    [AIL_WARN_MISSING_ARGLIST] = "4546",
    [AIL_WARN_IFDEF_0_MACRO] = "4574",
    [AIL_WARN_UNKNOWN_PRAGMA] = "4619",
    [AIL_WARN_UNKNOWN_MACRO] = "4668",
    [AIL_WARN_SECTION_NAME_TOO_LONG] = "4767",
    [AIL_WARN_PRINTF_FORMTAT] = "4774 4777",
    [AIL_WARN_SIGN_EXTENDED_CONVERSION] = "4826",
    [AIL_WARN_STRING_CAST] = "4905 4906",
    [AIL_WARN_UNBALANCED_PUSH_POP] = "5031 5032",
    [AIL_WARN_DEPRECATED] = "4996",
    [AIL_WARN_UNUSED_FUNCTION] = "4505 5245",
    [AIL_WARN_UNUSED_VARIABLE] = "5264",
    [AIL_WARN_ZERO_LENGTH_ARRAY] = "4200",
    [AIL_WARN_CAST_QUAL] = "5266",
    [AIL_WARN_IMPLICIT_INT] = "4431",
    [AIL_WARN_UNREACHABLE_CODE] = "4702",
};
static char* warn_names_empty[AIL_WARN_COUNT] = {0};

#define LEN(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct {
    char  *enable_prefix,  *enable_suffix;
    char  *disable_prefix, *disable_suffix;
    char  *error_prefix,   *error_suffix;
    char  *all_enable, *all_disable, *all_error;
    char  *name;
    char **names;
    int    n;
} Comp;
#define COMP(name, names, enable_prefix, enable_suffix, disable_prefix, disable_suffix, error_prefix, error_suffix, all_enable, all_disable, all_error) { enable_prefix, enable_suffix, disable_prefix, disable_suffix, error_prefix, error_suffix, all_enable, all_disable, all_error, name, names, LEN(names) }

static Comp comps[] = {
    COMP("AIL_COMP_CLANG", warn_names_clang, "clang diagnostic warning \\\"", "\\\"", "clang diagnostic ignored \\\"", "\\\"", "clang diagnostic error \\\"", "\\\"", NULL, NULL, NULL),
    COMP("AIL_COMP_GCC",   warn_names_gcc,   "GCC diagnostic warning \\\"",    "\\\"", "GCC diagnostic ignored \\\"",   "\\\"", "GCC diagnostic error \\\"",  "\\\"", NULL, NULL, NULL),
    COMP("AIL_COMP_MSVC || AIL_COMP_INTEL", warn_names_msvc, "warning(", ")", "warning(disable:", ")", "warning(error:", ")", "warning(push, 4)", "warning(push, 0)", "warning(error: 4061 4062 5262 4191 4242 4254 4287 4388 5219 4546 4574 4619 4668 4767 4774 4777 4826 4905 4906 5031 5032 4996 4505 5245 5264 4200 5266 4431)"),
};
Comp empty_comp = COMP("", warn_names_empty, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

#define PRINT_WARN_DEFS(warn, comp) do { \
    if (warn != AIL_WARN_COUNT) { \
        if (warn == AIL_WARN_ALL && comp.all_enable)  { \
            printf("#   define _AIL_WARN_ENABLE_"  STR(warn) " _Pragma(\"%s\")\n", comp.all_enable); \
            printf("#   define _AIL_WARN_DISABLE_" STR(warn) " _Pragma(\"%s\")\n", comp.all_disable); \
            printf("#   define _AIL_WARN_ERROR_"   STR(warn) " _Pragma(\"%s\")\n", comp.all_error); \
        } \
        else if (comp.names[warn]) { \
            printf("#   define _AIL_WARN_ENABLE_"  STR(warn) " _Pragma(\"%s%s%s\")\n", comp.enable_prefix,  comp.names[warn], comp.enable_suffix); \
            printf("#   define _AIL_WARN_DISABLE_" STR(warn) " _Pragma(\"%s%s%s\")\n", comp.disable_prefix, comp.names[warn], comp.disable_suffix); \
            printf("#   define _AIL_WARN_ERROR_"   STR(warn) " _Pragma(\"%s%s%s\")\n", comp.error_prefix,   comp.names[warn], comp.error_suffix); \
        } else { \
            printf("#   define _AIL_WARN_ENABLE_"  STR(warn) "\n"); \
            printf("#   define _AIL_WARN_DISABLE_" STR(warn) "\n"); \
            printf("#   define _AIL_WARN_ERROR_"   STR(warn) "\n"); \
        } \
    } \
    } while(0);

int main(void)
{
    printf(
        "/*\n"
        "*** Warning Kinds ***\n"
        "* A list of useful warnings of different compilers\n"
        "* Use AIL_Warn_Kind enum for disabling/enabling/erroring a specific warning in your code\n"
        "*\n"
        "* AIL_Warn_Kind is also used by `ail_build`\n"
        "*/\n"
        "\n"
        "#ifndef _AIL_WARN_H_\n"
        "#define _AIL_WARN_H_\n"
    );
    printf("typedef enum AIL_Warn_Kinds {\n");
    #define X(warn) printf("    " STR(warn) ",\n");
        WARNINGS
    #undef X
    printf("} AIL_Warn_Kinds;\n\n");

    for (unsigned long long int i = 0; i < LEN(comps); i++) {
        printf("#%s %s\n", (i ? "elif" : "if"), comps[i].name);
        #define X(warn) PRINT_WARN_DEFS(warn, comps[i])
            WARNINGS
        #undef X
    }
    printf("#else\n");
    #define X(warn) PRINT_WARN_DEFS(warn, empty_comp)
        WARNINGS
    #undef X
    printf("#endif\n");
    printf("\n#endif // _AIL_WARN_H_\n");
    return 0;
}
