/* Copyright (C) 2016 Jeremiah Orians
 * Copyright (C) 2020 deesix <deesix@tuta.io>
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

unsigned fread(char* buffer, unsigned size, unsigned count, FILE* f) {
	return read(f, buffer, size * count);
}

unsigned fwrite(char* buffer, unsigned size, unsigned count, FILE* f) {
	if(size == 0 || count == 0) {
		return 0;
	}

	return write(f, buffer, size * count);
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

int fclose(FILE* stream)
{
	int error = close(stream);
	return error;
}

int strlen(char* str )
{
	int i = 0;
	while(0 != str[i]) i = i + 1;
	return i;
}

void fputs(char* s, FILE* f)
{
	write(f, s, strlen(s));
}

/* The real memcpy has void* parameters and return types, but
 * for M2-Planet it doesn't matter and it leads to slightly better
 * codegen if they're char* from the start. */
char* memcpy(char* dst, char* src, unsigned count)
{
	if(NULL == dst) return dst;
	if(NULL == src) return NULL;

	unsigned i = 0;
	while(i < count)
	{
		dst[i] = src[i];
		i = i + 1;
	}

	return dst;
}

int strcmp(char* lhs, char* rhs)
{
	int i = 0;
	while(0 != lhs[i])
	{
		if(lhs[i] != rhs[i]) return lhs[i] - rhs[i];
		i = i + 1;
	}

	return lhs[i] - rhs[i];
}

int isspace(int c) {
	return c == ' ' || c == '\n' || c == '\t';
}

int isdigit(int c) {
	return c >= '0' && c <= '9';
}

int isalpha(int c) {
	c = c | 32; /* lowercase the char */
	return c >= 'a' && c <= 'z';
}

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

char* __MALLOC_PTR;
char* __BRK_PTR;
void* calloc(int count, int size)
{
	size = size * count;
	if(__BRK_PTR == 0) {
		__BRK_PTR = brk(0);
		__MALLOC_PTR = __BRK_PTR;
	}

	if (size % sizeof(int) != 0) {
		size = size + sizeof(int) - (size % sizeof(int));
	}

	if((__MALLOC_PTR + size) >__BRK_PTR) {
		__BRK_PTR = brk(__MALLOC_PTR + 262144 - (size % 262144));
		if(__BRK_PTR == -1) return 0;
	}

	char* ptr = __MALLOC_PTR;
	__MALLOC_PTR = __MALLOC_PTR + size;
	return ptr;
}

int abs(int n) {
    if (n < 0) {
        return -n;
    }
    return n;
}

