#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAGIC_STRING_SIZE 2
#define SECRET_EXTN 4
#define SECRET_BUF_SIZE 8

typedef struct _DecodeInfo
{
    /* Stego image Info */
    char *dec_stego_fname;
    char *dec_secret_fname;
    char magic_string[MAGIC_STRING_SIZE];
    FILE *fptr_stego;
    FILE *fptr_secret;
    unsigned char data;
    int secret_extn_size;
    char secret_extn[SECRET_EXTN];
    char secret_data[SECRET_BUF_SIZE];
    uint secret_file_size;

} DecodeInfo;

/*Read and validation Decode args from argv*/
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/*Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/*Get file pointer for i/p and o/p file*/
Status open_file_decode(DecodeInfo *decInfo);

/*store Magic string*/
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo);

/*decode data from stego image */
Status decode_lsb_data(DecodeInfo *decInfo);

/*decode secret file extension*/
Status decode_secret_extn_size(DecodeInfo *decInfo);

/*Decode extension */
Status decode_secret_extn(DecodeInfo *decInfo);

/* Decode secret file size */ 
Status secret_file_size(DecodeInfo *decInfo); 

/* storing file in decode_secret_file */
Status secret_file(DecodeInfo *decInfo); 

#endif
