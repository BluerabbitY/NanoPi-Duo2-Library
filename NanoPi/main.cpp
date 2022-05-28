#include <NPDError.h>
#include <NPDIIC.h>
#include <NPDSPI.h>
#include <NPDProtocol.h>
#include <NPDTCP.h>
#include <NPDCommon.h>

int main()
{
    // NPD::NPDDEV::NPDSPI test;
    //test.init(2,"test");

    // NPD::NPDCOMM::NPDProtocol ptest;
    // NPD::NPDBASE::NPDbuffer btest;
    // ptest.addPayload(btest);

    NPD_ERROR_TYPE test = NPD_NET_TCP_SERVER;
    AUTO_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_C_ECONNECT);

    auto testt = NPD::NPDBASE::GetVersion();
    auto testc = NPD::NPDBASE::GetCompileTime();

    printf("[%s]\n",testc.c_str());
    for(auto item:testt)
    {
        printf("0x%02X\n",item);
    }

    return 0;
}
