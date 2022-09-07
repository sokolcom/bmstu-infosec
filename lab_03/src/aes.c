#include <stdio.h>
#include <stdlib.h>

#include "../include/define.h"
#include "../include/aes.h"



const uint32_t AES_LOOKUP_TABLE[] = { \
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, \
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, \
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, \
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, \
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, \
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, \
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, \
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, \
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, \
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, \
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, \
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, \
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, \
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, \
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, \
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint32_t AES_INV_LOOKUP_TABLE[] = { \
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, \
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, \
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, \
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, \
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, \
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, \
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, \
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, \
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, \
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, \
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, \
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, \
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, \
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, \
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, \
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const uint8_t AES_MIXCOL_MATRIX[AES_MATRIX_SIZE][AES_MATRIX_SIZE] = {
    {2, 3, 1, 1},
    {1, 2, 3, 1},
    {1, 1, 2, 3},
    {3, 1, 1, 2}
};

const uint8_t AES_INV_MIXCOL_MATRIX[AES_MATRIX_SIZE][AES_MATRIX_SIZE] = {
    {0xe, 0xb, 0xd, 0x9},
    {0x9, 0xe, 0xb, 0xd},
    {0xd, 0x9, 0xe, 0xb},
    {0xb, 0xd, 0x9, 0xe}
};

const uint32_t AES_KEY_SCHEDULE[] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, \
    0x10, 0x20, 0x40, 0x80, 0x1b, \
    0x36, 0x6c, 0xd8, 0xab, 0x4d
};

uint8_t key_expansion[(AES_ROUNDS_128 + 1) * 16] = { 0 };

////////////////////////////////////////////////////////////////////////////////

static void xor(uint8_t matrix[][AES_MATRIX_SIZE], const uint128_t *const key) {
    for (int32_t col = 0; col < AES_MATRIX_SIZE; col++) {
        for (int32_t row = 0; row < AES_MATRIX_SIZE; row++) {
            matrix[row][col] ^= key->byte[col * AES_MATRIX_SIZE + row];
        }
    }
}

static void rotl(word_t *const word) {
    uint8_t temp = word->byte[0];
    for (uint8_t i = 1; i < AES_MATRIX_SIZE; i++) {
        word->byte[i - 1] = word->byte[i - 1];
    }
    word->byte[AES_MATRIX_SIZE - 1] = temp;
}

void sub_word_bytes(word_t *const word) {
    for (uint8_t i = 0; i < AES_MATRIX_SIZE; i++) {
        uint8_t row = word->byte[i] >> 4;
        uint8_t col = word->byte[i] & 0xF;
        word->byte[i] = AES_LOOKUP_TABLE[row * 16 + col];
    }
}

static void key_schedule(word_t *const word, const uint8_t idx) {
    word->byte[0] ^= AES_KEY_SCHEDULE[idx];
}

void create_key128_expansion(const uint128_t *const key) {
    for (uint8_t i = 0; i < 16; i++) {
        key_expansion[i] = key->byte[i];
    }

    const uint8_t dim = AES_MATRIX_SIZE * AES_MATRIX_SIZE;
    for (uint8_t i = 1; i < (AES_ROUNDS_128 + 1); i++) {
        word_t prev_last_word;
        for (uint8_t j = 0; j < AES_MATRIX_SIZE; j++) {
            prev_last_word.byte[j] = key_expansion[i * dim - AES_MATRIX_SIZE + j];
        }

        rotl(&prev_last_word);
        sub_word_bytes(&prev_last_word);
        key_schedule(&prev_last_word, i);

        for (uint8_t j = 0; j < AES_MATRIX_SIZE; j++) {
            key_expansion[i * dim + j] = key_expansion[(i - 1) * dim + j] ^ prev_last_word.byte[j];
        }

        for (uint8_t j = AES_MATRIX_SIZE; j < dim; j++) {
            key_expansion[i * dim + j] = key_expansion[(i - 1) * dim + j] ^ key_expansion[i * dim + j - AES_MATRIX_SIZE];
        }
    }
}

uint128_t get_round_key(const uint8_t round) {
    uint128_t key;
    for (uint8_t i = 0; i < 16; i++) {
        key.byte[i] = key_expansion[round * 16 + i];
    }

    return key;
}

static uint8_t mult(uint8_t a, uint8_t b) {
    // Multiplication in the Galois field
    // (Russian Peasant Multiplication algorithm)
    uint8_t result = 0x0;
	while (a && b) {
        // If b is odd, then add the corresponding a to p (final product = sum of all a's corresponding to odd b's)
        if (b & 1) {
            result ^= a;
        }
        // Result will overflow => reduce!
        if (a & 0x80) { 
            a = (a << 1) ^ 0x11b; // Irreducible primitive polynomial x^8 + x^4 + x^3 + x + 1 (0b1_0001_1011)  (may vary!)
        } else {
            a <<= 1;
        }
        b >>= 1;
	}
	return result;
}

static void aes_matrix_m12n(
        uint8_t result[][AES_MATRIX_SIZE], 
        const uint8_t matrix_1[][AES_MATRIX_SIZE], uint8_t matrix_2[][AES_MATRIX_SIZE]) {
    for (int32_t i = 0; i < AES_MATRIX_SIZE; i++) {
        for (int32_t j = 0; j < AES_MATRIX_SIZE; j++) {
            uint8_t res = 0x0;
            for (int32_t mul = 0; mul < AES_MATRIX_SIZE; mul++) {
                res ^= mult(matrix_1[i][mul], matrix_2[mul][j]);
            }
            result[i][j] = res;
        }
    }
}

static void copy_matrix(uint8_t dest[][AES_MATRIX_SIZE], uint8_t src[][AES_MATRIX_SIZE]) {
    for (int32_t i = 0; i < AES_MATRIX_SIZE; i++) {
        for (int32_t j = 0; j < AES_MATRIX_SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}


static void prepare_matrix(uint8_t matrix[][AES_MATRIX_SIZE], const uint128_t *const block) {
    // Column-major ordering
    for (int32_t col = 0; col < AES_MATRIX_SIZE; col++) {
        for (int32_t row = 0; row < AES_MATRIX_SIZE; row++) {
            matrix[row][col] = block->byte[col * AES_MATRIX_SIZE + row];
        }
    }
}

static uint128_t yield_block(uint8_t matrix[][AES_MATRIX_SIZE]) {
    uint128_t yielded;

    // Column-major ordering
    for (int32_t col = 0; col < AES_MATRIX_SIZE; col++) {
        for (int32_t row = 0; row < AES_MATRIX_SIZE; row++) {
            yielded.byte[col * AES_MATRIX_SIZE + row] = matrix[row][col];
        }
    }

    return yielded;
}

static void sub_bytes(uint8_t matrix[][AES_MATRIX_SIZE]) {
    for (int32_t row = 0; row < AES_MATRIX_SIZE; row++) {
        for (int32_t col = 0; col < AES_MATRIX_SIZE; col++) {
            uint8_t i = matrix[row][col] >> 4;
            uint8_t j = matrix[row][col] & 0xF;
            matrix[row][col] = AES_LOOKUP_TABLE[i * 16 + j];
        }
    }
}

static void inv_sub_bytes(uint8_t matrix[][AES_MATRIX_SIZE]) {
    for (int32_t row = 0; row < AES_MATRIX_SIZE; row++) {
        for (int32_t col = 0; col < AES_MATRIX_SIZE; col++) {
            uint8_t i = matrix[row][col] >> 4;
            uint8_t j = matrix[row][col] & 0xF;
            matrix[row][col] = AES_INV_LOOKUP_TABLE[i * 16 + j];
        }
    }
}

static void shift_rows(uint8_t matrix[][AES_MATRIX_SIZE]) {
    for (int32_t row = 1; row < AES_MATRIX_SIZE; row++) {
        for (int32_t shift = 0; shift < row; shift++) {
            uint8_t temp = matrix[row][0];
            for (int32_t i = 0; i < AES_MATRIX_SIZE - 1; i++) {
                matrix[row][i] = matrix[row][i + 1];
            }
            matrix[row][AES_MATRIX_SIZE - 1] = temp;
        }
    }
}

static void inv_shift_rows(uint8_t matrix[][AES_MATRIX_SIZE]) {
    for (int32_t row = 1; row < AES_MATRIX_SIZE; row++) {
        for (int32_t shift = 0; shift < row; shift++) {
            uint8_t temp = matrix[row][AES_MATRIX_SIZE - 1];
            for (int32_t i = AES_MATRIX_SIZE - 1; i > 0; i--) {
                matrix[row][i] = matrix[row][i - 1];
            }
            matrix[row][0] = temp;
        }
    }
}

static void mix_columns(uint8_t matrix[][AES_MATRIX_SIZE], const uint8_t mixcol_matrix[][AES_MATRIX_SIZE]) {
    uint8_t result[AES_MATRIX_SIZE][AES_MATRIX_SIZE] = { 0 };
    aes_matrix_m12n(result, mixcol_matrix, matrix);
    copy_matrix(matrix, result);
}

uint128_t rijndael_aes(const uint128_t *const block, const uint128_t *const key, const enum bool_t cipher) {  
    uint8_t state_matrix[AES_MATRIX_SIZE][AES_MATRIX_SIZE] = { 0 };
    prepare_matrix(state_matrix, block);

    create_key128_expansion(key);

    if (cipher) {
        uint128_t round_key = get_round_key(0);
        xor(state_matrix, &round_key);

        for (uint8_t i = 1; i <= AES_ROUNDS_128; i++) {
            sub_bytes(state_matrix);
            shift_rows(state_matrix);
            if (i != AES_ROUNDS_128) {
                mix_columns(state_matrix, AES_MIXCOL_MATRIX);
            }
            round_key = get_round_key(i);
            xor(state_matrix, &round_key);
        }
    } else {
        uint128_t round_key = get_round_key(AES_ROUNDS_128);
        xor(state_matrix, &round_key);

        for (int8_t i = AES_ROUNDS_128 - 1; i >= 0; i--) {
            inv_shift_rows(state_matrix);
            inv_sub_bytes(state_matrix);
            round_key = get_round_key(i);
            xor(state_matrix, &round_key);
            if (i != 0) {
                mix_columns(state_matrix, AES_INV_MIXCOL_MATRIX);
            }
        }
    }

    uint128_t processed_block = yield_block(state_matrix);
    return processed_block;
}