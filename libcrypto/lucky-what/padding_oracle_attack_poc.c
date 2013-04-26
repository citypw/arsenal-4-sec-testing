/*
 * This is a simple padding oracle attack Poc example. I just write it
 * for educate myself. Yes, the code looks messy a little bit. I'm a
 * lame coder. Please don't blame it!@#
 *
 * monkey-coders: Shawn the R0ck@copyleft, <citypw@gmail.com>
 *
 * Date: April 26 2013
 *
* This code is GPL'ed!
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

/* 11 bytes data + 5 bytes padding */
static char *data ="Hello World";

static unsigned char key[8] = "mydeskey";
static unsigned char iv[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static unsigned char C[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define EVP_DES_CBC EVP_des_cbc()
#define BLOCK_SIZE 8

EVP_CIPHER_CTX ctx;
int ret, update_len, final_len, i, c_len, total_len, offset = BLOCK_SIZE - 1, padding_num;
unsigned char buf[256], cbc_out[256], C2[16];


void cbc_print(unsigned char *str, int len)
{
	int i;
	
	for( i = 0; i < len; i++)
		printf("%.02x", str[i]);
        printf("\n");
}

void crack_1st_byte()
{

        for(i = 0; i < 255; i++){

                memset(buf, 0x00, sizeof(buf));
                C2[offset] += 1;

                ret = EVP_DecryptUpdate(&ctx, buf, &update_len, C2, total_len);
                if(ret == 0){
                        fprintf(stderr, "EVP_DecryptUpdate() failed!\n");
                        exit(1);
                }

                ret = EVP_DecryptFinal_ex(&ctx, buf + update_len , &final_len);

                if( ret == 1){
                        printf("%d is done: ", i);
                        return;
                }

        }
        printf("failed..\n");
}

int main(int argc, char *argv[])
{
        unsigned char c1[8], c2[8], P[16];
        int i;

        memset(buf, 0x00, sizeof(buf));
        memset(cbc_out, 0x00, sizeof(cbc_out));
	memset(C2, 0x00, sizeof(C2));
	memset(P, 0x00, sizeof(P));

        //        memcpy(plaintext, data, strlen(data));
        //memset(plaintext + strlen(data), 0x05, 5);

        EVP_CIPHER_CTX_init(&ctx);
        ret = EVP_EncryptInit_ex(&ctx, EVP_DES_CBC, NULL, key, iv);
        if(ret == 0){
                fprintf(stderr, "EVP_EncryptInit_ex() failed!\n");
                return -1;
        }

        ret = EVP_EncryptUpdate(&ctx, cbc_out, &update_len, data, strlen(data));
        if(ret == 0){
                fprintf(stderr, "EVP_EncryptUpdate() failed!\n");
                return -1;
        }

        ret = EVP_EncryptFinal_ex(&ctx, cbc_out + update_len, &final_len);

        final_len += update_len;
        printf("Ciphertext is %d bytes: ", final_len);
        for( i =0;i < final_len; i++)
                printf("%.02x", cbc_out[i]);

        memcpy(c1, cbc_out, 8);
        memcpy(c2, cbc_out + 8, 8);

	/* concatenate our own Cipherblock with */
	memcpy(C2 + 8, cbc_out + 8, 8);
        memcpy(C2, C, 8);

        printf("\nDecrypting now\n");
        EVP_DecryptInit_ex(&ctx, EVP_DES_CBC, NULL, key, iv);

        ret = EVP_DecryptUpdate(&ctx, buf, &update_len, C2, final_len);
        c_len = update_len;
        total_len = final_len;

        ret = EVP_DecryptFinal_ex(&ctx, buf + update_len , &final_len);

        /* the 1st byte is important */
        crack_1st_byte();
        cbc_print(C2, 16);
        P[7] = padding_num = 0x01 ^ c1[7] ^ C2[7];

        int xth, j = 0x02, k =0, count = 7;
        for(i = 7; i > 0; i--)
        {
                for( k = offset; k < BLOCK_SIZE; k++)
                {
                        C2[count] = j ^ P[count] ^ c1[count];
                        count--;
                }
                offset--;
                crack_1st_byte();
                cbc_print(C2, 16);
                P[i - 1] = j ^ c1[i - 1] ^ C2[ i - 1];

                j++;
                count = 7;
        }
        printf("The original plaintext is: \"%s\"\n", data);
        printf("The one last block of plaintext: ");
        cbc_print(P, 16);
        printf("----> %s\n", P);

        return 0;
}
