#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../include/define.h"
#include "../include/utils.h"


char *get_filename(const int32_t argc, char **argv) {
    char *filename = NULL;
    if (argc > 1) {
        filename = argv[1];
    } else {
        filename = argv[0];
    }
    return filename;
}

uint128_t create_key_128bit(const char *const *const key_string, const uint8_t base) {
    uint128_t key;
    for (int32_t i = 0; i < 16; i++)
    {
        key.byte[i] = strtoull(key_string[i], NULL, base);
    }
    return key;
}

void get_block(uint128_t *const block, const uint8_t *const content, const size_t idx) {
    for (uint8_t i = 0; i < 16; i++) {
        block->byte[i] = content[idx * 16 + i];
    }
}

void write_block(FILE *const fout, const uint128_t *const block) {
    fwrite(block->byte, sizeof(uint8_t), 16, fout);
}
