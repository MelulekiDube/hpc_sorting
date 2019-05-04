#include <stdio.h> //using the fgetc and othe file operations
#include <ctype.h> //for isdigit function
#include <stdlib.h>
#include <time.h>
#include "exp.h"
#include "base.h"

//defines
#define create(type, size) ((type*)malloc(sizeof(type)*size))
#define copy(type, dest, src, size) (type* memcpy(dest, src, size))
#define READ  "r"
#define NO_FILES 5

const char* ROOT_FILE = "./test_data/";
//global data
const char* files[] = {
	"test_50",
	"test_100",
	"test_500",
	"test_1000",
	"data",
};

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
	int begin = clock();
	quick_sort_serial(arr, 0, size-1);
	int end = clock();
	return (double)(end-begin)/CLOCKS_PER_SEC;
}

double execute_parallel(int *arr, int size){
	int begin =0, end =0;
	#pragma omp parallel
	{
		#pragma omp master
		{
			begin = clock();
			quick_sort_omp(arr, 0,size-1);
			end = clock();
		}
	}
	return (double)(end-begin)/CLOCKS_PER_SEC;
}

/** 
*	@Brief Method for launching the experiment
*	@returns: The list with all the numbers read from the file.
*/
void run_experiment(){
	int i =0;
	while(i < NO_FILES){
		char *filename = add(ROOT_FILE, files[i++]);
		FILE_CONT* ser_temp = read_file(filename);
		FILE_CONT* par_temp = read_file(filename);//redudant may wanna create a copy methof for the file contents
		double duration = execute_serial(ser_temp->arr, ser_temp->size);
		printf("Serial duration %.21f\n", duration);
		duration = execute_parallel(par_temp->arr, par_temp->size);
		printf("parallel duration %.21f\n", duration);
		free(ser_temp);
		free(par_temp);
	}
}

int main(){
	run_experiment();
}
