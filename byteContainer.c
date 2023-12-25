#include "byteContainer.h"
#include <stdio.h>

void addByte(unsigned char **byteContainer, size_t *currentSize, size_t *capacity, unsigned char byte) {
    if (*currentSize == *capacity) {
        // increase capacity
        *capacity *= 2;
        unsigned char* temp = realloc(*byteContainer, *capacity * sizeof(unsigned char));
        if (temp == NULL) {
            free(*byteContainer);
            fprintf(stderr, "Memeory allocation failed.\n");
            exit(1);
        }
        *byteContainer = temp;
    }
    (*byteContainer)[(*currentSize)++] = byte;
}
