#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define defer_ret(ret) do {\
    ret_val = ret;\
    goto defer;\
} while (0)

// This struct is to hold all the input file data
// after it's been parsed
typedef struct {
    char *key;
    char *text;
} FileInput;

FileInput *parse_file_data(const char data[], size_t len);
void encrypt(const FileInput *data, char enc[]);
void input_format() {
    fprintf(stderr,
            "Input file format:\n"
            "KEY ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
            "text...\n");
}
void test();

int main(int argc, char **argv) {
    int ret_val = 0;
    if (argc == 2 && strcmp(argv[1], "test") == 0) {
        printf("Testing: Encryption and Decryption...\n");
        test();
        return 0;
    }
    // Making sure enough arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
        input_format();
        return 1;
    }

    // Open the input file
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        perror("File couldn't be opened");
        return 1;
    }

    // Figuring out the file size
    fseek(source, 0, SEEK_END);
    size_t file_size = ftell(source);
    rewind(source);

    // Reading the input file data
    char *data = calloc(1, (file_size+1));
    if (!data) {
        perror("Couldn't allocate enough data.\n");
        defer_ret(1);
    }
    if (file_size != fread(data, 1, file_size, source)) {
        perror("Couldn't read the file");
        defer_ret(1);
    }

    // Opening/creating output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        perror("Could't open/create the output file");
        defer_ret(1);
    }

    // Parsing the input fiel data to separate data
    FileInput *file = parse_file_data(data, file_size);
    if (!file) {
        defer_ret(1);
    }

    // Encrypting the text
    char *enc = calloc(1, strlen(file->text)+1);
    if (!enc) {
        perror("Couldn't allocate enough memory");
        defer_ret(1);
    }

    encrypt(file, enc);

    // Showing the user plain text and the encrypted text
    printf("Plaintext: %s\n", file->text);
    printf("Encrypted: %s\n", enc);

    // Writing to the output file
    fwrite(enc, 1, strlen(file->text), out);

    // Freeing and closing:
defer:
    if(data)         free(data);
    if(out)          fclose(out);
    if(source)       fclose(source);
    if(file->text)   free(file->text);
    if(file->key)    free(file->key);
    if(file)         free(file);
    if(enc)          free(enc);

    return ret_val;
}

FileInput *parse_file_data(const char data[], size_t len) {
    // Declaring some variables
    char KEYWORD[] = "KEY ";
    size_t kw_len = strlen(KEYWORD);
    size_t key_len = 26;  // Key should include all the letters of the alphabets
    FileInput *parsed = malloc(sizeof(FileInput));

    // Making sure the key word exists
    for (size_t i = 0; i < kw_len; ++i) {
        if (KEYWORD[i] == data[i]) {
            continue;
        }
        else {
            input_format();
            free(parsed);
            return NULL;
        }
    }

    // Reading and storing the key
    char *key = calloc(1, (key_len+1));
    if (!key) {
        perror("Couldn't allocate enough memory for the key.\n");
        return NULL;
    }

    size_t txt_len = len - (kw_len + key_len+1);
    char *text = calloc(1, (txt_len+1));
    if (!text) {
        perror("Couldn't allocate enough memory for text.\n");
        free(key);
        free(parsed);
        return NULL;
    }
    size_t i = kw_len;
    size_t j = 0;
    while (data[i] != '\n' && j < key_len) {
        key[j] = data[i];
        ++i;
        ++j;
    }

    // Reading and storing the text
    i += 1; // To get rid of the newline character
    j = 0;
    while (i < len && j < txt_len) {
        text[j] = data[i];
        ++i;
        ++j;
    }

    // Storing the data into the input data type
    parsed->key = key;
    parsed->text = text;
    return parsed;
}

void encrypt(const FileInput *data, char enc[]) {
    // Going through each character in text
    for (size_t i = 0; i < strlen(data->text); ++i) {
        char c = data->text[i];
        // If character upper case
        if (isupper(c)) {
            enc[i] = data->key[(c-'A')];
        }
        // If character upper case do the same as above
        else if (islower(data->text[i])) {
            enc[i] = tolower(data->key[(c-'a')]);
        }
        // If it's not in the alphabet, don't do anything.
        else {
            enc[i] = data->text[i];
        }
    }
}

// Test:
void test() {
    char results[][6] = { "Ifmmp", "Uryyb", "Rovvy" };
    char *text = "Hello";
    char enc_keys[][27] = {
        "BCDEFGHIJKLMNOPQRSTUVWXYZA",
        "NOPQRSTUVWXYZABCDEFGHIJKLM",
        "KLMNOPQRSTUVWXYZABCDEFGHIJ"
    };
    char dec_keys[][27] = {
        "ZABCDEFGHIJKLMNOPQRSTUVWXY",
        "NOPQRSTUVWXYZABCDEFGHIJKLM",
        "QRSTUVWXYZABCDEFGHIJKLMNOP"
    };
    FileInput pt = {
        .key = "",
        .text = text,
    };

    char* enc = calloc(1, strlen(pt.text));

    /* ---------- Encrypt ---------- */
    size_t enc_len = (sizeof enc_keys / sizeof enc_keys[0])-1;
    size_t dec_len = (sizeof dec_keys / sizeof dec_keys[0])-1;

    for (size_t i = 0; i < enc_len; ++i) {
        pt.key = enc_keys[i];
        encrypt(&pt, enc);
        assert(strcmp(enc, results[i]) == 0);
    }

    /* ---------- Decrypt ---------- */
    for (size_t i = 0; i < dec_len; ++i) {
        pt.key = dec_keys[i];
        pt.text = results[i];
        encrypt(&pt, enc);
        assert(strcmp(enc, text) == 0);
    }
    // Test passed:
    printf("All tests passed!\n");
    free(enc);
}
