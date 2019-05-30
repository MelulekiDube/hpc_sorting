#define _POSIX_C_SOURCE 199309L //for time.h
#include <stdio.h> //using the fgetc and othe file operations
#include <ctype.h> //for isdigit function
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>  //for opening the testing directory
#include <omp.h>
#include "exp.h"
#include "base.h"

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
	fclose(file);
	return t;
}

char *add(const char *s1, const char *s2){
	int size1 = strlen(s1);
	int size2 = strlen(s2);
	char *result = create(char, (size1+size2)+1);
	strcpy(result, s1);
	strcpy(result+size1, s2);
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
	double s = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp master
		{
			quick_sort_omp(arr, 0,size-1);
		}
	}
	return omp_get_wtime()-s;
}

//gets rid of current directory and parent directory in a given directory
int is_valid_fname(const char *fname){
	return (strcmp(fname, ".") && strcmp(fname, ".."));
}

FILE_CONT* md_memcpy(FILE_CONT* src){
	FILE_CONT* temp= create(FILE_CONT, 1);
	temp->size = src->size;
	temp->arr= create(int, temp->size);
	for(int i=0; i < temp->size; ++i){
		temp->arr[i] = src->arr[i];
	}
	return temp;
}

/** 
*	@Brief Method for launching the experiment
*	@returns: The list with all the numbers read from the file.
*/
void run_experiment(){
	DIR_ptr de; // Pointer for directory entry 
	DIR *dr = opendir(ROOT_FILE);
	char *result_filename = "ompserial_results";
	FILE* f = fopen(result_filename, "a");
	while((de = readdir(dr))){
		if((is_valid_fname(de->d_name))){
			char* filename = add(ROOT_FILE, de->d_name);
			fprintf(f,"File: %s ",filename);
			FILE_CONT* file_contents = read_file(filename);
			double seq_duration = 0, par_duration=0;
			int size = file_contents->size;
			for(int i =0; i < 100; ++i){
				int *sarr = create(int, size), *parr = create(int, size);
				memcpy(parr, file_contents->arr, sizeof(int)*size);
				memcpy(sarr, file_contents->arr, sizeof(int)*size);
				//printf("parr: %p, sarrr: %p\n", parr, sarr);
				seq_duration += execute_serial(sarr, size);
				par_duration += execute_parallel(parr, size);
				free(sarr);
				free(parr);
			}
			fprintf(f,"Size: %d\n", size);
			fprintf(f,"\tS duration: %.9f\n", (seq_duration/100));
			fprintf(f,"\tP duration: %.9f\n", (par_duration/100));
			free(file_contents->arr);
			free(file_contents);
			free(filename);
			filename = NULL;
		}
	}
	fclose(f);
	closedir(dr); 
}


int main(){
	run_experiment();
}
