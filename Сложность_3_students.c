#include "main.h"
 
struct student* input_student(struct student*, int*, FILE*);//заполняет массив структур

void shell_sort_struct_students(struct student*, int);//сортирует структуры по ID
void shell_sort_struct_name(struct student*, int);

int menu_student();
void show_student(struct student*, int, int);// показывает студентов от первого инта до второго
struct student* add_student(struct student*, int*);
void delite_student(struct student*, int*);
struct student* from_beakup(struct student*, int*);

void do_beackup(struct student*, int);
char* file_name();

void find_student(struct student*, int);// ищет и выводит студентов по фамилии

struct library* newnode_student(FILE*, char*, char*, int*);
struct library* add_library_student(struct library*, FILE*, char*, char*, char*, int*, int*);
void show_inf_students(struct student*, int);


void students(int level)
{
	int number_students = 0;
	FILE* students = fopen("students.csv", "r");
	if (students == 0)
	{
		printf("Problem with students.csv\n");
		in_log("Problem with students.csv");
		exit(300);
	}
	struct student *stud = calloc(1, sizeof(struct student));
	stud = input_student(stud, &number_students, students);
	fclose(students);
	int from_menu = menu_student(level);
	while (from_menu)
	{
		switch(from_menu)
		{
			case 1: 
				in_log("Show students"); 
				shell_sort_struct_students(stud, number_students); 
				show_student(stud, 0, number_students); 
				from_menu = menu_student(level); 
				in_log("Success");
			break;
			case 2: 
				in_log("Add the student"); 
				stud = add_student(stud, &number_students); 
				from_menu = menu_student(level); 
				in_log("Success"); 
			break;
			case 3:
				in_log("Delite the student"); 
				delite_student(stud, &number_students); 
				from_menu = menu_student(level);
				in_log("Success");
			break;
			case 4: 
				in_log("Restabilish student from beakup"); 
				stud = from_beakup(stud, &number_students); 
				from_menu = menu_student(level);
				in_log("Success");
			break;
			case 5:
				in_log("Create beakup file"); 
				do_beackup(stud, number_students);
				from_menu = menu_student(level); 
				in_log("Success");
			break;
			case 6:
				in_log("Find student"); 
				find_student(stud, number_students); 
				from_menu = menu_student(level); 
				in_log("Success");
			break;
			case 7:
				in_log("show students with books");
				shell_sort_struct_students(stud, number_students);
				show_inf_students(stud, number_students);
				from_menu = menu_student(level);
				in_log("Success");
			break;
		}
	}
	free(stud);
}

void show_inf_students(struct student* ptr_struct, int num)
{
	printf("Write id of student\n");
	char** id_arr = calloc(num, sizeof(char*));
	for (int i = 0; i < num; ++i)
		id_arr[i] = (ptr_struct + i)->id;
	char* consol = input_str_consol(NULL);
	in_log(consol);
	int find = bin_search(id_arr, consol, num - 1);
	free(id_arr);
	if (find == -1)
	{
		printf("No such student\n");
		in_log("No such student");
		free(consol);
		return;
	}
	free(consol);
	FILE* lib = fopen("student_books.csv", "r");
	if (lib == 0)
	{
		printf("Problem with student_books.csv\n");
		in_log("Problem with student_books.csv");
		return;
	}
	char* lib_id;
	char* lib_ISBN;
	int Eof = 1, flag = 0;
	printf("\n|%8s|%15s|%15s|%15s|%5s|%s\n", ptr_struct[find].id, ptr_struct[find].familyname, ptr_struct[find].name, ptr_struct[find].fathername, ptr_struct[find].index, ptr_struct[find].specialist);
	while(Eof)
	{
		lib_ISBN = input_str_file(&Eof, lib);
		lib_id = input_str_file(&Eof, lib);
		if (string_cmd(ptr_struct[find].id, lib_id) == 0)
		{
			print_book(lib_ISBN);
			flag = 1;
			printf("%11s|\n", lib_ISBN);
		} else
			lib_ISBN = input_str_file(&Eof, lib);
	}
	fclose(lib);
	free(lib_id); free(lib_ISBN);
	if (flag == 0)
	{
		printf("This student does't take any book\n");
		in_log("This student does't take any book");
	}
}

void find_student(struct student* ptr_struct, int num)
{
	shell_sort_struct_name(ptr_struct, num);
	char** name_arr = calloc(num, sizeof(char*));
	for (int i = 0; i < num; ++i)
		name_arr[i] = (ptr_struct + i)->familyname;
	char* consol = input_str_consol(NULL);
	in_log(consol);
	int flag = bin_search(name_arr, consol, num - 1);
	if (flag == -1)
	{
		printf("\nNo such student %s\n\n", consol);
		in_log("No such student");
		return;
	}
	int left = flag, right = flag;
	while((ptr_struct + left - 1) > 0 && left != 0)
	{
		if (string_cmd((ptr_struct + left - 1)->familyname, consol) != -1)
			left -= 1;
		else
			break;
	}
	while((ptr_struct + right + 1) > 0 && right < num - 1)
	{
		if (string_cmd((ptr_struct + right + 1)->familyname, consol) != 1)
			right += 1;
		else
			break;
	}
	free(name_arr);
	free(consol);
	show_student(ptr_struct, left, right + 1);
}

char* file_name()
{
	long int s_time = time(NULL);
	struct tm *m_time;
	char str_t[18];
	m_time = localtime (&s_time);
	strftime(str_t, 18, "%d.%m.%y_%X", m_time);
	char s1[] = "students_";
	char* f_name = calloc(27, sizeof(char));
	int i, j = 0, k = 0;
	for (i = 0; i < 9; ++i)
		f_name[i] = s1[i];
	for (i = 9, j; j < 27; ++i, ++j)
		f_name[i] = str_t[j];
	return f_name;
}

void do_beackup(struct student* ptr_struct, int num)
{
	char* f_name = file_name();
	printf("File name is %s\n", f_name);
	FILE* beakup = fopen(f_name, "w");
	if (beakup == 0)
	{
		puts("Cant't create file\n");
		in_log("Cant't create file");
		return;
	}
	for (int i = 0; i < num; ++i)
		fprintf(beakup, "%s;%s;%s;%s;%s;%s\n", (ptr_struct + i)->id, (ptr_struct + i)->familyname, (ptr_struct + i)->name, (ptr_struct + i)->fathername, (ptr_struct + i)->index, (ptr_struct + i)->specialist);
	puts("Success\n");
	fclose(beakup);
	free(f_name);
}

struct student* from_beakup(struct student* ptr_struct, int* num)
{
	puts("Write name backup file");
	in_log("Write name backup file");
	char* file = input_str_consol(NULL);
	in_log(file);
	FILE* backup = fopen(file, "r");
	if (backup == 0)
	{
		printf("\nNo such file %s\n\n", file);
		in_log("No such file");
		return ptr_struct;
	}
	*num = 0;
	free(file);
	ptr_struct = input_student(ptr_struct, num, backup);
	fclose(backup);
	show_student(ptr_struct, 0, *num);
	FILE* csv = fopen("students.csv", "w");
	if (csv == 0)
	{
		printf("\nProblrm with students.csv\n\n");
		in_log("Problrm with students.csv");
		return ptr_struct;
	}
	for (int i = 0; i < *num; ++i)
		fprintf(csv, "%s;%s;%s;%s;%s;%s\n", (ptr_struct + i)->id, (ptr_struct + i)->familyname, (ptr_struct + i)->name, (ptr_struct + i)->fathername, (ptr_struct + i)->index, (ptr_struct + i)->specialist);
	fclose(csv);
	return ptr_struct;
}

void delite_student(struct student* ptr_struct, int* num)
{
	char** ID_mas = calloc(*num, sizeof(char*));
	shell_sort_struct_students(ptr_struct, *num);
	for (int i = 0; i < *num; ++i)
		ID_mas[i] = (ptr_struct + i)->id;//создаём массив id для поиска
	puts("Write id of the student you want delite");
	in_log("Write id of the student you want delite");
	char* consol = input_str_consol(NULL);
	in_log(consol);
	int del = bin_search(ID_mas, consol, *num - 1);
	free(ID_mas);
	if (del == -1)
	{
		printf("\nI don't know this student %s\n\n", consol);
		in_log("I don't know this student");
		return;
	}
	FILE* lib = fopen("student_books.csv", "r");
	if (lib == 0)
	{
		printf("Problem with student_books.csv\n");
		in_log("Problem with student_books.csv");
		free(consol);
		return;
	}
	struct library *library;
	library = NULL;
	int Eof = 1, flag = 1;
	while(Eof != 0 && flag != 0)
		library = add_library_student(library, lib, input_str_file(&Eof, lib), input_str_file(&Eof, lib), consol, &flag, &Eof);
	if (flag == 0)
	{
		printf("This student with book, cann't delite him\n");
		in_log("This student with book, cann't delite him");
		free(library);
		free(consol);
		fclose(lib);
		return;
	}

	free(library);
	free(consol);
	fclose(lib);
	if (del != *num - 1)
		ptr_struct[del] = ptr_struct[*num - 1];
	else
		ptr_struct[del] = ptr_struct[*num - 2];
	shell_sort_struct_students(ptr_struct, *num);
	FILE* csv = fopen("students.csv", "w");
	for (int i = 0; i < *num - 1; ++i)
		fprintf(csv, "%s;%s;%s;%s;%s;%s\n", (ptr_struct + i)->id, (ptr_struct + i)->familyname, (ptr_struct + i)->name, (ptr_struct + i)->fathername, (ptr_struct + i)->index, (ptr_struct + i)->specialist);
	fclose(csv);
	*num -= 1;
	printf("Success\n");
}

struct library* add_library_student(struct library* ptr_struct, FILE* txt, char* str2, char* str1, char* consol, int* flag, int* Eof)
{
	int cound;
	if (ptr_struct == NULL)
	{
		ptr_struct = newnode_student(txt, str1, str2, Eof);
		if ((cound = string_cmd(consol, str2)) == 0)
			*flag = 0;
	}
	else if ((cound = string_cmd(consol, str2)) == 0)
		*flag = 0;
	else if (cound < 0)
		ptr_struct->left = add_library_student(ptr_struct->left, txt, str1, str2, consol, flag, Eof);
	else
		ptr_struct->right = add_library_student(ptr_struct->right, txt, str1, str2, consol, flag, Eof);
	return ptr_struct;
}

struct library* newnode_student(FILE* txt, char* str1, char* str2, int* Eof)
{
	struct library* ptr_struct = calloc(1, sizeof(struct library));
	ptr_struct->ISBN = str1;
	ptr_struct->id = str2;
	ptr_struct->time = input_str_file(Eof, txt);
	ptr_struct->left = ptr_struct->right = NULL;
	return ptr_struct;
}

void shell_sort_struct_name(struct student* ptr_struct, int n)
{
	int gap, i, j;
	for (gap = n/2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; ++i)
		{//                                          prt_s[j] > ptr_s[j + gap]
			for (j = i - gap; j >= 0 && string_cmd(ptr_struct[j].familyname, ptr_struct[j + gap].familyname) == 1; j -= gap)
			{
				struct student temp = ptr_struct[j];
				ptr_struct[j] = ptr_struct[j + gap];
				ptr_struct[j + gap] = temp;
			}
		}
	}
}

void shell_sort_struct_students(struct student* ptr_struct, int n)
{
	int gap, i, j;
	for (gap = n/2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; ++i)
		{//                                          prt_s[j] > ptr_s[j + gap]
			for (j = i - gap; j >= 0 && string_cmd(ptr_struct[j].id, ptr_struct[j + gap].id) == 1; j -= gap)
			{
				struct student temp = ptr_struct[j];
				ptr_struct[j] = ptr_struct[j + gap];
				ptr_struct[j + gap] = temp;
			}
		}
	}
}

struct student* add_student(struct student* ptr_struct, int* num)
{
	FILE* txt = fopen("students.csv", "a");
	char* proverka;
	shell_sort_struct_students(ptr_struct, *num);
	char** ID_mas = calloc(*num, sizeof(char*));
	for (int i = 0; i < *num; ++i)
		ID_mas[i] = (ptr_struct + i)->id;//создаём массив id для поиска
	puts("Write student in rhis format");
	puts("1H3089;Ivanov;Makar;Makarovich;SM;specialnost");
	ptr_struct = realloc(ptr_struct, (*num + 1)*sizeof(struct student));
	proverka = input_str_consol(NULL);
	in_log(proverka);
	if (bin_search(ID_mas, proverka, *num - 1) != -1)
	{
		printf("\nThis Id already exists %s\n\n", proverka);
		in_log("This Id already exists");
		ptr_struct = realloc(ptr_struct, *num * sizeof(struct student));
		fclose(txt);
		free(ID_mas);
		char c;
		while((c = getchar()) != '\n');
		return ptr_struct;
	}
	(ptr_struct + *num)->id = proverka;
	fprintf(txt, "%s;", (ptr_struct + *num)->id);
	(ptr_struct + *num)->familyname = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->familyname);
	(ptr_struct + *num)->name = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->name);
	(ptr_struct + *num)->fathername = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->fathername);
	(ptr_struct + *num)->index = input_str_consol(NULL);
	fprintf(txt, "%s;", (ptr_struct + *num)->index);
	(ptr_struct + *num)->specialist = input_str_consol(NULL);
	fprintf(txt, "%s\n", (ptr_struct + *num)->specialist);
	*num += 1;
	fclose(txt);
	free(ID_mas);
	return ptr_struct;
}


void show_student(struct student* student, int i, int right)//i левая граница массива
{
	printf("|   ID   |  familyname   |     name      |  fathername   |index|specialist\n");
	printf("+--------+---------------+---------------+---------------+-----+----------\n");
	for (i; i < right; ++i)
		printf("|%8s|%15s|%15s|%15s|%5s|%s\n", student[i].id, student[i].familyname, student[i].name, student[i].fathername, student[i].index, student[i].specialist);
	printf("+--------+---------------+---------------+---------------+-----+----------\n");
}

struct student* input_student(struct student* ptr_struct, int* i, FILE* txt)
{
	int Eof = 1;
	while (Eof != 0)
	{
		(ptr_struct + *i)->id = input_str_file(&Eof, txt);
		(ptr_struct + *i)->familyname = input_str_file(&Eof, txt);
		(ptr_struct + *i)->name = input_str_file(&Eof, txt);
		(ptr_struct + *i)->fathername = input_str_file(&Eof, txt);
		(ptr_struct + *i)->index = input_str_file(&Eof, txt);
		(ptr_struct + *i)->specialist = input_str_file(&Eof, txt);
		*i += 1; ptr_struct = realloc(ptr_struct, (*i + 1) * sizeof(struct student));	
	}
	ptr_struct = realloc(ptr_struct, (*i - 1) * sizeof(struct student));//освобаждает лишню память
	*i -= 1;
	return ptr_struct;
}


int menu_student(int level)
{
	int menu_student;
	while (1)
	{
		puts("You're in students menu\n");
		in_log("In students menu");
		puts("Write:");
		puts("-i to show stusents with book");
		puts("-s to show all students"); // menu_student = 1
		if (level == 1)
		{
			puts("-a to add student"); // menu_student = 2
			puts("-d to delite student"); // menu_student = 3
			puts("-r to restabilish students from beakup"); // menu_student = 4
			puts("-b to create beckup file"); // menu_student = 5
		}
		puts("-f to find students for familyname"); // menu_student = 6
		puts("-0 to go to main menu"); // menu_student = 0
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
			switch (c1)
			{
				case 's': menu_student = 1; break;
				case 'a': menu_student = 2; break;
				case 'd': menu_student = 3; break;
				case 'r': menu_student = 4; break;
				case 'b': menu_student = 5; break;
				case 'f': menu_student = 6; break;
				case 'i': menu_student = 7; break;
				case '0': menu_student = 0; break;
				default: puts("\nI don't know such point\n"); in_log("Unknown point"); continue;
			}
		} else
		{
			switch (c1)
			{
				case 's': menu_student = 1; break;
				case 'f': menu_student = 6; break;
				case 'i': menu_student = 7; break;
				case '0': menu_student = 0; break;
				default: puts("\nI don't know such point\n"); in_log("Unknown point"); continue;
			}
		}
		break;
	}
	return menu_student;
}
