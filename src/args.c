#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "args.h"

void usage(const char * arg0, int exit_code)
{
	fprintf(stderr, "Usage: %s [-a | -r] [-n nodes] [subcmd ...]\n\n", arg0);
	fprintf(stderr, "  Separate sub-commands from optional arguments with -- to avoid mistaking optional arguments.\n");
	fprintf(stderr, "  For example: \"%s -n 1-5 -- ls -a\" ignores the last -a option.\n", arg0);
	exit(exit_code);
}

void String_print(FILE * fp, String * str)
{
	if (!fp || !str || !str->ptr)	return;
	for (size_t i = 0; i < str->len; i++) {
		fprintf(fp, "%c", str->ptr[i]);
	}
}

static int check_number(const char * s, size_t len)
{
	for (int i = 0; i < len; i++) {
		if (s[i] < '0' && s[i] > '9') {
			return 0;
		}
	}
	return 1;
}

static int parse_nodes(char * optarg, Args * args)
{
	char * token = strtok(optarg, ",");
	while (token) {
		char * hyphen = strchr(token, '-');
		if (hyphen &&
			check_number(token, hyphen - token) && 
			check_number(hyphen, strlen(hyphen))) {
			int start = atoi(token);
			int end = atoi(hyphen + 1);
			for (int node = start; node <= end; node++) {
				args->numerical_nodes[args->numerical_nodes_len++] = node;
			}
		} else {
			args->literal_nodes[args->literal_nodes_len++] = token;
			// String * ln = &args->literal_nodes[args->literal_nodes_len++];
			// ln->ptr = token;
			// ln->len = strlen(token);
		}
		token = strtok(NULL, ",");
	}
	return 0;
}

int parse_args(int argc, char * argv[], Args * args)
{
	if (argc == 1) {
		usage(argv[0], 0);
	}

	args->argc = argc;
	args->argv = argv;
	args->quiet = 0;
	args->numerical_nodes_len = 0;
	args->literal_nodes_len = 0;

	int opt;
    short acquire = 0, release = 0;

    while ((opt = getopt(argc, argv, "ahn:qr")) != -1) {
        switch (opt) {
			case 'h':
				usage(argv[0], 0);
				break;
            case 'a':
                acquire = 1;
                break;
            case 'r':
                release = 1;
                break;
			case 'q':
				args->quiet = 1;
				break;
			case 'n':
				if (parse_nodes(optarg, args) < 0) {
					fprintf(stderr, "Illegal nodes set: %s\n", optarg);
					return -1;
				}
				break;
            default:
				usage(argv[0], EXIT_FAILURE);
				return -1;
        }
    }

    if (acquire && !release) {
		args->mode = 1;
	} else if (!acquire && release) {
		args->mode = 2;
	} else {
		args->mode = 0;
	}

	args->subcmd_idx = optind;

	return 0;
}

void show_args(Args * args)
{
	if (args->quiet)	return;

	fprintf(stdout, "========= Config =========\n");
	switch (args->mode) {
		case 0: fprintf(stdout, "Mode:    acquire+release\n"); break;
		case 1: fprintf(stdout, "Mode:    acquire\n"); break;
		case 2: fprintf(stdout, "Mode:    release\n"); break;
		default:	break;
	}
	fprintf(stdout, "Target:  ");
	for (size_t i = 0; i < args->numerical_nodes_len; i++) {
		fprintf(stdout, "node%d ", args->numerical_nodes[i]);
	}
	for (size_t i = 0; i < args->literal_nodes_len; i++) {
		fprintf(stdout, "%s ", args->literal_nodes[i]);
		//String_print(stdout, args->literal_nodes[i]);
		//fprintf(stdout, ' ');
	}
	fprintf(stdout, "\nCommand: ");
	for (int i = args->subcmd_idx; i < args->argc; i++) {
		fprintf(stdout, "%s ", args->argv[i]);
	}
	fprintf(stdout, "\n========= Config END =========\n");
}
