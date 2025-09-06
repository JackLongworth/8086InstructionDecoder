#include <stdio.h>
#include <string.h>

void parse_instruction(unsigned char* instruction_buffer);
void mov_instruction(unsigned char first_byte, unsigned char second_byte);


static const char *BYTE_REGS[8] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
static const char *WORD_REGS[8] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };


FILE *file_src;
FILE *file_dest;

void init_files(char* src_name, char* dest_name) {
  file_src = fopen(src_name, "r");
  file_dest = fopen(dest_name, "w");
}

int main(int argc, char* argv[]) {
  init_files(argv[1], argv[2]);

  unsigned char instruction_buffer[2];
  while (fread(instruction_buffer, 1, 2, file_src)) {
    parse_instruction(instruction_buffer);
  }

  fclose(file_src);
  fclose(file_dest);
  return 0;
}


void parse_instruction(unsigned char* instruction_buffer) {
  unsigned char first_byte = instruction_buffer[0];
  unsigned char second_byte = instruction_buffer[1];

  printf("%hhb\t", first_byte);
  printf("%hhb\n", second_byte);

  unsigned char opcode = (first_byte & 0b11111100) >> 2;

  switch (opcode) {
    case 0b100010:
      mov_instruction(first_byte & 0b00000011, second_byte);
    break;
  }
}

void mov_instruction(unsigned char dw, unsigned char mod_reg_rm) {
  const char *src, *dest;

  unsigned char reg = (mod_reg_rm & 0b00111000) >> 3;
  unsigned char rm = (mod_reg_rm & 0b00000111);


  switch (dw & 0b00000001) {
    case 0b00000001:
      src = WORD_REGS[reg];
      dest = WORD_REGS[rm];
      break;
    case 0b00000000:
      src = BYTE_REGS[reg];
      dest = BYTE_REGS[rm];
      break;
  }

  fprintf(file_dest, "mov %s, %s\n", dest, src);
}
