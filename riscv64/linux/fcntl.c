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

#ifndef __FCNTL_C
#define __FCNTL_C

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 00100
#define O_EXCL 00200
#define O_TRUNC 001000
#define O_APPEND 002000

#define S_IXUSR 00100
#define S_IWUSR 00200
#define S_IRUSR 00400
#define S_IRWXU 00700


int _open(char* name, int flag, int mode)
{
	asm("RD_A0 !-100 ADDI" /* AT_FDCWD */
	    "RD_A1 RS1_FP !-8 LD"
	    "RD_A2 RS1_FP !-16 LD"
	    "RD_A3 RS1_FP !-24 LD"
	    "RD_A7 !56 ADDI"
	    "ECALL");
}

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#endif
