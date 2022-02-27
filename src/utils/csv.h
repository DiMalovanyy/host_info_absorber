#ifndef CSV_H
#define CSV_H

#include <stdint.h>

static const uint8_t MAX_CSV_CELL_SIZE = 255;

//TODO: Implement as Linked list where node is row

typedef struct {
	uint16_t rows_size;
	uint16_t column_size;
} csv_size_t;

typedef struct {
	uint16_t current_row_index;
	uint16_t current_column_index;
} csv_index_t;

typedef struct {
	csv_size_t size;
	csv_index_t index;
	char delimiter;
} csv_metadata_t;

typedef struct {
	char (*cells)[MAX_CSV_CELL_SIZE];
} csv_row_t;

typedef struct {
	csv_row_t* rows;
	csv_metadata_t metadata;
} csv_t;

csv_t* new_csv(uint16_t row_count, uint16_t column_count);
void free_csv(csv_t* csv);

int serialize_csv(const csv_t* csv, int output_fd);

csv_size_t get_csv_size(const csv_t* csv);

int insert_cell(csv_t* csv, csv_index_t index);


#endif
