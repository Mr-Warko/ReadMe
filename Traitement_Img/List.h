#include <stdio.h>
#include <stdlib.h>

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
			if(*((nextl->value)) < *((tmp->value)+2)){
				*(nextl->value) = *(tmp->value);
				*((nextl->value)+2) = *((tmp->value)+2);
				*((nextl->value)+3) = *((tmp->value)+3);
			}
			else if(prev){
				if(*((tmp->value)) > *((prev->value)) && 
					 *((tmp->value)+2) < *((prev->value)+2))
					*((prev->value)+3) = *((tmp->value)+3);
				else{
				tmp->next = prev->next;
				prev->next = tmp;
				}
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



void print_list(struct list *l){
	for(;l;l = l->next){
		int *datarray = l->value;
		printf("xMin:%i | yMin:%i | xMax:%i | yMax:%i \n",*datarray,*(datarray+1),
					*(datarray+2),*(datarray+3));
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

/*=========================*/
/*==========l_list=========*/
/*=========================*/

struct l_list{
	struct l_list *next;
	struct list *value;
};


struct l_list *empty_l_list(){return NULL;}

int l_list_Length(struct l_list *ll){
	int i = 0;
	for(;ll;ll = ll->next)
		i++;
	return i;
}
struct l_list *add_l_list(struct l_list *ll, struct list *l){
	struct l_list *tmp;
	tmp = malloc(sizeof(struct l_list));
	tmp->value = l;
	tmp->next = ll;
	return tmp;
}

struct l_list *l_list_append(struct l_list *ll,struct list *l){
	struct l_list **pl_list = &ll;
	while (*pl_list)
		pl_list = &(*pl_list)->next;
	*pl_list =  add_l_list(*pl_list,l);
	if (*pl_list)
		return ll;
	else
		return NULL;
}

struct l_list *l_list_remove_first(struct l_list *l){
	struct l_list *first = l;
	l = l -> next;
	list_destroy(first->value);
	free(first);
	return l;
}

void l_list_destroy(struct l_list *ll){
	while (ll)
		ll = l_list_remove_first(ll);
}

void print_l_list(struct l_list *ll){
	size_t i =0;
	for(;ll;ll = ll->next){
		size_t l = list_length(ll->value);
		printf("\nligne numéro: %lu | taille: %lu\n",i,l);
		print_list(ll->value);
		i++;
	}
}
