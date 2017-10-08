#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

enum Opcode {
  EXIT = 0,
  ADDI,
  GETC,
  HASH,
  JNZ,
  PUTC,
  SUBM,
};

typedef struct Instruction_t {
  int opcode;
  int arg0;
  int arg1;
  int arg2;
} Instruction;

#define MEM_SIZE 0x100

#define ADDR_STR       0x00
#define REG_STRLEN     0x09
#define REG_HASH       (REG_STRLEN + 1)
#define REG_Z          0xFF

#define MAX_STR_LEN (REG_STRLEN - ADDR_STR - 1)

static const Instruction CODE[] = {
  {PUTC, 'H', 'e', 'l'},
  {PUTC, 'l', 'o', '.'},
  {PUTC, '.', '.', '\n'},
  {PUTC, 'P', 'l', 'e'},
  {PUTC, 'a', 's', 'e'},
  {PUTC, ' ', 'e', 'n'},
  {PUTC, 't', 'e', 'r'},
  {PUTC, ' ', 'p', 'a'},
  {PUTC, 's', 's', 'w'},
  {PUTC, 'o', 'r', 'd'},
  {PUTC, 0, ':', '\n'},
  {GETC, ADDR_STR, REG_STRLEN, 0},
  {ADDI, REG_STRLEN, 1, 0},
  {SUBM, REG_Z, REG_STRLEN, MAX_STR_LEN},
  {JNZ, -4, 123, 666},
  {PUTC, 'C', 'h', 'e'},
  {PUTC, 'c', 'k', 'i'},
  {PUTC, 'n', 'g', '.'},
  {PUTC, '.', '.', ' '},
  {HASH, ADDR_STR, REG_STRLEN, 0x666},
  {ADDI, REG_STRLEN, -1, 0},
  {SUBM, REG_Z, REG_STRLEN, 0},
  {JNZ, -4, 0x1337, 0xB00B5},
  {ADDI, REG_HASH, -0x319A, 0},
  {SUBM, REG_Z, REG_HASH, 0},
  {JNZ, 6, 0, 0},
  {PUTC, 'F', 'l', 'a'},
  {PUTC, 'g', '{', 'f'},
  {PUTC, 'u', 'c', 'k'},
  {PUTC, '_', '0', 'f'},
  {PUTC, 'f', '}', '\n'},
  {EXIT, 0, 0, 0},
  {PUTC, 'W', 'r', 'o'},
  {PUTC, 'n', 'g', ' '},
  {PUTC, ':', '/', 0},
  {PUTC, '\n', 0, 0},
};

//#define DEV
#ifdef DEV
  #define DEBUG(...) fprintf(stdout, __VA_ARGS__)
#else
  #define DEBUG(...)
#endif

void interpret(int ip) {
  int mem[MEM_SIZE] = {0};
  int code_length = sizeof(CODE) / sizeof(CODE[0]);

  while (ip >= 0 && ip < code_length) {
    Instruction i = CODE[ip];
    int args[3] = {i.arg0, i.arg1, i.arg2};

    DEBUG("\n");
    for (int j=0; j<21; j++) {
      DEBUG("%02X ", mem[j]);
    }
    DEBUG("STRLEN: %d\n", mem[REG_STRLEN]);
    DEBUG("\n");

    switch (i.opcode) {
      case ADDI:
        mem[i.arg0] += i.arg1;
        break;
      case GETC:
        mem[i.arg0 + mem[i.arg1]] = getc(stdin);
        break;
      case HASH:
        mem[REG_HASH] += mem[i.arg0 + mem[i.arg1]] ^ i.arg2;
        break;
      case JNZ:
        if (mem[REG_Z]) ip += i.arg0;
        break;
      case PUTC:
        for (int a=0; a<3; a++) { if (args[a] != 0) putc(args[a], stdout); }
        break;
      case SUBM:
        mem[i.arg0] = mem[i.arg1] - i.arg2;
        break;
      case EXIT:
        ip = -2;
        break;
    }
    ip++;
  }
}

int main(int argc, char *argv[]) {
  interpret(0);
  return 0;
}

