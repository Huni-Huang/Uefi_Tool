#define _CRT_SECURE_NO_WARNINGS 1

#include "contact.h"
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>



void SetupMenu() {
	printf("****************************************\n");
	printf("**------------Setup  Menu-------------**\n");
	printf("**------------------------------------**\n");
	printf("**--------1.Add      2.Delete---------**\n");
	printf("**------------------------------------**\n");
	printf("**--------3.Modify   4.Show-----------**\n");
	printf("**------------------------------------**\n");
	printf("**--------5.Search   0.Exit-----------**\n");
	printf("**------------------------------------**\n");
	printf("****************************************\n");
}


int ProgressBar() {
	char* arr = NULL;
	int a = 0;
	arr = (char*)malloc(33*sizeof(char));
	if (arr==NULL)
	{
		return 0;
	}
	else {
		memset(arr, 0, 33);
	}
	while (a < 32) {
		printf("[%-31s] [%.0f%%]\r",arr, a*3.21);
		//printf("程序加载中 [%.0f%%]\r", a * 3.21);
		//fflush(stdout);
		arr[a++] = '#';
		Sleep(10);
	}
	printf("\n");
	free(arr);
	return 0;
}


void init(contact_size *ps) {
	ps->size = 0;
	ps = malloc(1000 * (sizeof(ps)));
	//printf("%zd\n", sizeof(ps));
	memset(ps, 0, sizeof(ps));
	free(ps);
}


void AddContact(contact_size *ps) {
	if (ps->size >= MAX_SIZE)
	{
		printf("通讯录已满！\n");
	}
	else
	{
		printf("请输入姓名->\n");
		scanf("%s", &ps->info[ps->size].name);
		printf("请输入性别->\n");
		scanf("%s", &ps->info[ps->size].sex);
		printf("请输入电话->\n");
		scanf("%s", &ps->info[ps->size].tel);
		printf("请输入地址->\n");
		scanf("%s", &ps->info[ps->size].addr);
		printf("请输入年龄->\n");
		while ((scanf("%d", &ps->info[ps->size].age)!=1))
		{
			while (getchar()!='\n')
			{}
			printf("年龄输入错误，请重新输入->\n");
		}
		printf("添加[%s]成功！\n", &ps->info[ps->size].name);
		ps->size++;
	}
}

void ShowContact(contact_size* ps) {
	if (ps->size ==0)
	{
		printf("当前没有联系人信息，请添加！\n");
	}
	else {
		printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "姓名", "性别", "电话", "地址", "年龄");
		for (int i = 0; i < ps->size; i++)
		{
			printf("%-10s\t%-4s\t%-12s\t%-20s\t%d\n",
				ps->info[i].name,
				ps->info[i].sex,
				ps->info[i].tel,
				ps->info[i].addr,
				ps->info[i].age);
		}
		
	}
}

static int findname(contact_size* ps,char* name) {
	int i = 0;
	for ( i = 0; i < ps->size; i++)
	{
		if (strcmp(name, ps->info[i].name) == 0) {
			return i;
		}
	}
	return -1;
}

static int findtel(contact_size* ps,char *tel) {
	int i = 0;
	for ( i = 0; i < ps->size; i++)
	{
		if (strcmp(tel,ps->info[i].tel)== 0)
		{
			return i;
		}	
	}
	return -1;
}


void SeaContact(contact_size* ps) {
	int input = 0,result;
	char name[MAX_NAME];
	char tel[MAX_TEL];
	if (ps->size == 0)
	{
		printf("当前没有联系人信息，查找失败！\n");
	}
	else
	{
		do
		{
			printf("请选择查找方式或退出：1.姓名  2.电话  0.退出\n");
			while ((scanf("%d", &input)!=1))
			{
				while (getchar() !='\n')
				{
				}
				printf("请输入正确的查找功能->\n");
			}
			switch (input)
			{
			case 1:
				printf("请输入联系人姓名->\n");
				scanf("%s", name);
				result = findname(ps,name);
				if (result!=-1)
				{
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "姓名", "性别", "电话", "地址", "年龄");
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%d\n",
						ps->info[findname(ps, name)].name,
						ps->info[findname(ps, name)].sex,
						ps->info[findname(ps, name)].tel,
						ps->info[findname(ps, name)].addr,
						ps->info[findname(ps, name)].age);
				}
				else
				{
					printf("没找到[%s]，请确认是否输入正确.\n",name);
				}
				break;
			case 2:
				printf("请输入联系人电话->\n");
				scanf("%s", tel);
				result = findtel(ps, tel);
				if (result != -1)
				{
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "姓名", "性别", "电话", "地址", "年龄");
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%d\n",
						ps->info[findtel(ps, tel)].name,
						ps->info[findtel(ps, tel)].sex,
						ps->info[findtel(ps, tel)].tel,
						ps->info[findtel(ps, tel)].addr,
						ps->info[findtel(ps, tel)].age);
				}
				else
				{
					printf("没找到[%s]，请确认是否输入正确.\n", tel);
				}
				break;
			case 0:
				break;
			default:
				printf("当前没有[%d]功能，请重新选择\n",input);
				break;
			}
		} while (input);
		
	}
}


void DelContact(contact_size* ps) {
	char name[MAX_NAME];
	int result = 0,input = 0,i;
	if (ps->size==0)
	{
		printf("通讯录为空.\n");
	}
	else {
		start:
		printf("请输入删除人的名字\n");
		//start1:
		scanf("%s", name);
		result =findname(ps, name);
		if (result == -1)
		{
			printf("您要删除的联系人不存在\n");
			goto start;
		}
		else
		{
			start3:
			printf("请确认是否删除-> 1.确认删除  0.取消删除\n");
			while ((scanf("%d", &input)!=1))
			{
				while (getchar()!='\n')
				{
				}
				printf("输入无效，请重新输入\n");
			}
			switch (input)
			{
			case 1:
				ProgressBar();
				for (i = result; i < ps->size-1; i++)
				{
					ps->info[i] = ps->info[i + 1];
				}
				printf("删除[%s]成功->\n", ps->info[result].name);
				ps->size--;
				break;
			case 0:
				ProgressBar();
				printf("取消删除成功，请重新输入删除人姓名->\n");
				//goto start1;
				break;
			default:
				printf("功能选择错误，请重新选择\n");
				goto start3;
				break;
			}

		}
	}
}


void ModifyContactName(contact_size*ps,int ret) {
	char name[MAX_NAME] = {0};
	printf("请输入新的联系人姓名->\n");
	scanf("%s", name);
	if (strcmp(ps->info[ret].name,name)!=0)
	{
		printf("正在修改，请稍等->\n");
		ProgressBar();
		strcpy(ps->info[ret].name, name);
		printf("修改成功，联系人姓名已修改为[%s]->\n",name);
		
	}
	else {
		printf("修改失败，修改姓名与当前姓名一致->\n");
	}
}


void ModifyContactSex(contact_size* ps, int ret) {
	char sex[MAX_SEX] = { 0 };
	printf("请输入新的联系人性别->\n");
	scanf("%s", sex);
	if (strcmp(ps->info[ret].sex, sex) != 0)
	{
		printf("正在修改，请稍等->\n");
		ProgressBar();
		strcpy(ps->info[ret].sex, sex);
		printf("修改成功，联系人性别已修改为[%s]->\n", sex);

	}
	else {
		printf("修改失败，修改性别与当前性别一致->\n");
	}
}


void ModifyContactTel(contact_size* ps, int ret) {
	char tel[MAX_TEL] = { 0 };
	printf("请输入新的联系人电话->\n");
	scanf("%s", tel);
	if (strcmp(ps->info[ret].tel, tel) != 0)
	{
		printf("正在修改，请稍等->\n");
		ProgressBar();
		strcpy(ps->info[ret].tel, tel);
		printf("修改成功，联系人性别已修改为[%s]->\n", tel);

	}
	else {
		printf("修改失败，修改电话与当前电话一致->\n");
	}
}


void ModifyContactAddr(contact_size* ps, int ret) {
	char addr[MAX_ADDR] = { 0 };
	printf("请输入新的联系人地址->\n");
	scanf("%s", addr);
	if (strcmp(ps->info[ret].addr, addr) != 0)
	{
		printf("正在修改，请稍等->\n");
		ProgressBar();
		strcpy(ps->info[ret].addr, addr);
		printf("修改成功，联系人性别已修改为[%s]->\n", addr);

	}
	else {
		printf("修改失败，修改地址与当前地址一致->\n");
	}
}

void ModifyContactAge(contact_size* ps, int ret) {
	int age = 0;
	printf("请输入新的联系人年龄->\n");
	while ((scanf("%d", &age))!=1)
	{
		while (getchar()!='\n')
		{
		}
		printf("请确认输入格式是否正确并重新输入->");
	}
	if (ps->info[ret].age != age)
	{
		printf("正在修改，请稍等->\n");
		ProgressBar();
		ps->info[ret].age = age;
		printf("修改成功，联系人年龄已修改为[%d]->\n", age);
	}
	else {
		printf("修改失败，修改年龄与当前年龄一致->\n");
	}
}


void ModifyContact(contact_size* ps) {
	char input[MAX_NAME];
	int ret = 0,modify = 0;
	if (ps->size == 0)
	{
		printf("当前通讯录为空，请添加->\n");
	}
	else
	{
		printf("请输入修改联系人姓名->\n");
		scanf("%s", input);
		ret =  findname(ps, input);
		if (ret == -1)
		{
			printf("您要修改的联系人不存在\n");
		}
		else
		{
			do
			{
				printf("请选择您要修改的信息-> 1.姓名 2.性别 3.电话 4.地址 5.年龄 6.显示 0.取消\n");
				while ((scanf("%d", &modify))!=1)
				{
					while (getchar() != '\n')
					{
					}
					printf("输入无效\n请重新选择您要修改的信息-> 1.姓名 2.性别 3.电话 4.地址 5.年龄 0.取消\n");
				}
				switch (modify)
				{
				case 1:
					ModifyContactName(ps, ret);
					break;
				case 2:
					ModifyContactSex(ps, ret);
					break;
				case 3:
					ModifyContactTel(ps, ret);
					break;
				case 4:
					ModifyContactAddr(ps, ret);
					break;
				case 5:
					ModifyContactAge(ps, ret);
					break;
				case 6:
					printf("正在显示通讯录信息->\n");
					ProgressBar();
					ShowContact(ps);
					break;
				case 0:
					printf("正在取消->\n");
					ProgressBar();
					break;
				default:
					printf("修改功能输入错误->\n");
					ProgressBar();
					break;
				}
			} while (modify!=0);
			
		}
	}
}
