#include "main.h"

void students(int);
void book(int);
int main_menu();
void login(int*, int*);

int main(int argc, char const *argv[])
{
	
	int level_student;
	int level_book;
	login(&level_book, &level_student);
	
	int from_main_menu = main_menu();
	while(from_main_menu)
	{
		switch(from_main_menu)
		{
			case 1: students(level_student); from_main_menu = main_menu(); break;
			case 2: book(level_book); from_main_menu = main_menu(); break;
		}
	}
	return 0;
}
static char* user_in;

void login(int* level_book, int* level_student)
{
	FILE* user = fopen("user.csv", "r");
	if (user == 0)
	{
		printf("Cann't conected\n");
		exit(100);
	}
	char* user_file;
	char* user_consol;
	char* password_file;
	char* password_consol;
	printf("Print your login\n");
	int Eof = 1;
	user_consol = input_str_consol(NULL);
	while (Eof != 0)
	{
		user_file = input_str_file(&Eof, user);
		if (string_cmd(user_file, user_consol) == 0)
		{
			printf("Wrire your password %s\n", user_file);
			password_consol = input_str_consol(NULL);
			password_file = input_str_file(&Eof, user);
			if (string_cmd(password_file, password_consol) == 0)
			{
				printf("You're in))\n");
				in_log("Come in");
				break;
			}else 
			{
				printf("Wrong password\n");
				in_log("wrong password");
				printf("Try again\n");
				fclose(user);
				exit(200);
			}
		}else
		{
			for (int i = 0; i < 3; ++i)//счтываем лишние поля user.csv
				user_file = input_str_file(&Eof, user);
		}
	}
	free(user_consol);
	free(password_file);
	free(password_consol);
	if (Eof == 0)
	{
		printf("No such login\n");
		printf("Try again\n");
		fclose(user);
		exit(201);
	}
	*level_book = getc(user) - '0';
	*level_student = getc(user);// съедаем точку запятой
	*level_student = getc(user) - '0';
	user_in = user_file;
	fclose(user);
}

int main_menu()
{

	int main_menu;
	while(1)
	{
		puts("\nYou in main menu");
		in_log("In main menu");
		puts("-s to go to student menu");
		puts("-b to go to book menu");
		puts("-q to quite");
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
			puts("\nI don't know such point \n");
			in_log("Unknown point");
			while (c1 != '\n') c1 = getchar();
			continue;
		}
		switch(c1)
		{
			case 's': main_menu = 1; break;
			case 'b': main_menu = 2; break;
			case 'q': main_menu = 0; in_log("Exit"); in_log("//////////////////////"); break;
			default: puts("\nI don't know such point\n"); in_log("Unknown point"); continue;
		}
		break;
	}
	return main_menu;
}

int int_from_str(char* str)//вытаскивает из строки число
{
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		res *= 10;
		if (str[i] <= '9' && str[i] >= '0')
			res = res + (str[i] - '0');
		else
		{
			printf("Problem with file\n");
			in_log("Problem with file");
			printf("No such number\n");
			exit(401);
		}
	}
	return res;
}

char* input_str_file(int* Eof, FILE* txt)//считывает строку в случае конца файла возвращает 0
{
	int i = 0, c;// \n и ; не считываются
	char* ptr_str = calloc(1, sizeof(char));
	while ((c = getc(txt)) != ';' && c != '\n' && c != EOF)
	{
		*(ptr_str + i) = c;
		i += 1;
		ptr_str = realloc(ptr_str, (i + 1) * sizeof(char));
	}
	*(ptr_str + i) = '\0';
	if (c == EOF)
		*Eof = 0;
	return ptr_str;
}

char* input_str_consol(int* Eof)
{
	int i = 0, c;// \n и ; не считываются
	char* ptr_str = calloc(1, sizeof(char));
	while ((c = getchar()) != ';' && c != '\n' && c != EOF)
	{
		*(ptr_str + i) = c;
		i += 1;
		ptr_str = realloc(ptr_str, (i + 1) * sizeof(char));
	}
	*(ptr_str + i) = '\0';
	if (c == EOF)
		*Eof = 0;
	return ptr_str;
}

int string_cmd(char* s1, char* s2)//сравнивает строки
{                                       // flag = 0 равны flag = 1 первая больше второй
	int flag, i;						// flag = -1 вторая больше первой в программе это не используется
	for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i)
	{
		if (s1[i] != s2[i])
		{
			if (s1[i] > s2[i])
				flag = 1;
			else
				flag = -1;
			return flag;
		}
	}
	if (s1[i] > s2[i])
		flag = 1;
	else if (s1[i] < s2[i])
	{
		flag = -1;
	}else
		flag = 0;
	return flag;
}

int bin_search(char** mas_str, char* x, int high)
{
	int low = 0;
	while (low <= high)
	{
		int mid = low + (high - low) / 2;
		if (string_cmd(mas_str[mid], x) == 1) //mas_str[mid] > x
			high = mid - 1;
		else if (string_cmd(x, mas_str[mid]) == 1)// x > mas_str[mid]
			low = mid + 1;
		else 
			return mid;
	}
	return -1;
}

void in_log(char* masege)
{
	long int s_time = time(NULL);
	struct tm *m_time;
	char str_t[18];
	m_time = localtime (&s_time);
	strftime(str_t, 18, "%d.%m.%y_%X", m_time);
	FILE* log = fopen("library.log", "a");
	if (log == 0)
	{
		printf("Problem with library.log\n");
		exit(1000);
	}
	fprintf(log, "%s;%s;%s\n", str_t, user_in, masege);
	fclose(log);
}

void print_student(char* id)
{
	int number_students = 0;
	FILE* students = fopen("students.csv", "r");
	if (students == 0)
	{
		printf("Problem with students.csv\n");
		in_log("Problem with students.csv");
		return;
	}
	struct student *stud = calloc(1, sizeof(struct student));
	stud = input_student(stud, &number_students, students);
	fclose(students);
	shell_sort_struct_students(stud, number_students);
	char** id_arr = calloc(number_students, sizeof(char*));
	for (int i = 0; i < number_students; ++i)
		id_arr[i] = (stud + i)->id;
	int find = bin_search(id_arr, id, number_students - 1);
	free(id_arr);
	if (find == -1)
	{
		printf("Student was delited\n");
		in_log("there is not students in db");
		return;
	}
	printf("|%8s|%15s|%15s|%15s|%5s|%s|", stud[find].id, stud[find].familyname, stud[find].name, stud[find].fathername, stud[find].index, stud[find].specialist);
	free(stud);
}

void print_book(char* ISBN)
{
	int number_book = 0;
	FILE* book_f = fopen("students.csv", "r");
	if (book_f == 0)
	{
		printf("Problem with students.csv\n");
		in_log("Problem with students.csv");
		return;
	}
	struct book *book = calloc(1, sizeof(struct book));
	book = input_book(book, &number_book, book_f);
	fclose(book_f);
	shell_sort_struct_book(book, number_book);
	char** ISBN_arr = calloc(number_book, sizeof(char*));
	for (int i = 0; i < number_book; ++i)
		ISBN_arr[i] = (book + i)->ISBN;
	int find = bin_search(ISBN_arr, ISBN, number_book - 1);
	free(ISBN_arr);
	if (find == -1)
	{
		printf("Book was delited\n");
		in_log("there is not book in db");
		return;
	}
	printf("|%11s|%20s|%31s|%4d|%7d|", book[find].ISBN, book[find].author, book[find].title, book[find].all, book[find].out);
	free(book);
}
