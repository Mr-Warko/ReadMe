#include <stdio.h>
#include <stdlib.h>

/*
struct l_struct list{
	struct list *next;
	void *data;
};

struct list *list_create(void *data){

	struct list *list = malloc(sizeof(list));
	if (list){
		list -> data = data;
		list -> next = NULL;
	}
	return list;
}

struct list *list_prepend(struct list *old, void *data){
	struct list *list = list_create(data);
	if(list)
		list -> next = old;
	return list;
}

struct list *list_append(struct list *list, void *data){
	struct list **plist = &list;
	while (*plist)
		plist = &(*plist)->next;
	
	*plist = list_create(data);
	if (*plist)
		return list;
	else
		return NULL;
}

struct list *list_remove_first(struct list *list){
	struct list *first = list;
	list = list -> next;
	free(first);
	return list;
}

void list_destroy(struct list *list){
	while (list)
		list = list_remove_first(list);
}
*/


struct list{
	struct list *next;
	int *value;
};

struct list *empty_list() { return NULL; }

int list_is_empty(struct list *l){
	return l == NULL;
}

int list_length(struct list *list){
	int length = 0;
	while (list){
		length++;
		list = list -> next;
	}
	return length;
}


struct list *add(struct list *l, int *x){
	struct list *tmp;
	tmp = malloc(sizeof(struct list));
	tmp->value = x;
	tmp->next = l;
	return tmp;
}

struct list *list_append(struct list *list,int *data){
	struct list **plist = &list;
	while (*plist)
		plist = &(*plist)->next;
	*plist =  add(*plist,data);
	if (*plist)
		return list;
	else
		return NULL;
}
/*
struct list *insert(struct list *l,int *data){
	struct list *tmp, *prev, *nextl;
	tmp = NULL;
	tmp = add(tmp,data);
	if(!l)
		l = tmp;
	else{
		prev = NULL;
		nextl = l;
		while(nextl && *(nextl->value) <= *(data)){
			prev = nextl;
			nextl = nextl->next;
		}
		if(!nextl)
			prev->next = tmp;
		else{
			if(prev){
				tmp->next = prev->next;
				prev->next = tmp;
			}
			else{
				tmp->next = l;
				l = tmp;
			}
		}
	}
	return l;
}
*/

struct list *insert(struct list *l,int *data){
	struct list *tmp, *prev, *nextl;
	tmp = NULL;
	tmp = add(tmp,data);
	if(!l)
		l = tmp;
	else{
		prev = NULL;
		nextl = l;
		while(nextl && *(nextl->value) <= *(data)){
			prev = nextl;
			nextl = nextl->next;
		}
		if(!nextl)
			prev->next = tmp;
		else{
			if(*((nextl->value)+2) < *((tmp->value)+2)){
				*(nextl->value) = *(tmp->value);
				*((nextl->value)+2) = *((tmp->value)+2);
				*((nextl->value)+3) = *((tmp->value)+3);
			}
			else if(prev){
				tmp->next = prev->next;
				prev->next = tmp;
			}
			else{
				tmp->next = l;
				l = tmp;
			}
		}
	}
	return l;
}
//altered the head
void addin(struct list *l1, int *x){
	struct list **l = &l1;
	struct list *tmp;
	tmp = malloc(sizeof (struct list));
	tmp->value = x;
	tmp->next = *l;
	*l = tmp; 

}

int list_len(struct list *l){
	int len;
	for(len =0;l;l = l->next)
		len += 1;
	return len;
}

void print_list(struct list *l){
	for(;l;l = l->next){
		int *datarray = l->value;
		printf("xMin:%i | yMin:%i | xMax:%i | yMax:%i \n",*datarray,*(datarray+1),*(datarray+2),*(datarray+3));
	}
}

struct list *list_remove_first(struct list *l){
	struct list *first = l;
	l = l -> next;
	free(first);
	return l;
}

void list_destroy(struct list *l){
	while (l)
		l = list_remove_first(l);
}


/*
int main(){
	int array[] = {0,1,2,3};
	struct list *list = list_create(array);
	int *datarray = (list->data);
	printf("array 1: %i\n", *(datarray+1));
*}*/
