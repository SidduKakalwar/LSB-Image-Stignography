#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>

int main( int argc, char *argv[] ) 
{


// check operation type function defination
	if (argc > 2)
	{
	if (check_operation_type(argv) == e_encode)
	{
		printf("Selected Encoding\n");
		EncodeInfo encInfo;
		if(read_and_validate_encode_args(argv,&encInfo) == e_success)
		{
			printf("read and validate encode id succes\n");
			if ( do_encoding (&encInfo) == e_success)
			{
				printf("Encoding is success\n");
			}
			else 
			{
				printf("Encoding failed \n");
				return 1;
			}

	
		}


		else 
		{
			printf("read and validate encode is not succes\n");
			return 1;
		}
	}
	else if (check_operation_type(argv) == e_decode)
	{
		printf("Selected decoding\n");	
		DecodeInfo decInfo;
		if(read_and_validate_decode_argv(argv,&decInfo) == d_success)
		{
			printf("read and validate decode is succes\n");
			
			if(do_decoding(&decInfo) == d_success)
			{
				printf("Decoding is success\n");
			}
			else
			{
				printf("Decoding is failed\n");
				return 1;
			}
		}
		else
		{
			printf("read and validate decode is failed\n");
			return 1;
		}
	
	}
	else 
		printf("Invalid input!!\nUsagr for decoding \n:./a.out -e beatiful.bmp secrete.txt [stegp.bmp]\n usage for decoding :\n./a.out -d stego.bmp[decode.txt] ");
	}
	else 
	{

		printf("Usage for decoding \nUsagr for decoding \n:./a.out -e beatiful.bmp secrete.txt [stegp.bmp]\n usage for decoding :\n./a.out -d stego.bmp[decode.txt] ");
	}


	return 0;
}


