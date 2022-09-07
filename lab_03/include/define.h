#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <stdint.h>
#include <stdlib.h>


#define SUCCESS 0
#define OPTION_ERROR 5
#define N_ARGS_ERROR 10
#define FILE_NOT_FOUND_ERROR 20
#define N_TICKETS_ERROR 30
#define ALLOCATION_ERROR 40
#define OPENSSL_ERROR 50

#define MAX_STRING_LENGTH 128
#define MAX_FILENAME_LENGTH 256
#define N_ARGS 7
#define BITS_PER_BYTE 8

#define AES_MATRIX_SIZE 4  // 4x4 matrix of 128bits (each cell = 1 byte)
#define AES_ROUNDS_128 10
#define AES_ROUNDS_256 14


    
enum bool_t {
    FALSE = 0,
    TRUE
};

typedef struct {
    uint8_t byte[16];
} uint128_t;


typedef struct {
    uint8_t byte[32];
} uint256_t;


typedef struct {
    uint8_t byte[AES_MATRIX_SIZE];
} word_t;


#endif