/*
*** Warning Kinds ***
* A list of useful warnings of different compilers
* Use AIL_Warn_Kind enum for disabling/enabling/erroring a specific warning in your code
*
* AIL_Warn_Kind is also used by `ail_build`
*/

#ifndef _AIL_WARN_H_
#define _AIL_WARN_H_

typedef enum AIL_Warn_Kind {
    AIL_WARN_NONE,
    AIL_WARN_ALL,
    AIL_WARN_EXHAUSTIVE_SWITCH,
    AIL_WARN_UNSAFE_CONVERSION,
    AIL_WARN_MISSING_ARGLIST,
    AIL_WARN_IFDEF_0_MACRO,
    AIL_WARN_SECTION_NAME_TOO_LONG,
    AIL_WARN_PRINTF_FORMTAT,
    AIL_WARN_SIGN_EXTENDED_CONVERSION,
    AIL_WARN_STRING_CAST,
    AIL_WARN_UNBALANCED_PUSH_POP,
    AIL_WARN_DEPRECATED,
    AIL_WARN_UNKNOWN_PRAGMA,
    AIL_WARN_UNKNOWN_MACRO,
    AIL_WARN_UNUSED_FUNCTION,
    AIL_WARN_UNUSED_VARIABLE,
    AIL_WARN_ZERO_LENGTH_ARRAY,
    AIL_WARN_CAST_QUAL,
    AIL_WARN_IMPLICIT_INT,
    AIL_WARN_UNREACHABLE_CODE,
    AIL_WARN_COUNT,
} AIL_Warn_Kind;

#if AIL_COMP_CLANG
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL
#   define _AIL_WARN_ERROR_AIL_WARN_ALL
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("clang diagnostic error \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic warning \"-Wunknown-pragmas\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("clang diagnostic error \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic warning \"-Wunused-function\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("clang diagnostic error \"-Wunused-function\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic warning \"-Wunused-variable\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic ignored \"-Wunused-variable\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("clang diagnostic error \"-Wunused-variable\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic warning \"-Wzero-length-array\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic ignored \"-Wzero-length-array\"")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("clang diagnostic error \"-Wzero-length-array\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic warning \"-Wcast-qual\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("clang diagnostic error \"-Wcast-qual\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
#elif AIL_COMP_GCC
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL
#   define _AIL_WARN_ERROR_AIL_WARN_ALL
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("GCC diagnostic error \"-Wdeprecated-declarations\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic warning \"-Wunknown-pragmas\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("GCC diagnostic error \"-Wunknown-pragmas\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic warning \"-Wunused-function\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("GCC diagnostic error \"-Wunused-function\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic warning \"-Wunused-variable\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("GCC diagnostic error \"-Wunused-variable\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic warning \"-Wpedantic\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("GCC diagnostic error \"-Wpedantic\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic warning \"-Wcast-qual\"")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic ignored \"-Wcast-qual\"")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("GCC diagnostic error \"-Wcast-qual\"")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
#elif AIL_COMP_MSVC || AIL_COMP_INTEL
#   define _AIL_WARN_ENABLE_AIL_WARN_NONE
#   define _AIL_WARN_DISABLE_AIL_WARN_NONE
#   define _AIL_WARN_ERROR_AIL_WARN_NONE
#   define _AIL_WARN_ENABLE_AIL_WARN_ALL _Pragma("warning(push, 4)")
#   define _AIL_WARN_DISABLE_AIL_WARN_ALL _Pragma("warning(push, 0)")
#   define _AIL_WARN_ERROR_AIL_WARN_ALL _Pragma("warning(error: 4061 4062 5262 4191 4242 4254 4287 4388 5219 4546 4574 4619 4668 4767 4774 4777 4826 4905 4906 5031 5032 4996 4505 5245 5264 4200 5266 4431)")
#   define _AIL_WARN_ENABLE_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(4061 4062 5262)")
#   define _AIL_WARN_DISABLE_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(disable:4061 4062 5262)")
#   define _AIL_WARN_ERROR_AIL_WARN_EXHAUSTIVE_SWITCH _Pragma("warning(error:4061 4062 5262)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(disable:4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNSAFE_CONVERSION _Pragma("warning(error:4191 4242 4254 4287 4388 5219)")
#   define _AIL_WARN_ENABLE_AIL_WARN_MISSING_ARGLIST _Pragma("warning(4546)")
#   define _AIL_WARN_DISABLE_AIL_WARN_MISSING_ARGLIST _Pragma("warning(disable:4546)")
#   define _AIL_WARN_ERROR_AIL_WARN_MISSING_ARGLIST _Pragma("warning(error:4546)")
#   define _AIL_WARN_ENABLE_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(4574)")
#   define _AIL_WARN_DISABLE_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(disable:4574)")
#   define _AIL_WARN_ERROR_AIL_WARN_IFDEF_0_MACRO _Pragma("warning(error:4574)")
#   define _AIL_WARN_ENABLE_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(4767)")
#   define _AIL_WARN_DISABLE_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(disable:4767)")
#   define _AIL_WARN_ERROR_AIL_WARN_SECTION_NAME_TOO_LONG _Pragma("warning(error:4767)")
#   define _AIL_WARN_ENABLE_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(4774 4777)")
#   define _AIL_WARN_DISABLE_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(disable:4774 4777)")
#   define _AIL_WARN_ERROR_AIL_WARN_PRINTF_FORMTAT _Pragma("warning(error:4774 4777)")
#   define _AIL_WARN_ENABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(4826)")
#   define _AIL_WARN_DISABLE_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(disable:4826)")
#   define _AIL_WARN_ERROR_AIL_WARN_SIGN_EXTENDED_CONVERSION _Pragma("warning(error:4826)")
#   define _AIL_WARN_ENABLE_AIL_WARN_STRING_CAST _Pragma("warning(4905 4906)")
#   define _AIL_WARN_DISABLE_AIL_WARN_STRING_CAST _Pragma("warning(disable:4905 4906)")
#   define _AIL_WARN_ERROR_AIL_WARN_STRING_CAST _Pragma("warning(error:4905 4906)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(5031 5032)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(disable:5031 5032)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNBALANCED_PUSH_POP _Pragma("warning(error:5031 5032)")
#   define _AIL_WARN_ENABLE_AIL_WARN_DEPRECATED _Pragma("warning(4996)")
#   define _AIL_WARN_DISABLE_AIL_WARN_DEPRECATED _Pragma("warning(disable:4996)")
#   define _AIL_WARN_ERROR_AIL_WARN_DEPRECATED _Pragma("warning(error:4996)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(4619)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(disable:4619)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_PRAGMA _Pragma("warning(error:4619)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(4668)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(disable:4668)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNKNOWN_MACRO _Pragma("warning(error:4668)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(4505 5245)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(disable:4505 5245)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_FUNCTION _Pragma("warning(error:4505 5245)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(5264)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(disable:5264)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNUSED_VARIABLE _Pragma("warning(error:5264)")
#   define _AIL_WARN_ENABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(4200)")
#   define _AIL_WARN_DISABLE_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(disable:4200)")
#   define _AIL_WARN_ERROR_AIL_WARN_ZERO_LENGTH_ARRAY _Pragma("warning(error:4200)")
#   define _AIL_WARN_ENABLE_AIL_WARN_CAST_QUAL _Pragma("warning(5266)")
#   define _AIL_WARN_DISABLE_AIL_WARN_CAST_QUAL _Pragma("warning(disable:5266)")
#   define _AIL_WARN_ERROR_AIL_WARN_CAST_QUAL _Pragma("warning(error:5266)")
#   define _AIL_WARN_ENABLE_AIL_WARN_IMPLICIT_INT _Pragma("warning(4431)")
#   define _AIL_WARN_DISABLE_AIL_WARN_IMPLICIT_INT _Pragma("warning(disable:4431)")
#   define _AIL_WARN_ERROR_AIL_WARN_IMPLICIT_INT _Pragma("warning(error:4431)")
#   define _AIL_WARN_ENABLE_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(4702)")
#   define _AIL_WARN_DISABLE_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(disable:4702)")
#   define _AIL_WARN_ERROR_AIL_WARN_UNREACHABLE_CODE _Pragma("warning(error:4702)")
#   define _AIL_WARN_ENABLE_AIL_WARN_COUNT
#   define _AIL_WARN_DISABLE_AIL_WARN_COUNT
#   define _AIL_WARN_ERROR_AIL_WARN_COUNT
#endif

#endif // _AIL_WARN_H_
