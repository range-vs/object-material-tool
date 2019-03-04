#define WIN32_LEAN_AND_MEAN
#pragma once

// c++/cli
using namespace System;

// c
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>

// define

#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
the file. See http://code.google.com/p/inih/issues/detail?id=21 */
#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

/* Nonzero to use stack, zero to use heap (malloc/free). */
#ifndef INI_USE_STACK
#define INI_USE_STACK 1
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 200
#endif

// c++
#include <string>
#include <ctime>

// typedef
using u_8 =  unsigned char;
using u_16 = unsigned short;
using u_32 = unsigned int;

// program
#include "Lines.h"

class ObjectDec
{
	// types
	static enum MAX_const
	{
		MAX_t_path = 55,
		MAX_e_shader = 35,
		MAX_c_shader = 40,
		MAX_g_material = 35,
		MAX_ini_str = 700,
		MAX_SECTION = 50,
		MAX_NAME = 50
	};

   struct ini_str {
		char t_path[MAX_t_path];
		char e_shader[MAX_e_shader];
		char c_shader[MAX_c_shader];
		char g_material[MAX_g_material];
		int  flags;
	} ;

   // static
	static time_t log_time;
	static tm * tml;
public:
	static char inifile[];
	static int ini_count;
	static ini_str ini_sting[MAX_ini_str];

	// private
private:
	std::string file_name;

	unsigned long searchEndString(char *buffer, unsigned long buff_ptr, unsigned long maxsize); // ищем 0x00. Возвращем длину данных без 00
	int ininame_search(char* ininame); // ищем секцию в ini - файле
public:
	ObjectDec() {}
	ObjectDec(const char* fn):file_name(fn){}
	int stlk_object_mtpatch(System::Windows::Forms::RichTextBox^ % log, short mode); // сканирование и разбор объекта + вывод в лог
	int stlk_object_parce(System::Windows::Forms::RichTextBox^ % log, const char *out_file_name); // сканирование и парсинг объекта+ вывод в файл
	int stlk_object_gmpatch(System::Windows::Forms::RichTextBox^ % log, const char *out_file_name, std::ofstream& Log, int mod); // настройка обыкновенного object
	void scandir(System::Windows::Forms::RichTextBox^ % log, char* _log, char *dir, char *outdir); // пакетная обработка папок

	// static
	static int ini_parse(void* user);
	static int ini_parse_file(FILE* file,
		int(*handler)(void*, const char*, const char*,
			const char*),
		void* user);
private:
	static char* lskip(const char* s);
	static char* rstrip(char* s);
	static char* find_char_or_comment(const char* s, char c);
	static char* strncpy0(char* dest, const char* src, size_t size);
	static int iniTOarray(void* user, const char* section, const char* name, const char* value);
	static void StrTOLower(char *a); // строку в нижний регистр

};