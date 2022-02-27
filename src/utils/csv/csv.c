#include "csv.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/errno.h>

#include "csv_utils.h"

csv_t* create_csv(uint16_t columns_count, char delimiter) {
	csv_metadata_t* metadata = (csv_metadata_t*)malloc(sizeof(csv_metadata_t));
	metadata->size.columns_count = columns_count;
	metadata->delimiter = delimiter;

	csv_t* csv = (csv_t*)malloc(sizeof(csv_t));
	csv->csv_row = NULL;
	csv->metadata = metadata;
	return csv;
}

void free_csv(csv_t* csv) {
	struct csv_row* cur_row = csv->csv_row;
	while(cur_row) {
		struct csv_row* prev_row = cur_row;
		cur_row = cur_row->next;
		free_csv_row(prev_row->data, csv->metadata->size.columns_count);
		free(prev_row);
	}
	free(csv->metadata);
	free(csv);
}

void append_cell_to_csv(csv_t* csv, const char* data) {
	struct csv_row* cur_row = csv->csv_row;
	while(cur_row && cur_row->next) {
		cur_row = cur_row -> next;
	}

	uint16_t cells_amount = cur_row ? cur_row->data.cells_amount : 0;
	if (cur_row && cells_amount < csv->metadata->size.columns_count) {
		cur_row->data.cells[cells_amount] = malloc(sizeof(char) * strlen(data) + 1);
		strncpy(cur_row->data.cells[cells_amount], data, MAX_CSV_CELL_SIZE);
		cur_row->data.cells_amount++;
	} else {	
		struct csv_row* new_row = (struct csv_row*)malloc(sizeof(struct csv_row));
		new_row->data = alloc_csv_row(csv->metadata->size.columns_count);
		strncpy(new_row->data.cells[0], data, MAX_CSV_CELL_SIZE);

		if (cur_row) {
			new_row->prev = cur_row;
			cur_row->next = new_row;
			new_row->next = NULL;
		} else {
			new_row->prev = NULL;
			new_row->next = NULL;
			cur_row = new_row;

			debug_print_csv_row(csv->csv_row->data, 3, stdout);
		}
	}

}

void append_row_to_csv(csv_t* csv, const csv_data_t row_data) {
	struct csv_row* cur_row = csv->csv_row;
	while(cur_row && cur_row->next) {
		cur_row = cur_row->next;
	}
	struct csv_row* new_row = (struct csv_row*)malloc(sizeof(struct csv_row));
	new_row->data = row_data;
	new_row->prev = cur_row ? cur_row : NULL;
	new_row->next = NULL;
	cur_row->next = new_row;
}

int serialize_csv(const csv_t* csv, int output_fd) {
	//Check if file descriptor open
	if (fcntl(output_fd, F_GETFD) == -1) {
		printf("Csv file descriptor should be open. Error %d\n", errno);
		return -1;
	}

	struct csv_row* cur_row = csv->csv_row; 
	if (!cur_row) {
		printf("csv structure is empty\n");
		return -1;
	}
	const csv_metadata_t* metadata = csv->metadata;
	if(!metadata) {
		printf("csv has no metadata\n");
		return -1;
	}

	for(;cur_row != NULL; cur_row = cur_row -> next) {
		char row_buffer[metadata->size.columns_count * MAX_CSV_CELL_SIZE + 1];
		concat_csv_row(cur_row->data, metadata, row_buffer);
		dprintf(output_fd, "%s\n", row_buffer);
	}
	return 0;
}
