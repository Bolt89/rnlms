
#ifndef _RNLMS_H_
#define _RNLMS_H_

#include "global.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

//#define FILTER_LEN 300;

typedef struct 
{
	NUM *coeff;
	NUM *sig;
	NUM BETTA;
	NUM SIGMA;
	size_t len;
} SimpleIIRFilter ;

//typedef SimpleIIRFilter* (*InitFilterStruct)(void);

SimpleIIRFilter* rlms_init(void *mem, NUM BETTA, NUM SIGMA, size_t filter_len);
NUM rlms_func(SimpleIIRFilter*, NUM, NUM, NUM* err, NUM* out);
size_t rlms_sizeOfRequiredMemory(size_t filter_len);

#endif /* _RNLMS_H_ */


