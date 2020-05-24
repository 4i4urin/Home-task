#ifndef _TEST_H_
#define _TEST_H_
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct library
{
	char* ISBN;
	char* id;
	char* time;
	struct library* right;
	struct library* left;	
};

struct student
{
	char* id;
	char* familyname;
	char* name;
	char* fathername;
	char* index;
	char* specialist;
};

struct book
{
	char* ISBN;
	char* author;
	char* title;
	int all;
	int out;
};

// общие функции реализованы в main.c 
char* input_str_file(int*, FILE*);//считывает строку с файла
char* input_str_consol(int*);//считывает строку с консоли
int int_from_str(char*);//вытаскивает из строки число
int string_cmd(char*, char*);// сравнивает две строки 1 первая строка больше -1 вторая 0 равны
int bin_search(char**, char*, int);// ищет строку из массива строк
void in_log(char*);// записывает файлы в лог
void print_student(char*);
void print_book(char*);
//функции реализованные для соответсвующих файлов 
// нужны для работы print_*
struct student* input_student(struct student*, int*, FILE*);
void shell_sort_struct_students(struct student*, int);//сортирует структуры по ID
struct book* input_book(struct book*, int*, FILE*);
void shell_sort_struct_book(struct book*, int);

#endif
