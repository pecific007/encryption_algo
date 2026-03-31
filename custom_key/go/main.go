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
type input struct {
	key  string
	text string
}

// Method to encrypt the data
func (ip *input) encrypt() string {
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
	ip, err := parse_file_data(data)
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

func parse_file_data(data []byte) (input, error) {
	keyword := "KEY "

	dataStr := string(data)

	// If key exists then chop off else error
	if strings.HasPrefix(dataStr, keyword) {
		dataStr = dataStr[len(keyword):]
	} else {
		message := fmt.Sprintf("The keyword '%v' not found. Make sure input file is properly formatted.\n", keyword)
		err := errors.New(message)
		input_format()
		return input{}, err
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

	// Getting the text
	text = dataStr

	ip := input{
		key:  string(key),
		text: text,
	}
	return ip, nil
}
