#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processPlaintext(FILE *plaintext, char **plainContent, char **processContent);

int main(int argc, char* argv[]){
    if(argc < 5){
        printf("reenter command");
        return 1;
    }
    else if(strcmp(argv[1], "encrypt") != 0){
        printf("need encrypt argument");
        return 1;
    }
        
    printf("Your depth is: %s\n", argv[4]);


    FILE *key = fopen(argv[2], "r");
    FILE *plaintext = fopen(argv[3], "r");

    fseek(key, 0, SEEK_END);
    long keySize = ftell(key);
    rewind(key);
// moves &key to end of file, reads how many bits are in file by reading pointer position (at end of file) to fileSize, then puts &key back to beginning of file

    char *keyContent = malloc(keySize + 1);
    fread(keyContent, 1, keySize, key);
    keyContent[keySize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at &key and ending at byte fileSize into fileContent
    
    char *plainContent = NULL;
    char *processContent = NULL;
    processPlaintext(plaintext, &plainContent, &processContent);

    printf("%s\n\n", keyContent);
    printf("************************************\n\n");
    printf("%s\n\n", plainContent);
    printf("************************************\n\n");
    printf("%s", processContent);
    
    free(keyContent);
    free(plainContent);
    free(processContent);
    fclose(key);
    fclose(plaintext);

    return 0;
}

void processPlaintext(FILE *plaintext, char **plainContent, char **processContent){
    fseek(plaintext, 0, SEEK_END);
    long plainSize = ftell(plaintext);
    rewind(plaintext);
// moves cursor to end of file, reads how many bits are in file by reading position (at end of file) to fileSize, then puts cursor back to beginning of file

    *plainContent = malloc(plainSize + 1);
    fread(*plainContent, 1, plainSize, plaintext);
    *plainContent[plainSize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at bytefileSize of file and ending at byte 0 into fileContent
// then appending fileSize with \0 to make it a valid string

    
    *processContent = malloc(plainSize +1);
    strcpy(*processContent, *plainContent);

    int j = 0;
    for(int i = 0; i < (plainSize +1); i++){
        if(*processContent[i] >= 'a' && *processContent[i] <= 'z'){
            *processContent[i] -= 32;
        }
        if(*processContent[i] >= 'A' && *processContent[i] <= 'Z'){
            *processContent[j++] = *processContent[i];
        }
    }
    *processContent[j] = '\0';
// capitalizes all a-z characters and deletes everything else
}