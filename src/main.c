#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/errno.h>

#include "utils/protocol.h"
#include "utils/csv/csv.h"
#include "utils/links.h"

#define FILE_NAME_SIZE 256

void print_help(char *script_name) {
	printf("Usage: %s %s [%s]\n", script_name, "<input hosts>", "<output file>\n");
}

void write_header(csv_t* output_csv) {
	append_cell_to_csv(output_csv, "Address");
	append_cell_to_csv(output_csv, "Ip addresses");
	append_cell_to_csv(output_csv, "DNS Servers");
	
	for (uint8_t protocol_index = 0; protocol_index < protocol_size; ++protocol_index) {
		char protocol_buffer[MAX_CSV_CELL_SIZE];
		snprintf(protocol_buffer, MAX_CSV_CELL_SIZE, "%d:%s", protocols[protocol_index].port_num, protocols[protocol_index].port_description);
		append_cell_to_csv(output_csv, protocol_buffer);
	}
	append_cell_to_csv(output_csv, "Other open Ports");
	append_cell_to_csv(output_csv, "Is Working");
	commit_row_to_csv(output_csv);
}


int main(int argc, char **argv) {
	//TODO: Functional for single parse
	if (argc < 2 || argc > 3) {
		print_help(argv[0]);
		exit(-1);
	}
	
	FILE* input_fd = fopen(argv[1], "r");
	if (input_fd == NULL) {
		printf("Error on open %s. Error code: %d\n", argv[1], errno);
		exit(-1);
	}
	FILE* output_fd = NULL;
	char output_file_name[255];

	if (argc == 3) {
		output_fd = fopen(argv[2], "w+");
		if (output_fd == NULL) {
			printf("Error on open output file %s. Error code: %d\n", argv[2], errno);
			exit(-1);
		}
		strcpy(output_file_name, argv[2]);
	} else {
		output_fd = fopen("output.csv", "w+");
		if (output_fd == NULL) {
			printf("Error on open output.csv file. Error code: %d\n", errno);
			exit(-1);
		}
		strcpy(output_file_name, "output.csv");
	}

	// Create csv
	csv_t* output_csv = create_csv(20, ',');

	//Write header to CSV
	write_header(output_csv);

	//Parse links from file
	links_t links = read_links(input_fd);
	//debug_print_links(links, stdout);




	//Write CSV to output file
	serialize_csv(output_csv, fileno(output_fd));

	//free_links(links);
	fclose(output_fd);
	fclose(input_fd);

	char system_command_buf[512];
	sprintf(system_command_buf, "open %s", output_file_name);
	
	// Unkoment it for auto open created .csv file
	//system(system_command_buf);
	exit(0);
}
