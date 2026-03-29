// Caesar's algo => C(i) = (p(i) + k) % 26
// Where p(i) = Plain text | k = Caesar key | % 26 = Remainder after deviding by 26
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(char plain[], int key, char enc[]);

int main(int argc, char **argv) {
    // Making sure enough arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        return 1;
    }
    // Opening the source file in read mode
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        fprintf(stderr, "Fild %s not found. Makesure you're entered the correct name.\n", argv[1]);
        return 1;
    }

    // Opening/creating the output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stdout, "Couldn't open the output file.\n");
        fclose(source);
        return 1;
    }

    // Figuring out the size of the input file:
    fseek(source, 0, SEEK_END);
    int file_size = ftell(source);
    rewind(source);

    if (file_size == 0) {
        fprintf(stderr, "The input file cannot be empty.");
        fclose(source);
        fclose(out);
        return 1;
    }

    // making the buffer
    char *text = malloc(sizeof(char)*file_size);
    if (!text) {
        fprintf(stdout, "Couldn't allocate enough memory!\n");
        fclose(source);
        fclose(out);
        return 1;
    }
    // Reading the data into the buffer
    if (1 != fread(text, file_size, 1, source)) {
        fprintf(stderr, "Couldn't read the file.\n");
        fclose(source);
        fclose(out);
        free(text);
        return 1;
    }

    // Getting the key from user
    int key;
    printf("Enter key: ");
    scanf("%d", &key);

    // Encrypting the text
    char *enc = malloc(sizeof(char)*file_size);
    encrypt(text, key, enc);

    // Showing the user plain text and encrypted text
    printf("Plaintext:\n\t%s\n", text);
    printf("Encrypted:\n\t%s\n", enc);

    // Writing encrypted text to ouput file
    fprintf(out, "%s", enc);

    // Closing and freeing:
    fclose(source);
    fclose(out);
    free(text);
    free(enc);
    return 0;
}

void encrypt(char plain[], int key, char enc[]) {
    // Setint up useful variables
    int len = strlen(plain);
    int ascii_A = 'A';
    int ascii_a = 'a';
    // iterating through each character of the plani text
    for (int i = 0; i < len; ++i) {
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
    // Set the last eleent to nul
    enc[len] = '\0';
}
