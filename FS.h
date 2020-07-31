#ifndef _FS
#define _FS

#include "Initi.h"
namespace FS
{
    struct row
    {
        int count_columns;
        Column_list *columns = nullptr;
    };
    struct table
    {
        FILE *stream;
    };
    
    bool Create(Query &me)
    {

    }
    bool Drop(Query &me)
    {

    }
    bool Insert(Query &me)
    {

    }
    bool Update(Query &me)
    {

    }
    bool Delete(Query &me)
    {

    }
    bool Select(Query &me)
    {

    }
} // namespace File System

#endif