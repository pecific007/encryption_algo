package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"unicode"
)

func main() {
	args := os.Args
	if len(args) == 2 && args[1] == "test" {
		fmt.Println("Testing: Encryption and Decryption...")
		test();
		return
	}
	// Making sure enough arguemtns are provided
	if len(args) != 4 {
		fmt.Fprintf(os.Stderr, "Usage: %v <input> <output> <key>\n", args[0])
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

	// Get key
	key, err := strconv.Atoi(args[3])
	if err != nil {
		log.Fatal(err)
	}

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
			base := ((c - 'A') + rune(key))
			if base < 0 {
				base = 26 + base
			}
			c = base % 26 + 'A'
		case unicode.IsLower(c):
			base := ((c - 'a') + rune(key))
			if base < 0 {
				base = 26 + base
			}
			c = base % 26 + 'a'
		}
		enc = append(enc, c)
	}
	return string(enc)
}

func assert(condition bool) {
	if !condition {
		log.Fatal("Assetion Failed.")
		panic("Assertion failed!")
	}
}

func test() {
	pt := "Hello"
	/* ---------- Encrypt ---------- */
	key := 1
	enc := encrypt(pt, key)
	assert(enc == "Ifmmp")
	key = 13
	enc = encrypt(pt, key)
	assert(enc == "Uryyb")
	key = 10
	enc = encrypt(pt, key)
	assert(enc == "Rovvy")

	/* ---------- Decrypt ---------- */
	key = -1
	enc = encrypt("Ifmmp", key)
	assert(enc == pt)
	key = -13
	enc = encrypt("Uryyb", key)
	assert(enc == pt)
	key = -10
	enc = encrypt("Rovvy", key)
	assert(enc == pt)

	fmt.Println("All tests passed!")
}
