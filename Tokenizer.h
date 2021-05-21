#pragma once
#ifndef _Token
#define _Token

namespace DBmn_Token {
	using namespace DBmn;
	/* xxxx xxxx 0:1
	x0: string:number
	x1: data:keyword
	x2: 1 operator

	x4: 1 open list
	x5: 1 open array
	x6: 1 close last opened
	*/
#define Token_Number 0X00
#define Token_String 0X01
#define Token_Keyword 0X02
#define Token_Operator 0X04
#define Token_List 0X10
#define Token_Array 0X20
#define Token_Close 0X40

	struct Token_list
	{
		char *token;
		byte type;

		Token_list *next = nullptr;
	};

	void Add_Token(Token_list *(&q), char str[], int s, int i, byte t) {
		Token_list *p = new Token_list;
		p->token = new char[i - s + 1];
		for (int j = s; j < i; j++) {
			p->token[j - s] = str[j];
		}
		p->token[i - s] = '\0';
		p->type = t;
		if (q != nullptr)
		{
			q->next = p;
		}
		q = p;
	}

	Token_list* Tokenizer(char* query)
	{
		char d = '0';
		bool is_number = false;
		byte type = 0x00;
		Token_list *Tokens = nullptr, *p = nullptr;

		int s, i;
		for (i = 0, s = 0; query[i] != '\0'; i++)
		{
			if (d != '0') {
				if (query[i] == '\\') {
					continue;
				}
				if (d == query[i]) {
					Add_Token(p, query, s, i, Token_String | type);
					s = i + 1;
					d = '0';
					type = 0x00;
				}
			}
			else {
				switch (query[i]) {
				case '(':
					type |= Token_List;
					s = i + 1;
					continue;
				case '[':
					type |= Token_Array;
					s = i + 1;
					continue;
				case '\'':
				case '"':
				case '`':
					d = query[i];
					s = i + 1;
					continue;
				case ')':
				case ']':
					type |= Token_Close;
				case ' ':
				case '\n':
					if (s == i) {
						s = i + 1;
						continue;
					}
					if (is_number) {
						type |= Token_Number;
					}
					else {
						type |= Token_Keyword;
					}
					Add_Token(p, query, s, i, type);
					s = i + 1;
					type = 0x00;
					break;
				case '=':
				case '>':
				case '<':
				case '!':
				case '+':
				case '-':
				case '*':
				case '/':
				case '%':
				case ',':
					if (s < i) {
						if (is_number) {
							type |= Token_Number;
						}
						else {
							type |= Token_Keyword;
						}
						Add_Token(p, query, s, i, type);
						type = 0x00;
					}
					Add_Token(p, query, i, i + 1, Token_Operator);
					s = i + 1;
					break;
				default:
					is_number = (
						(query[i] > 47 && query[i] < 58)
						&&
						(is_number || s == i)
						)
						||
						(query[i] == 45 && s == i)
						||
						(s < i && is_number && query[i] == 46);
					continue;
				}
			}
			if (Tokens == nullptr && p != nullptr)
			{
				Tokens = p;
			}

		}
		if (is_number) {
			type |= Token_Number;
		}
		else {
			type |= Token_Keyword;
		}
		Add_Token(p, query, s, i, type);
		return Tokens;
	}
}
#endif