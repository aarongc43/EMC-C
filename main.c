#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byteContainer.h"

typedef struct {
    char key[100];
    int value;
} KeyValuePair;

void addUInt32ToByteContainerBigEndian(unsigned char **byteContainer, size_t *currentSize, size_t *capacity, uint32_t value) {
    for (int i = 3; i >= 0; i--) {
        unsigned char byte = (value >> (i * 8)) & 0xFF;
        addByte(byteContainer, currentSize, capacity, byte);
    }
}

 void addStringToByteContainer(unsigned char **byteContainer, size_t *currentSize, size_t *capacity, const char *str) {
    for(size_t i = 0; i < strlen(str); i++) {
        addByte(byteContainer, currentSize, capacity, str[i]);
    }
}

int read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[1014];
    char key[100];
    int value;

    KeyValuePair *pairs = NULL;
    int numberOfPairs = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%s = %d", key, &value) == 2) {
            pairs = realloc(pairs, (numberOfPairs + 1) * sizeof(KeyValuePair));
            strcpy(pairs[numberOfPairs].key, key);
            pairs[numberOfPairs].value = value;
            printf("Key: %s, Value: %d\n", key, value);
        } else {
            printf("Line format incorrect: %s", line);
        }
    }

    free(pairs);
    fclose(file);
    return 0;
}

int write_to_file(const char *filename, const unsigned char *bytes, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    if (fwrite(bytes, sizeof(unsigned char), size, file) != size) {
        perror("Error writing to file");
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0) {
        perror("Error closing file");
        return-1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    /*
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    */

    // byte container creation 
    size_t capacity = 20;
    size_t currentSize = 0;
    unsigned char* byteContainer = malloc(capacity * sizeof(unsigned char));
    if (byteContainer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    const char* str1 = "FOR1";
    addStringToByteContainer(&byteContainer, &currentSize, &capacity, str1);

    uint32_t num = 4;
    addUInt32ToByteContainerBigEndian(&byteContainer, &currentSize, &capacity, num);

    const char* str2 = "BEAM";
    addStringToByteContainer(&byteContainer, &currentSize, &capacity, str2);

    for (size_t i = 0; i < currentSize; i++) {
        printf("byteContainer[%zu] = %d\n", i, byteContainer[i]);
    }

    const char* filepath = "main.beam";
    if (write_to_file(filepath, byteContainer, currentSize) != 0) {
        free(byteContainer);
        return -1;
    }

    free(byteContainer);

    return 0;
}
