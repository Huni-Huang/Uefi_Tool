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
		//printf("��������� [%.0f%%]\r", a * 3.21);
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
		printf("ͨѶ¼������\n");
	}
	else
	{
		printf("����������->\n");
		scanf("%s", &ps->info[ps->size].name);
		printf("�������Ա�->\n");
		scanf("%s", &ps->info[ps->size].sex);
		printf("������绰->\n");
		scanf("%s", &ps->info[ps->size].tel);
		printf("�������ַ->\n");
		scanf("%s", &ps->info[ps->size].addr);
		printf("����������->\n");
		while ((scanf("%d", &ps->info[ps->size].age)!=1))
		{
			while (getchar()!='\n')
			{}
			printf("���������������������->\n");
		}
		printf("���[%s]�ɹ���\n", &ps->info[ps->size].name);
		ps->size++;
	}
}

void ShowContact(contact_size* ps) {
	if (ps->size ==0)
	{
		printf("��ǰû����ϵ����Ϣ������ӣ�\n");
	}
	else {
		printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "����", "�Ա�", "�绰", "��ַ", "����");
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
		printf("��ǰû����ϵ����Ϣ������ʧ�ܣ�\n");
	}
	else
	{
		do
		{
			printf("��ѡ����ҷ�ʽ���˳���1.����  2.�绰  0.�˳�\n");
			while ((scanf("%d", &input)!=1))
			{
				while (getchar() !='\n')
				{
				}
				printf("��������ȷ�Ĳ��ҹ���->\n");
			}
			switch (input)
			{
			case 1:
				printf("��������ϵ������->\n");
				scanf("%s", name);
				result = findname(ps,name);
				if (result!=-1)
				{
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "����", "�Ա�", "�绰", "��ַ", "����");
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%d\n",
						ps->info[findname(ps, name)].name,
						ps->info[findname(ps, name)].sex,
						ps->info[findname(ps, name)].tel,
						ps->info[findname(ps, name)].addr,
						ps->info[findname(ps, name)].age);
				}
				else
				{
					printf("û�ҵ�[%s]����ȷ���Ƿ�������ȷ.\n",name);
				}
				break;
			case 2:
				printf("��������ϵ�˵绰->\n");
				scanf("%s", tel);
				result = findtel(ps, tel);
				if (result != -1)
				{
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%s\n", "����", "�Ա�", "�绰", "��ַ", "����");
					printf("%-10s\t%-4s\t%-12s\t%-20s\t%d\n",
						ps->info[findtel(ps, tel)].name,
						ps->info[findtel(ps, tel)].sex,
						ps->info[findtel(ps, tel)].tel,
						ps->info[findtel(ps, tel)].addr,
						ps->info[findtel(ps, tel)].age);
				}
				else
				{
					printf("û�ҵ�[%s]����ȷ���Ƿ�������ȷ.\n", tel);
				}
				break;
			case 0:
				break;
			default:
				printf("��ǰû��[%d]���ܣ�������ѡ��\n",input);
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
		printf("ͨѶ¼Ϊ��.\n");
	}
	else {
		start:
		printf("������ɾ���˵�����\n");
		//start1:
		scanf("%s", name);
		result =findname(ps, name);
		if (result == -1)
		{
			printf("��Ҫɾ������ϵ�˲�����\n");
			goto start;
		}
		else
		{
			start3:
			printf("��ȷ���Ƿ�ɾ��-> 1.ȷ��ɾ��  0.ȡ��ɾ��\n");
			while ((scanf("%d", &input)!=1))
			{
				while (getchar()!='\n')
				{
				}
				printf("������Ч������������\n");
			}
			switch (input)
			{
			case 1:
				ProgressBar();
				for (i = result; i < ps->size-1; i++)
				{
					ps->info[i] = ps->info[i + 1];
				}
				printf("ɾ��[%s]�ɹ�->\n", ps->info[result].name);
				ps->size--;
				break;
			case 0:
				ProgressBar();
				printf("ȡ��ɾ���ɹ�������������ɾ��������->\n");
				//goto start1;
				break;
			default:
				printf("����ѡ�����������ѡ��\n");
				goto start3;
				break;
			}

		}
	}
}


void ModifyContactName(contact_size*ps,int ret) {
	char name[MAX_NAME] = {0};
	printf("�������µ���ϵ������->\n");
	scanf("%s", name);
	if (strcmp(ps->info[ret].name,name)!=0)
	{
		printf("�����޸ģ����Ե�->\n");
		ProgressBar();
		strcpy(ps->info[ret].name, name);
		printf("�޸ĳɹ�����ϵ���������޸�Ϊ[%s]->\n",name);
		
	}
	else {
		printf("�޸�ʧ�ܣ��޸������뵱ǰ����һ��->\n");
	}
}


void ModifyContactSex(contact_size* ps, int ret) {
	char sex[MAX_SEX] = { 0 };
	printf("�������µ���ϵ���Ա�->\n");
	scanf("%s", sex);
	if (strcmp(ps->info[ret].sex, sex) != 0)
	{
		printf("�����޸ģ����Ե�->\n");
		ProgressBar();
		strcpy(ps->info[ret].sex, sex);
		printf("�޸ĳɹ�����ϵ���Ա����޸�Ϊ[%s]->\n", sex);

	}
	else {
		printf("�޸�ʧ�ܣ��޸��Ա��뵱ǰ�Ա�һ��->\n");
	}
}


void ModifyContactTel(contact_size* ps, int ret) {
	char tel[MAX_TEL] = { 0 };
	printf("�������µ���ϵ�˵绰->\n");
	scanf("%s", tel);
	if (strcmp(ps->info[ret].tel, tel) != 0)
	{
		printf("�����޸ģ����Ե�->\n");
		ProgressBar();
		strcpy(ps->info[ret].tel, tel);
		printf("�޸ĳɹ�����ϵ���Ա����޸�Ϊ[%s]->\n", tel);

	}
	else {
		printf("�޸�ʧ�ܣ��޸ĵ绰�뵱ǰ�绰һ��->\n");
	}
}


void ModifyContactAddr(contact_size* ps, int ret) {
	char addr[MAX_ADDR] = { 0 };
	printf("�������µ���ϵ�˵�ַ->\n");
	scanf("%s", addr);
	if (strcmp(ps->info[ret].addr, addr) != 0)
	{
		printf("�����޸ģ����Ե�->\n");
		ProgressBar();
		strcpy(ps->info[ret].addr, addr);
		printf("�޸ĳɹ�����ϵ���Ա����޸�Ϊ[%s]->\n", addr);

	}
	else {
		printf("�޸�ʧ�ܣ��޸ĵ�ַ�뵱ǰ��ַһ��->\n");
	}
}

void ModifyContactAge(contact_size* ps, int ret) {
	int age = 0;
	printf("�������µ���ϵ������->\n");
	while ((scanf("%d", &age))!=1)
	{
		while (getchar()!='\n')
		{
		}
		printf("��ȷ�������ʽ�Ƿ���ȷ����������->");
	}
	if (ps->info[ret].age != age)
	{
		printf("�����޸ģ����Ե�->\n");
		ProgressBar();
		ps->info[ret].age = age;
		printf("�޸ĳɹ�����ϵ���������޸�Ϊ[%d]->\n", age);
	}
	else {
		printf("�޸�ʧ�ܣ��޸������뵱ǰ����һ��->\n");
	}
}


void ModifyContact(contact_size* ps) {
	char input[MAX_NAME];
	int ret = 0,modify = 0;
	if (ps->size == 0)
	{
		printf("��ǰͨѶ¼Ϊ�գ������->\n");
	}
	else
	{
		printf("�������޸���ϵ������->\n");
		scanf("%s", input);
		ret =  findname(ps, input);
		if (ret == -1)
		{
			printf("��Ҫ�޸ĵ���ϵ�˲�����\n");
		}
		else
		{
			do
			{
				printf("��ѡ����Ҫ�޸ĵ���Ϣ-> 1.���� 2.�Ա� 3.�绰 4.��ַ 5.���� 6.��ʾ 0.ȡ��\n");
				while ((scanf("%d", &modify))!=1)
				{
					while (getchar() != '\n')
					{
					}
					printf("������Ч\n������ѡ����Ҫ�޸ĵ���Ϣ-> 1.���� 2.�Ա� 3.�绰 4.��ַ 5.���� 0.ȡ��\n");
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
					printf("������ʾͨѶ¼��Ϣ->\n");
					ProgressBar();
					ShowContact(ps);
					break;
				case 0:
					printf("����ȡ��->\n");
					ProgressBar();
					break;
				default:
					printf("�޸Ĺ����������->\n");
					ProgressBar();
					break;
				}
			} while (modify!=0);
			
		}
	}
}
