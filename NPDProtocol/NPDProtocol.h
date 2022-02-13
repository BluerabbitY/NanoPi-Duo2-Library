#ifndef _NPD_PROTOCOL_H_
#define _NPD_PROTOCOL_H_

#include "NPDDataType.h"
#include "NPDBuffer.h"
#include "NPDError.h"
#include "NPDCommon.h"

#define NPDProtocolHead   0xEFAA
#define NPDProtocolEnd    0xEFBB

/*
+----------------------------------+
| header |  NPDProtocolHead        |
+----------------------------------+
| length |  uint8_t                |
+----------------------------------+
|  type  |  uint8_t                |
+----------------------------------+
| count  |  uint16_t               |
+----------------------------------+
| plength|  uint16_t               |
+----------------------------------+
|  data  |  可变                    |
+----------------------------------+
| tState |  bool                   |
+----------------------------------+
| CRC32  |  uint32_t               |
+----------------------------------+
|  end   |  NPDProtocolEnd         |
+----------------------------------+
*/

namespace NPD {

class NPDProtocol {
    private:
        struct sProtocol{
            uint16_t        header = NPDProtocolHeader;
            uint8_t         length;
            uint8_t         type;
            uint16_t        count;
            uint16_t        payloadlength;
            NPD::NPDbuffer  data;
            bool            tState;
            uint32_t        CRC32;
            uint16_t        end = NPDProtocolEnd;
        };
        typedef struct sProtocol* Protocol;
    /* 函数 */
    public:
        //
        NPDProtocol();
        NPDProtocol(NPD::NPDbuffer ndata);
        NPDProtocol(const char *ndata);
        // 添加数据
        bool addPayload(NPD::NPDbuffer ndata);
        // 添加数据
        bool addPayload(const char *ndata);
        // 撤销上一步操作
        bool revoke();
        // 解析数据
        NPD::NPDbuffer parse();
    private:
        // 检查错误
        bool check();
        // 打包
        void package();
    private:
        // 暂存
        int Temporarysize = 0;
        void TemporaryStorage();
};

}

#endif /* _NPD_PROTOCOL_H_ */