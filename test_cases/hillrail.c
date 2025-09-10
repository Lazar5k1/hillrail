#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc < 5){
        printf("reenter command");
        return 1;
    }
    else if(strcmp(argv[1], "encrypt") != 0){
        printf("need encrypt argument");
        return 1;
    }
    printf("Your depth is: %s  :O oh my gahhh!!\n", argv[3]);

    FILE *key;
    FILE *plaintext;

    key = fopen(argv[2], "r");
    plaintext = fopen(argv[3], "r");

    fseek(key, 0, SEEK_END);
    long keySize = ftell(key);
    rewind(key);
// moves &key to end of file, reads how many bits are in file by reading pointer position (at end of file) to fileSize, then puts &key back to beginning of file
    fseek(plaintext, 0, SEEK_END);
    long plainSize = ftell(plaintext);
    rewind(plaintext);
// same thing but for plaintext

    char *keyContent = malloc(keySize + 1);
    fread(keyContent, 1, keySize, key);
    keyContent[keySize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at &key and ending at byte fileSize into fileContent, then appending fileSize with \0 to make it a valid string
    char *plainContent = malloc(plainSize + 1);
    fread(plainContent, 1, plainSize, plaintext);
    plainContent[plainSize] = '\0';
// again, same thing for plaintext

    printf("%s\n\n", keyContent);
    printf("%s", plainContent);
    free(keyContent);
    free(plainContent);
    fclose(key);
    fclose(plaintext);

    return 0;
}