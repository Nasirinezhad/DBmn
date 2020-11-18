#ifndef _FS
#define _FS

#include "Initi.h"
namespace FS
{
	/*
		data structures
	*/
	struct table
	{
		bool isOpen = false;
		FILE *stream, *stc;
		row columns;
		row *data = nullptr;
	}Tmp;

	/*
		Identifing Functions
	*/
	bool Conditional(Query &, row *);
	void par(Data_list *, Data_list *);
	void reStore(Query &, row *);


	/*
		Methods
	*/
	bool Open(Query &me)
	{
		if (Tmp.isOpen)
			return true;
		char *table = _strdup(me.table);
		Tmp.stc = fopen(strcat(table, "_struct.csv"), me.struct_flag);
		if (Tmp.stc == NULL)
			return false;
		table = _strdup(me.table);
		Tmp.stream = fopen(strcat(table, ".csv"), me.table_flag);
		if (Tmp.stream == NULL)
			return false;
		Tmp.isOpen = true;
		if (Tmp.data != nullptr)
		{
			row *p;
			while (Tmp.data != nullptr)
			{
				p = Tmp.data->next;
				delete Tmp.data;
				Tmp.data = p;
			}
		}
		return true;
	}

	void Close(){
		_fcloseall();
		Tmp.isOpen = false;
		Data_list *k;
		Tmp.columns.count_columns = 0;
		while (Tmp.columns.columns != nullptr)
		{
			k = Tmp.columns.columns->next;
			delete Tmp.columns.columns;
			Tmp.columns.columns = k;
		}
	}

	bool LoadStruct(Query &me) //load fild names of file
	{
		bool r = false;
		if (Tmp.isOpen)
		{
			char buf[2048];
			fgets(buf, 2048, Tmp.stc);
			char* token = strtok(buf, ";");
			while (token) {
				Tmp.columns.push(token);
				token = strtok(NULL, ";");
				r = true;
			}
		}
		return r;
	}
	row* fetchRow() // read a line of file and explode files
	{
		row *r = new row;
		char buf[2048];
		if (fgets(buf, 2048, Tmp.stream)){
			char* token = strtok(buf, ";");
			for (int i = 0; i < Tmp.columns.count_columns; i++)
			{
				r->push(token);
				token = strtok(NULL, ";");
			}
			return r;
		}
		return nullptr;
	}
	Data_list *reStruct(Query &me) // struct exist datas to table struture sort
	{
		bool g = false;
		row r;
		Column_list *p;
		Data_list *w = Tmp.columns.columns;
		while (w != nullptr)
		{
			fputs("\n", Tmp.stream);
			p = me.datas;
			g = true;
			while (p != nullptr)
			{
				if (strcmp(p->name, w->value) == 0)
				{
					r.push(p->value);
					g = false;
					break;
				}
				p = p->next;
			}
			if (g)
				r.push(nullptr);
			w = w->next;
		}
		return r.columns;
	}
	bool Create(Query &me)
	{
		bool r = false;
		if (Open(me))
		{
			while (me.datas != nullptr)
			{
				fputs(me.datas->name, Tmp.stc);
				fputs(";", Tmp.stc);
				r = true;
				me.datas = me.datas->next;
			}
		}
		return r;
	}
	bool Drop(Query &me)
	{
		char *table = _strdup(me.table);
		if (remove(strcat(table, ".csv")) == 0)
		if (remove(strcat(me.table, "_struct.csv")) == 0)
			return true;
		return false;
	}
	bool Insert(Query &me)
	{
		bool r = false;
		if (Open(me))
		{
			if (LoadStruct(me))
			{
				Column_list *p, *pt;
				Data_list *w = Tmp.columns.columns;
				while (w != nullptr)
				{
					p = me.datas;
					while (p != nullptr && w->value != nullptr)
					{
						if (strcmp(p->name, w->value) == 0)
						{
							fputs(p->value, Tmp.stream);
							r = true;
							break;
						}
						p = p->next;
					}
					fputs(";", Tmp.stream);
					w = w->next;
				}
				if (r)
					fputs("\n", Tmp.stream);
			}
		}
		return r;
	}
	bool Update(Query &me)
	{
		bool r = false;
		if (Open(me))
		{
			if (LoadStruct(me))
			{
				row *all, *p;
				Data_list *datas;
				datas = reStruct(me);
				all = fetchRow();
				p = all;
				if (Conditional(me, p))
					par(p->columns, datas);
				while (p->next = fetchRow(), p->next != nullptr){
					if (Conditional(me, p->next))
						par(p->next->columns, datas);
					p = p->next;
				}
				reStore(me, all);
				r = true;
			}
		}
		return r;
	}
	bool Delete(Query &me)
	{
		bool r = false;
		if (Open(me))
		{
			if (LoadStruct(me))
			{
				row *all, *p;
				all = fetchRow();
				p = all;
				while (p != nullptr && Conditional(me, p)){
					delete p;
					all = fetchRow();
					p = all;
				}
				while (p->next = fetchRow(), p->next != nullptr){
					if (Conditional(me, p->next)){
						delete p->next;
						p->next = nullptr;
					}
					else{
						p = p->next;
					}
				}
				reStore(me, all);
				r = true;
			}
		}
		return r;
	}
	bool Select(Query &me)
	{
		bool r = false;
		if (Open(me))
		{
			if (LoadStruct(me))
			{
				row *p;
				Tmp.data = fetchRow();
				p = Tmp.data;
				while (p != nullptr && !Conditional(me, p)){
					delete p;
					Tmp.data = fetchRow();
					p = Tmp.data;
				}
				while (p->next = fetchRow(), p->next != nullptr){
					if (!Conditional(me, p->next)){
						delete p->next;
						p->next = nullptr;
					}
					else{
						p = p->next;
					}
				}
				r = true;
			}
		}
		return r;
	}

	Column_list* row2col(Data_list *r) // a structed row to Column list (name & value)
	{
		Column_list *c = nullptr, *w = nullptr, *y = nullptr;
		Data_list *p = Tmp.columns.columns;
		while (r != nullptr)
		{
			if (c == nullptr)
			{
				c = new Column_list;
				c->name = _strdup(p->value);
				c->value = _strdup(r->value);
				w = c;
			}
			else{
				y = new Column_list;
				y->name = _strdup(p->value);
				y->value = _strdup(r->value);
				w->next = y;
				w = w->next;
			}
			r = r->next;
			p = p->next;
		}
		return c;
	}

	char *columnVal(Column_list *p, char *name) // return value of a column in column list
	{
		while (p != nullptr)
		{
			if (strcmp(p->name, name) == 0)
				return p->value;
			p = p->next;
		}
		return nullptr;
	}

	void check(Cond_tree *p, Column_list *(&r))
	{
		if (p->left == nullptr && p->right == nullptr){
			p->me = p->cuase.con(columnVal(r, p->cuase.col), p->cuase.oh);
		}
		else{
			bool l, r;
			if (p->left == nullptr)
				l = false;
			else{
				l = p->left->me;
				p->left->me = false;
			}
			if (p->right == nullptr)
				r = false;
			else{
				r = p->right->me;
				p->right->me = false;
			}
			switch (p->opts)
			{
			case '&':
				p->me = l && r;
				break;
			case '|':
				p->me = l || r;
			}
		}
	}
	void postEffect(Cond_tree *p, Column_list *(&r))
	{
		if (p == nullptr)return;
		postEffect(p->left, r);
		postEffect(p->right, r);
		check(p, r);
	}
	bool Conditional(Query &me, row *r) // check conditions for a row
	{
		if (me.conditions == nullptr)
			return true;
		Column_list *c = row2col(r->columns);
		postEffect(me.conditions, c);
		bool res = me.conditions->me;
		me.conditions->me = false;
		return res;
	}

	void reStore(Query &me, row *data) // clean table file and store row list
	{
		fclose(Tmp.stream);
		Tmp.stream = fopen(strcat(me.table, ".csv"), "w");
		while (data != nullptr)
		{
			while (data->columns != nullptr)
			{
				if (data->columns->value != nullptr)
					fputs(data->columns->value, Tmp.stream);
				fputs(";", Tmp.stream);
				delete data->columns->value;
				data->columns = data->columns->next;
			}
			data = data->next;
			fputs("\n", Tmp.stream);
		}
	}
	void par(Data_list *r, Data_list *d) //replace existed filds in scend data list in to first one
	{
		while (r != nullptr)
		{
			if (d->value != nullptr)
			{
				r->value = d->value;
			}
			r = r->next;
			d = d->next;
		}
	}
} // namespace File System

#endif
