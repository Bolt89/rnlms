#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "global.h"
#include "rnlms.h"


#define FRAME_SIZE    80
#define FILTER_LEN   300


typedef NUM (*FunctionOfTwoArgs)(void* pFilter, NUM, NUM, NUM*, NUM*);

void testBlock(FunctionOfTwoArgs func, void  *filterStruct, const NUM *far, const NUM *near, NUM *err, NUM* output, size_t arrayLen)
{
	size_t i=0;
	for (; i< arrayLen; ++i)
	{
		func(filterStruct, far[i], near[i], &err[i], &output[i]);
	}
}

void testAlgo(FunctionOfTwoArgs func, void* filterStruct, 
	      const char *far_filename, 
	      const char *near_filename, 
	      const char *err_filename, 
	      const char *output_filename)
{
//	void *filterStruct = initializer();
	
	FILE *far_file = NULL,
		*near_file = NULL,
		*err_file = NULL,
		*output_file = NULL;
	
	far_file = fopen(far_filename, "r");
	near_file = fopen(near_filename, "r");
	err_file = fopen(err_filename, "w");
	output_file = fopen(output_filename, "w");


	
	NUM far [FRAME_SIZE];
	NUM near [FRAME_SIZE];
	NUM err [FRAME_SIZE];
	NUM output [FRAME_SIZE];

	while(!feof(far_file) && !feof(near_file)) 
	{
		size_t readedNums=0;
		//читает один блок
		while(!feof(far_file) && !feof(near_file) && readedNums < FRAME_SIZE)
		{
			int16_t f_, n_;
			/* fread((void*)&far[readedNums], sizeof(NUM), 1, far_file); */
			/* fread((void*)&near[readedNums], sizeof(NUM), 1, near_file); */

			fread(&f_, sizeof(int16_t), 1, far_file);
			fread(&n_, sizeof(int16_t), 1, near_file);
			far[readedNums] = f_;
			near[readedNums] = n_;
			
//			printf ("%d, %d, %g, %g\n", f_, n_, far[readedNums], near[readedNums]);
			++readedNums;
		}
		testBlock(func, filterStruct, far, near, err, output, readedNums);
		//сохраняет обработанные данные
		size_t writedNums=0;
		for(; writedNums < readedNums; ++writedNums)
		{
			/* fwrite((void*)&err[writedNums], sizeof(NUM), 1, err_file); */
			/* fwrite((void*)&output[writedNums], sizeof(NUM), 1, output_file); */

			int16_t e_, o_;
			e_ = err[writedNums];
			o_ = output[writedNums]; //FIXME round
			printf("%d %d %g %g\n", e_, o_, err[writedNums], output[writedNums]);
			fwrite(&e_, sizeof(int16_t), 1, err_file);
			fwrite(&o_, sizeof(int16_t), 1, output_file);
		}
	}

	free(filterStruct);
	fclose(far_file);
	fclose(near_file);
	fclose(err_file);
	fclose(output_file);
}


int  main()
{
	void *filterStruct = malloc(rlms_sizeOfRequiredMemory(FILTER_LEN));
	rlms_init(filterStruct, 100, 0.1, 0.999, FILTER_LEN);

	testAlgo(rlms_func, filterStruct,       \
		 "g165/filtered_noise_10.dat", \
		 "g165/echo_10_128.dat", \
		 "error.dat", \
		 "output.dat"
		);
}
 
