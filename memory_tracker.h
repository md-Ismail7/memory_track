#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <stddef.h>  
void log_event(const char* event, void* ptr, size_t size, const char* type);

void add_block(void* ptr, size_t size, const char* type);
void free_block(void* ptr);
void* my_realloc(void* old_ptr, size_t new_size, const char* new_type);
void save_memory_report_to_file();
void print_memory_report();

#endif
