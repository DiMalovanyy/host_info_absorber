#ifndef TEST_CSV_UTILS_H
#define TEST_CSV_UTILS_H

#include <utils/csv/csv_utils.h>

#include "framework.h"

static const uint16_t test_column_count = 5;

csv_data_t generated_test_data(int seed) {
	csv_data_t csv_row = alloc_csv_row(test_column_count);

	uint16_t cell_index;
	for(cell_index = 0; cell_index < test_column_count; cell_index++) {
		csv_row.cells[cell_index] = malloc(sizeof(char)*(3+cell_index));
		
		char cell_buf[sizeof(char)*(3+cell_index)];
		sprintf(cell_buf, "%d%s%d", cell_index, "some", seed);
		strcpy(csv_row.cells[cell_index], cell_buf);
	}
	csv_row.cells_amount = test_column_count;
	return csv_row;
}

void test_csv_utils_alloc_free() {
	TEST_TITLE("test csv row allocation and free");
	csv_data_t csv_row = alloc_csv_row(test_column_count);
	free_csv_row(csv_row);

	TEST_SUCCESS();
}

void test_csv_utils_compare_rows() {
	TEST_TITLE("compare two rows");

	csv_data_t test1 = generated_test_data(1);
	csv_data_t test2 = generated_test_data(1);
	csv_data_t test3 = generated_test_data(2);

	bool res = compare_csv_rows(test1, test2, test_column_count);
	if (!res) {
		TEST_FAILED();
		goto fail;
	} 
	
	res = compare_csv_rows(test1, test3, test_column_count);
	if (res) {
		TEST_FAILED();
		goto fail;
	}

	TEST_SUCCESS();

	fail:
	free_csv_row(test1);
	free_csv_row(test2);
	free_csv_row(test3);
}

void test_csv_utils_copy_rows() {
	TEST_TITLE("copy two rows");

	csv_data_t test1 = generated_test_data(1);
	csv_data_t test2 = copy_csv_row(test1, test_column_count);
	if (!test2.cells || !compare_csv_rows(test1, test2, test_column_count)) {
		TEST_FAILED();
		goto fail;
	}

	TEST_SUCCESS();
	fail:
	if (test2.cells) {
		free_csv_row(test2);
	}	
	free_csv_row(test1);
}

void test_csv_utils_concatanate_rows() {
	TEST_TITLE("contcatanate row to CSV format with delimiter");

	csv_metadata_t metadata;
	metadata.size.columns_count = test_column_count;
	metadata.delimiter = ',';

	csv_data_t test = generated_test_data(1);
	const char seed_1_data_with_delim[] = "0some1,1some1,2some1,3some1,4some1";

	char concated_csv_row_buf[1024];
	memset(concated_csv_row_buf, 0, sizeof(concated_csv_row_buf));

	if ( concat_csv_row(test, &metadata, concated_csv_row_buf) == -1) {
		TEST_FAILED();
		goto fail; 
	}
	
	if ( strcmp(seed_1_data_with_delim, concated_csv_row_buf) != 0) {
		TEST_FAILED();
		goto fail;
	}
	
	TEST_SUCCESS();
	fail:
	free_csv_row(test);

}


void test_cvs_utils_all() {
	TEST_TITLE("CSV_UTILS");
	test_csv_utils_alloc_free();
	test_csv_utils_compare_rows();
	test_csv_utils_copy_rows();
	test_csv_utils_concatanate_rows();
}

#endif
