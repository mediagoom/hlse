// hls.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define BLOCK_LEN   16

int _tmain(int argc, _TCHAR* argv[])
{

	aes_init();


	if (argc != 6 || toupper(*argv[3]) != 'D' && toupper(*argv[3]) != 'E')
	{
		printf("usage: hls in_filename out_filename [d/e] key_file sequence_number\n");
		return 1;
	}

	FILE            *fin = 0, *fout = 0, *fkey = 0;

	if (!(fin = fopen(argv[1], "rb")))   // try to open the input file
	{
		printf("The input file: %s could not be opened\n", argv[1]);
		return 2;
	}

	if (!(fout = fopen(argv[2], "wb")))  // try to open the output file
	{
		printf("The output file: %s could not be opened\n", argv[2]);
		return 3;
	}

	if (!(fkey = fopen(argv[4], "rb")))   // try to open the key file
	{
		printf("The input file: %s could not be opened\n", argv[4]);
		return 4;
	}

	unsigned char key[BLOCK_LEN];

	// read key
	size_t len = fread((char*)key, 1, BLOCK_LEN, fkey);

	if (len != BLOCK_LEN)
	{
		printf("The key file %s has an invalid size\n", argv[4]);
		return 5;
	}

	unsigned char iv[BLOCK_LEN];

	::memset(iv, 0, BLOCK_LEN);

	int sn = atoi(argv[5]);
	
	iv[15] =  sn        & 0x000000FF;
	iv[14] = (sn >> 8)  & 0x000000FF;
	iv[13] = (sn >> 16) & 0x000000FF;
	iv[12] = (sn >> 24) & 0x000000FF;
	
	if (toupper(*argv[3]) == 'E') // encryption in Cipher Block Chaining mode
	{
		aes_encrypt_ctx ctx[1];

		/*
		aes_encrypt_key((unsigned char*)key, key_len, ctx);

		err = encfile(fin, fout, ctx);

		*/

		AES_RETURN ret = aes_encrypt_key(key, BLOCK_LEN, ctx);

		fclose(fkey);

		unsigned long len = 0;

		unsigned char ibuf[BLOCK_LEN];
		unsigned char obuf[BLOCK_LEN];

		len = fread(ibuf, 1, BLOCK_LEN, fin);

		while (len)
		{
			ret = aes_cbc_encrypt(ibuf, obuf, BLOCK_LEN, iv, ctx);
			fwrite(obuf, 1, len, fout);

			len = fread(ibuf, 1, BLOCK_LEN, fin);
		}


		fclose(fout);
		fclose(fin);


	}
	else                         // decryption in Cipher Block Chaining mode
	{
		aes_decrypt_ctx ctx[1];

		//aes_decrypt_key((unsigned char*)key, key_len, ctx);

		AES_RETURN ret = aes_decrypt_key(key, BLOCK_LEN, ctx);

		fclose(fkey);

		unsigned long len = 0;

		unsigned char ibuf[BLOCK_LEN];
		unsigned char obuf[BLOCK_LEN];

		len = fread(ibuf, 1, BLOCK_LEN, fin);

		while (len)
		{
			ret = aes_cbc_decrypt(ibuf, obuf, BLOCK_LEN, iv, ctx);
			fwrite(obuf, 1, len, fout);

			len = fread(ibuf, 1, BLOCK_LEN, fin);
		}

		
		fclose(fout);
		fclose(fin);

	}


	return 0;
}

