#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processKey(FILE *key, int **keyArray, char **chunk, int *keyLength);
void processPlaintext(FILE *plaintext, char **plainContent, char **message);
void hill(int block, int *keyArray, char *message, char **hillText);

int main(int argc, char* argv[]){
    if(argc < 5){
        printf("reenter command");
        return 1;
    }
    else if(strcmp(argv[1], "encrypt") != 0){
        printf("need encrypt argument");
        return 1;
    }

    FILE *key = fopen(argv[2], "r");
    FILE *plaintext = fopen(argv[3], "r");
    
    int *keyArray;
    char *chunk;
    int keyLength = 0;
    processKey(key, &keyArray, &chunk, &keyLength);
    char *plainContent;
    char *message;
    processPlaintext(plaintext, &plainContent, &message);
// process text files

    int block = *chunk - '0';
    char *hillText;
    hill(block, keyArray, message, &hillText);

    printf("block = %c\n", *chunk);
    for(int i = 0; i < keyLength; i++){
        printf("%d", keyArray[i]);
    }
    printf("\n************************************\n\n");
    printf("%s\n", plainContent);
    printf("************************************\n\n");
    printf("%s", message);
    
    free(keyContent);
    free(keyArray);
    free(plainContent);
    free(message);
    fclose(key);
    fclose(plaintext);

    return 0;
}

void processKey(FILE *key, int **keyArray, char **chunk, int *keyLength){
    fseek(key, 0, SEEK_END);
    long keySize = ftell(key);
    rewind(key);
// moves &key to end of file, reads how many bits are in file by reading pointer position (at end of file) to fileSize, then puts &key back to beginning of file

    char *keyContent = malloc(keySize + 1);
    fread(keyContent, 1, keySize, key);
    keyContent[keySize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at &key and ending at byte fileSize into fileContent

    **chunk = keyContent[0];
    keyContent[0] = ' ';
    char *end;
    long num;
    int i = 0;
// *************************************************************************************************************************************************************** work here <----
    while(*keyContent != '\0'){
        num = strtol(keyContent, &end, 10);
        if(keyContent == end){
            keyContent++;
        }
        else{
            (*keyArray)[*keyLength] = (int)num;
            keyContent = end;
        }
        *keyLength++;
    }
    
}

void processPlaintext(FILE *plaintext, char **plainContent, char **message){
    fseek(plaintext, 0, SEEK_END);
    long plainSize = ftell(plaintext);
    rewind(plaintext);
// moves cursor to end of file, reads how many bits are in file by reading position (at end of file) to fileSize, then puts cursor back to beginning of file

    *plainContent = malloc(plainSize + 1);
    fread(*plainContent, 1, plainSize, plaintext);
    (*plainContent)[plainSize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at bytefileSize of file and ending at byte 0 into fileContent
// then appending fileSize with \0 to make it a valid string
    
    *message = malloc(plainSize +1);
    strcpy(*message, *plainContent);
    
    int j = 0;
    for(int i = 0; i < (plainSize + 1); i++){
        if((*message)[i] >= 'a' && (*message)[i] <= 'z'){
            (*message)[i] -= 32;
        }
        if((*message)[i] >= 'A' && (*message)[i] <= 'Z'){
            (*message)[j++] = (*message)[i];
        }
    }
    (*message)[j] = '\0';
// capitalizes all a-z characters and deletes everything else
}

void hill(int block, int *keyArray, char *message, char **hillText){
    for(block; block > 0; block--){

    }
}