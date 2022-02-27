#ifndef csv_h
#define csv_h

#include <stdint.h>

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
	//csv_index_t index;
	char delimiter;
} csv_metadata_t;

typedef struct {
	char ** cells;
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

//Important: this function don't take ownership of row_data
int append_row_to_csv(csv_t* csv, const csv_data_t* row_data);
int serialize(const csv_t* csv, int output_fd);


#endif
