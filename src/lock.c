#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "args.h"
#include "lock.h"

static int prepare()
{
    // Check if directory exists
    struct stat st;
    if (stat(TARGET_DIR, &st) || !S_ISDIR(st.st_mode)) {
        // Create directory
        if (mkdir(TARGET_DIR, 0755)) {
			perror("mkdir");
			exit(EXIT_FAILURE);
        }
    }

	if (chdir(TARGET_DIR)) {
		perror("chdir");
		exit(EXIT_FAILURE);
	}

    int fd = open(STATUS_FILE, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Lock the file exclusively to prevent concurrent access
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        exit(EXIT_FAILURE);
    }
	return fd;
}

static void handle_status(int fd, Args * args)
{
    // Read the current status from the file
    char status;
    if (read(fd, &status, sizeof(status)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Print the current status
    printf("Current status: %c\n", status);

    // Prompt the user to enter a new status
    printf("Enter a new status (A, B, or C): ");
    char new_status;
    scanf(" %c", &new_status);

    // Write the new status to the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    if (write(fd, &new_status, sizeof(new_status)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    printf("Status updated to %c\n", new_status);
}

static void cleanup(int fd)
{
    // Release the lock and close the file
    if (flock(fd, LOCK_UN) == -1) {
        perror("flock");
        exit(EXIT_FAILURE);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}


void maybe_handle_locks(Args * args)
{
	int fd = prepare();

	handle_status(fd, args);

	cleanup(fd);
}
