#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <unistd.h>

/*
--> MAGIC STRING IS USED AS PASSWORD. FIRST YOU NEED TO ENCODE MAGIC STRING, BECAUSE YOU ARE GOING TO SEND N NUMBER OF FILES. ONCE IF YOU FIND MAGIC
STRING FROM ANY OF THE FILE START ENCODING OTHER FILES. NOTE: FIRST 54 BYTES ARE FILLED WITH HEADER BYTES, FROM THERE SOME BYTES ARE FILLED WITH RGB
DATA. (IT IS KIND OF PASSWORD, BECAUSE WE SEND MULTIPLE FILES TO FIND THAT FILE WE ARE SENDING PASSWORD)

--> 1. source file size
    2. secret file size * 8
    3. Extension size

    check capacity (.bmp,stego.bmp)

    " (src_file_size) >  16+32+(extn size * 8) + 32 + (data size * 8) + 54 + 1"


    first add 54 bytes of header of .bmp file
    encode_magic_string (#*) 16
    encode_extn_size 32
    encode_extn size * 8
    encode_secret_data_size 32
    encode_secret_data (text file data size) size * 8
    copy remaining data
    EOF


*/
int extension_size;
OperationType check_operation_type(char *argv)
{
    // Check argv is -e or not
    // Yes -> return e_encode, NO
    if (strcmp(argv, "-e") == 0)
    {
        return e_encode;
    }
    // Check argv is -d or not
    // Yes-> return e_decode, NO
    else if (strcmp(argv, "-d") == 0)
    {
        return e_decode;
    }
    // return e_unsupported

    else
    {
        return e_unsupported;
    }
}
// Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) // This code was giving segmentation fault if i give only beautiful , improved code is given below
// {
//     // argv[2] is .bmp file or not
//     // IF YES -> Store agrv[2] to structre member GOTO STEP 3, if NO print error message and then return e_failure
//     if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
//     {
//         encInfo->src_image_fname = argv[2];
//     }
//     else
//     {
        
//         printf("STATUS -> ERROR: In 2nd Argument .bmp File Not Exists!!!\n\n");
//         return e_failure;
//     }

//     // check argv[3] is .txt file or not
//     // IF YES -> Store argv[3] to structre member GOTO STEP 5, if NO print error message and then return e_failure
//     if (strcmp(strstr(argv[3], "."), ".txt") == 0)
//     {
//         encInfo->secret_fname = argv[3];
//         strcpy(encInfo->extn_secret_file, ".txt");
//     }
//     else
//     {

//         printf("STATUS -> ERROR: In 3rd Argument .txt File Not Exists!!!\n\n");
//         return e_failure;
//     }
//     // Check argv[4] is passed or not
//     // If passed -> check .bmp or not
//     // IF YES -> Store agrv[4] to structre member GOTO STEP 8, if NO Default name into strucutre member

//     if (argv[4] != NULL)
//     {
//         // Check .bmp or not
//         // Yes-> storing into structure member and return e_success, if  NO print ERROR message and then return e_failure
//         if (strcmp(strstr(argv[4], "."), ".bmp") == 0)
//         {
//             encInfo->stego_image_fname = argv[4];
//         }
//         else
//         {

//             printf("STATUS -> ERROR: In 4th Argument .bmp File Not Exists!!!\n\n");
//             return e_failure;
//         }
//     }
//     else
//     {
//         encInfo->stego_image_fname = "stego.bmp";

//         printf("STATUS -> Third File is Creating by Default File\n\n");
//     }

//     return e_success;
// } 

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check if argv[2] has an extension
    char *src_extension = strstr(argv[2], ".");
    if (src_extension != NULL && strcmp(src_extension, ".bmp") == 0)
    {
        // If it has a valid .bmp extension, store it
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        // If no extension or invalid extension
        printf("STATUS -> ERROR: In 2nd Argument, a .bmp File is Required!!!\n\n");
        return e_failure;
    }

    // Check if argv[3] has an extension
    char *secret_extension = strstr(argv[3], ".");
    if (secret_extension != NULL && strcmp(secret_extension, ".txt") == 0)
    {
        // If valid .txt file, store it
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".txt");
    }
    else
    {
        // If no extension or invalid extension
        printf("STATUS -> ERROR: In 3rd Argument, a .txt File is Required!!!\n\n");
        return e_failure;
    }

    // Check if argv[4] is passed
    if (argv[4] != NULL)
    {
        // Check if argv[4] has a valid .bmp extension
        char *stego_extension = strstr(argv[4], ".");
        if (stego_extension != NULL && strcmp(stego_extension, ".bmp") == 0)
        {
            // If valid .bmp file, store it
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            // If no extension or invalid extension
            printf("STATUS -> ERROR: In 4th Argument, a .bmp File is Required!!!\n\n");
            return e_failure;
        }
    }
    else
    {
        // If no 4th argument is provided, use default name
        encInfo->stego_image_fname = "stego.bmp";
        printf("STATUS -> Third File is Created with Default Name: stego.bmp\n\n");
    }

    return e_success;
}


/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");

        fprintf(stderr, "STATUS -> ERROR: Unable to open file %s\n\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");

        fprintf(stderr, "STATUS -> ERROR: Unable to open file %s\n\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");

        fprintf(stderr, "STATUS -> ERROR: Unable to open file %s\n\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    // write check capacity function

    // Find the size of source file
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    // Find secret file size
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    // Extension file size
    char *extension = strstr(encInfo->secret_fname, ".");

    if (extension != NULL)
    {
        extension_size = strlen(extension);
    }

    // Checking condition for the source file size is greater than below conditions
    if ((encInfo->image_capacity) > 54 + strlen(MAGIC_STRING) + (extension_size) + (extension_size * 8) + (encInfo->size_secret_file) + (encInfo->size_secret_file) * 8)
    {
        return e_success;
    }

    else
    {
        return e_failure;
    }
}

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    // printf("WIDTH = %u\n\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    // printf("HEIGHT = %u\n\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr_file_size)
{
    fseek(fptr_file_size, 0, SEEK_END);
    int size = ftell(fptr_file_size);

    return size;
}

// Function for copying header file of 54 bytes
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    char header[54];

    if (fread(header, 1, 54, fptr_src_image))
    {
        if (fwrite(header, 1, 54, fptr_dest_image))
        {
            return e_success;
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }
}

// Function for Magic string Encoding
Status encode_magic_string(const char *magic_string, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    encode_data_to_image(MAGIC_STRING, fptr_src_image, fptr_stego_image);
    return e_success;
}

// Encoding data to image
Status encode_data_to_image(char *data, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    int data_length = strlen(data);
    // read 8 bytes of data from src file and store into a arr
    for (int i = 0; i < data_length; i++)
    {
        fread(buffer, 1, 8, fptr_src_image);
        // Call function encode_byte_to_lsb(buffer,data[0])
        encode_byte_to_lsb(data[i], buffer);
        // Write 8 bytes of data to destination file
        fwrite(buffer, 1, 8, fptr_stego_image);
        // Repeat the operation data_length times
    }
    return e_success;
}

// Replacing LSB for Encoding
Status encode_byte_to_lsb(char ch, char *buffer)
{
    char res1[8];
    char res2[8];

    // Clear a bit in the arr[0];
    for (int i = 0; i < 8; i++)
    {
        res1[i] = buffer[i] & (~1);
    }

    // Get a bit from ch
    for (int i = 0; i < 8; i++)
    {
        res2[i] = (ch >> i) & 1;
    }

    // Replace the got bit into arr[0]th lsb position
    for (int i = 0; i < 8; i++)
    {
        buffer[i] = res1[i] | res2[i];
    }
    // Repeat the operation 8 times*/
}

// Function for Encoding Secret File Extension Size
Status encode_secret_file_extn_size(int extn_size, FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[32];
    // Read 32 bytes from source file and store into an array

    fread(buffer, 1, 32, fptr_src_image);
    // CALL encode_size_to_lsb(buffer, extn_size);
    encode_size_to_lsb(buffer, extension_size);
    fwrite(buffer, 1, 32, fptr_dest_image);

    return e_success;
}

// Status encode_size_to_lsb(char *buffer, int size)
// {
//     //CLEAR A BIT IN THE ARR[0]
//     //CALL ENCODE_BYTE_TO_LSB
//     //WRITE 8 BYTES OF DATA TO DEST FILE
//     //REPEAT THE OPERATION DATA_LENGTH TIME
//     for (int i = 0; i < 32; i++)
//     {
//         buffer[i] = /*clearing last bit*/ (buffer[i] & 0xfffffffe) | /*getting last bit */ ((size & (1 << (31 - i))) >> (31 - i));
//     }
// } or
// Function to encode the size into the least significant bits (LSB) of the buffer
Status encode_size_to_lsb(char *buffer, int size)
{
    char res1[32];
    char res2[32];

    // Clear the least significant bit (LSB) in the buffer
    for (int i = 0; i < 32; i++)
    {
        res1[i] = buffer[i] & (~1);
    }

    // Get the bits from the size (32-bit integer) and store them in res2
    for (int i = 0; i < 32; i++)
    {
        res2[i] = (size >> (31 - i)) & 1;
    }

    // Replace the cleared LSB in the buffer with the corresponding bit from size
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = res1[i] | res2[i];
    }

    // Repeat the operation for the length of 32 bits (size is 4 bytes = 32 bits)
}

// Function for Encoding Secret File Extension
Status encode_secret_file_extn(char *extn, FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // call encode_data_to_image(extn, file pointers)
    encode_data_to_image(extn, fptr_src_image, fptr_dest_image);
    return e_success;
}

// Function for Encoding Secret Data Size
Status encode_secret_file_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    // printf("%d\n", size);
    char buffer[32];
    fread(buffer, 1, 32, fptr_src_image);
    encode_size_to_lsb(buffer, size);

    fwrite(buffer, 32, 1, fptr_stego_image);
    return e_success;
}

// Function for Encoding Secret Data
Status encode_secret_file_data(FILE *fptr_src_image, FILE *fptr_stego_image, FILE *fptr_secret, int size)
{
    rewind(fptr_secret);
    char ch;
    char buffer[8];
    char res1[8];
    char res2[8];
    for (int i = 0; i < size; i++)
    {
        fread(&ch, 1, 1, fptr_secret);
        fread(buffer, 1, 8, fptr_src_image);

        for (int i = 0; i < 8; i++)
        {
            res1[i] = buffer[i] & (~1);
            res2[i] = (ch >> i) & 1;
            buffer[i] = res1[i] | res2[i];
        }

        fwrite(buffer, 1, 8, fptr_stego_image);
    }

    return e_success;
}
// Function for copying remaining data
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src_image) == 1)
    {
        fwrite(&ch, 1, 1, fptr_stego_image);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    // Call Open file function(encInfo)
    // Check e_success or e_failure
    // e_success -> GOTO step 4, e_failure -> print Error message and return e_failure
    if (open_files(encInfo) == e_success)
    {

        printf("STATUS -> Source Image Opened Successfully!!!\n\n");

        printf("STATUS -> Secret File Opened Successfully!!!\n\n");

        printf("STATUS -> Destination File Opened Successfully!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: FILE CANNOT BE OPENED!!!\n\n");
        return e_failure;
    }

    // call function check_capacity(encInfo)
    // Comparison -> true -> GOTO STEP 6:return e_success, False -> return e_failure
    // e_success -> print msg, GOTO STEP 7, e_failure -> print error msg and return e_failure.
    if (check_capacity(encInfo) == e_success)
    {

        printf("STATUS -> File Capacity has been calculated Successfully!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Checking File Capacity!!!\n\n");
        return e_failure;
    }
    // COPY bmp header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        printf("STATUS -> Successfully Copied Header File!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Copying .bmp header file!!!\n\n");
        return e_failure;
    }

    // ENCODING MAGIC STRING
    if (encode_magic_string(MAGIC_STRING, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        printf("STATUS -> Successfully Encoded Magic String!!!\n\n");
    }
    else
    {

        printf("STATUS -> Encoding Magic String Failed!!!\n\n");
        return e_failure;
    }

    // ENCODING EXTENSION SIZE
    if (encode_secret_file_extn_size(extension_size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        printf("STATUS -> Extension of File Size is Successfully Encoded!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Encoding Extension Size!!!\n\n");
        return e_failure;
    }

    // ENCODING EXTENSION
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        printf("STATUS -> Secret File Extension Successfully Encoded!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Encoding Extension!!!\n\n");
        return e_failure;
    }

    // ENCODING SECRET DATA SIZE
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        // CALL same encoding extension
        printf("STATUS -> Secret File Size is Encoded SuccessFully!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Encoding Secret file size!!!\n\n");
        return e_failure;
    }

    // ENCODING SECRET DATA
    if (encode_secret_file_data(encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo->fptr_secret, encInfo->size_secret_file) == e_success)
    {

        printf("STATUS -> Secret Data Is Encoded Successfully!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Encoding Secret file data!!!\n\n");
        return e_failure;
    }

    // COPYING REMAINING DATA
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {

        printf("STATUS -> Remaining Data Copied Successfuly!!!\n\n");
    }
    else
    {

        printf("STATUS -> ERROR: In Copying Remaining Data!!!\n\n");
        return e_failure;
    }
}
