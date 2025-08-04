#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_tracker.h"

int main() {
    void* ptrList[100];
    int ptrCount = 0;
    int choice;

    do {
        printf("\n====== Memory Tracker Menu ======\n");
        printf("1. Allocate Memory\n");
        printf("2. Free Memory\n");
        printf("3. Show Memory Report\n");
        printf("4. Exit\n");
        printf("5. Reallocate Memory\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                size_t size;
                char type[50], allocType[10];

                printf("Enter size in bytes: ");
                scanf("%zu", &size);
                getchar();

                printf("Enter data type name: ");
                fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = '\0';

                printf("Use malloc or calloc? (m/c): ");
                fgets(allocType, sizeof(allocType), stdin);

                void* ptr = NULL;
                if (allocType[0] == 'm')
                    ptr = malloc(size);
                else if (allocType[0] == 'c')
                    ptr = calloc(1, size);

                if (ptr) {
                    add_block(ptr, size, type);
                    ptrList[ptrCount++] = ptr;
                    printf("Allocated at: %p\n", ptr);
                } else {
                    printf("Allocation failed.\n");
                }
                break;
            }

            case 2: {
                if (ptrCount == 0) {
                    printf("No allocated pointers to free.\n");
                    break;
                }

                printf("Allocated pointers:\n");
                for (int i = 0; i < ptrCount; i++) {
                    printf("%d. %p\n", i + 1, ptrList[i]);
                }

                int index;
                printf("Enter index to free: ");
                scanf("%d", &index);
                getchar();

                if (index >= 1 && index <= ptrCount) {
                    free_block(ptrList[index - 1]);
                } else {
                    printf("Invalid index.\n");
                }
                break;
            }

            case 3:
                print_memory_report();
                break;

            case 4:
                save_memory_report_to_file();
                printf("Exiting. Memory report saved to memory_log.txt\n");
                break;

            case 5: {
                if (ptrCount == 0) {
                    printf("No pointers available to realloc.\n");
                    break;
                }

                printf("Allocated pointers:\n");
                for (int i = 0; i < ptrCount; i++) {
                    printf("%d. %p\n", i + 1, ptrList[i]);
                }

                int index;
                printf("Enter index to realloc: ");
                scanf("%d", &index);
                getchar();

                if (index >= 1 && index <= ptrCount) {
                    size_t new_size;
                    char new_type[50];

                    printf("Enter new size in bytes: ");
                    scanf("%zu", &new_size);
                    getchar();

                    printf("Enter new type name: ");
                    fgets(new_type, sizeof(new_type), stdin);
                    new_type[strcspn(new_type, "\n")] = '\0';

                    void* new_ptr = my_realloc(ptrList[index - 1], new_size, new_type);
                    if (new_ptr) {
                        ptrList[index - 1] = new_ptr;
                        printf("Reallocated to: %p\n", new_ptr);
                    } else {
                        printf("Reallocation failed.\n");
                    }
                } else {
                    printf("Invalid index.\n");
                }
                break;
            }

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);

    return 0;
}
