#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* function defination */

Status read_and_validate_decode_argv(char *argv[], DecodeInfo *decInfo)
{
	if( strstr(argv[2], ".bmp"))
	{
		decInfo -> src_image_fname = argv[2];
	}
	else
	{
		return d_failure;
	}

	decInfo -> secret_fname = malloc(12);
	if (argv[3])
	{
		strcpy(decInfo -> secret_fname, argv[3]);
	}
	else
	{

		strcpy(decInfo -> secret_fname, "output");
	}
	return d_success;
}

/* Start decoding */

Status do_decoding(DecodeInfo *decInfo)
{
	printf("## Decoding Procedur Started ##\n");

	if (open_img_file(decInfo) == d_success)
	{
		printf("INFO :Opening required file successfull\n");
		printf("INFO :Opening stego_beatiful.bmp\n");
		printf("INFO :Done \n");

		printf("Opened %s\n", decInfo -> src_image_fname);
		if (decode_magic_string(decInfo -> fptr_src_image) == d_success)
		{

			printf("INFO : Decoding Magic String Signature is done successfull\n");
			printf("INFO : Done \n");
			if (decode_secret_file_extn_size(decInfo) == d_success)
			{
				printf("INFO : Decoding output File Extenstion \n");
				printf("Opened decode.txt\n");
				if (decode_secret_file_extn(decInfo) == d_success)
				{ 
					printf("INFO : opened all files successful\n");
					if (decode_file_data_size(decInfo) == d_success)
					{
						printf("INFO : Decoding decoded file.txt File size\n");
						printf("Done\n");

						if (decode_file_data(decInfo) == d_success)
						{ 

						printf("Decoding decoded file.txt File data\n");
						printf("Done\n");

						printf("##Decoding Done succesfull##\n");

						}
						else
						{
							printf("Decoding secret file data is  failed\n");
							return d_failure;
						}
					}
					else
					{
						printf("Decoding secret file data size is failed\n");
						return d_failure;
					}
				}  
				else
				{   
					printf("Decoding secret file extn is failed\n");
					return d_failure;
				}
			}
			else
			{
				printf("Decoding secret file extn size failed\n");
				return d_failure;
			}
		}
		else
		{
			printf("Decoding magic string  failed\n");
		}
	}
	else
	{
		printf("opening file is failed\n");
		return d_failure;
	}
	return d_success; 
}

//opening encoded source image
Status open_img_file(DecodeInfo *decInfo)
{
	decInfo -> fptr_src_image = fopen(decInfo -> src_image_fname, "r");   
	if ( decInfo -> fptr_src_image == NULL )
	{
		perror("fopen");
		fprintf(stderr, "ERROR : Unable to open file %s\n", decInfo -> src_image_fname);
		return d_failure;
	}
	return d_success;
}
// decode magic string from encoded file and compare its currect or not
Status decode_magic_string(FILE *fptr_image)
{
	char buffer[8];
	char *ch = malloc(strlen(MAGIC_STRING));
	fseek(fptr_image, 54, SEEK_SET);

	for ( int i = 0 ; i < strlen(MAGIC_STRING) ; i++ )
	{
		fread(buffer, 8, 1, fptr_image);
		ch[i] = decode_byte_from_lsb(buffer);
	}

	if (strcmp(ch, MAGIC_STRING) == 0)
	{
		return d_success;
	}
	else
	{
		return d_failure;
	}
}

char decode_byte_from_lsb(char *buffer)
{
	char ch = 0;
	//running loop 8 time 
	for( int i = 0 ; i < 8 ; i++ )
	{
		//retrieving the lsb bit,moving and storing in ch
		ch = (buffer[i] & 1) | (ch << 1);
	}
	return ch;
}
//decode encoded source file extn size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char buffer[32];
	//reading 32 bytes from source image and storing into buffer
	fread(buffer, 32, 1, decInfo -> fptr_src_image);
	//getting size and storing in the structure member
	decInfo -> size_extn = decode_size_from_lsb(buffer);
	return d_success;
}

long decode_size_from_lsb(char *buffer)
{
	long size = 0;

	for (int i = 0 ; i < 32 ; i++)
	{
		size = (buffer[i] & 1) | (size << 1);
	}	
	return size;
}
//decode extention of encoded source file
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	char buffer[8];

	for (int i = 0 ; i < decInfo -> size_extn ; i++)
	{
		fread(buffer, 8, 1, decInfo -> fptr_src_image);
		decInfo -> extn_secret_file[i] = decode_byte_from_lsb(buffer);
	}

	printf(" Creating %s%s file\n", decInfo -> secret_fname, decInfo -> extn_secret_file);
	//opening new file for decode and stored in output.txt
	decInfo -> fptr_secret = fopen(strcat(decInfo -> secret_fname, (char *)decInfo -> extn_secret_file), "w");

	printf("Opened %s\n", decInfo -> secret_fname);
	if ( decInfo -> fptr_secret == NULL )
	{
		perror("fopen");
		fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo -> secret_fname);
		return d_failure;
	}
	return d_success;
}
//output.txt size
Status decode_file_data_size(DecodeInfo *decInfo)
{
	char buffer[32];
	fread(buffer, 32, 1, decInfo -> fptr_src_image);
	decInfo -> size_secret_file = decode_size_from_lsb(buffer);
	return d_success;
}
//output.txt is stored data
Status decode_file_data(DecodeInfo *decInfo)
{
	char buffer[8], ch;

	for (int i = 0 ; i < decInfo -> size_secret_file - 1 ; i++)
	{
		fread(buffer, 8, 1, decInfo -> fptr_src_image);
		ch = decode_byte_from_lsb(buffer);
		fwrite(&ch, 1, 1, decInfo -> fptr_secret);
	}
	ch = '\n';
	fwrite(&ch, 1, 1, decInfo -> fptr_secret);

	return d_success;
}

