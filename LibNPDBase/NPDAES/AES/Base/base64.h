#ifndef BASE_64_H
#define BASE_64_H

/************* HEADER FILE ****************/
#include <string>
/******************************************/

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

//use C++ implementation of Base64 algorithm

/*
* @function : Base64 Encode
* @param bytes_to_encode : String need to encode
* @param in_len : length
* @return Encoded string
*/
std::string base64_encode(uint8_t const* , uint32_t len);

/*
* @function : Base64 Decode
* @param bytes_to_encode : String need to decode
* @return Decoded string
*/
std::string base64_decode(std::string const& s);

#endif
 