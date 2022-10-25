#define MAX_NAME 10
#define MAX_SEX 4
#define MAX_TEL 12
#define MAX_ADDR 20
#define MAX_SIZE 1000
#include <stdio.h>
#include <stdlib.h>


typedef struct Contact_infor
{
	char name[MAX_NAME];
	char sex[MAX_SEX];
	char tel[MAX_TEL];
	char addr[MAX_ADDR];
	int age;
}contact_infor;

typedef struct Contact_size
{
	contact_infor info[MAX_SIZE];
	int size;
}contact_size;

enum Option
{
	EXIT,
	ADD,
	DEL,
	MODIFY,
	SHOW,
	SEARCH

};


void SetupMenu();
int ProgressBar();
void init(contact_size* ps);
void AddContact(contact_size* ps);
void ShowContact(contact_size* ps);
void SeaContact(contact_size* ps);
void DelContact(contact_size* ps);
void ModifyContact(contact_size* ps);