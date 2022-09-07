#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

char *get_filename(const int32_t argc, char **argv);
uint128_t create_key_128bit(const char *const *const key_string, const uint8_t base);

void get_block(uint128_t *const block, const uint8_t *const content, const size_t idx);
void write_block(FILE *const fout, const uint128_t *const block);

#endif