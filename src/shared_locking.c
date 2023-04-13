#include "args.h"
#include "lock.h"
#include "subcommand.h"

int main(int argc, char * argv[])
{
	Args args;
	parse_args(argc, argv, &args);
	show_args(&args);

	maybe_handle_locks(&args);
	run_subcommands(&args);
	maybe_handle_locks(&args);

    return 0;
}
