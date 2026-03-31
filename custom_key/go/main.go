package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"unicode"
)

// This struct to store all data
// related to the input file
type input struct {
	key      string
	key_len  uint
	text     string
	text_len uint
}

// Method to encrypt the data
func (ip *input) encrypt() string {
	// Creating an associative-array/map
	// to easily reference key with alphabets
	alpha := map[rune]rune{}
	for i := range ip.key_len {
		alpha[rune('A'+i)] = rune(ip.key[i])
	}

	// Variable to store the encrypted text
	var enc []rune

	// Gonig through each character in text
	for _, c := range ip.text {
		switch {
		// If character is upper-case
		case unicode.IsUpper(c):
			// add the corresponding key-letter
			c = alpha[c]
		case unicode.IsLower(c):
			// Add the lower-case of the upper-case's corresponding key-letter
			c = unicode.ToLower(alpha[unicode.ToUpper(c)])
		}
		// Append to enc: Upper-case | Lower-case | not alphabet
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
	// Making sure enough arguments are provided
	args := os.Args

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
	ip := parse_file_data(data)

	// Encryping the data
	enc := ip.encrypt()

	// Showing the user plain text and encrypted text
	fmt.Printf("Plaintext: %v\n", ip.text)
	fmt.Printf("Encrypted: %v\n", enc)

	// Writing to the output file
	out.WriteString(enc)
}

func parse_file_data(data []byte) input {
	keyword := "KEY "

	dataStr := string(data)

	// If key exists then chop off else error
	if strings.Contains(dataStr, keyword) {
		dataStr = dataStr[len(keyword):]
	} else {
		fmt.Fprintf(os.Stderr,
			"The keyword '%v' not found. Make sure input file is properly formatted.\n",
			keyword)
		input_format()
		os.Exit(1)
	}

	var key []rune
	var text string

	// Getting the key
	key_len := 26
	for i, k := range dataStr {
		if i == key_len {
			if k == '\n' || k == ' ' {
				dataStr = dataStr[i+1:]
			} else {
				dataStr = dataStr[i:]
			}
			break
		}
		switch {
		case unicode.IsUpper(k):
			key = append(key, k)
		case unicode.IsLower(k):
			key = append(key, unicode.ToUpper(k))
		}
	}

	// Getting the ke
	text = dataStr

	ip := input{
		key:      string(key),
		text:     text,
		text_len: uint(len(text)),
		key_len:  uint(len(string(key))),
	}
	return ip
}
