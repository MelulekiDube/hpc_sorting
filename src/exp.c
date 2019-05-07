#include <stdio.h> //using the fgetc and othe file operations
#include <ctype.h> //for isdigit function
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>  //for opening the testing directory
#include <omp.h>
#include "exp.h"
#include "base.h"

//defines
#define copy(type, dest, src, size) (type* memcpy(dest, src, size))
#define READ  "r"
#define NO_FILES 5
const char* ROOT_FILE = "./test_data/";

typedef struct dirent * DIR_ptr;

//-----------------------Starting of  file defination------------------------------------------
/**
*	@Brief Checks if a given character is a digit and checks or can be contained in a number
*	@Param the character we are checking 
*	@returns: return a non-zero number if the character can be contained in a number
*/
int isdigit_md(int c){
	if(isdigit(c) || c == '-')
		return 1;
	return 0;
}

/** 
*	@Brief Method to read the file with the numbers to be sorted and puts it into a list then
*			returns that list
*	@Param filename: The name of the file that we are going to be reading
*	@returns: The list with all the numbers read from the file.
*/
FILE_CONT* read_file(char *filename){
	int min, max, size; /*max = max number in file; min  min num in file*/
	FILE* file;
	if(!(file = fopen(filename, READ))){
		fprintf(stderr, "can't open %s\n" , filename);
		exit(2);
	}
	
	//initialize variables for creating the list
	fscanf(file,"%d %d %d", &size, &max, &min);
	//now create our list array
	int *arr = create(int, size);
	for(int i=0; i< size; ++i){
		fscanf(file, "%d", &arr[i]);
	}
	FILE_CONT* t = create(FILE_CONT, 1);
	t->size = size;
	t->arr = arr;
	t->min = min;
	t->max = max;
	fclose(file);
	return t;
}

char *add(const char *s1, const char *s2){
	int size1 = sizeof(s1)/sizeof(s1[0]);
	int size2 = sizeof(s2)/sizeof(s2[0]);
	char *result = create(char, (size1+size2));
	const char *p = s1;
	char *t = result;
	while((*t++ = *p++));
	--t;
	p  = s2;
	while((*t++ = *p++));
	return result;
}

double execute_serial(int *arr, int size){
	struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	quick_sort_serial(arr, 0, size-1);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	return elapsed;
}

double execute_parallel(int *arr, int size){
	/**struct timespec start, finish;
	double elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);*/
	double s = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp master
		{
			quick_sort_omp(arr, 0,size-1);
		}
	}
	return omp_get_wtime()-s;
	/*clock_gettime(CLOCK_MONOTONIC, &finish);
	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	return elapsed;*/
}
int is_valid_fname(const char *fname){
	return (strcmp(fname, ".") && strcmp(fname, ".."));
}
/** 
*	@Brief Method for launching the experiment
*	@returns: The list with all the numbers read from the file.
*/
void run_experiment(){
	DIR_ptr de; // Pointer for directory entry 
	DIR *dr = opendir(ROOT_FILE);
	while((de = readdir(dr))){
		if((is_valid_fname(de->d_name))){
			char* filename = add(ROOT_FILE, de->d_name);
			FILE_CONT* ser_temp = read_file(filename);
			FILE_CONT* par_temp = read_file(filename);//redudant may wanna create a copy methof for the file contentsssss
			double duration = execute_serial(ser_temp->arr, ser_temp->size);
			printf("Size: %d\n", ser_temp->size);
			printf("\tS duration: %.21f\n", duration);
			duration = execute_parallel(par_temp->arr, par_temp->size);
			printf("\tP duration: %.21f\n", duration);
			free(ser_temp->arr);
			free(par_temp->arr);
			free(ser_temp);
			free(par_temp);
		}
	}
	 closedir(dr); 
}

int main(){
	run_experiment();
}
