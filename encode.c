#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

//step 1.1 step validation check (used for both encoding and decoding)
Status validation_check(int argc)
{
    if( argc > 2 && argc <= 5 )
    {
	printf("validation successfull\n");
	return e_success;
    }
    else if(argc > 5)
    {
	printf("Too many argument\n");
	return e_failure;
    }
    else
    {
	return e_failure;
    }
}

//step 2.1 check operation type (used for both encoding and decoding )
OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1],"-e") == 0 )
	{	
	    return e_encode;
	}
	else if(strcmp(argv[1],"-d") == 0 )
	{	
	    return e_decode;
	}
	else
	{
	    return e_unsupported;
	}
}

//step 3.1 read and validation for encoding
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".")!=NULL)
    {
    	if(strcmp(strstr(argv[2],"."),".txt") == 0)
    	{
		encInfo->secret_fname=argv[2];
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

    if(strstr(argv[3],".") != NULL)
    {
    	if(strcmp(strstr(argv[3],"."),".bmp") == 0)
    	{
		encInfo->src_image_fname=argv[3];
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

    if(argv[4] != NULL)
    {	
	 if(strstr(argv[4],".") != NULL)
	 {

   	 	if(strcmp(strstr(argv[4],"."),".bmp") == 0)
   	 	{
			encInfo->stego_image_fname=argv[4];
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
    else
	encInfo->stego_image_fname="stego.bmp";

    return e_success;
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

// step 6.3 finding size of src bmp
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

//step 5.1 open files
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

//step 6.1 checking capacity
Status check_capacity(EncodeInfo *encInfo)
{
    //step 6.2 get the beautiful.bmp file size
   encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
   //get the secret.txt file size
   encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
   //check image capacity is capable of storing secret file
   if(encInfo->image_capacity > ( 54 + (2+4+4+encInfo->size_secret_file) * 8 ) )
   {
       return e_success;
   }
   else
   {
       return e_failure;
   }

}

uint get_file_size(FILE *fptr)
{
	fseek(fptr,0,SEEK_END);
	return ftell(fptr);
}

//step 7.1 copying bmp header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	char str[54];
	fseek(fptr_src_image,0,SEEK_SET);
	fread(str,54,1,fptr_src_image);
	fwrite(str,54,1,fptr_dest_image);
	return e_success;
}

//step 8.1 encoding magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    //step 8.2 encoding data to stego image
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

// encoding data to stego image(common function)
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image,EncodeInfo *encInfo)
{
    for(int i=0 ; i < size ; i++)
    {
	fread(encInfo->image_data,8,1,fptr_src_image);
	//encoding 1 byte data into lsb of 8 bytes of stego imgae
	encode_byte_to_lsb(data[i],encInfo->image_data);
	fwrite(encInfo->image_data,8,1,fptr_stego_image);
    }
}

//encoding 1 byte of data into lsb of 8 bytes of stego image(common function)
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;
    unsigned int mask=1<<7;
    for( i = 0 ; i < 8 ; i++ )
    {
	image_buffer[i]=( image_buffer[i] & 0xfe ) | ( (data & mask) >> (7-i) );
	mask = mask >> 1;
    }
}

//step 9.2 encoding size to stego image
Status encode_size(int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
    char str[32];
    fread(str,32,1,fptr_src_image);
    //step 9.3 encoding size in lsb of stego image
    encode_size_to_lsb(str,size);
    fwrite(str,32,1,fptr_stego_image);
    return e_success;
}

//encoding size in lsb of stego image(common function)
Status encode_size_to_lsb(char *buffer,int size)
{
    int i;
    unsigned int mask=1<<31;
    for( i = 0 ; i < 32 ; i++ )
    {
	buffer[i]=( buffer[i] & 0xfe ) | ( ( size & mask) >> (31-i) );
	mask = mask >> 1;
    }
    return e_success;
}

//step 10.1 encoding scret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    	//step 10.2 encode data to image
	encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
	return e_success;
}

//step 11.1 encoding secret file size
Status encode_secret_file_size(unsigned int file_size, EncodeInfo *encInfo)
{
	
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    //step 11.2 encoding size to lsb 
    encode_size_to_lsb(str,file_size);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//step 12.1 encoding secret file data 
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    char str[encInfo->size_secret_file];
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
    //step 12.2 encoding data into stego image
    encode_data_to_image(str,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
    return e_success;
}

//step 13.1 copying remaining data to stego image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src) > 0 )
    {
	fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

//step 4.1 encoding started
Status do_encoding(EncodeInfo *encInfo)
{
	printf("Started Encoding\n");
	//step 5.0 Open the file
	if(open_files(encInfo) == e_success)
	{
	    printf("Open file is successfull\n");
	    //step 6.0 check capacity
	    if(check_capacity(encInfo) == e_success)
	    {
		printf("check capacity is a successfull\n");
		//step 7.0 copy header file
		if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
		{
		    printf("Copied bmp header successfully\n");
		    //step 8.0 encode magic string
		    if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		    {
			printf("Magic string encoded successfully\n");
			//step 9.0 Encode the file extension size
			if(encode_size(strlen( strstr(encInfo->secret_fname,".")),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
			{
			    printf("Encoded secret file extension size\n");
			    //step 10.0 Encode the secret file extension
			    if( encode_secret_file_extn(strstr(encInfo->secret_fname,"."),encInfo) == e_success)
			    {
				printf("Encode secret file extension successfully\n");
				//step 11.0 encode the secret file size
				if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
				{
				    printf("Encoded secret file size successfully\n");
				    //step 12.0 Encode secret file data
				    if( encode_secret_file_data(encInfo)== e_success )
				    {
					printf("Encoded secret file data successfully\n");
					//step 13.0 copy remaining data
					if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
					{
					    printf("Copied remaining bytes successfully\n");
					}
					else
					{
					    printf("Copying failed\n");
					    return e_failure;
					}
				    }
				    else
				    {
					printf("Couldn't encode the file data\n");
					return e_failure;
				    }
				}
				else
				{
				    printf("Falied to encode the file size\n");
				    return e_failure;
				}
			    }
			    else
			    {
				printf("Falied to encode the file extension\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("Failed to encode the file extension size\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("Couldn't encode Magic string\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Couldn't copy the header\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Check capcity falied\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Open file falied\n");
	    return e_failure;
	}
	return e_success;
}
