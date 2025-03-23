#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

#define WR_DATA _IOW('a', 'a', int32_t*)
#define RD_DATA _IOR('a', 'b', int32_t*)

typedef struct data_t {
    int32_t num1;
    int32_t num2;
} data_t;

#endif