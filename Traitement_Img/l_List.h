#include <stdio.h>
#include <stdlib.h>

typedef struct l_List List;

struct l_List{
	List *next;
	void *data;
};

List *list_create(void *data){

	List *list = malloc(sizeof(list));
	if (list){
		list -> data = data;
		list -> next = NULL;
	}
	return list;
}

List *list_prepend(List *old, void *data){
	List *list = list_create(data);
	if(list)
		list -> next = old;
	return list;
}

List *list_append(List *list, void *data){
	List **plist = &list;
	while (*plist)
		plist = &(*plist)->next;
	
	*plist = list_create(data);
	if (*plist)
		return list;
	else
		return NULL;
}

List *list_remove_first(List *list){
	List *first = list;
	list = list -> next;
	free(first);
	return list;
}

void list_destroy(List *list){
	while (list)
		list = list_remove_first(list);
}

int list_length(List *list){
	int length = 0;
	while (list){
		length++;
		list = list -> next;
	}
	return length;
}
/*
int main(){
	int array[] = {0,1,2,3};
	List *list = list_create(array);
	int *datarray = (list->data);
	printf("array 1: %i\n", *(datarray+1));
}*/
