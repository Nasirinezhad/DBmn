#ifndef Init
#define Init

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define T_BOOL 1
#define T_INT 2
#define T_LONG 3
#define T_FLOAT 4
#define T_DOUBLE 5
#define T_STRING 6

typedef struct Query;
typedef bool(*Condition)(char *, char *);
typedef bool(*Task)(Query&);

// Condition Fuctions
bool Great(char *a, char *b) // >
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str > *b.str)
                return true;
        }
    return false;    
}
bool Less(char *a, char *b) // <
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str < *b.str)
                return true;
        }
    return false;
}
bool Equal(char *a, char *b) // ==
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str != *b.str)
                return false;
        }
        return true;
}
bool NotEqual(char *a, char *b) // !=
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str != *b.str)
                return true;
        }
    return false;
}
bool GreatEqual(char *a, char *b) // >=
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str < *b.str)
                return false;
        }
        return true;
}
bool LessEqual(char *a, char *b) // <=
{
    for (; *a.str != '\0' && *b.str != '\0'; a.str++, b.str++)
        {
            if (*a.str > *b.str)
                return false;
        }
        return true;
}

	struct Column_list
	{
		char *name;
		char *value;
		Column_list *next = nullptr;
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
		Cond_list *left = nullptr, *right = nullptr;
	};

	struct Query
	{
		char *table;
		char *table_flag;
		char *struct_flag;

		Column_list *datas;
		Cond_tree *conditions;

		Task execute;
	};

#endif