#include <stdlib.h>
#include <stdio.h>
//#include "img.c"
/*
int main(){

	struct list *l = empty_list();
	int array[] = {0,1,2,3};
	l = list_append(l, array);
	int array2[] = {4,5,6,7};
	l = list_append(l, array2);
	print_list(l);	
	return 0;


	struct list *l = empty_list();
	int array2[] = {7,1,2,3};
	l = add(l,array2);
	for(int i = 0; i < 6; ++i){
		int *array = malloc(4*sizeof(int));
		*array = 0;
		*(array) += i; *(array+1)=1; *(array+2)=2; *(array+3)=3;
		l = insert(l,array);
	}
	print_list(l);
}

	struct list *l = NULL;
	int array[] = {1};
	l = add(l,array);
	struct list **j = &l;
	int i = &(*j)->value;
	printf("**j = %i \n",**j);
	printf("j = %i \n", *((*j)->value));
}*/

int main(int argc, int *argv[]){
call(argv[1]);
return 0; }
