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
	csv_t* test_csv = create_csv(4, ',');

	append_cell_to_csv(test_csv, "aaa");
	append_cell_to_csv(test_csv, "bbb");
	commit_row_to_csv(test_csv);

	
	append_cell_to_csv(test_csv, "bbb");
	commit_row_to_csv(test_csv);

	append_cell_to_csv(test_csv, "aaa");
	append_cell_to_csv(test_csv, "aaa");
	append_cell_to_csv(test_csv, "aaa");
	append_cell_to_csv(test_csv, "bbb");
	append_cell_to_csv(test_csv, "bbb");
	append_cell_to_csv(test_csv, "bbb");

	//serialize_csv(test_csv, 1);
	//debug_print_csv(test_csv, stdout);

	TEST_SUCCESS();
	free_csv(test_csv);
}

void csv_all() {
	TEST_TITLE("CSV");
	csv_constructor_desctructor();
	csv_basic_usage();

	test_cvs_utils_all();
}

