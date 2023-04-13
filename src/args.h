#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdio.h>
#include <stddef.h>

#define TARGET_DIR "/etc/shared-lock"
#define STATUS_FILE "status.txt"
#define NUMERICAL_NODES_LEN_MAX 64
#define LITERAL_NODES_LEN_MAX 64

typedef struct _tString {
	char * ptr;
	size_t len;
} String;
void String_print(FILE * fp, String * str);

typedef struct _tArgs {
	int argc;
	char ** argv;
	int mode;	// acquire: 1, release: 2, acquire+release: 0
	int subcmd_idx;
	int quiet;
	int numerical_nodes[NUMERICAL_NODES_LEN_MAX];
	size_t numerical_nodes_len;
	char * literal_nodes[LITERAL_NODES_LEN_MAX];
	//String literal_nodes[LITERAL_NODES_LEN_MAX];
	size_t literal_nodes_len;
} Args;

void usage(const char * arg0, int exit_code);
int parse_args(int argc, char * argv[], Args * args);
void show_args(Args * args);

#endif /* args.h */
