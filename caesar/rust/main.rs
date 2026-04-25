fn main() {
    let plain_text = "Hello world";
    let key: u8 = 1;
    let enc = encrypt(&plain_text, key);
    println!("Plaintext: {plain_text}");
    println!("Encrypted: {enc}");
}

fn encrypt(text: &str, key: u8) -> String {
    text.chars()
        .map(|c| {
            if c.is_ascii_alphabetic() {
                let base = if c.is_ascii_lowercase() { b'a' } else { b'A' };
                let enc_c = ((c as u8 - base + key) % 26) + base;
                enc_c as char
            } else {
                c
            }
        })
    .collect()
}
