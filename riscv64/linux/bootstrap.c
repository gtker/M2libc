/* Copyright (C) 2016 Jeremiah Orians
 * Copyright (C) 2021 Andrius Štikonas
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

enum
{
	stdin = 0,
	stdout = 1,
	stderr = 2,
};

enum
{
	EOF = 0xFFFFFFFF,
	NULL = 0,
};

enum
{
	EXIT_FAILURE = 1,
	EXIT_SUCCESS = 0,
};

enum
{
	TRUE = 1,
	FALSE = 0,
};


int fgetc(FILE* f)
{
	asm("rd_a0 rs1_fp !-8 ld"
	    "rs1_sp rs2_a0 sd"
	    "rd_a1 rs1_sp mv"
	    "rd_a2 !1 addi"
	    "rd_a7 !63 addi"
	    "ecall"
	    "rd_a1 mv"
	    "rd_t0 rs1_a0 mv"
	    "rd_a0 rs1_sp ld"
	    "rs1_t0 @8 bnez"
	    "rd_a0 !-1 addi");
}

unsigned fread(char* buffer, unsigned size, unsigned count, FILE* f) {
	count = size * count;

	unsigned i = 0;
	for(; i < count; i = i + 1) {
		buffer[i] = fgetc(f);
	}

	return i;
}

void fputc(char s, FILE* f)
{
	asm("rd_a0 rs1_fp !-16 ld"
	    "rd_a1 rs1_fp !-8 addi"
	    "rd_a2 !1 addi"   /* 1 byte */
	    "rd_a7 !64 addi"  /* write */
	    "ecall");
}

void fputs(char* s, FILE* f)
{
	while(0 != s[0])
	{
		fputc(s[0], f);
		s = s + 1;
	}
}

unsigned fwrite(char* buffer, unsigned size, unsigned count, FILE* f) {
	count = size * count;

	unsigned i = 0;
	for(; i < count; i = i + 1) {
		fputc(buffer[i], f);
	}

	return i;
}

FILE* open(char* name, int flag, int mode)
{
	asm("rd_a0 !-100 addi" /* AT_FDCWD */
	    "rd_a1 rs1_fp !-8 ld"
	    "rd_a2 rs1_fp !-16 ld"
	    "rd_a3 rs1_fp !-24 ld"
	    "rd_a7 !56 addi"   /* openat */
	    "ecall");
}

FILE* fopen(char* filename, char* mode)
{
	FILE* f;
	if('w' == mode[0])
	{ /* 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal */
		f = open(filename, 577 , 384);
	}
	else
	{ /* Everything else is a read */
		f = open(filename, 0, 0);
	}

	/* Negative numbers are error codes */
	if(0 > f)
	{
		return 0;
	}
	return f;
}

int close(int fd)
{
	asm("rd_a0 rs1_fp !-8 ld"
	    "rd_a7 !57 addi"    /* close */
	    "ecall");
}

int fclose(FILE* stream)
{
	int error = close(stream);
	return error;
}

int brk(void *addr)
{
	asm("rd_a0 rs1_fp !-8 ld"
	    "rd_a7 !214 addi"   /* brk */
	    "ecall");
}

long _malloc_ptr;
long _brk_ptr;

void* malloc(int size)
{
	if(NULL == _brk_ptr)
	{
		_brk_ptr = brk(0);
		_malloc_ptr = _brk_ptr;
	}

	if(_brk_ptr < _malloc_ptr + size)
	{
		_brk_ptr = brk(_malloc_ptr + size);
		if(-1 == _brk_ptr) return 0;
	}

	long old_malloc = _malloc_ptr;
	_malloc_ptr = _malloc_ptr + size;
	return old_malloc;
}

int strlen(char* str )
{
	int i = 0;
	while(0 != str[i]) i = i + 1;
	return i;
}

void* memset(void* ptr, int value, int num)
{
	char* s;
	for(s = ptr; 0 < num; num = num - 1)
	{
		s[0] = value;
		s = s + 1;
	}
}

void* calloc(int count, int size)
{
	void* ret = malloc(count * size);
	if(NULL == ret) return NULL;
	memset(ret, 0, (count * size));
	return ret;
}

void free(void* l)
{
	return;
}

void exit(int value)
{
	asm("rd_a0 rs1_fp !-8 ld"
	    "rd_a7 !93 addi"   /* exit */
	    "ecall");
}
