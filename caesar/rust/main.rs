// Command to run this: `$ rustc main.rs && ./main`

use std::fs::File;
use std::io::prelude::*;

#[allow(unused_assignments)]
fn main() -> std::io::Result<()> {
    let mut ret = Ok(());
    let mut src = File::open("../plain_text.txt")?;
    let mut content = String::new();
    src.read_to_string(&mut content)?;
    let mut key = Default::default();
    println!("Enter key: ");
    std::io::stdin()
        .read_line(&mut key)
        .expect("Couldn't process input!");
    let key: u8 = key.trim().parse().expect("The key must be an integer");
    let key = key % 26;
    let enc = encrypt(&content, key);
    println!("Plaintext: {content}");
    println!("Encrypted: {enc}");
    let mut out = File::create("out.txt")?;
    ret = out.write_all(enc.as_bytes());
    ret
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
