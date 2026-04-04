#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// This struct is to hold all the input file data
// after it's been parsed
typedef struct {
    char *key;
    size_t key_size;
    char *text;
    size_t text_size;
} FileInput;

FileInput *parse_file_data(const char data[], size_t len);
void encrypt(const FileInput *data, char enc[]);

int main(int argc, char **argv) {
    // Making sure enough arguments are provided
    if (argc != 3) {
        fprintf(stderr,
            "Usage: %s <input> <output>\n"
            "Input file format:\n"
            "KEY ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
            "text...\n"
            , argv[0]);
        return 1;
    }

    // Open the input file
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        fprintf(stderr, "%s couldn't be opened. Make sure you're put the file name correctly\n", argv[1]);
        return 1;
    }

    // Figuring out the file size
    fseek(source, 0, SEEK_END);
    size_t file_size = ftell(source);
    rewind(source);

    // Reading the input file data
    char *data = calloc(1, (file_size+1));
    if (!data) {
        fprintf(stderr, "Couldn't allocate enough data.\n");
        fclose(source);
        return 1;
    }
    if (file_size != fread(data, 1, file_size, source)) {
        fprintf(stderr, "Couldn't read the file.\n");
        fclose(source);
        free(data);
        return 1;
    }

    // Opening/creating output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        fprintf(stderr, "Could't open/create the output file.\n");
        free(data);
        fclose(source);
        return 1;
    }

    // Parsing the input fiel data to separate data
    FileInput *file = parse_file_data(data, file_size);
    if (!file) {
        free(data);
        fclose(source);
        fclose(out);
        return 1;
    }

    // Encrypting the text
    char *enc = calloc(1, file->text_size);
    if (!enc) {
        fprintf(stderr, "Couldn't allocate enough memory for encrypted text.\n");
        free(data);
        fclose(source);
        free(file->key);
        free(file->text);
        free(file);
        return 1;
    }

    encrypt(file, enc);

    // Showing the user plain text and the encrypted text
    printf("Plaintext: %s\n", file->text);
    printf("Encrypted: %s\n", enc);

    // Writing to the output file
    fwrite(enc, 1, file->text_size, out);

    // Freeing and closing:
    free(data);
    fclose(out);
    fclose(source);
    free(file->text);
    free(file->key);
    free(file);
    free(enc);

    return 0;
}

FileInput *parse_file_data(const char data[], size_t len) {
    // Declaring some variables
    char KEYWORD[] = "KEY ";
    size_t kw_len = (sizeof(KEYWORD) / sizeof(KEYWORD[0])) - 1;
    size_t key_len = 26;  // Key should include all the letters of the alphabets
    FileInput *parsed = malloc(sizeof(FileInput));

    // Making sure the key word exists
    for (size_t i = 0; i < kw_len; ++i) {
        if (KEYWORD[i] == data[i]) {
            continue;
        }
        else {
            fprintf(stderr,
                "Key word %s not found. Make sure input file is properly formatted\n"
                "Input file format:\n"
                "KEY ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                "text...\n"
                , KEYWORD);
            free(parsed);
            return NULL;
        }
    }

    // Reading and storing the key
    char *key = calloc(1, (key_len+1));
    if (!key) {
        fprintf(stderr, "Couldn't allocate enough memory for the key.\n");
        return NULL;
    }

    size_t txt_len = len - (kw_len + key_len+1);
    char *text = calloc(1, (txt_len+1));
    if (!text) {
        fprintf(stderr, "Couldn't allocate enough memory for text.\n");
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
    parsed->key_size = key_len;
    parsed->text = text;
    parsed->text_size = txt_len;
    return parsed;
}

void encrypt(const FileInput *data, char enc[]) {
    // Going through each character in text
    for (size_t i = 0; i < data->text_size; ++i) {
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
