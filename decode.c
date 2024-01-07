#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

//step 3.1 read and validate decoding
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
	if(strstr(argv[2],".") != NULL)
	{
    		if(strcmp(strstr(argv[2],"."),".bmp") == 0)
		{
	    		decInfo->dec_stego_fname=argv[2];
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
	
	if(argv[3] != NULL)
	{
	    if(strstr(argv[3],".") != NULL )
	    {
	     	if(strcmp(strstr(argv[3],"."),".txt") == 0)
	    	{
			decInfo->dec_secret_fname=argv[3];
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
	{
	   decInfo->dec_secret_fname="decode_secret.txt";
	}

	return e_success;
}

//step 5.1 opening file
Status open_file_decode(DecodeInfo *decInfo)
{	
    	//stego image
	decInfo->fptr_stego=fopen(decInfo->dec_stego_fname,"r");
	//Do Error handling
	if(decInfo->fptr_stego == NULL)
	{
	    perror("fopen");
	    fprintf(stderr,"ERROR: Unable to open file %s \n",decInfo->dec_stego_fname);
	    return e_failure;
	}

	//decoded secret file
	decInfo->fptr_secret=fopen(decInfo->dec_secret_fname,"w");
	//Do Error handling
	if(decInfo->fptr_secret == NULL)
	{
	    perror("fopen");
	    fprintf(stderr,"ERROR: Unable to open file %s \n",decInfo->dec_secret_fname);
	    return e_failure;
	}
	
	//No failure return e_success 
	return e_success;
}

//step 6.1 decoding magic string
Status decode_magic_string(const char *magic_str,DecodeInfo *decInfo)
{
    int size=strlen(magic_str);
    fseek(decInfo->fptr_stego,54,SEEK_SET);
    for(int i=0;i<size;i++)
    {	
	fread(decInfo->secret_data,8,1,decInfo->fptr_stego);
	//step 6.2 read lsb from 8 bytes of stego.bmp
	decode_lsb_data(decInfo);
	decInfo->magic_string[i]=decInfo->data;	
    }
	
    if(strcmp(decInfo->magic_string,magic_str)==0)
    {
	printf("Magic string :\"%c%c\"\n",decInfo->magic_string[0],decInfo->magic_string[1]);
	return e_success;
    }

    return e_failure;
}

//decoding 8 bytes lsb used for all decoding function
Status decode_lsb_data(DecodeInfo *decInfo)
{
    decInfo->data=0;
    for(int i=0 ;i < 8 ;i++)
    {
	decInfo->data = decInfo->data<<1;
	decInfo->data = decInfo->data | (decInfo->secret_data[i] & 0x01);
    }
    return e_success;
}

//step 7.1 decoding secret extn size
Status decode_secret_extn_size(DecodeInfo *decInfo)
{
    	decInfo->secret_extn_size=0;
	for(int i = 0 ; i < (sizeof(int));i++)
	{
	    	decInfo->secret_extn_size= decInfo->secret_extn_size<<8;
		fread(decInfo->secret_data,8,1,decInfo->fptr_stego);
		//step 7.2 read lsb from 8 bytes of stego.bmp
		decode_lsb_data(decInfo);
		decInfo->secret_extn_size |= decInfo->data;
	}	    
	printf("Secret File Extn size : %d\n",decInfo->secret_extn_size);
	return e_success;
}

//step 8.1 decoding secret extension
Status decode_secret_extn(DecodeInfo *decInfo)
{
    for(int i=0 ; i < decInfo->secret_extn_size ; i++ )
    {	
	fread(decInfo->secret_data,8,1,decInfo->fptr_stego);
	//step 8.2 read lsb from 8 bytes of stego.bmp
	decode_lsb_data(decInfo);
	decInfo->secret_extn[i]=decInfo->data;
    }
	printf("Extension is : ");
    for(int i=0 ; i < decInfo->secret_extn_size ; i++ )
    {
	printf("%c",decInfo->secret_extn[i]);
    }
    printf("\n");

    return e_success;
}

//step 9.1 decoding secret file size
Status secret_file_size(DecodeInfo *decInfo)
{

    	decInfo->secret_file_size=0;
	for(int i = 0 ; i < (sizeof(int));i++)
	{
	    	decInfo->secret_file_size = decInfo->secret_file_size<<8;
		fread(decInfo->secret_data,8,1,decInfo->fptr_stego);
		//step 9.2 read lsb from 8 bytes of stego.bmp
		decode_lsb_data(decInfo);
		decInfo->secret_file_size |= decInfo->data;
	}	    
	printf("Secret File size : %u\n",decInfo->secret_file_size);
	return e_success;
}

//step 10.1 decoding secret file and storing in new file
Status secret_file(DecodeInfo *decInfo)
{
        char ch;
	for(int i = 0 ; i < decInfo->secret_file_size ;i++)
	{
		fread(decInfo->secret_data,8,1,decInfo->fptr_stego);
		//step 10.2 read lsb from 8 bytes of stego.bmp
		decode_lsb_data(decInfo);
		ch= decInfo->data;
		fwrite(&ch,1,1,decInfo->fptr_secret);
	}	    

   
    return e_success;
}

//step 4.1 do decoding
Status do_decoding(DecodeInfo *decInfo)
{
	printf("Started Decoding\n");
	//step 5 opening file
	if(open_file_decode(decInfo)==e_success)
	{
		printf("Open file for decoding successfull\n");
		//step 6 decoding magic string	
		if( decode_magic_string(MAGIC_STRING,decInfo) == e_success )
		{
			printf("Magic string found successfuly\n");
			//step 7 decoding secret extn size
			if(decode_secret_extn_size(decInfo)==e_success)
			{
			    printf("Extension size decoding successfull\n");
			    //step 8 decoding secret extn
			    if(decode_secret_extn(decInfo) == e_success)
			    {
				printf("Secret file Extension decoding successfull\n");
				//step 9 decode secret file size
				if( secret_file_size(decInfo) == e_success )
				{
					printf("Secret file size decoding successfull\n");
					//step 10 decode secret file
					if(secret_file(decInfo) == e_success)
					{
					    printf("Decoded and stored secret data in file successfully\n");
					}
					else
					{
					    printf("Decoded and stored secret data  failed\n");
					    return e_failure;
					}

				}
				else
				{
					printf("Secret file size decoding successfull\n");
					return e_failure;
				}
			    }
			    else
			    {
				printf("Secret file Extension decoding failed\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("Extension size decoding failed\n");
			    return e_failure;
			}
		}
		else
		{
		    printf("Magic string not found\n");
		    return e_failure;
		}

	}
	else
	{
		printf("Open file for decoding failed\n");
		return e_failure;
	}

	//No failure return e_success
	return e_success;
}
