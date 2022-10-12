/*
1) 학생 입력(수시 추가가능)
2) 전체 학생 리스트
3) 이름으로 검색
4) 학생 정보 수정
5) 삭제
6) 종료
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define MAX_SIZE 10

#define IN(val, min, max) (((min)<=(val)&&(val)<=(max))?(true):(false))

typedef struct
{
	int std_num;
	char name[10];
	char addr[50];
	int tuition;
	int birthYear;
} Student;

void addInfo(Student studentList[MAX_SIZE])
{
	printf("Add Info\n");
}
void showInfoAll(Student studentList[MAX_SIZE])
{
	printf("Show Info ALL\n");
}
void searchInfoByName(Student studentList[MAX_SIZE])
{
	printf("Search Info By Name\n");
}
void editInfoByStdNum(Student studentList[MAX_SIZE])
{
	printf("Edit Info\n");
}
void deleteInfoByStdNum(Student studentList[MAX_SIZE])
{
	printf("Del Info\n");
}

int getInt(const char* str)
{
	int n;
	if (strcmp(str, ""))
	{
		printf("%s", str);
	}
	scanf("%d", &n);
	return n;
}
void printMenu(const char funcInfo[][30], const int funcInfoLen, const int endIndex)
{
	for (int i = 0; i < funcInfoLen; i++)
	{
		printf("%d) %s\n", i + 1, funcInfo[i]);
	}
	printf("%d) End\n", endIndex);
}
int runFunc(void(*funcs[])(Student studentList[MAX_SIZE]), const int funcsLen, const int index, const int endIndex)
{
	Student dump[MAX_SIZE];
	if (index == endIndex)
	{
		return 2;
	}
	if (IN(index - 1, 0, funcsLen - 1))
	{
		funcs[index - 1](dump);
		return 0;
	}
	return 1;
}
int menu(const int endIndex)
{
	const int funcInfoLen = 5;
	const char funcInfo[][30] = {
		"New student info",
		"Show all student info",
		"Search by name",
		"Edit student info",
		"Delete student info",
	};
	void(*funcs[])(Student studentList[MAX_SIZE]) = {
		addInfo,
		showInfoAll,
		searchInfoByName,
		editInfoByStdNum,
		deleteInfoByStdNum,
	};
	printMenu(funcInfo, funcInfoLen, endIndex);
	int index = getInt("Input : ");
	int result = runFunc(funcs, funcInfoLen, index, endIndex);
	// 0-> return 0, 1->print, return 0, 2->return -1
	if (result == 2)
		return -1;
	else if (result == 1)
		printf("Out of Menu Index Range\n");
	return 0;
}


int main()
{
	const int endIndex = 6;
	while (menu(endIndex) == 0);
	printf("End\n");
}