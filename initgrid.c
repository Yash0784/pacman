#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	FILE* fptr = fopen("grid.txt", "w");
	int rows = 31;
	int cols = 62;
	for(int i = 0; i < cols; i++){
		fprintf(fptr, "#");
	}
	fprintf(fptr, "\n");
	for(int j = 0; j < rows - 2; j++){
		fprintf(fptr, "#");
		for(int i = 0; i < cols - 2; i++){
			fprintf(fptr, " ");	
		}
		fprintf(fptr, "#\n");
	}
	for(int i = 0; i < cols; i++){
		fprintf(fptr, "#");
	}
	fclose(fptr);
}

