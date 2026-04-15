#ifndef MAGB_TYPEDEFS_H
#define MAGB_TYPEDEFS_H

constexpr int MAX_ADDRESS = 0xFFFF;

typedef unsigned char byte_t;
typedef signed char   sbyte_t;

typedef unsigned short int word_t;
typedef unsigned int       dword_t;
typedef unsigned long long qword_t;

typedef short int unsigned address_t;

constexpr address_t MAGB_IE_ADDR = 0xFFFF;
constexpr address_t MAGB_IF_ADDR = 0xFF0F;

#endif // MAGB_TYPEDEFS_H