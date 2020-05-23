#include "main.h"

struct book
{
	char* ISBN;
	char* author;
	char* title;
	int all;
	int out;
};

void show_book(struct book*, int, int);
void shell_sort_struct_book(struct book*, int);
void shell_sort_struct_author(struct book*, int);
int menu_book(int);
struct book* add_book(struct book*, int*);
void delite_book(struct book*, int*);
void find_book(struct book*, int);
struct book* input_book(struct book*, int*, FILE*);

void book(int level)
{
	int number_book = 0;
	FILE* book_f = fopen("book.csv", "r");
	if (book == 0)
	{
		printf("Problem with books.csv\n");
		in_log("Problem with book.csv");
		exit(301);
	}
	struct book* book = calloc(1, sizeof(struct book));
	book = input_book(book, &number_book, book_f);
	fclose(book_f);
	int book_menu = menu_book(level);
	while(book_menu)
	{
		switch(book_menu)
		{
			case 1:
				in_log("Add book");
				book = add_book(book, &number_book); 
				book_menu = menu_book(level); 
				in_log("Success");
			break;
			case 2: 
				in_log("Delite book");
				delite_book(book, &number_book);
				book_menu = menu_book(level); 
				in_log("Success");
			break;
			case 3: 
				in_log("Find book");
				find_book(book, number_book);
				book_menu = menu_book(level); 
				in_log("Success");
			break;
			case 4:
				in_log("Show book");
				shell_sort_struct_book(book, number_book);
				show_book(book, 0, number_book);
				book_menu = menu_book(level);
				in_log("Success");
			break;
		}
	}
	free(book);
}

void find_book(struct book* ptr_struct, int num)
{
	shell_sort_struct_author(ptr_struct, num);
	char** author_arr = calloc(num, sizeof(char*));
	for (int i = 0; i < num; ++i)
		author_arr[i] = (ptr_struct + i)->author;
	char* consol = input_str_consol(NULL);
	in_log(consol);
	int flag = bin_search(author_arr, consol, num - 1);
	if (flag == -1)
	{
		printf("\nNo such book %s\n\n", consol);
		in_log("No such book");
		return;
	}
	int left = flag, right = flag;
	while((ptr_struct + left - 1) > 0 && left != 0)
	{
		if (string_cmd((ptr_struct + left - 1)->author, consol) != -1)
			left -= 1;
		else
			break;
	}
	while((ptr_struct + right + 1) > 0 && right < num - 1)
	{
		if (string_cmd((ptr_struct + right + 1)->author, consol) != 1)
			right += 1;
		else
			break;
	}
	free(author_arr);
	free(consol);
	show_book(ptr_struct, left, right + 1);
}

void delite_book(struct book* ptr_struct, int* num)
{
	char** ISBN_mas = calloc(*num, sizeof(char*));
	shell_sort_struct_book(ptr_struct, *num);
	for (int i = 0; i < *num; ++i)
		ISBN_mas[i] = (ptr_struct + i)->ISBN;//создаём массив id для поиска
	puts("Write ISBN of the book you want delite");
	char* consol = input_str_consol(NULL);
	in_log(consol);
	int del = bin_search(ISBN_mas, consol, *num - 1);
	free(ISBN_mas);
	if (del == -1)
	{
		printf("\nI don't know this book %s\n\n", consol);
		in_log("Don't know such point");
		return;
	}
	free(consol);
	if (del != *num - 1)
	{
		ptr_struct[del] = ptr_struct[*num - 1];
	} else
	{
		ptr_struct[del] = ptr_struct[*num - 2];
	}
	shell_sort_struct_book(ptr_struct, *num);
	FILE* csv = fopen("book.csv", "w");
	for (int i = 0; i < *num - 1; ++i)
		fprintf(csv, "%s;%s;%s;%d;%d\n", (ptr_struct + i)->ISBN, (ptr_struct + i)->title, (ptr_struct + i)->title, (ptr_struct + i)->all, (ptr_struct + i)->out);
	fclose(csv);
	*num -= 1;
}

struct book* add_book(struct book* ptr_struct, int* num)
{
	FILE* txt = fopen("book.csv", "a");
	char* proverka;
	shell_sort_struct_book(ptr_struct, *num);
	char** ISBN_mas = calloc(*num, sizeof(char*));
	for (int i = 0; i < *num; ++i)
		ISBN_mas[i] = (ptr_struct + i)->ISBN;//создаём массив id для поиска
	puts("Write book in rhis format");
	puts("9666965348;Strugatskie;Grad obrechenni;10;15");
	ptr_struct = realloc(ptr_struct, (*num + 1)*sizeof(struct book));
	proverka = input_str_consol(NULL);
	in_log(proverka);
	if (bin_search(ISBN_mas, proverka, *num - 1) != -1)
	{
		printf("\nThis ISBN already exists %s\n\n", proverka);
		in_log("This ISBN already exists");
		ptr_struct = realloc(ptr_struct, *num * sizeof(struct book));
		fclose(txt);
		free(ISBN_mas);
		char c;
		while((c = getchar()) != '\n');
		return ptr_struct;
	}
	(ptr_struct + *num)->ISBN = proverka;
	fprintf(txt, "%s;", (ptr_struct + *num)->ISBN);
	(ptr_struct + *num)->author = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->author);
	(ptr_struct + *num)->title = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->title);
	(ptr_struct + *num)->all = int_from_str(input_str_consol(NULL));
	fprintf(txt, "%d;", (ptr_struct + *num)->all);
	(ptr_struct + *num)->out = int_from_str(input_str_consol(NULL));
	fprintf(txt, "%d", (ptr_struct + *num)->out);
	*num += 1;
	fclose(txt);
	free(ISBN_mas);
	return ptr_struct;
}

void show_book(struct book* book, int i, int right)
{
	printf("|    ISBN   |       author       |              title            | all|student|\n");
	printf("+-----------+--------------------+-------------------------------+----+-------+\n");
	for (i; i < right; ++i)
	{
		printf("|%11s|%20s|%31s|%4d|%7d|\n", book[i].ISBN, book[i].author, book[i].title, book[i].all, book[i].out);
	}
	printf("+-----------+--------------------+-------------------------------+----+-------+\n");
}

void shell_sort_struct_book(struct book* ptr_struct, int n)
{
	int gap, i, j;
	for (gap = n / 2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; ++i)
		{
			for (j = i - gap; j >= 0 && string_cmd(ptr_struct[j].ISBN, ptr_struct[j + gap].ISBN) == 1; j -= gap)
			{
				struct book temp = ptr_struct[j];
				ptr_struct[j] = ptr_struct[j + gap];
				ptr_struct[j + gap] = temp;
			}
		}
	}
}

void shell_sort_struct_author(struct book* ptr_struct, int n)
{
	int gap, i, j;
	for (gap = n / 2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; ++i)
		{
			for (j = i - gap; j >= 0 && string_cmd(ptr_struct[j].title, ptr_struct[j + gap].title) == 1; j -= gap)
			{
				struct book temp = ptr_struct[j];
				ptr_struct[j] = ptr_struct[j + gap];
				ptr_struct[j + gap] = temp;
			}
		}
	}
}

int menu_book(int level)
{
	int menu;
	while(1)
	{
		puts("You're in book menu\n");
		in_log("In book menu");
		if (level == 1)
		{
			puts("-a to add new book");
			puts("-d to delite book");
		}
		puts("-f to find book for auth");
		puts("-s to show all books");
		puts("-0 to go to main menu");
		char c1 = getchar();
		if (c1 != '-')
		{
			puts("\nI don't know such point\n");
			in_log("Unknown point");
			while (c1 != '\n') c1 = getchar();//убирает оставшиесе символы
			continue;
		}
		c1 = getchar();
		char c2 = getchar();
		if (c2 != '\n')
		{
			puts("\nI don't know such point \\n\n");
			in_log("Unknown point");
			while (c1 != '\n') c1 = getchar();
			continue;
		}
		if (level == 1)
		{
			switch(c1)
			{
				case 'a': menu = 1; break;
				case 'd': menu = 2; break;
				case 'f': menu = 3; break;
				case 's': menu = 4; break;
				case '0': menu = 0; break;
				default: puts("\nI don't know such point\n"); in_log("Unknown point"); continue;
			}
		} else
		{
			switch(c1)
			{
				case 'f': menu = 3; break;
				case 's': menu = 4; break;
				case '0': menu = 0; break;
				default: puts("\nI don't know such point\n"); in_log("Unknown point"); continue;
			}

		}
		break;
	}
	return menu;
}

struct book* input_book(struct book* ptr_struct, int* i, FILE* txt)
{
	int Eof = 1;
	while (Eof != 0)
	{
		(ptr_struct + *i)->ISBN = input_str_file(&Eof, txt);
		(ptr_struct + *i)->author = input_str_file(&Eof, txt);
		(ptr_struct + *i)->title = input_str_file(&Eof, txt);
		(ptr_struct + *i)->all = int_from_str(input_str_file(&Eof, txt));
		(ptr_struct + *i)->out = int_from_str(input_str_file(&Eof, txt));
		*i += 1; ptr_struct = realloc(ptr_struct, (*i + 1) * sizeof(struct book));		
	}
	ptr_struct = realloc(ptr_struct, (*i - 1) * sizeof(struct book));//освобаждает лишню память
	*i -= 1;
	return ptr_struct;
}
