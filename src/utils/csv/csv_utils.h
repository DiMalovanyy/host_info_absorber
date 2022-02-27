#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include "csv.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline csv_data_t alloc_csv_row(uint16_t column_amount) {
	csv_data_t data;
	uint16_t cell_index;
	data.cells = malloc(sizeof(char*) * column_amount);
	for(cell_index = 0; cell_index < column_amount; cell_index++) {
		data.cells[cell_index] = (char*)malloc(sizeof(char) * MAX_CSV_CELL_SIZE);
	}
	data.cells_amount = 0;
	return data;
}

static inline csv_data_t copy_csv_row(const csv_data_t src, uint16_t column_amount) {
	csv_data_t data = alloc_csv_row(column_amount);
	uint16_t cell_index;
	for (cell_index = 0; cell_index < column_amount; cell_index++) {
		if (strncpy(data.cells[cell_index], src.cells[cell_index], MAX_CSV_CELL_SIZE) == NULL) {
			data.cells = NULL;
			return data;
		}
	}
	return data;
}

static inline void free_csv_row(csv_data_t src, uint16_t column_amount) {
	char* csv_cell;
	uint16_t cell_index;
	for (cell_index = 0; cell_index < column_amount; cell_index++) {
		free(src.cells[cell_index]);
	}
	free(src.cells);
}

static inline void debug_print_csv_row(const csv_data_t csv_row, FILE* output) {
	uint16_t cell_index;
	fprintf(output, "Row: {");
	for(cell_index = 0; cell_index < csv_row.cells_amount; cell_index++) {
		fprintf(output, " \"%s\"", csv_row.cells[cell_index]);
	}
	fprintf(output, " }\n");
}

static inline int concat_csv_row(const csv_data_t src, const csv_metadata_t* metadata, char* dst) {	
	char delim_buf[2];
	delim_buf[0] = metadata->delimiter;
	delim_buf[1] = '\0';
	uint16_t cell_index;
	for(cell_index = 0; cell_index < src.cells_amount; cell_index++) {
		if (cell_index != 0) {
			if (!strcat(dst, delim_buf)) {
				return -1;
			}
		}
		if (!strcat(dst, src.cells[cell_index])) {
			return -1;
		}
	}
	return 0;
}

static inline bool compare_csv_rows(const csv_data_t fst, const csv_data_t snd, uint16_t column_amount) {
	uint16_t cell_index;
	for(cell_index = 0; cell_index < column_amount; cell_index++) {
		if (strncmp(fst.cells[cell_index], snd.cells[cell_index], MAX_CSV_CELL_SIZE) != 0) {
			return false;
		}
	}
	return true;
}

#endif
