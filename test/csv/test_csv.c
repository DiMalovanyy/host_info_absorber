#include "test_csv.h"
#include "framework.h"
#include <utils/csv/csv.h>

#include "test_csv_utils.h"

void csv_constructor_desctructor() {
	TEST_TITLE("test basic usage of constructor and destructor of csv");

	// Create-delete without data
	csv_t* test_csv = create_csv(5, ',');
	free_csv(test_csv);

}

//Check on working
void csv_basic_usage() {
	TEST_TITLE("test basic usage of csv");
	csv_t* test_csv = create_csv(3, ',');

	append_cell_to_csv(test_csv, "aaa");
	append_cell_to_csv(test_csv, "bbb");
	append_cell_to_csv(test_csv, "eee");


	//debug_print_csv_row(test_csv->csv_row->data, 3, stdout);

	serialize_csv(test_csv, 1);

	free_csv(test_csv);
}

void csv_all() {
	TEST_TITLE("CSV");
	csv_constructor_desctructor();
	csv_basic_usage();

	test_cvs_utils_all();
}

