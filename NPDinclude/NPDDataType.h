#ifndef _NPD_DATA_TYPE_H_
#define _NPD_DATA_TYPE_H_

#include <stdint.h>
#include <stddef.h>

/*****************************************************
  Date types(Compiler specific) 数据类型（和编译器相关）
*****************************************************/
// typedef unsigned char uint8;      /* Unsigned 8 bit quantity  */
// typedef signed char int8;         /* Signed 8 bit quantity    */
// typedef unsigned short uint16;    /* Unsigned 16 bit quantity */
// typedef signed short int16;       /* Signed 16 bit quantity   */
// typedef unsigned int uint32;      /* Unsigned 32 bit quantity */
// typedef signed int int32;         /* Signed 32 bit quantity   */
// typedef float fp32;               /* Single precision         */
//                                   /* floating point           */
// typedef double fp64;              /* Double precision         */
//                                   /* floating point           */

namespace NPD{

typedef enum {
    NPD_UINT8,
    NPD_UINT16,
    NPD_UINT32,
    NPD_UINT64,
    NPD_BOOL,
    NPD_CHAR,
    NPD_SHORT,
    NPD_INT,
    NPD_FLOAT,
    NPD_STRING,
}NPD_DATA_TYPE;

}

#endif /*_DATA_TYPE_H_ */
