#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processKey(FILE *key, int **keyArray, int *block, int *keyLength);
void processPlaintext(FILE *plaintext, char **message);
void hill(char *message, int **hillText, int *keyArray, int *cipherLength, int block);
void railfence(int *hillText, int **cipherText, int depth, int cipherLength);

int main(int argc, char* argv[]){
    if(argc < 5){
        printf("reenter command");
        return 1;
    }
// checks if all arguments are entered (kinda)
    else if(strcmp(argv[1], "encrypt") != 0){
        printf("need encrypt argument");
        return 1;
    }
// I don't understand why we need an encrypt argument if all we're doing is encrypting so the if statement here is all it's gonna get

    FILE *key = fopen(argv[2], "r");
    FILE *plaintext = fopen(argv[3], "r");
// opens files from arguments
    
    int *keyArray = NULL;
    int block;
    int keyLength = 0;
    processKey(key, &keyArray, &block, &keyLength);
// takes first character in key file and stores it in block, then rips out all the numbers and shoves them into an array keyArray, keyLength is the length of the array

    char *message;
    processPlaintext(plaintext, &message);
// reads the plaintext file, capitalizes all a-z characters, deletes everything else and returns as a really fat string

    int *hillText;
    int cipherLength = 0;
    hill(message, &hillText, keyArray, &cipherLength, block);
// encrypts message with the hill cipher and returns as an int array

    int *cipherText;
    int depth = (int)((*argv[4]) - '0');
    railfence(hillText, &cipherText, depth, cipherLength);

    printf("Key matrix:\n");
    for(int i = 0; i < keyLength; i ++){
        if((i + 1) % block != 0){
        printf("%d\t", keyArray[i]);
        }
        else{
        printf("%d\n", keyArray[i]);
        }
    }
    printf("\nPlaintext:\n");
    for(int i = 0; i < strlen(message); i ++){
        if((i + 1) % 80 != 0){
        printf("%c", message[i]);
        }
        else if(i == strlen(message)){
        printf("%c", message[i]);
        }
        else{
            printf("%c\n", message[i]);
        }
    }
    printf("\n\nCiphertext:\n");
    for(int i = 0; i < cipherLength; i++){
        if((i + 1) % 80 != 0){
        printf("%c", (char)(cipherText[i] + 65));
        }
        else if(i == cipherLength){
        printf("%c", (char)(cipherText[i] + 65));
        }
        else{
        printf("%c\n", (char)(cipherText[i] + 65));
        }
    }
    printf("\n\nDepth: %d\n", depth);
// print output with correct format
    
    free(keyArray);
    free(message);
    fclose(key);
    fclose(plaintext);
// free memory and close files

    return 0;
}

void processKey(FILE *key, int **keyArray, int *block, int *keyLength){
    fseek(key, 0, SEEK_END);
    long keySize = ftell(key);
    rewind(key);
// moves cursor to end of file, reads how many bits are in file by reading cursor position (at end of file) to fileSize, then puts cursor back to beginning of file to read again

    char *keyContent = malloc(keySize + 1);
    fread(keyContent, 1, keySize, key);
    keyContent[keySize] = '\0';
// allocates memory of file's length to fileContent for accurate storage, reads 1 byte at a time starting at cursor 0 and ending at byte fileSize into fileContent then appends with '\0' to convert to string

    *block = keyContent[0] - '0';
// takes first character in file and stores it as int into block in main
    keyContent[0] = ' ';
// removes character
    char *end;
    long num;
    while(*keyContent != '\0'){
        num = strtol(keyContent, &end, 10);
        if(keyContent == end){
            keyContent++;
        }
        else{
            *keyArray = realloc(*keyArray, (*keyLength + 1) * sizeof(int));
            (*keyArray)[*keyLength] = (int)num;
            keyContent = end;
            (*keyLength)++;
        }
    }
// everytime there is whitespace or a non dec 10 number, move beginning of string to right after it and check again
// if dec 10, store all dec 10 values into current keyArray index until next non dec 10 value, increment index, move beginning of string, check again

// tldr the while loop skips all white space and stores the numbers in the key file into an array so that "10 3 21" == [10, 3, 21] and not [10321]. Then return the length of the array and its contents to main
}

void processPlaintext(FILE *plaintext, char **message){
    fseek(plaintext, 0, SEEK_END);
    long plainSize = ftell(plaintext);
    rewind(plaintext);
// moves cursor to end of file, reads how many bits are in file by reading position (at end of file) to fileSize, then puts cursor back to beginning of file

    char *plainContent = malloc(plainSize + 1);
    fread(plainContent, 1, plainSize, plaintext);
    (plainContent)[plainSize] = '\0';
// allocates file's amount of memory to fileContent for accurate storage, reads 1 byte at a time starting at bytefileSize of file and ending at byte 0 into fileContent
// then appending fileSize with \0 to make it a valid string
    
    *message = malloc(plainSize +1);
    strcpy(*message, plainContent);
// originally I returned plainContent to main as well but it was unnecessary for the output in the end. But I'm keeping it like this because it works and I don't feel like changing it
    
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

void hill(char *message, int **hillText, int *keyArray, int *cipherLength, int block){
    *cipherLength = strlen(message);
    int *hillNum = NULL;
    int padding = 0;
// for ease of use

    if(*cipherLength % block == 0){
        *hillText = malloc(*cipherLength * sizeof(int));
        hillNum = malloc(*cipherLength * sizeof(int));
        for(int i = 0; i < *cipherLength; i++){
            hillNum[i] = message[i] - 'A';
        }
    }
// allocates memory for hillText and hillNum then takes all characters in message and turns them into % 26 int for hillNum
    else{
        padding = (block - (*cipherLength % block));
        *hillText = malloc((*cipherLength + padding) * sizeof(int));
        hillNum = malloc((*cipherLength + padding) * sizeof(int));
        for(int i = 0; i < *cipherLength; i++){
            hillNum[i] = message[i] - 'A';
        }
        for(int i = 0; i < padding; i++){
            hillNum[*cipherLength+i] = 23;
        }
        *cipherLength += padding;
    }
// appends with 'X' if padding is needed
    int tempHill = 0;
    for(int i = 0; i < *cipherLength; i += block){
        for(int j = 0; j < block * block; j += block){
            for(int l = 0; l < block; l++){
                tempHill += keyArray[j+l] * hillNum[i+l];
            }
            (*hillText)[i + (j / block)] = tempHill % 26;
            tempHill = 0;
        }
    }
// encrypts message with the hill cipher and stores it in hillText
    free(hillNum);
}

void railfence(int *hillText, int **cipherText, int depth, int cipherLength){
    *cipherText = malloc(cipherLength * sizeof(int));
    if(depth == 1){
        for(int i = 0; i < cipherLength; i++){
            (*cipherText)[i] = hillText[i];
        }
    }
    else if(depth == 2){
        int count = 0;
        for(int i = 0, j = 0; j < cipherLength; i++, j += 2){
            (*cipherText)[i] = hillText[j];
            count++;
        }
        for(int i = 0, j = 0; j < cipherLength; i++, j += 2){
            (*cipherText)[count+i] = hillText[j+1];
        }
    }
    else if(depth == 3){
        int i = 0;
        for(int j = 0; j < cipherLength; j += 4){
            (*cipherText)[i++] = hillText[j];
        }
        for(int j = 1; j < cipherLength; j += 2){
            (*cipherText)[i++] = hillText[j];
        }
        for(int j = 2; j < cipherLength; j += 4){
            (*cipherText)[i++] = hillText[j];
        }
    }
}