/* Copyright (C) 2016 Jeremiah Orians
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
	stdin = 0x1100,
	stdout = 0x1101,
	stderr = 0,
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
	asm("LOAD R1 R14 0"
	    "FGETC");
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
	asm("LOAD R0 R14 0"
	    "LOAD R1 R14 4"
	    "FPUTC");
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

FILE* open_read(int filename)
{
	asm("LOAD R0 R14 0"
	    "FOPEN_READ");
}

FILE* open_write(int filename)
{
	asm("LOAD R0 R14 0"
	    "FOPEN_WRITE");
}

FILE* fopen(char* filename, char* mode)
{
	FILE* f;
	int fd = 0;
	if(match(filename, "STDIN") || match(filename, "tape_01"))
	{
		fd = 0x1100;
	}
	else if(match(filename, "STDOUT") || match(filename, "tape_02"))
	{
		fd = 0x1101;
	}

	if('w' == mode[0])
	{
		f = open_write(fd);
	}
	else
	{ /* Everything else is a read */
		f = open_read(fd);
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
	asm("LOAD R0 R14 0"
	    "FCLOSE");
}

int fclose(FILE* stream)
{
	int error = close(stream);
	return error;
}

int brk(void *addr)
{
	asm("LOAD R0 R14 0"
	    "ADDU R0 R12 R0"
	    "SWAP R0 R12");
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
	asm("LOAD R0 R14 0"
	    "HALT");
}
