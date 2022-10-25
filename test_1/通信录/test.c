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
		
		printf("请选择通讯录功能->\n");
		while ((scanf("%d", &input) != 1))
		{
			while (getchar() != '\n')
			{
			}
			printf("请输入正确的通讯录功能->\n");
		}
		//fflush(stdin);
		//rewind(stdin);
		//while ((c = getchar()) != '\n' && c != EOF);
		switch (input)
		{
		case ADD:
			printf("正在打开添加联系人信息功能\n");
			ProgressBar();
			AddContact(&ps);
			break;
		case DEL:
			printf("正在打开删除联系人信息功能\n");
			ProgressBar();
			DelContact(&ps);
			break;
		case MODIFY:
			printf("正在打开修改联系人信息功能\n");
			ProgressBar();
			ModifyContact(&ps);
			break;
		case SHOW:
			printf("正在打开显示联系人信息功能\n");
			ProgressBar();
			ShowContact(&ps);
			break;
		case EXIT:
			printf("正在打开退出通讯录功能\n");
			ProgressBar();
			printf("退出成功\n");
			return 0;
			break;
		case SEARCH:
			printf("正在打开查找联系人信息功能\n");
			ProgressBar();
			SeaContact(&ps);
			break;
		default:
			printf("功能选择错误，请重新选择！\n");
			break;
		}
	} while (input);
	return 0;
}