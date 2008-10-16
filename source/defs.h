#ifndef DEFS_H
#define DEFS_H

#define VERSION_STRING "1.09"

typedef unsigned char uchar;
typedef unsigned int uint;

#define ALLOCA(var, size) char* var = (char*) alloca(size)

#define FALSE 0
#define TRUE 1
#ifndef NULL
#define NULL 0L
#endif

#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define FRONTIER 3

#define PASSBUTTON 0

#ifndef MIN
# define MIN(a, b) ( (a)<(b)? (a):(b) )
#endif
#ifndef MAX
# define MAX(a, b) ( (a)>(b)? (a):(b) )
#endif
#ifndef ABS
# define ABS(a) ( (a)<0? -(a):(a) )
#endif

extern int cur_time;




#define TIME_FACTOR 1000

#define CKM_STORED 1


#endif
