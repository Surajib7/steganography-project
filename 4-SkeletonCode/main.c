#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{

    int op_type = check_operation_type(argv[1]);
    // Check op_type == e_encode or not
    // Yes -> Start Our Encoding part, No-> STEP 3
    if (op_type == e_encode)
    {
        if (argc > 3)
        {
            
            printf("STATUS -> You opted for Encoding!!!\n\n");
            
            // Creating structure variable
            EncodeInfo encInfo;
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                // Proceed to next
                // call function do_encoding
                
                printf("STATUS -> Read and Validated Successfully!!!\n\n");
                do_encoding(&encInfo);
            }
            else
            {
                
                printf("STATUS -> ERROR: Read and Validation Failed!!!\n\n");
                return 0;
            }
        }
        else
        {
            
            printf("INFO : Please Pass Valid Arguments.\n");
            
            printf("INFO : Encoding - minimum 4 Arguments\n");
            
            printf("INFO : Decoding - minimum 3 Arguments\n");
            
            return 0;
        }
    }

    // Check op_type is e_decode or not
    // Yes-> Start decoding part,
    else if (op_type == e_decode)
    {
        if (argc > 2)
        {
            printf("STATUS -> You opted for Decoding!!!\n\n");
            // Creating structures variable
            DecodeInfo decoInfo;
            if (read_and_validate_decode_args(argv, &decoInfo) == e_success)
            {
                // Poceed to next
                // call function do_decoding()
                
                printf("STATUS -> Read and Validated Successfully!!!\n\n");
                do_decoding(&decoInfo);
            }
            else
            {
                
                printf("STATUS -> ERROR: Read and Validation Failed!!!\n\n");
                return 0;
            }
        }
        else
        {
            
            printf("INFO : Please Pass Valid Arguments.\n");
            
            printf("INFO : Encoding - minimum 4 Arguments\n");
            
            printf("INFO : Decoding - minimum 3 Arguments\n");
            
            return 0;
        }
    }
    // Print the error message and terminate the program (return e_failure)
    else
    {
        
        printf("STATUS -> ERROR: ENTER VALID COMMAND\n\n");
        return 0;
    }
}

