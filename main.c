#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[100];
    int value;
} KeyValuePair;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
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
