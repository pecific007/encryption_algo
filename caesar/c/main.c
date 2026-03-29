// Caesar's algo => C(i) = (p(i) + k) % 26
// Where p(i) = Plain text | k = Caesar key | % 26 = Remainder after dividing by 26
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_SIZE 1024*1024

void encrypt(char plain[], size_t len, int key, char enc[]);

int main(int argc, char **argv) {
    // Making sure enough arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        return 1;
    }
    // Opening the source file in read mode
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        fprintf(stderr, "File %s not found. Makesure you've entered the correct name.\n", argv[1]);
        return 1;
    }

    // Opening/creating the output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Couldn't open the output file.\n");
        fclose(source);
        return 1;
    }

    // Figuring out the size of the input file:
    fseek(source, 0, SEEK_END);
    size_t file_size = ftell(source);
    rewind(source);
    // Input file cannot be empty
    if (file_size == 0) {
        fprintf(stderr, "The input file cannot be empty.");
        fclose(source);
        fclose(out);
        return 1;
    } else if (file_size > MAX_FILE_SIZE) {
        fprintf(stderr, "The input file is too large.\nMax size: 1 MB");
        fclose(source);
        fclose(out);
        return 1;
    }

    // making the buffer
    char *text = malloc(file_size+1);
    if (!text) {
        fprintf(stdout, "Couldn't allocate enough memory!\n");
        fclose(source);
        fclose(out);
        return 1;
    }
    // Reading the data into the buffer
    if (file_size != fread(text, 1, file_size, source)) {
        fprintf(stderr, "Couldn't read the file.\n");
        fclose(source);
        fclose(out);
        free(text);
        return 1;
    }
    text[file_size] = '\0'; // adding null ternimator

    // Getting the key from user
    int key;
    printf("Enter key: ");
    if (scanf("%d", &key) != 1) {
        fprintf(stderr, "Invalid key.\n");
        return 1;
    }
    key = ((key % 26) + 26) % 26;

    // Encrypting the text
    char *enc = malloc(file_size+1);
    if (!enc) {
        fprintf(stderr, "Couldn't allocate enough memory.\n");
        fclose(source);
        fclose(out);
        free(text);
        return 1;
    }
    encrypt(text, file_size, key, enc);

    // Showing the user plain text and encrypted text
    printf("Plaintext:\n\t%s\n", text);
    printf("Encrypted:\n\t%s\n", enc);

    // Writing encrypted text to ouput file
    fwrite(enc, 1, file_size, out);

    // Closing and freeing:
    fclose(source);
    fclose(out);
    free(text);
    free(enc);
    return 0;
}

void encrypt(char plain[], size_t len, int key, char enc[]) {
    // Setint up useful variables
    int ascii_A = 'A';
    int ascii_a = 'a';
    // iterating through each character of the plani text
    for (size_t i = 0; i < len; ++i) {
        // If the character is uppercase
        if (isupper(plain[i])) {
            enc[i] = (((plain[i] - ascii_A) + key) % 26) + ascii_A;
        }
        // If the character is lowercase
        else if (islower(plain[i])) {
            enc[i] = (((plain[i] - ascii_a) + key) % 26) + ascii_a;
        }
        // If not an alphabet
        else {
            enc[i] = plain[i];
        }
    }
    // Adding null terminator
    enc[len] = '\0';
}
