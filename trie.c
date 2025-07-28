/*================================================
\\	TDL-DSSE Implementation						||
||	trie.c   									\\
\\----------------------------------------------||
||	Author: Laltu Sardar                        \\
\\	Junior Research Fellow						||
||	Cryptology and Security Research Unit		\\
\\	Indian Statistical Institute				||
||	Kolkata, India								\\
\\----------------------------------------------||
||	Other Team Member: Binanda Sengupta       	\\
\\  Supervisor: Dr. Sushmita Ruj                ||
===============================================*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "dsse.h"

void dictionary_free(DICT_NODE *dict){
	for (int i = 0 ; i < 40 ; i++)
		if (dict -> next[i])
			dictionary_free(dict -> next[i]) ;
	free(dict);
}


DICT_NODE * createDicNode(){
	int i;
	DICT_NODE *new;
	new = (DICT_NODE *) malloc(sizeof(DICT_NODE));
	if(new == NULL){
		printf("\nNo Space\n");
		exit(0);
	}

	for( i = 0 ;i < 40 ; i++  ){
		new -> next[i] = NULL;
	}
	new -> end = 0;
	return new;
}

/*********************************************/
int dictionary_findIndex( char c){

	if(c <= 'z' && c>= 'a'){
		return c-'a';
	}
	else if(c <= 'Z' && c>= 'A'){
		return c-'A';
	}
	else if(c <= '9' && c>= '0'){
		return (c-'0'+26);
	}
	else {
		switch(c){
			case '@':
				return 36 ;
			case '!':
				return  37;
			case '-':
				return  38;
			case 39:
				return  39;
		}
	}

	return -1;
}

/*********************************************/
int  dictionary_insert(DICT_NODE *root, char *str, unsigned int num){
	int i,index = 0;
	DICT_NODE *temp = root;
	while(1){
		if(str[index] == '\0'){
			temp -> end = num;
			break;
		}

		else {
/*            if (str[index]==127) {
                index++;
                continue;
            }
*/
            i = dictionary_findIndex (str[index]);

    		if(i == -1) {
    			printf("\n\tThe invalid character %c with ascii %d is present\n",str[index], str[index]);
    			return 0;
    		}
    		else if(temp-> next[i] == NULL)
    			temp-> next[i]= createDicNode();

    		temp = temp -> next[i] ;
    		index++ ;

		}
	}
    return 1;
}

/***************************************************/
unsigned int  dictionary_search(DICT_NODE *root, char *str ){
	int i, index =0;
	DICT_NODE *temp = root;

	while(1){
		if(str[index] == '\0'){
			return (temp) -> end ;
		}
		else {
			i = dictionary_findIndex (str[index]);
			if(1 == -1) {
				return 0;
			}
			if((temp)-> next[i] == NULL){
				return 0;
			}
			temp = (temp) -> next[i] ;
			index++ ;
		}
	}
}

DICT_NODE * dictionary_init (char * filePath){
	char w[50];
	unsigned int count = 1;
	FILE *f;
	DICT_NODE *dic = createDicNode();

	f = fopen(filePath,"r");
    if (f== NULL) {
        printf("Error: in opening %s\n",filePath );
        exit(0);
    }

	while(1){
		if(fscanf(f,"%s",w) == EOF)
			break;
		if (dictionary_insert(dic,w,count) == 0 ){
            printf("%s %d\n", w, count);
        }
		count++;
	}
	fclose(f);
	//printf("%u words inserted in dict\n",count );
	return dic;
}






/***************************************************
void searchTrie(DICT_NODE *root, char *str, int index ){
	int i;

	if(str[index] == '\0'){
		if((root) -> end == 0){
			printf("\n The word does not exist\n");
		}
		else
			printf("\n The word exist\n");
		return;
	}
	else {
		i = dictionary_findIndex (str[index]);
		if(1 == -1) {
			printf("\n The word does not exist\n");
			return;
		}

		if((root)-> next[i] == NULL){
			printf("\n The word does not exist\n");
			return;
		}
		searchTrie( (root) -> next[i] , str, index+1  );
	}
}
*************************************************************/
