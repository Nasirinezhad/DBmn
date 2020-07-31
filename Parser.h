#ifndef _Parser
#define _Parser

#include "Initi.h"
#include "FS.h"
#include "Tokenizer.h"
namespace parser
{
	Query Parse(Token_list *tokens){
		Query q;
		if (tokens != nullptr && tokens->next != nullptr)
		{
			if (tokens->type & 0x0F == Token_Keyword){
				int s = look(tokens->token);
				switch (s)
				{
				case 3769925: //create
					q.table_flag = "w"; //w for open new file
					q.struct_flag = "w";
					q.task = &FS::Create; //Create function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						tokens = tokens->next->next;
						if (tokens != nullptr && exp_struct(tokens, q)){//expects struct filds
							return q;
						}
					}
					/* error */
					break;
				case 59496: //drop
					q.table_flag = nullptr; //null for delete file
					q.struct_flag = nullptr;
					q.task = &FS::Drop; //Drop function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						return q;
					}
					break;
				case 3957572: //insert
					q.table_flag = "a"; //a for open file to append data
					q.struct_flag = "r"; //r for open file to read data
					q.task = &FS::Insert; //Insert function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						tokens = tokens->next->next;
						if (tokens != nullptr && exp_data(tokens, q)){//expects datas
							return q;
						}
					}
					break;
				case 4351045: //update
					q.table_flag = "r+"; //r+ for open file to edit data
					q.struct_flag = "r"; //r for open file to read data
					q.task = &FS::Update; //Update function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						tokens = tokens->next->next;
						if (tokens != nullptr && exp_data(tokens, q)){//expects datas
							exp_where(tokens, q); //expects where cuases
							return q;
						}
					}
					break;
				case 3753285: //delete
					q.table_flag = "r+"; //r+ for open file to edit data
					q.struct_flag = "r"; //r for open file to read data
					q.task = &FS::Delete; //Delete function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						tokens = tokens->next->next;
						if (tokens != nullptr && exp_where(tokens, q)){//expects where cuases
							return q;
						}
					}
					break;
				case 4244684: //select
					q.table_flag = "r"; //r for open file to read data
					q.struct_flag = "r"; //r for open file to read data
					q.task = &FS::Select; //Select function in file manager
					if (exp_table(tokens->next, q.table)) //expects next token be table name
					{
						tokens = tokens->next->next;
						exp_where(tokens, q);//expects where cuases
						return q;
					}
					break;
				default:
					/* error */
					break;
				}
			}
			else
			{
				/* error */
			}
		}
	}

	bool exp_table(Token_list *token, char *(&table)){
		if (token->type & 0x0F == Token_String)
		{
			table = strdup(token->token);
			return true;
		}
		return false;
	}

	bool exp_struct(Token_list *(&tokens), Query &q){
		if (tokens->type & 0xF0 == Token_List) // open (
		{
			Column_list *p = nullptr;
			unsigned char t;
			while (tokens->type&Token_Close != Token_Close) // close )
			{
				t = tokens->type & 0x0F;
				if (t == Token_String) // column name
				{
					if (p == nullptr)
					{
						q.datas = new Column_list;
						p = q.datas;
					}
					else
					{
						p->next = new Column_list;
						p = n->next;
						p->name = strdup(tokens->token);
					}
					tokens = tokens->next;
				}
				else
				{
					/* error */
					return false;
				}
			}
		}
		return true;
	}
	bool exp_data(Token_list *(&tokens), Query &q){
		if (tokens->type & 0xF0 == Token_List) // open (
		{
			Column_list *p = nullptr;
			unsigned char t;
			while (tokens->type&Token_Close != Token_Close) // close )
			{
				t = tokens->type & 0x0F;
				if (t == Token_Keyword) // column name
				{
					if (p == nullptr)
					{
						q.datas = new Column_list;
						p = q.datas;
					}
					else
					{
						p->next = new Column_list;
						p = n->next;
						p->name = strdup(tokens->token);
					}
					tokens = tokens->next;
				}
				else
				if (p != nullptr) //column name must be set
				{
					if (t == Token_Operator)
					{
						if (*tokens->token == '=')
						{
							tokens = tokens->next;
							if (tokens->type & 0x0F == Token_String | Token_Number)
							{
								p->value = strdup(tokens->token);
							}
							else
							{
								/* error */
								return false;
							}
						}
						else
						{
							/* error */
							return false;
						}
						tokens = tokens->next;
					}
					else
					{
						/* error */
						return false;
					}
				}
				else
				{
					/* error */
					return false;
				}
			}
		}
		return true;
	}
	bool exp_where(Token_list *(&tokens), Query &q){
		if (token->type & 0x0F == Token_Keyword && look(tokens->token) == 548149)
		{
			tokens = tokens->next;
			int bruc = 0, st;
			bool bc = false;
			Cond_tree *p = nullptr, *y = nullptr, *w = nullptr, *r = nullptr;
			unsigned char t;
			while (tokens != nullptr)
			{
				t = tokens->type & 0x0F;
				st = look(tokens->token);
				if (tokens->type&Token_List == Token_List){
					bruc++;
					bc = false;
				}
				if (t == Token_Keyword) 
				{
					if (st == 7188 || st == 1002) // and / or
					{
						if (bc)
						{
							bc = false;
							if (r == nullptr)
							{
								r = new Cond_tree;
								r->me = false;
								r->left = p;
								r->opts = st==7188 ? '&':'|';
							}else
							{
								if (r->right == nullptr)
								{
									r->right = p;
								}else{
									y = r;
									r = new Cond_tree;
									r->me = false;
									r->left = y;
									r->right = p;
									r->opts = st==7188 ? '&':'|';
								}
							}
							p = nullptr;
						}else
						if (p == nullptr)
						{
							p = new Cond_tree;
							p->opts = st==7188 ? '&':'|';
							p->me = false;
							p->left = w;
						}else{
							if (p->right == nullptr)
							{
								p->right = w;
							}else{
								y = p;
								p = new Cond_tree;
								p->opts = st==7188 ? '&':'|';
								p->me = false;
								p->left = y;
							}
						}
						
					}else // column name
					{
						w = new Cond_tree;
						w->cuase.col = strdup(tokens->token);
						tokens = tokens->next;
						if (tokens->type&0x0f == Token_Operator)
						{
							if (tokens->next->type&0x0f == Token_Operator)
							{
								w->cuase.con = condLook(tokens->token, tokens->next->token);
							}else{
								w->cuase.con = condLook(tokens->token, nullptr);
							}
						}else
						{
							/* error */
							return false;
						}
						tokens = tokens->next;
						if (tokens->type&Token_Close == Token_Close){
							bruc--;
							bc = true;
						}
						if (tokens->type & 0x0F == Token_String | Token_Number)
						{
							w->cuase.oh = strdup(tokens->token);
						}
						else
						{
							/* error */
						}
						
					}
				}else
				{
					/* error */
				}
				tokens = tokens->next;
			}

			if (r == nullptr) 
			{
				if (p == nullptr)
				{
					if (w == nullptr)
					{
						q.conditions = nullptr;
						return false;
					}else
					{
						r = w;
						r->me = false;
					}
				}else
				{
					r = w;
					r->me = false;
				}
			}
			q.conditions = r;
			return true;
		}
		return false;
	}
	int look(char *p){
		int s = 0;
		for (; *p; ++p) {
			*p = tolower(*p);
			s = (s << 3) + *p;
		}
		return s;
	}
	Condition condLook(char *op1, char *op2=nullptr){
		if (op2 == nullptr)
		{
			switch (*op1)
			{
			case '>':
				return &Great;
				break;
			case '<':
				return &Less;
				break;
			}
		}else
		if (*op2 == '=')
		{
			switch (*op1)
			{
			case '!':
				return &NotEqual;
				break;
			case '>':
				return &GreatEqual;
				break;
			case '<':
				return &LessEqual;
				break;
			case '=':
				return &Equal;
				break;
			}
		}
		/* error */
	}
} // namespace parser, 

#endif
