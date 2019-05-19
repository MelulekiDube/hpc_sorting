#ifndef EXP_H
#define EXP_H

#define READ  "r"
#define ROOT_FILE "./test_data/"

typedef struct FILE_CONT{
	int *arr;
	int size;
}FILE_CONT;

/** 
*	@Brief Method to read the file with the numbers to be sorted and puts it into a list then
*			returns that list
*	@Param filename: The name of the file that we are going to be reading
*	@returns: The list with all the numbers read from the file.
*/
FILE_CONT* read_file(char *filename);

/** 
*	@Brief Method for launching the experiment
*	@returns: The list with all the numbers read from the file.
*/
void run_experiment();
#endif