package main

import (
	"errors"
	"fmt"
	"log"
	"os"
	"strings"
	"unicode"
)

// This struct to store all data
// related to the input file
type FileInput struct {
	key  string
	text string
}

// Method to encrypt the data
func (ip *FileInput) encrypt() string {
	var alpha [26]rune
	for i := 0; i < len(ip.key); i++ {
		alpha[i] = rune(ip.key[i])
	}

	// Variable to store the encrypted text
	var enc []rune

	// Gonig through each character in text
	for _, c := range ip.text {
		switch {
		case unicode.IsUpper(c):
			c = alpha[c-'A']
		case unicode.IsLower(c):
			c = unicode.ToLower(alpha[c-'a'])
		}
		enc = append(enc, rune(c))
	}
	return string(enc)
}

// Fucntion to print the input file formatting to the stderr
func input_format() {
	fmt.Fprintf(os.Stderr, "Input file format:\n")
	fmt.Fprintf(os.Stderr, "KEY ABCDEFGHIJKLMNOPQRSTUVWXYZ\n")
	fmt.Fprintf(os.Stderr, "Some text to encrypt...\n")
}

func main() {
	args := os.Args
	if len(args) == 2 && args[1] == "test" {
		fmt.Println("Testing: Encryption and Decryption...")
		test();
		return
	}

	// Making sure enough arguments are provided
	if len(args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage: %v <input> <output>\n", args[0])
		input_format()
		os.Exit(1)
	}

	// Reading the data form file
	data, err := os.ReadFile(args[1])
	if err != nil {
		log.Fatal(err)
	}

	// Making sure the input file is within size limits
	maxSize := 1024 * 1024
	if len(data) == 0 {
		fmt.Fprintf(os.Stderr, "The input file cannot be empty.\n")
		input_format()
		os.Exit(1)
	} else if len(data) > maxSize {
		fmt.Fprintf(os.Stderr, "The input file is too big. Max size: %v\n", maxSize)
		os.Exit(1)
	}

	// Creating/replacing output file
	out, err := os.Create(args[2])
	if err != nil {
		log.Fatal(err)
	}
	defer out.Close() // Close output file when fucntion finishes

	// Getting the parsed data
	ip, err := parse_file_data(string(data))
	if err != nil {
		log.Fatal(err)
	}

	// Encryping the data
	enc := ip.encrypt()

	// Showing the user plain text and encrypted text
	fmt.Printf("Plaintext: %v\n", ip.text)
	fmt.Printf("Encrypted: %v\n", enc)

	// Writing to the output file
	_, err = out.WriteString(enc)
	if err != nil {
		log.Fatal(err)
	}

}

func parse_file_data(data string) (FileInput, error) {
	keyword := "KEY "
	// If key exists then chop off else error
	if strings.HasPrefix(data, keyword) {
		data = data[len(keyword):]
	} else {
		message := fmt.Sprintf("The keyword '%v' not found. Make sure input file is properly formatted.\n", keyword)
		err := errors.New(message)
		input_format()
		return FileInput{}, err
	}
	// Getting the key
	key_len := 26
	key := data[:key_len]
	key = strings.ToUpper(key)
	// Chopping off the key with newline char
	data = data[key_len+1:]
	// Getting the text
	text := data
	ip := FileInput{
		key:  key,
		text: text,
	}
	return ip, nil
}

func assert(cond bool) {
	if !cond {
		log.Fatal("Assert Failed!")
	}
}

// Test:
func test() {
	defer fmt.Println("All tests passed!")
	text := "Hello"
	results := []string {"Ifmmp", "Uryyb", "Rovvy" }
	enc_keys := []string {
		"BCDEFGHIJKLMNOPQRSTUVWXYZA",
		"NOPQRSTUVWXYZABCDEFGHIJKLM",
		"KLMNOPQRSTUVWXYZABCDEFGHIJ",
	}
	dec_keys := []string {
		"ZABCDEFGHIJKLMNOPQRSTUVWXY",
		"NOPQRSTUVWXYZABCDEFGHIJKLM",
		"QRSTUVWXYZABCDEFGHIJKLMNOP",
	}
	pt := FileInput {
		key: "",
		text: text,
	}
	for i, k := range enc_keys {
		pt.key = k
		enc := pt.encrypt()
		assert(enc == results[i])
	}
	for i, k := range dec_keys {
		pt.key = k
		pt.text = results[i]
		enc := pt.encrypt()
		assert(enc == text)
	}
}
