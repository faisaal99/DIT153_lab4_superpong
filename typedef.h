#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__


typedef signed   char  i8;
typedef unsigned char  u8;

typedef signed   short i16;
typedef unsigned short u16;

typedef signed   int   i32;
typedef unsigned int   u32;

typedef unsigned long  ulong;

typedef enum
{
	false, // 0
	true   // 1
} bool;


#ifndef NULL
#define NULL 0
#endif


#endif // __TYPEDEF_H__