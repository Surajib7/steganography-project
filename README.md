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

./steganography encode input_image.bmp secret_message.txt output_image.bmp
./steganography decode output_image.bmp extracted_message.txt

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
Status encode_magic_string(const char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image);
Status encode_secret_file_extn_size(int extn_size, FILE *fptr_src_image, FILE *fptr_dest_image);
Status encode_secret_file_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image);
Status encode_secret_file_data(FILE *fptr_src_image, FILE *fptr_stego_image, FILE *fptr_secret, int size);
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image);
Status decode_magic_string(FILE *fptr_stego_image);
Status decode_secret_file_size(FILE *fptr_stego_image, int *size);
Status decode_secret_file_data(FILE *fptr_stego_image, FILE *fptr_output, int size);

