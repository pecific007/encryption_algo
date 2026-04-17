#!/usr/bin/env python
import sys


class fileInput:
    def __init__(self, key, text):
        self.key = key
        self.text = text

    def encrypt(self):
        alpha = {chr(ord("A") + i): self.key[i] for i in range(len(self.key))}
        enc = []

        for t in self.text:
            if t.isupper():
                t = alpha[t]
            elif t.islower():
                t = alpha[t.upper()].lower()
            enc.append(t)

        return "".join(enc)


def main():
    args = sys.argv
    if len(args) == 2 and args[1] == "test":
        print("Testing: Encryption and Decryption...")
        test()
        exit(0)
    if len(args) != 3:
        sys.stderr.write(f"Usage: {args[0]} <input> <output>")
        sys.exit(1)

    try:
        with open(args[1]) as source:
            data = source.read()
    except FileNotFoundError:
        sys.stderr.write(
            f"File {args[1]} couldn't be oepend. Make sure you typed file name correctly"
        )
        sys.exit(1)

    file, err = parse_input_file(data)
    if err:
        sys.exit(1)

    enc = file.encrypt()

    # Showing the user plain text and encrypted text
    print(f"Plaintext: {file.text}")
    print(f"Encrypted: {enc}")

    with open(args[2], "w") as out:
        out.write(enc + "\n")


def input_format():
    message = "Input file format:\n \
    KEY ABCDEFGHIJKLMNOPQRSTUVWXYZ\n \
    Some text to encrypt...\n"
    sys.stderr.write(message)


def parse_input_file(data):
    keyword = "KEY "
    lines = data.splitlines()
    if lines[0].startswith(keyword):
        lines[0] = lines[0][len(keyword) :]
    else:
        sys.stderr.write(f"Couldn't find keyword '{keyword}' in input file.\n")
        input_format()
        return fileInput("", ""), True

    key = lines[0]
    if len(key) != 26:
        sys.stderr.write("The key must be 26 characters.")
        input_format()
        return fileInput("", ""), True

    text = lines[1]

    ip = fileInput("".join(key), text)
    return ip, False

def test():
    text = "Hello"
    results = [ "Ifmmp", "Uryyb", "Rovvy", ]
    enc_key = [
        "BCDEFGHIJKLMNOPQRSTUVWXYZA",
        "NOPQRSTUVWXYZABCDEFGHIJKLM",
        "KLMNOPQRSTUVWXYZABCDEFGHIJ",
    ]
    dec_key = [
        "ZABCDEFGHIJKLMNOPQRSTUVWXY",
        "NOPQRSTUVWXYZABCDEFGHIJKLM",
        "QRSTUVWXYZABCDEFGHIJKLMNOP",
    ]

    ''' ---------- Encrypt ----------'''
    pt = fileInput("", text)
    for i in range(len(enc_key)):
        pt.key = enc_key[i]
        enc = pt.encrypt()
        assert enc == results[i], f"Enc: Error on key[{i}]"

    ''' ---------- Decrypt ----------'''

    for i in range(len(dec_key)):
        enc = pt.encrypt()
        pt.key = dec_key[i]
        pt.text = results[i];
        enc = pt.encrypt()
        assert enc == text, f"Dec: Error on key[{i}]"

    print("All tests passed!")

if __name__ == "__main__":
    main()
