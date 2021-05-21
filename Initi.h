#pragma once
#ifndef Init
#define Init

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
namespace DBmn {
#define T_BOOL 1
#define T_INT 2
#define T_LONG 3
#define T_FLOAT 4
#define T_DOUBLE 5
#define T_STRING 6

	typedef unsigned char byte;
	typedef struct Query;
	typedef bool(*Condition)(char *, char *);
	typedef bool(*Task)(Query&);

	// Condition Fuctions
	bool AllTrue(char *a, char *b)
	{
		return true;
	}
	bool Great(char *a, char *b) // >
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a > *b)
				return true;
		}
		return false;
	}
	bool Less(char *a, char *b) // <
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a < *b)
				return true;
		}
		return false;
	}
	bool Equal(char *a, char *b) // ==
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a != *b)
				return false;
		}
		return true;
	}
	bool NotEqual(char *a, char *b) // !=
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a != *b)
				return true;
		}
		return false;
	}
	bool GreatEqual(char *a, char *b) // >=
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a < *b)
				return false;
		}
		return true;
	}
	bool LessEqual(char *a, char *b) // <=
	{
		for (; *a != '\0' && *b != '\0'; a++, b++)
		{
			if (*a > *b)
				return false;
		}
		return true;
	}

	struct Column_list
	{
		char *name;
		char *value = nullptr;
		Column_list *next = nullptr;
	};

	struct Data_list
	{
		char *value = nullptr;
		Data_list *next = nullptr;
	};

	struct Cuase
	{
		char *col;
		char *oh;
		Condition con;
	};

	struct Cond_tree
	{
		bool me;
		Cuase cuase;
		char opts;
		Cond_tree *left = nullptr, *right = nullptr;
	};

	struct Query
	{
		char *table;
		char *table_flag;
		char *struct_flag;

		Column_list *datas = nullptr;
		Cond_tree *conditions = nullptr;

		Task execute;
	};
	struct row //contains list of fild datas
	{
		int count_columns = 0;
		Data_list *columns = nullptr;// column values
		void push(char* val) // append new column value
		{
			if (columns == nullptr)
			{
				columns = new Data_list;
				if (val != nullptr)
					columns->value = _strdup(val);
				count_columns++;
			}
			else
			{
				Data_list *w, *p = columns;
				w = new Data_list;
				while (p->next != nullptr)
					p = p->next;
				p->next = w;
				if (val != nullptr)
					w->value = _strdup(val);
				count_columns++;
			}
		}
		row *next = nullptr; // for making table data list
	};
}
#endif