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

//////////////// 리스트 구현
#define LIST_TYPE Student
typedef struct ListNode
{
	LIST_TYPE data;
	struct ListNode* next;
} ListNode;
typedef struct List
{
	ListNode* firstNode;
	ListNode* endNode;
	int len;
} List;
List* create_List()
{
	List* header = (List*)malloc(sizeof(List));
	header->firstNode		= (ListNode*)malloc(sizeof(ListNode));
	header->endNode			= (ListNode*)malloc(sizeof(ListNode));
	header->firstNode->next = header->endNode;
	header->endNode = NULL;
	header->len = 0;
	return header;
}
ListNode* endCheck_List(List* list, ListNode* node)
{
	if (node == list->endNode)
	{
		return NULL;
	}
	return node;
}
ListNode* access_List(List* list, int index)
{
	ListNode* node = list->firstNode;
	for (int i = 0; i < index+1; i++)
	{
		node = node->next;
	}
	return endCheck_List(list, node);
}
void pushBack_List(List* list, const LIST_TYPE data)
{
	ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
	newNode->data = data;
	ListNode* preNode = access_List(list, list->len - 1);
	newNode->next = preNode->next;
	preNode->next = newNode;
	++list->len;
}
LIST_TYPE getData_List(List* list, int index)
{
	return access_List(list, index)->data;
}
void setData_List(List* list, int index, LIST_TYPE data)
{
	access_List(list, index)->data = data;
}
void deleteNode_List(List* list, int index)
{
	ListNode* preNode = access_List(list, index - 1);
	ListNode* delNode = preNode->next;
	preNode->next = preNode->next->next;
	free(delNode);
	--list->len;
}
List* destroy_List(List* list)
{
	for (int i = 0; i < list->len; i++)
	{
		deleteNode_List(list, 0);
	}
	free(list->endNode);
	free(list->firstNode);
	free(list);
	return NULL;
}

//////////////// 유틸리티 함수 구현부
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
void getStr(char* getStr, const char* str)
{
	if (strcmp(str, ""))
	{
		printf("%s", str);
	}
	scanf("%s", getStr);
}

//////////////// 파일 입출력 함수
int openFile(const char* url, int flag)
{
	int fd;
	fd = (flag & O_CREAT) ? open(url, flag, 0644) : open(url, flag);
	if (fd != -1)
	{
		return fd;
	}
	printf("Failed Open\n");
}
void writeFile(const int fd, const Student data)
{
	write(fd, &data, sizeof(data));
}
int loadFile(const int fd, Student* stdnt)
{
	return read(fd, stdnt, sizeof(*stdnt));
}
void closeFile(int* fd)
{
	close(*fd);
	*fd = 0;
}

void writeListAllFile(const char* url, List* list)
{
	int fd = openFile(url, O_WRONLY | O_CREAT | O_TRUNC);
	lseek(fd, 0, SEEK_SET);
	for (int i = 0; i < list->len; i++)
	{
		writeFile(fd, getData_List(list, i));
	}
	closeFile(&fd);
}
void loadListAllFile(const char* url, List* list)
{
	Student student;
	int fd = openFile(url, O_RDONLY | O_CREAT);
	while (loadFile(fd, &student) > 0)
	{
		pushBack_List(list, student);
	}
	closeFile(&fd);
}

//////////////// 기능 구현용 서브 함수 구현부
Student setStudent()
{
	Student stdt;
	stdt.std_num = getInt("Student Number : ");
	getStr(stdt.name, "Name : ");
	getStr(stdt.addr, "Address : ");
	stdt.tuition = getInt("Tuition : ");
	stdt.birthYear = getInt("Birth Year (YYYY) : ");
	return stdt;
}
void showStudentInfo(const Student data)
{
	printf("%d %s %s %d %d\n", data.std_num, data.name, data.addr, data.tuition, data.birthYear);
}

//////////////// 기능 구현부 
void addInfo(List* list)
{
	pushBack_List(list, setStudent());
}
void showInfoAll(List* list)
{
	for (int i = 0; i < list->len; i++)
	{
		showStudentInfo(getData_List(list, i));
	}
}
void searchInfoByName(List* list)
{
	Student student;
	char name[30];
	getStr(name, "검색할 이름 : ");
	for (int i = 0; i < list->len; i++)
	{
		student = getData_List(list, i);
		if (!strcmp(name, student.name))
		{
			showStudentInfo(student);
		}
	}
}
void editInfoByStdNum(List* list)
{
	Student student;
	int studentNumber = getInt("학번 : ");
	for (int i = 0; i < list->len; i++)
	{
		student = getData_List(list, i);
		if (studentNumber == student.std_num)
		{
			setData_List(list, i, setStudent());
			return;
		}
	}
}
void deleteInfoByStdNum(List* list)
{
	Student student;
	int studentNumber = getInt("학번 : ");
	for (int i = 0; i < list->len; i++)
	{
		student = getData_List(list, i);
		if (studentNumber == student.std_num)
		{
			deleteNode_List(list, i);
		}
	}
}

//////////////// 메뉴 구현부
typedef struct
{
	const char funcInfo[30];
	void(*funcs)(List* list);
} Func;

void printMenu(const Func func[], const int funcInfoLen, const int endIndex)
{
	for (int i = 0; i < funcInfoLen; i++)
	{
		printf("%d) %s\n", i + 1, func[i].funcInfo);
	}
	printf("%d) End\n", endIndex);
}
int runFunc(const Func func[], const int funcsLen, const int index, const int endIndex, List* list)
{
	if (index == endIndex)
	{
		return 2;
	}
	if (IN(index - 1, 0, funcsLen - 1))
	{
		func[index - 1].funcs(list);
		return 0;
	}
	return 1;
}
int menu(List* list, const int endIndex)
{
	const int funcInfoLen = 5;
	const Func func[] = {
		{"New student info",		addInfo},
		{"Show all student info",	showInfoAll},
		{"Search by name",			searchInfoByName},
		{"Edit student info",		editInfoByStdNum},
		{"Delete student info",		deleteInfoByStdNum},
	};

	printf("\n\n\n\n\n\n\n\n");

	printMenu(func, funcInfoLen, endIndex);
	int index = getInt("Input : ");
	int result = runFunc(func, funcInfoLen, index, endIndex, list);
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
	List* studentList = create_List();
	const char url[50] = "C:/Users/USER/Desktop/test.txt";
	loadListAllFile(url, studentList);

	while (menu(studentList, endIndex) == 0);

	writeListAllFile(url, studentList);
	studentList = destroy_List(studentList);
	printf("End\n");
}