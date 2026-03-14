#include <stdio.h>
#include <string.h>

#define KEY 0xAA

void writeFile(const char *text) {
    FILE *file = fopen("message.jpc", "wb");

    fwrite("JPC1", 1, 4, file);

    while (*text) {
        char encoded = *text ^ KEY;
        fwrite(&encoded, 1, 1, file);
        text++;
    }

    fclose(file);
}

void readFile() {
    FILE *file = fopen("message.jpc", "rb");

    char header[5];
    fread(header, 1, 4, file);
    header[4] = '\0';

    if (strcmp(header, "JPC1") != 0) {
        printf("Invalid file\n");
        fclose(file);
        return;
    }

    char c;

    while (fread(&c, 1, 1, file)) {
        printf("%c", c ^ KEY);
    }

    fclose(file);
}

int main() {

    writeFile("Hello Hackathon!");

    printf("Decoded text: ");
    readFile();

    return 0;
}