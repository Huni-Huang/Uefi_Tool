#define _CRT_SECURE_NO_WARNINGS 1
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>

contact_size ps;

int main() {
	int input = 0,c=0;
	init(&ps);
	do
	{
		SetupMenu();
		
		printf("��ѡ��ͨѶ¼����->\n");
		while ((scanf("%d", &input) != 1))
		{
			while (getchar() != '\n')
			{
			}
			printf("��������ȷ��ͨѶ¼����->\n");
		}
		//fflush(stdin);
		//rewind(stdin);
		//while ((c = getchar()) != '\n' && c != EOF);
		switch (input)
		{
		case ADD:
			printf("���ڴ������ϵ����Ϣ����\n");
			ProgressBar();
			AddContact(&ps);
			break;
		case DEL:
			printf("���ڴ�ɾ����ϵ����Ϣ����\n");
			ProgressBar();
			DelContact(&ps);
			break;
		case MODIFY:
			printf("���ڴ��޸���ϵ����Ϣ����\n");
			ProgressBar();
			ModifyContact(&ps);
			break;
		case SHOW:
			printf("���ڴ���ʾ��ϵ����Ϣ����\n");
			ProgressBar();
			ShowContact(&ps);
			break;
		case EXIT:
			printf("���ڴ��˳�ͨѶ¼����\n");
			ProgressBar();
			printf("�˳��ɹ�\n");
			return 0;
			break;
		case SEARCH:
			printf("���ڴ򿪲�����ϵ����Ϣ����\n");
			ProgressBar();
			SeaContact(&ps);
			break;
		default:
			printf("����ѡ�����������ѡ��\n");
			break;
		}
	} while (input);
	return 0;
}