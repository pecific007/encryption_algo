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
		fmt.Printf("Usage: %v <input> <output>\n", args[0])
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
		return
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

	// Wrintg to the output file
	_, err = out.WriteString(enc)
	if err != nil {
		log.Fatal(err)
	}
}

func encrypt(text string, key int) string {
	// Setting up some variables
	ascii_A := 'A'
	ascii_a := 'a'
	var enc []rune
	// Going throgh each character in text
	for _, c := range text {
		if unicode.IsUpper(c) {
			// If character is uppercase
			ap := (((c - ascii_A) + rune(key)) % 26) + ascii_A
			enc = append(enc, ap)
		} else if unicode.IsLower(c) {
			// If character is lowercase
			ap := (((c - ascii_a) + rune(key)) % 26) + ascii_a
			enc = append(enc, ap)
		} else {
			// If character isn't alphabet
			enc = append(enc, c)
		}
	}
	// Converting from array or rune to string
	enc_s := string(enc)
	return enc_s
}
