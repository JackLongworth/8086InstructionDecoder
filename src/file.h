#ifndef FILE_H
#define FILE_H

#include "includes.h"

void init_file_io(char* src_name, char* dest_name);
uint8_t *read_instructions_from_file(size_t *file_size_out);
void write_mov_reg_mem_instruction(const char *dest, const char *src);
void write_mov_w_immediate_to_reg_instruction(const char *dest, const uint16_t immediate);
void write_mov_b_immediate_to_reg_instruction(const char *dest, const uint8_t immediate);

void cleardown_file_io();

#endif
