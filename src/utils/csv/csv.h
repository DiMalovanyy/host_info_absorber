#ifndef csv_h
#define csv_h

#include <stdint.h>
#include <stdio.h>

//For sources

static const uint8_t MAX_CSV_CELL_SIZE = 255;

//todo: finish csv implementation

typedef struct {
	uint16_t columns_count;
} csv_size_t;

typedef struct {
	uint16_t current_row_index;
	uint16_t current_column_index;
} csv_index_t;

typedef struct {
	csv_size_t size;
	char delimiter;
} csv_metadata_t;

typedef struct {
	char ** cells;
	uint16_t cells_amount;
} csv_data_t;

//bi-directional csv row linked list
struct csv_row {
	csv_data_t data;
	struct csv_row* next;
	struct csv_row* prev;
};

typedef struct {
	csv_metadata_t* metadata;
	struct csv_row* csv_row;
} csv_t;

csv_t* create_csv(uint16_t columns_count, char delimiter);
void free_csv(csv_t* csv);

void append_cell_to_csv(csv_t* csv, const char* data);
void append_row_to_csv(csv_t* csv, const csv_data_t row_data);
int serialize_csv(const csv_t* csv, int output_fd);

uint16_t get_rows_amount(csv_t* csv);
void debug_print_csv(csv_t* csv, FILE* output_stream);


#endif
