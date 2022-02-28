#ifndef UTILS_LINKS_H
#define UTILS_LINKS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	char **links;
	size_t amount;
} links_t;

static inline links_t read_links(FILE* input) {
	links_t result;
	//Get lines count for alloc required memory
	size_t lines_amount = 0;
	char ch;
	while(!feof(input)) {
		ch = fgetc(input);
		if(ch == '\n') {
			lines_amount++;
		}
	}
	result.amount = lines_amount;
	// Move the file pointer to the start.
    fseek(input, 0, SEEK_SET);

	ssize_t read_bytes;
	char* line = NULL;
	size_t len = 0;
	size_t line_index = 0;
	result.links = malloc(sizeof(char*) * lines_amount);
	while((read_bytes = getline(&line, &len, input)) != -1) {
		//Remove trailing newline
		line[strcspn(line, "\n")] = 0;
		result.links[line_index] = malloc(sizeof(char) * len + 1);
		strncpy(result.links[line_index], line, len);
		line_index++;
	}
	if(line) {
		free(line);
	}

	return result;
}

static inline void free_links(links_t links) {
	int i;
	for (i = 0; i < links.amount; i++) {
		free(links.links[i]);
	}
	free(links.links);
}

static inline void debug_print_links(links_t links, FILE* output) {
	fprintf(output, "Links amount: %zu\n", links.amount);
	int i;
	for(i = 0; i < links.amount; i++) {
		fprintf(output, "Link: %s\n", links.links[i]);
	}
}

#endif
