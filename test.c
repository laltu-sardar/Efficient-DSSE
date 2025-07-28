#include<stdio.h>
#include<string.h>
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

int main(){
	FILE *f, *g;
	char c;

	f = fopen("pre-processing/DistWordsV8" ,"r");
    c = fgetc(f);
    do {

        if (dictionary_findIndex(c)==-1) {
            printf("%d %c\n",c,c );
        }
        c = fgetc(f);
    } while(c != EOF);

	fclose(f);
	return 0;
}


/*
f = fopen("pre-processing/DistWordsV7" ,"r");
g = fopen("pre-processing/DistWordsV8" ,"w");
do {
    c = fgetc(f);
    if (((int)(c) != -1) ){
        fputc(c, g);
    }else{
        fputc('\n', g);
    }
} while(c != EOF);

fclose(f);
fclose(g);


*/
