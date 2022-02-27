#include "csv.h"

#include <stdio.h>
#include <stdlib.h>


csv_t* new_csv(uint16_t row_count, uint16_t column_count) {
	return malloc(sizeof(csv_metadata_t) + row_count * (column_count * MAX_CSV_CELL_SIZE));
}

void free_csv(csv_t* csv) {
	free(csv);
}

int serialize_csv(const csv_t* csv, int output_fd) {
	uint16_t row_index, column_index;
	for(row_index = 0; row_index < csv->metadata.size.rows_size; row_index++) {
		char row_buffer[csv->metadata.size.column_size * MAX_CSV_CELL_SIZE];
		for(column_index = 0; column_index < csv->metadata.column_size; column_index++) {
			//TODO: Сейчас чет впадло
		}
		dprintf(output_fd, "%s\n", row_buffer);
	}
	return 0;
}

csv_size_t get_csv_size(const csv_t* csv) {
	return csv->metadata.size;
}
