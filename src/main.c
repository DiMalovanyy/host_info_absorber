#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/errno.h>

#define FILE_NAME_SIZE 256

void print_help(char *script_name) {
	printf("Usage: %s %s [%s]\n", script_name, "<input hosts>", "<output file>");
}

int main(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
		print_help(argv[0]);
		exit(-1);
	}
	
	int input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1) {
		printf("Error on open %s. Error code: %d", argv[1], errno);
		exit(-1);
	}
	int output_fd = -1;

	if (argc == 3) {
		output_fd = open(argv[2], O_WRONLY);
		if (output_fd == -1) {
			printf("Error on open output file %s. Error code: %d", argv[2], errno);
			exit(-1);
		}
	}

	






	if (output_fd != -1) {
		close(output_fd);
	}
	close(input_fd);
	exit(0);
}
