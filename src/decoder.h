#ifndef DECODER_H
#define DECODER_H

#include "includes.h"
#include "file.h"

void decode_instruction(const uint8_t first_byte_of_instruction);
void reg_mem_mov_instruction(const uint8_t first_byte);
void mov_register_to_register(const uint8_t dw, const uint8_t mode, const uint8_t reg, const uint8_t rm); 
void mov_register_memory_mode(const uint8_t dw, const uint8_t mode, const uint8_t reg, const uint8_t rm);
void mov_memory_mode_no_disp(const uint8_t first_byte, const uint8_t mode_reg_rm);
void mov_memory_mode_8_bit_disp(const uint8_t first_byte, const uint8_t mode_reg_rm);
void mov_immediate_to_register_instruction(const uint8_t first_byte);

#endif
