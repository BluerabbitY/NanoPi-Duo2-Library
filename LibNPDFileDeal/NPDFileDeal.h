#ifndef _NPD_FILE_DEAL_H_
#define _NPD_FILE_DEAL_H_

#include <string>

namespace NPD {

typedef enum{
    FILE_CSV,
    FILE_XML,
    FILE_JSON,
}FILE_Type;

typedef int FDErrorno;
struct sFDError{
    /** 错误编号 */
    FDErrorno fdno;
    /** 语句执行状态 */
    bool execState;
    /** 错误解释 */
    std::string ErrorExplanation;
};
typedef struct sFDError* FDError;

class NPDFileDeal{
public:
    NPDFileDeal(FILE_Type typeno);
public:
    // 创建文件
    bool create();
    // 打开文件
    bool open();
    // 检查错误
    bool fd_error_handler(const FDError &fderror);
private:
    FILE_Type _ftype;
    std::string _fullfilename;
};

}

#endif /* _NPD_FILE_DEAL_H_ */