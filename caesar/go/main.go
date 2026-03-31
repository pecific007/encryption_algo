package main

import (
	"fmt"
	"log"
	"os"
	"unicode"
)

func main() {
	args := os.Args
	// Making sure enough arguemtns are provided
	if len(args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage: %v <input> <output>\n", args[0])
		return
	}

	// Reading from the input file
	text, err := os.ReadFile(args[1])
	if err != nil {
		log.Fatal(err)
	}

	// Input file cannot be empty
	if len(text) == 0 {
		log.Fatal("The input file cannot be empty.\n")
	}

	// Creating and opening the output file
	out, err := os.Create(args[2])
	if err != nil {
		log.Fatal(err)
	}
	// Automatically closing the output file when function exits
	defer out.Close()
	text_s := string(text) // Converting to string from []byte

	// Get key from user
	var key int
	fmt.Print("Enter key: ")
	fmt.Scanf("%d", &key)

	// Encrypting text
	enc := encrypt(text_s, key)

	// Showing the user plain text and encrypted text
	fmt.Printf("Plaintext:\n\t%s\n", text_s)
	fmt.Printf("Encrypted:\n\t%s\n", enc)

	// Writing to the output file
	_, err = out.WriteString(enc)
	if err != nil {
		log.Fatal(err)
	}
}

func encrypt(text string, key int) string {
	// Variable to store encrypted text
	var enc []rune

	// Going throgh each character in text
	for _, c := range text {
		switch {
		case unicode.IsUpper(c):
			c = (c - 'A') + rune(key)%26 + 'A'
		case unicode.IsLower(c):
			c = (c - 'a') + rune(key)%26 + 'a'
		}
		enc = append(enc, c)
	}
	return string(enc)
}
