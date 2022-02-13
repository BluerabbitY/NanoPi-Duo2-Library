#include "NPDDataBase.h"

using namespace NPD;

bool NPDDataBase::open(std::string filename)
{
    NPDError DError = nullptr;
    if(sqlite3_open(filename.c_str(), &_db) != SQLITE_OK)
    {
        DError = errorCreate(NPD_ERROR_DATABASE, false, NPD_ERROR_ENO_NULL,(void*)_db);
    }
    DError = errorCreate(NPD_ERROR_DATABASE);
    return errorHandler(DError);
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
        DError = errorCreate(NPD_ERROR_DATABASE, false, NPD_ERROR_ENO_NULL,(void*)_db);
    }
    DError = errorCreate(NPD_ERROR_DATABASE);
    return errorHandler(DError);
}

const QueryResults NPDDataBase::results()
{
    return _result;
}

const std::string NPDDataBase::version()
{
    return std::string(SQLITE_VERSION);
}
