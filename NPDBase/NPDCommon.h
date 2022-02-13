#ifndef _NPD_COMMON_H_
#define _NPD_COMMON_H_

#include "NPDDataType.h"

namespace NPD {

    /**
    *  @brief  进行CRC32校验.
    *  @param  data  要校验的数据.
    *  @param  size  要校验数据的长度.
    *  @return  CRC32校验码.
    */
    static uint32_t CRC32(uint8_t *data, uint32_t size);

}

#endif /* _NPD_COMMON_H_ */