import sys


def main():
    args = sys.argv
    # Makesure enough arguments are provided
    if len(args) != 4:
        print(f"Usage: {args[0]} <input> <output> <key>")
        return 1

    # Try to read fro input file
    try:
        with open(args[1]) as source:
            text = source.read()
    except FileNotFoundError:
        print(f"File {args[1]} not found. Makesure you've entered correct name")
        return 1

    if len(text) == 0:
        print("Input file cannot be empty.")
        return 1

    # Opening/creating output file
    out = open(args[2], "w")

    # Getting key
    try:
        key = int(args[3])
    except ValueError:
        print("Invalid key.")
        out.close()
        return 1

    # Encrypt text
    enc = encrypt(text, key)

    # Show the user plain text and encrypted text
    print(f"Plaintext:\n\t{text}")
    print(f"Encrypted:\n\t{enc}")

    # Writing to the output file and closing it
    out.write(enc)
    out.close()
    return 0


def encrypt(text, key):
    # Setting up some vriables
    ascii_A = ord("A")
    ascii_a = ord("a")
    enc = []
    # Going through each character in text
    for c in text:
        # If character is uppercase
        if c.isupper():
            ap = (((ord(c) - ascii_A) + key) % 26) + ascii_A
            enc.append(chr(ap))
        # If character is lowercase
        elif c.islower():
            ap = (((ord(c) - ascii_a) + key) % 26) + ascii_a
            enc.append(chr(ap))
        # If character isn't alphabet
        else:
            enc.append(c)
    # Join list into a string and return
    return "".join(enc)


# Calling the main function
if __name__ == "__main__":
    main()
