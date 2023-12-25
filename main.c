#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byteContainer.h"

typedef struct {
    char key[100];
    int value;
} KeyValuePair;

unsigned char* code_chunk(size_t* size) {
    *size = 0;
    return 0;
}

unsigned char* atom_chunk(size_t* size) {
    *size = 0;
    return 0;
}

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
        return -1;
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

    const char* chunkName = "Code";
    const uint32_t instructionSet = 0;
    const uint32_t opcodeMax = 169;
    const uint32_t labelCount = 7;
    const uint32_t functionCount = 3;

    size_t codeDataSize = 0;
    unsigned char* codeData = NULL;

    uint32_t subSize = 4 * sizeof(uint32_t);
    uint32_t chunkSize = subSize + codeDataSize;

    // BEAM container
    size_t beamCapacity = 20;
    size_t beamSize = 0;
    unsigned char* beam = malloc(beamCapacity * sizeof(unsigned char));
    if (beam == NULL) {
        fprintf(stderr, "Memory allocation failed for beam.\n");
        return 1;
    }

    addStringToByteContainer(&beam, &beamSize, &beamCapacity, "BEAM");

    // calculate code chunk size
    size_t codeSize;
    unsigned char* code = code_chunk(&codeSize);
    for (size_t i = 0; i < codeSize; i++) {
        addByte(&beam, &beamSize, &beamCapacity, code[i]);
    }

    // byte container creation 
    size_t bytesCapacity = 40;
    size_t bytesSize = 0;
    unsigned char* bytes = malloc(bytesCapacity * sizeof(unsigned char));
    if (bytes == NULL) {
        fprintf(stderr, "Memory allocation failed for bytes.\n");
        return 1;
    }

    addStringToByteContainer(&bytes, &bytesSize, &bytesCapacity, "FOR1");
    uint32_t beamLength = beamSize;
    addUInt32ToByteContainerBigEndian(&bytes, &bytesSize, &bytesCapacity, beamLength);

    for (size_t i = 0; i < beamSize; i++) {
        addByte(&bytes, &bytesSize, &bytesCapacity, beam[i]);
    }

    addStringToByteContainer(&beam, &beamSize, &beamCapacity, chunkName);
    chunkSize = subSize + codeDataSize;
    // calculate chunksize based of content that will be added
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, chunkSize);
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, subSize);
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, instructionSet);
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, opcodeMax);
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, labelCount);
    addUInt32ToByteContainerBigEndian(&beam, &beamSize, &beamCapacity, functionCount);

    for (size_t i = 0; i < codeDataSize; i++) {
        addByte(&beam, &beamSize, &beamCapacity, codeData[i]);
    }

    if (write_to_file("main.beam", bytes, bytesSize) != 0) {
        free(bytes);
        free(beam);
        return -1;
    }

    free(bytes);
    free(beam);
    return 0;
}
