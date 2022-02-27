#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <string.h>
#include <stdio.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

//[TEST] - test.c::some_function : 
#define TEST_TITLE(title_str) \
	(printf("\t[TEST] - %s::%s : %s\n", __FILENAME__, __FUNCTION__, title_str))
	
#define TEST_FAILED() \
	(printf(RED "\t\t[ FAILED ]\n" RESET))

#define TEST_SUCCESS() \
	(printf(GREEN "\t\t[ SUCCUESS ]\n" RESET))


#endif
