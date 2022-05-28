/**
 * @file NPDDataBase.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief SQLite3数据库管理
 * @version 0.1
 * @date 2022-03-04 10:55:53 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDDataBase.h"

using namespace NPD::NPDDB;

#include <NPDError.h>

bool NPDDataBase::open(std::string filename)
{
    NPDError DError = nullptr;
    if(sqlite3_open(filename.c_str(), &_db) != SQLITE_OK)
    {
        DError = ErrorCreate(NPD_ERROR_DATABASE, false, NPD_ERROR_ENO_NULL,(void*)_db);
    }
    DError = ErrorCreate(NPD_ERROR_DATABASE);
    return ErrorHandler(DError);
}

void NPDDataBase::close()
{
    if(!_db){return;}
    sqlite3_close(_db);
}

bool NPDDataBase::exec(std::string sql)
{
    if(!_result)
    {
        _result = (QueryResults)malloc(sizeof(struct sQueryResults));
    }
    if(_result->resultp)
    {
        sqlite3_free_table(_result->resultp);
        _result->resultp = nullptr;
    }
    NPDError DError = nullptr;
    char *errmsg = nullptr;

    sqlite3_get_table(_db,sql.c_str(),&(_result->resultp),&(_result->nrow),&(_result->ncolumn),&errmsg);
    if(errmsg)
    {
        sqlite3_free_table(_result->resultp);
        errmsg = nullptr;
        DError = ErrorCreate(NPD_ERROR_DATABASE, false, NPD_ERROR_ENO_NULL,(void*)_db);
    }
    DError = ErrorCreate(NPD_ERROR_DATABASE);
    return ErrorHandler(DError);
}

const QueryResults NPDDataBase::results()
{
    return _result;
}

const std::string NPDDataBase::version()
{
    return std::string(SQLITE_VERSION);
}
