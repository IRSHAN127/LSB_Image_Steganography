#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main( int argc,char *argv[] )
{
    //step 1 validate the CLA 
    if(validation_check(argc) == e_success)
    {
    	//step 2 check operation type
    	if(check_operation_type(argv) == e_encode)
    	{
		printf("Selected Encoding\n");
		//step 3 Read and validate the CLA for decoding
		EncodeInfo encInfo;
		if(read_and_validate_encode_args(argv,&encInfo) == e_success )
		{
	    		printf("Read and validate of encode is  successfull\n");
	    		//step 4 Start Encoding
	    		if(do_encoding(&encInfo) == e_success )
	    		{
				printf("Encoding is successfull\n");
	    		}
	    		else
	    		{
				printf("Encoding Failed\n");
				return 1;
	    		}
		}
		else
		{
	    		printf("Read and Validate is failed\nPlease pass\n\t\t./a.out -e secret.txt beautiful.bmp\n\t\tor\n\t\t");
			printf("./a.out -e secret.txt beautiful.bmp stego<filename>.bmp\n");
	    		return 1;
		} 
    	}
    	else if(check_operation_type(argv)==e_decode)
    	{
	        //step 2 check operation type
		printf("Selected Decoding\n");
		//step 3 read and validation of CLA for decoding
		DecodeInfo decInfo;
		if(read_and_validate_decode_args(argv,&decInfo) == e_success )
		{
	    		printf("Read and validate of decode is  successfull\n");
			//step 4 start decoding
			if(do_decoding(&decInfo) == e_success)
			{
			    printf("Decoding is succesfull\n");
			}
			else
			{
				printf("Decoding failed\n");
				return 1;
			}
		}
		else
		{
		    	printf("Read and Validate is falied\nPlase pass\n\t\t./a.out -d stego.bmp\n\t\tor\n\t\t./a.out -d stego.bmp secret<filename>.txt\n");
			return 1;
		}

    	}
    	else
    	{
		printf("invalid input\nEncoding :\n\t\t./a.out -e secret.txt beautiful.bmp\n\t\t");
		printf("or\n\t\t./a.out -e secret.txt beautiful.bmp stego<filename>.bmp\n\n");
		printf("Decoding:\n\t\t./a.out -d stego.bmp\n\t\t");
		printf("or\n\t\t./a.out -d stego.bmp secret<filename>.txt\n");
		return 1;
    	}

    }
    else
    {
		printf("invalid input\nEncoding :\n\t\t./a.out -e secret.txt beautiful.bmp\n\t\t");
		printf("or\n\t\t./a.out -e secret.txt beautiful.bmp stego<filename>.bmp\n\n");
		printf("Decoding:\n\t\t./a.out -d stego.bmp\n\t\t");
		printf("or\n\t\t./a.out -d stego.bmp secret<filename>.txt\n");
		return 1;
    }
    return 0;
}

