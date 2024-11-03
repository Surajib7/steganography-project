# Steganography Project

## Project Overview
This project demonstrates a steganography technique to hide secret data within BMP images using the Least Significant Bit (LSB) method. It provides functionality to encode secret text into an image and decode the text from an image.

## How It Works
The project uses the LSB method to modify the least significant bits of the image data to embed the secret message. The process includes:

- **Encoding**:
  - Copy the BMP header from the source image.
  - Embed a magic string to indicate the presence of hidden data.
  - Encode the size and data of the secret file.
  - Copy the remaining image data.

- **Decoding**:
  - Read the BMP header and detect the magic string.
  - Decode the size and extract the secret data.

## Features
- Encode and hide a text file inside a BMP image.
- Decode and extract the hidden text from a stego BMP image.
- Command-line interface for user interaction.

## Prerequisites
- C compiler (e.g., GCC)
- BMP image file for encoding
- Text file containing the secret message

## Setup and Compilation
To compile the project, use:
```bash
gcc steganography.c -o steganography
