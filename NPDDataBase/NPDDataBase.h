#ifndef _NPD_DATA_BASE_H_
#define _NPD_DATA_BASE_H_

#include <string>
#include "sqlite3.h"
#include "NPDError.h"

namespace NPD {

    struct sQueryResults{
        /** 查询结果指针 */
        char **resultp;
        /** 查询结果行数 */
        int nrow;
        /** 查询结果列数 */
        int ncolumn;
    };
    typedef struct sQueryResults* QueryResults;

    class NPDDataBase {
    public:
        /**
        *  @brief  打开数据库.
        *  @param  filename  要打开数据库的位置.
        *  @return  是否正确打开.
        */
        bool open(std::string filename);
        /**
        *  @brief  关闭数据库.
        *  @return  无返回.
        */
        void close();
        /**
        *  @brief  执行SQL语句.
        *  @param  sql  SQL语句.
        *  @return  是否正确查询.
        */
        bool exec(std::string sql);
        /**
        *  @brief  获取SQL语句查询结果.
        *  @return  查询结果结构体.
        */
        const QueryResults results();
        /**
        *  @brief  获取数据库版本号.
        *  @return  数据库当前版本号.
        */
        const std::string version();
    private:
        std::string _DB_NAME;
        QueryResults _result = nullptr;
        sqlite3* _db;
    };
}

#endif /* _NPD_DATA_BASE_H_ */