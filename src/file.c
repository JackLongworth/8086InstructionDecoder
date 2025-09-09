#include "file.h"

FILE *file_src, *file_dest;
char file_cursor;

void init_file_io(char* src_name, char* dest_name) {
  file_src = fopen(src_name, "r");
  file_dest = fopen(dest_name, "w");
}

uint8_t* read_instructions_from_file(size_t *file_size_out) {
  if (fseek(file_src, 0, SEEK_END) != 0) {
    perror("fseek");
    fclose(file_src);
    return NULL;
  }

  long size = ftell(file_src);
  if (size < 0) {
    perror("ftell");
    fclose(file_src);
    return NULL;
  }

  rewind(file_src);

  uint8_t *file_buffer = (uint8_t *) malloc(size);
  if (!file_buffer) {
    perror("malloc");
    fclose(file_src);
    return NULL;
  }

  size_t bytes_read = fread(file_buffer, 1, size, file_src);

  if (file_size_out) {
    *file_size_out = (size_t) size;
  }
  fclose(file_src);

  return file_buffer;
}

void write_mov_reg_mem_instruction(const char *dest, const char *src) {
  printf("mov %s, %s\n", dest, src);
  fprintf(file_dest, "mov %s, %s\n", dest, src);
}


void write_mov_b_immediate_to_reg_instruction(const char *dest, const uint8_t immediate) {
  printf("mov %s, %d\n", dest, immediate);
  fprintf(file_dest, "mov %s, %d\n", dest, immediate);
}

void write_mov_w_immediate_to_reg_instruction(const char *dest, const uint16_t immediate) {
  printf("mov %s, %d\n", dest, immediate);
  fprintf(file_dest, "mov %s, %d\n", dest, immediate);
}


void cleardown_file_io() {
  fclose(file_dest);
}

