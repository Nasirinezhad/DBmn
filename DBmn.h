#pragma once
#ifndef DBMN
#define DBMN
#define _CRT_SECURE_NO_WARNINGS 1
#include "Initi.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "FS.h"

DBmn::row* entryQuery(char *query)
{
	DBmn_Token::Token_list *a = DBmn_Token::Tokenizer(query);
	DBmn::Query b = DBmn_parser::Parse(a);
	b.execute(b);
	DBmn_FS::Close();
	return DBmn_FS::Tmp.data;
}

#endif