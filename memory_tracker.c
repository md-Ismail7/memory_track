#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_tracker.h"

// ================= Logger Function =================
void log_event(const char* event, void* ptr, size_t size, const char* type) {
    FILE* file = fopen("memory_log.txt", "a");
    if (!file) return;
    fprintf(file, "[%s] %p | %zu bytes | Type: %s\n", event, ptr, size, type);
    fclose(file);
}

// ================= Memory Tracker Structure =================
typedef struct MemBlock {
    void* address;
    size_t size;
    char type[50];
    int isFreed;
    struct MemBlock* next;
} MemBlock;

static MemBlock* head = NULL;
static size_t totalAllocated = 0;
static size_t totalFreed = 0;

// ================= Add New Allocation =================
void add_block(void* ptr, size_t size, const char* type) {
    MemBlock* block = malloc(sizeof(MemBlock));
    block->address = ptr;
    block->size = size;
    strncpy(block->type, type, 49);
    block->type[49] = '\0';
    block->isFreed = 0;
    block->next = head;
    head = block;

    totalAllocated += size;
    log_event("ALLOC", ptr, size, type);
}

// ================= Free a Block =================
void free_block(void* ptr) {
    MemBlock* curr = head;
    while (curr) {
        if (curr->address == ptr && !curr->isFreed) {
            curr->isFreed = 1;
            totalFreed += curr->size;
            free(ptr);
            log_event("FREE", ptr, curr->size, curr->type);
            return;
        }
        curr = curr->next;
    }
    printf("Pointer not found or already freed.\n");
}

// ================= Reallocate Memory =================
void* my_realloc(void* old_ptr, size_t new_size, const char* new_type) {
    if (!old_ptr) return NULL;

    MemBlock* curr = head;
    while (curr) {
        if (curr->address == old_ptr && !curr->isFreed) {
            void* new_ptr = realloc(old_ptr, new_size);
            if (!new_ptr) return NULL;

            totalAllocated = totalAllocated - curr->size + new_size;

            log_event("REALLOC_OLD", old_ptr, curr->size, curr->type);

            curr->address = new_ptr;
            curr->size = new_size;
            strncpy(curr->type, new_type, 49);
            curr->type[49] = '\0';

            log_event("REALLOC_NEW", new_ptr, new_size, new_type);
            return new_ptr;
        }
        curr = curr->next;
    }

    printf("Pointer not found or already freed.\n");
    return NULL;
}

// ================= Save Memory Report to File =================
void save_memory_report_to_file() {
    FILE* file = fopen("memory_log.txt", "a");
    if (!file) return;

    fprintf(file, "\n--- MEMORY REPORT ---\n");
    fprintf(file, "Total Allocated: %zu bytes\n", totalAllocated);
    fprintf(file, "Total Freed    : %zu bytes\n", totalFreed);
    fprintf(file, "Memory Leaked  : %zu bytes\n", totalAllocated - totalFreed);

    MemBlock* curr = head;
    int leakCount = 0;
    while (curr) {
        if (!curr->isFreed) {
            fprintf(file, "[LEAK] %p | %zu bytes | Type: %s\n",
                    curr->address, curr->size, curr->type);
            leakCount++;
        }
        curr = curr->next;
    }

    if (leakCount == 0)
        fprintf(file, "No memory leaks detected.\n");

    fprintf(file, "-----------------------------\n");
    fclose(file);
}

// ================= Print Memory Report on Console =================
void print_memory_report() {
    printf("\n--- MEMORY REPORT ---\n");
    printf("Total Allocated: %zu bytes\n", totalAllocated);
    printf("Total Freed    : %zu bytes\n", totalFreed);
    printf("Memory Leaked  : %zu bytes\n", totalAllocated - totalFreed);

    MemBlock* curr = head;
    int leakCount = 0;
    while (curr) {
        if (!curr->isFreed) {
            printf("[LEAK] %p | %zu bytes | Type: %s\n",
                curr->address, curr->size, curr->type);
            leakCount++;
        }
        curr = curr->next;
    }

    if (leakCount == 0)
        printf("No memory leaks detected.\n");

    printf("-----------------------------\n");
}
