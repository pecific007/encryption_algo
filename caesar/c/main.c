// Caesar's algo => C(i) = (p(i) + k) % 26
// Where p(i) = Plain text | k = Caesar key | % 26 = Remainder after dividing by 26
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This macro I learned from: https://www.youtube.com/@Tsodingdaily
// This is essentially simulating defer statements in Go... kind of
#define defer_ret(ret) do {\
    ret_val = ret;\
    goto defer;\
} while (0)

#define MAX_FILE_SIZE 1024*1024

void encrypt(char plain[], size_t len, int key, char enc[]);
void test();

int main(int argc, char **argv) {
    int ret_val = 0;
    // For testing:
    if (argc == 2 && strcmp(argv[1], "test") == 0) {
        printf("Testing: Encryption and Decryption...\n");
        test();
        return 0;
    }
    // Making sure enough arguments are provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input> <output> <key>\n", argv[0]);
        return 1;
    }
    // Opening the source file in read mode
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        perror("File couldn't be oepend");
        return 1;
    }

    // Opening/creating the output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        perror("Couldn't open the output file");
        defer_ret(1);
    }

    // Figuring out the size of the input file:
    fseek(source, 0, SEEK_END);
    size_t file_size = ftell(source);
    rewind(source);
    // Input file cannot be empty
    if (file_size == 0) {
        fprintf (stderr, "The input file cannot be empty.\n");
        defer_ret(1);
    } else if (file_size > MAX_FILE_SIZE) {
        fprintf (stderr, "The input file is too large.\nMax size: 1 MB\n");
        defer_ret(1);
    }

    // making the buffer
    char *text = malloc(file_size+1);
    if (!text) {
        perror ("Couldn't allocate enough memory!");
        defer_ret(1);
    }
    // Reading the data into the buffer
    if (file_size != fread(text, 1, file_size, source)) {
        perror ("Couldn't read the file");
        defer_ret(1);
    }
    text[file_size] = '\0'; // adding null ternimator

    // Getting the key from user
    int key = 0;
    key = atoi(argv[3]) % 26;


    // Encrypting the text
    char *enc = malloc(file_size+1);
    if (!enc) {
        perror ("Couldn't allocate enough memory");
        defer_ret(1);
    }
    encrypt(text, file_size, key, enc);

    // Showing the user plain text and encrypted text
    printf("Plaintext:\n\t%s\n", text);
    printf("Encrypted:\n\t%s\n", enc);

    // Writing encrypted text to ouput file
    fwrite(enc, 1, file_size, out);

    // Closing and freeing:
defer:
    if(source) fclose(source);
    if(out)    fclose(out);
    if(text)   free(text);
    if(enc)    free(enc);
    return ret_val;
}

void encrypt(char plain[], size_t len, int key, char enc[]) {
    // iterating through each character of the plani text
    for (size_t i = 0; i < len; ++i) {
        // If the character is uppercase
        if (isupper(plain[i])) {
            int base = ((plain[i] - 'A') + key);
            if (base < 0) {
                base = 26 + base;
            }
            enc[i] = (base % 26) + 'A';
        }
        // If the character is lowercase
        else if (islower(plain[i])) {
            int base = ((plain[i] - 'a') + key);
            if (base < 0) {
                base = 26 + base;
            }
            enc[i] = (base % 26) + 'a';
        }
        // If not an alphabet
        else {
            enc[i] = plain[i];
        }
    }
    // Adding null terminator
    enc[len] = '\0';
}


// Test:
void test() {
    char keys[] = { 1, 13, 10 };
    char results[][6] = { "Ifmmp", "Uryyb", "Rovvy" };
    char pt[] = "Hello";
    int len = strlen(pt);
    char* enc = calloc(1, len+1);
    size_t len_res = (sizeof results / sizeof results[0])-1;

    /* ---------- Encrypt ---------- */
    for (size_t i = 0; i < len_res; ++i) {
        encrypt(pt, len, keys[i], enc);
        assert(strcmp(enc, results[i]) == 0);
    }

    /* ---------- Decrypt ---------- */
    for (size_t i = 0; i < len_res; ++i) {
        encrypt(results[i], len, -keys[i], enc);
        assert(strcmp(enc, pt) == 0);
    }
    // Test passed:
    printf("All tests passed!\n");
    free(enc);
}
