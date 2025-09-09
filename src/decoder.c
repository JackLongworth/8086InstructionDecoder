#include "decoder.h"

#define REG_MEM_TO_FROM_REG 0b10001000
#define IMMEDIATE_TO_REG 0b10110000

static const char *BYTE_REGS[8] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
static const char *WORD_REGS[8] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };

static const char *MEMORY_ACCESS_CALCS[8] = { "bx + si", "bx + di", "bp + si","bp + di", "si", "di", "bp", "bx" }; 

uint8_t *instruction_bytes = NULL;
size_t instruction_bytes_size = 0;
size_t byte_index = 0;

int main(int argc, uint8_t* argv[]) {
  init_file_io(argv[1], argv[2]);
  instruction_bytes = read_instructions_from_file(&instruction_bytes_size);

  while (byte_index < instruction_bytes_size) {
    uint8_t first_byte_of_instruction = instruction_bytes[byte_index++];
    decode_instruction(first_byte_of_instruction);
  }

  cleardown_file_io();
  free(instruction_bytes);

  return 0;
}

void decode_instruction(uint8_t first_byte) {
  const uint8_t opcode_6_bit = first_byte & 0b11111100;
  const uint8_t opcode_4_bit = first_byte & 0b11110000;

  switch (opcode_6_bit) {
    case REG_MEM_TO_FROM_REG:
      reg_mem_mov_instruction(first_byte);
      break;
  }

  switch (opcode_4_bit) {
    case IMMEDIATE_TO_REG:
      mov_immediate_to_register_instruction(first_byte);
      break;
  }
}

void reg_mem_mov_instruction(const uint8_t first_byte) {
  int8_t second_byte = instruction_bytes[byte_index++];

  static const uint8_t dw_mask = 0b00000011;

  static const uint8_t mode_mask = 0b11000000;
  static const uint8_t register_mask = 0b00111000;
  static const uint8_t rm_mask = 0b00000111;

  const uint8_t dw = first_byte & dw_mask;
  const uint8_t mode = second_byte & mode_mask;
  const uint8_t reg = second_byte & register_mask;
  const uint8_t rm = second_byte & rm_mask;

  if (mode == 0b11000000) {
    mov_register_to_register(dw, mode, reg, rm);
  }
  else {
    mov_register_memory_mode(dw, mode, reg, rm);
  }
}

void mov_register_to_register(const uint8_t dw, const uint8_t mode, const uint8_t reg, const uint8_t rm) {

  const char *reg_register, *rm_register;
  const char *src, *dest;

  const uint8_t reg_index = reg >> 3;

  if (dw & 0b00000001) {
    reg_register = WORD_REGS[reg_index];
    rm_register = WORD_REGS[rm];
  } 
  else {
    reg_register = BYTE_REGS[reg_index];
    rm_register = BYTE_REGS[rm];
  }

  if (dw & 0b00000010) {
    src = rm_register;
    dest = reg_register;
  }
  else {
    src = reg_register;
    dest = rm_register;
  }

  write_mov_reg_mem_instruction(dest, src);
}

void mov_register_memory_mode(const uint8_t dw, const uint8_t mode, const uint8_t reg, const uint8_t rm) {
  const uint8_t reg_index = reg >> 3;
  
  const char *reg_name, *memory_access_calculation;
  const char *dest, *src;

  memory_access_calculation = MEMORY_ACCESS_CALCS[rm];
  char mac_display[128];

  switch(mode) {
    case 0b00000000:
      sprintf(mac_display, "[%s]", memory_access_calculation);
      break;
    case 0b01000000:
      uint8_t disp_8_bit = instruction_bytes[byte_index++];
      if (disp_8_bit) {
        sprintf(mac_display, "[%s + %d]", memory_access_calculation, disp_8_bit);
      } 
      else {
        sprintf(mac_display, "[%s]", memory_access_calculation);
      }
      break;
    case 0b10000000:
      uint16_t disp_16_bit = instruction_bytes[byte_index++] | instruction_bytes[byte_index++] << 8;
      if (disp_16_bit) {
        sprintf(mac_display, "[%s + %d]", memory_access_calculation, disp_16_bit);
      }
      else {
        sprintf(mac_display, "[%s]", memory_access_calculation);
      }
      break;
  }
  
  if (dw & 0b00000001) {
    reg_name = WORD_REGS[reg_index];
  }
  else {
    reg_name = BYTE_REGS[reg_index];
  }

  if (dw & 0b00000010) {
    src = mac_display;
    dest = reg_name;
  }
  else {
    src = reg_name;
    dest = mac_display;
  }

  write_mov_reg_mem_instruction(dest, src);
}

void mov_immediate_to_register_instruction(const uint8_t first_byte) {
  const uint8_t wide_instruction_mask = 0b00001000;
  const uint8_t register_mask = 0b00000111;

  const uint8_t register_index = first_byte & register_mask;
const char *reg;

  if (first_byte & wide_instruction_mask) {
    reg = WORD_REGS[register_index];
    uint16_t data = instruction_bytes[byte_index++] | instruction_bytes[byte_index++] << 8;
    write_mov_w_immediate_to_reg_instruction(reg, data);
  }
  else {
    reg = BYTE_REGS[register_index];
    uint8_t data = instruction_bytes[byte_index++];
    write_mov_b_immediate_to_reg_instruction(reg, data);
  }
}

