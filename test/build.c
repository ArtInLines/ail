#define AIL_BUILD_IMPL
#define AIL_FS_IMPL
#define AIL_SV_IMPL
#include "../ail_build.h"
#include "../ail_fs.h"
#include "../ail_sv.h"
#include <stdlib.h> // For exit
#include <string.h> // For strcmp
#include <stdio.h>  // For printf

typedef enum {
	ARGS_FLAG_NO_DEBUG = 1 << 0,
	ARGS_FLAG_RUN      = 1 << 1,
	ARGS_FLAG_RUN_ALL  = 1 << 2,
} ArgsFlags;

typedef struct {
	AIL_Build_Comp cc;
	ArgsFlags flags;
	AIL_DA(AIL_SV) tests;
} Args;

Args parse_args(u32 n, char **args, char *prog)
{
	Args res = {
		.tests = ail_da_new(AIL_SV),
	};
	for (u32 i = 0; i < n; i++) {
		char *arg = args[i];
		if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
			printf("Usage: %s [-h|--help] [-d|--no-debug] [-t|--test <name>]* [-r|--run [all]] [-c|--cc|--comp <compiler>]\n", prog);
			printf("  -h|--help:      Print this help menu\n");
			printf("  -d|--no-debug:  Build in release mode instead of debug mode\n");
			printf("  -t|--test:      Specify a test file to build. If none is specified, all tests will be built. This option can be specified repeatedly to build several tests.\n");
			printf("  -r|--run:       Run the tests immediately after building. If 'all' is set afterward, every supported compiler will be used to compile each test seperately and immediately run them. This is useful for testing that the test files work for all supported compilers.\n");
			printf("  -c|--cc|--comp: Set the compiler to build the tests with. If no compile is chosen, the default compiler for your platform will be chosen. The compiler should be specified as the name of its executable.\n");
			exit(0);
		} else if (!strcmp(arg, "-d") || !strcmp(arg, "--no-debug")) {
			static b32 is_set = false;
			if (is_set) printf("Warning: You specified the 'no-debug' option repeatedly\n");
			is_set = true;
			res.flags |= ARGS_FLAG_NO_DEBUG;
		} else if (!strcmp(arg, "-t") || !strcmp(arg, "--test")) {
			i++;
			if (i == n) {
				printf("Error: Expected a compiler name after the '%s' flag", arg);
				exit(1);
			}
			ail_da_push(&res.tests, ail_sv_from_cstr(args[i]));
		} else if (!strcmp(arg, "-r") || !strcmp(arg, "--run")) {
			static b32 is_set = false;
			if (is_set) printf("Warning: You specified the 'run' option repeatedly\n");
			is_set = true;
			res.flags |= ARGS_FLAG_RUN;
			if (i + 1 < n && !strcmp(args[i+1], "all")) {
				i++;
				res.flags |= ARGS_FLAG_RUN_ALL;
			}
		} else if (!strcmp(arg, "-c") || !strcmp(arg, "--cc") || !strcmp(arg, "--comp")) {
			static b32 is_set = false;
			if (is_set) printf("Warning: You specified the compiler repeatedly.\nOnly the last specified compiler will be used. If you want to test all compilers, use '-r all'.\nSee '--help' for more info.\n");
			is_set = true;
			i++;
			if (i == n) {
				printf("Error: Expected a compiler name after the '%s' flag", arg);
				exit(1);
			}
			res.cc = ail_build_comp_from_str(args[i]);
		} else {
			printf("Unexpected argument: '%s'\n", arg);
			printf("See --help for supported command line arguments.\n");
			exit(1);
		}
	}
	return res;
}

void build_tests(AIL_Build_Comp cc, AIL_Build_Flags cflags, AIL_DA(AIL_SV) tests, b32 run)
{
	if (!tests.len) {
		AIL_FS_Read_Dir_Res dir = ail_fs_read_dir_init(".");
		AIL_FS_Dirent dirent;
		do {
			dirent = ail_fs_read_dir_next(dir);
			if (dirent.type == AIL_FS_ENTRY_FILE) {
				AIL_SV name = ail_sv_from_cstr(dirent.name);
				if (ail_sv_ends_with(name, AIL_SV_FROM_LITERAL_T(".c"))) ail_da_push(&tests, name);
			}
		} while (!ail_fs_dirent_is_nil(dirent));
		ail_fs_read_dir_deinit(dir);
	} else {
		// @TODO: Cleanup test names
	}
	if (!tests.len) printf("No tests to run...");

	for (u32 i = 0; i < tests.len; i++) {
		if (!ail_fs_is_file(tests.data[i].str)) {
			printf("Cannot build '%s': No such file exists\n", tests.data[i].str);
			continue;
		}
		AIL_ASSERT(ail_sv_ends_with(tests.data[i], AIL_SV_FROM_LITERAL_T(".c")));
		char *out_name = ail_build_get_exe_name(tests.data[i].str, tests.data[i].len, ail_default_allocator);
		ail_build(cc, cflags, out_name, tests.data[i].str, ail_default_allocator);
		if (run) {
			char *arg[1] = { out_name };
			AIL_DA(str) args = ail_da_from_parts(arg, 1, 1, ail_default_allocator);
			AIL_Build_Proc_Res proc = ail_build_proc_exec(&args, ail_default_allocator);
			if (proc.succ) puts(proc.out);
		}
	}
}

int main(int argc, char **argv)
{
	AIL_ASSERT(argc >= 1);
	// ail_build_rebuild_urself(argc, argv);
	Args args = parse_args(argc-1, &argv[1], argv[0]);

	AIL_Build_Flags cflags = 0;
	if (!(args.flags & ARGS_FLAG_NO_DEBUG)) cflags |= AIL_BUILD_FLAG_DEBUG;
	cflags |= AIL_BUILD_FLAG_STD_C99;

	if (args.flags & ARGS_FLAG_RUN_ALL) {
		for (u32 i = 0; i < AIL_BUILD_COMP_COUNT; i++) {
			AIL_Build_Comp cc = { .type = i };
			if (AIL_BUILD_COMP_IS_SPECIAL(cc.type)) continue;
			build_tests(cc, cflags, args.tests, true);
		}
	} else {
		build_tests(args.cc, cflags, args.tests, (args.flags & ARGS_FLAG_RUN));
	}
	return 0;
}