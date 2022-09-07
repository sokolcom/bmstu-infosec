#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/define.h"
#include "../include/crypto.h"
#include "../include/utils.h"
#include "../include/aes.h"


// 256bit key: 0xD14A98557E9B6838C8A74BB62033E8198914D2B394405F96FC7C4B8F2A5DDCEF 
// 128bit key: 0xD14A98557E9B6838C8A74BB62033E81912FB244FD7F000CFDCEFC2D3FA
const char* AES_KEY_BIN[] = {
    "11010001", "01001010", "10011000", "01010101", \
    "01111110", "10011011", "01101000", "00111000", \
    "11001000", "10100111", "01001011", "10110110", \
    "00100000", "00110011", "11101000", "00011001"};
    // "10001001", "00010100", "11010010", "10110011", 
    // "10010100", "01000000", "01011111", "10010110", 
    // "11111100", "01111100", "01001011", "10001111", 
    // "00101010", "01011101", "11011100", "11101111" };


int32_t _processing(const char *const src, const char *const dst, const enum bool_t mode) {
    FILE *fin = fopen(src, "rb");
    FILE *fout = fopen(dst, "wb");
    if (!fin || !fout) {
        return FILE_NOT_FOUND_ERROR;
    }

    // Obtain filesize
    fseek(fin, 0 , SEEK_END);
    size_t fsize = ftell(fin);
    rewind(fin);

    // Load file (padding bytes - simply zeroes...)
    size_t content_size = fsize + ((fsize % 16) ? (16 - fsize % 16) : 0);
    uint8_t *content = (uint8_t *)calloc(content_size, sizeof(uint8_t));
    if (!content) {
        return ALLOCATION_ERROR;
    }
    fread(content, sizeof(uint8_t), fsize, fin);
    fclose(fin);

    uint128_t block;
    uint128_t private_key = create_key_128bit(AES_KEY_BIN, 2);
    for (size_t i = 0; i < content_size / 16; i++) {

        get_block(&block, content, i);
        uint128_t cipher_block = rijndael_aes(&block, &private_key, mode);
        write_block(fout, &cipher_block);
    }

    free(content);
    fclose(fout);

    return SUCCESS;
}

int32_t encrypt(const char *const src, const char *const dst) {
    return _processing(src, dst, TRUE);
}

int32_t decrypt(const char *const src, const char *const dst) {
    return _processing(src, dst, FALSE);
}