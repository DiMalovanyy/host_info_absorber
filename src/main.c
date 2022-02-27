#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/errno.h>

#include "utils/protocol.h"
#include "utils/csv/csv.h"

#define FILE_NAME_SIZE 256

void print_help(char *script_name) {
	printf("Usage: %s %s [%s]\n", script_name, "<input hosts>", "<output file>\n");
}

int write_header(int output_fd) {
		
	

	return 0;
}

int main(int argc, char **argv) {
	//TODO: Functional for single parse
	if (argc < 2 || argc > 3) {
		print_help(argv[0]);
		exit(-1);
	}
	
	int input_fd = open(argv[1], O_RDONLY);
	if (input_fd == -1) {
		printf("Error on open %s. Error code: %d\n", argv[1], errno);
		exit(-1);
	}
	int output_fd = -1;

	if (argc == 3) {
		output_fd = open(argv[2], O_WRONLY | O_TRUNC);
		if (output_fd == -1) {
			printf("Error on open output file %s. Error code: %d\n", argv[2], errno);
			exit(-1);
		}
	} else {
		output_fd = open("output.csv", O_WRONLY | O_CREAT | O_TRUNC);
		if (output_fd == -1) {
			printf("Error on open output.csv file. Error code: %d\n", errno);
			exit(-1);
		}
	}
	
	if (write_header(output_fd) == -1) {
		printf("Could not write header to csv file\n");
		exit(-1);
	}

	close(output_fd);
	close(input_fd);
	exit(0);
}
