#ifndef _TEST_H_
#define _TEST_H_
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// общие функции реализованы в main.c 
char* input_str_file(int*, FILE*);//считывает строку с файла
char* input_str_consol(int*);//считывает строку с консоли
int int_from_str(char*);//вытаскивает из строки число
int string_cmd(char*, char*);// сравнивает две строки 1 первая строка больше -1 вторая 0 равны
int bin_search(char**, char*, int);// ищет строку из массива строк
void in_log(char*);// записывает файлы в лог
 
#endif
