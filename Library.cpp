#include "DBmn.h"
#include <conio.h>
using namespace std;

void addBook(char *isbn, char *name, char *author)
{
	char query[1024] = "Insert 'books' (name='";
	strcat(query, name);
	strcat(query, "',author='");
	strcat(query, author);
	strcat(query, "',isbn='");
	strcat(query, isbn);
	strcat(query, "')");
	entryQuery(query);
}
void updateBook(char *isbn, char *name, char *author = nullptr)
{
	char query[1024] = "Update 'books' (";
	if (name != nullptr) {
		strcat(query, "name='");
		strcat(query, name);
		if (author != nullptr)
			strcat(query, "',");
	}
	if (author != nullptr) {
		strcat(query, "author='");
		strcat(query, author);
	}
	strcat(query, "') Where isbn=");
	strcat(query, isbn);
	entryQuery(query);
}
void deleteBook(char *isbn)
{
	char query[1024] = "Delete 'books' Where isbn=='";
	strcat(query, isbn);
	strcat(query, "'");
	entryQuery(query);
}
DBmn::row* getBooks(char *isbn = nullptr, char *name = nullptr, char *author = nullptr)
{
	char query[1024] = "Select 'books' ";
	bool w = false;
	if (isbn != nullptr)
	{
		if (w) {
			strcat(query, " Where isbn=='");
			strcat(query, isbn);
			strcat(query, "'");
		}
	}
	if (name != nullptr)
	{
		if (w) {
			strcat(query, " Where name=='");
			strcat(query, name);
			strcat(query, "'");
		}
		else {
			strcat(query, " and name=='");
			strcat(query, name);
			strcat(query, "'");
		}
	}
	if (author != nullptr)
	{
		if (w) {
			strcat(query, " Where author=='");
			strcat(query, author);
			strcat(query, "'");
		}
		else {
			strcat(query, " and author=='");
			strcat(query, author);
			strcat(query, "'");
		}
	}

	return entryQuery(query);
}
void install()
{
	entryQuery("Create 'books' ('isbn','name','author')");
}

void main()
{
	char k;
	char isbn[64], author[64], name[64];
	while (true)
	{
		printf("1.add book\n2.edit book\n3.remove book\n4.find book\n5.list of books\n0.exit\n");
		k = _getch();
		switch (k)
		{
		case '1':
			printf("Enter isbn: ");
			scanf("%s", isbn);
			printf("Enter name: ");
			scanf("%s", name);
			printf("Enter author: ");
			scanf("%s", author);
			addBook(isbn, name, author);
			break;
		case '2':
			printf("Enter isbn of book to edit: ");
			scanf("%s", isbn);
			printf("Enter new name: ");
			scanf("%s", name);
			printf("Enter new author: ");
			scanf("%s", author);
			updateBook(isbn, name, author);
			break;
		case '3':
			printf("Enter isbn of book to delete: ");
			scanf("%s", isbn);
			deleteBook(isbn);
			break;
		case '4': {
			printf("Enter name of book loking for: ");
			scanf("%s", name);
			DBmn::row *q = getBooks(nullptr, name);
			if (q == nullptr || q->columns == nullptr)
			{
				printf("Not Found!");
			}
			else {
				printf("isbn \t name \t author\n");
				while (q != nullptr) {
					printf("%s \t %s \t %s \n", q->columns->value, q->columns->next->value, q->columns->next->next->value);
					q = q->next;
				}
			}
		}break;
		case '5':{
			DBmn::row *q = getBooks();
			if (q == nullptr || q->columns == nullptr)
			{
				printf("No Book Found!");
			}
			else {
				printf("isbn \t name \t author\n");
				while (q != nullptr) {
					printf("%s \t %s \t %s \n", q->columns->value, q->columns->next->value, q->columns->next->next->value);
					q = q->next;
				}
			}
		}break;
		case '6':
			install();
			break;
		}
	}
}