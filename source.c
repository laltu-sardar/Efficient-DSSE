/*================================================
\\	TDL-DSSE Implementation						||
||	source.c									\\
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

/*================== HEADER FILES ==================================*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "dsse.h"

/*========= Global Variables =======================================*/
unsigned int ArraySIZE;
unsigned int TableSSiZE;
unsigned int TableDSiZE;
unsigned int no_of_key_file_pair;
unsigned int noOfAddedFiles=0;

/*========== Main Functions ==================================*/

void initialize_arraySize(unsigned int total_no_of_words_in_W,
    unsigned int max_no_of_files_supported, unsigned int max_no_of_files_2b_uploaded){
    ArraySIZE = max_no_of_files_2b_uploaded*AVG_Fw; //
    TableSSiZE = total_no_of_words_in_W; //443 957
    TableDSiZE = max_no_of_files_supported * 5;
    printf("%u %u %u \n",ArraySIZE, TableDSiZE, TableSSiZE );
    return;
}



KEY * KeyGen(unsigned int lmbda ){

    KEY * K;
    K = (KEY *)malloc(sizeof(KEY));

    K->k    = rand();
    K->k1   = rand();
    K->k11  = rand();
    K->k2   = rand();
    K->k22  = rand();
    K->k3   = rand();

    return K;
}


/********************************************************************/
InvINDEX * build(KEY *K, char * fileNameSourceFileName, unsigned int noOfFiles,DICT_NODE *dic ){
    unsigned int loop_counter , word_counter;
    char fileName[FileNameLen];
	TokenADD *t_a;
    InvINDEX * I;
    FILE  *fileNameSourceFile;

    I = (InvINDEX *)malloc(sizeof(InvINDEX));
    I->A 	= allocateCell(ArraySIZE);
    I->Ts 	= allocateSNode(TableSSiZE);
    I->Td 	= allocateDNode(TableDSiZE);
    I->C 	= (char **)malloc (MAX_NUM_FILE*sizeof(char*));

    make_free_list(I->A, (ArraySIZE*7)/10);

    fileNameSourceFile =  fopen (fileNameSourceFileName,"r");
    if (fileNameSourceFile== NULL) {
        printf("Error: in opening %s\n",fileNameSourceFileName );
        exit(0);
    }

    loop_counter = 0 ;
    word_counter = 0 ;
    while (loop_counter<noOfFiles){
        if (fscanf(fileNameSourceFile, "%[^'\n']\n",fileName) == EOF ){
			printf("Failed for 1 %s\n", fileName );
			continue ;
		}
        //printf("entering add token\n =================== \n" );
        t_a = AddToken(K, fileName, dic);
        word_counter += t_a->noOfDistWords;
        //printf("entering add\n =============== \n" );
        Add(t_a, fileName, I);
        //printf(" Added %s\n",fileName );
        loop_counter ++;
        //printf("%d files added\n", loop_counter);
    }
    fclose(fileNameSourceFile);
    no_of_key_file_pair = word_counter;
    return I;
}



/********************************************************************/
TokenADD * AddToken(KEY *K, char * fileId, DICT_NODE *dic ){

	char word[HashValueSize], *tmpstr;
    FILE *filePtr;
	unsigned int noOfWords=0, counter;
    unsigned int  kf, r, kw, temp_fkw ;
	PART * P;
    TokenADD * t_a ;

    t_a = (TokenADD *)malloc(sizeof(TokenADD));
    //printf("222 %s\n", fileId);
    if ((filePtr = fopen(fileId, "r")) == NULL){
      printf("Couldnot Open %s\n",fileId);
      perror("\nError! opening file \n");
      return (NULL);
    }
    else{
        //printf("223%s\n", fileId);
        noOfWords = 0;
        while (1){
            if (fscanf(filePtr,"%s",word) ==    EOF){
                fclose(filePtr);
                filePtr = fopen(fileId, "r");
                if (filePtr == NULL) printf("Couldnot Open %s\n",fileId );
                break ;
            }
            noOfWords++ ;
        }
        //printf("noOfWords  = %d\n",noOfWords );
    	P = (PART *)malloc(noOfWords * sizeof(PART));
        kf = func_H (K -> k3 , fileId);

        counter = 0;
        while(1){
        //for (counter = 0; counter < noOfWords; counter ++) {
    		if (fscanf(filePtr,"%s",word) == EOF )
                break;
            //temp_fkw = func_F(K -> k1 , word) % TableSSiZE ;
            temp_fkw = dictionary_search(dic, word);
            if ( temp_fkw == 0){
                continue;
            }
            //printf("temp word = %u\t", temp_fkw );
            //dictionary_insert(&dic, temp_fkw);

            changeToLowerCase(word);

            kw = func_H(K -> k3 , word);
            r = rand();
            //r = stringToInt(word);

            P[counter].r = r;
            //P[counter].fkw  = func_F(K -> k1 , word) % TableSSiZE;//size of table should be defined in other place
            P[counter].fkw = temp_fkw ;
            P[counter].gkw  = func_G(K -> k2 , word);
    		P[counter].H1   = func_H1(kf, r);
    		P[counter].H2   = func_H2(kf, r);
    		P[counter].H3   = func_H3(kf, r);
    		P[counter].H4   = func_H4(kw, r);
    		tmpstr = (char *) malloc(HashValueSize * sizeof(char));
            if (tmpstr== NULL) printf(" Failure 1\n" );
    		stpcpy(tmpstr,fileId);
    		P[counter].idf  = tmpstr;

            tmpstr = (char *) malloc(HashValueSize * sizeof(char));
            if (tmpstr== NULL) printf(" Failure 2\n" );
            stpcpy(tmpstr,word);
            P[counter].word = tmpstr;
            //printf("%u \t%u \t%s %d\n", P[counter].fkw, P[counter].gkw ,P[counter].word, counter);
            counter ++;
        }

        t_a->fkf   = func_F(K -> k11 , fileId) % TableDSiZE;
    	t_a->gkf   = func_G(K -> k22 , fileId) ;
        t_a->parts = P;
    	t_a->noOfDistWords = counter;

        fclose(filePtr);
    	return t_a;

    }
    fclose(filePtr);
}


/******************************************************************************/
unsigned int Add(TokenADD *t_a, char *str, InvINDEX *I){

	char *newstr;
	unsigned int i, noOfWords;
	unsigned int N, N1, N_ , dmy;
	PART * P;
    int flag = 0;

    CELL * A; /*The main array*/
    TabSNODE * Ts;
    TabDNODE * Td;
    char ** C;

    A   = I-> A;
    Ts  = I-> Ts;
    Td  = I-> Td;
    C   = I-> C;

	noOfWords = t_a->noOfDistWords;
	P = t_a->parts;


	for (i = 0; i < noOfWords; i++){
        N = free_list_remove(A);
//        N = free_op( A,  0, 0);
        // 1. Choose a new node A[N]

        if (Ts[P[i].fkw].ifEncrypted == 0){//if not encrypted encrypt it
            //dmy = free_op( A,  0, 0);
            dmy = free_list_remove(A);
            //A[dmy].left      = A[dmy].left    ^ P[i].H1 ;     //encrypting A[N]
            //A[dmy].right     = A[dmy].right   ^ P[i].H2 ;
            A[dmy].rightS    =  P[i].gkw ;
            Ts[P[i].fkw].ptr = dmy ^ P[i].gkw ;
            Ts[P[i].fkw].ifEncrypted = 1;
        }

        //printf(" fkw= %u h1= %u h2= %u h4= %u %s\n", P[i].fkw, P[i].H1, P[i].H2, P[i].H4, P[i].word);
        // 2. Find First Node A[N1], two cases ,i) Adding First time in Ts, ii) Added Before
        dmy = Ts[P[i].fkw].ptr ^ P[i].gkw  ;

        if (dmy >= ArraySIZE ) {
            printf("Failure: %u %s %s\n",dmy,P[i].idf, P[i].word );
            free_list_insert( A, N);
            //free_op( A,  1, N);
            return 0;
        }

        N1 = A[dmy].rightS ^P[i].gkw ;
        //N1 = A[dmy].rightS ^ func_H4(kw, A[dmy].r) ;
        if (N1 > ArraySIZE){
               printf(" fkw= %u h1= %u h2= %u h4= %u %s ", P[i].fkw, P[i].H1, P[i].H2, P[i].H4, P[i].word);
               printf(" gkw = %u\t dmy= %u\tN= %u\tN1= %u\n", P[i].gkw, dmy, N, N1);
        }
        // 3. update  A[N]
        A[N].left   = dmy ^ P[i].H1; /* Extra Line */
        A[N].right  = N1 ^ P[i].H2 ;
        A[N].rightS = N1 ^ P[i].H4;
        A[N].r      = P[i].r;
        A[N].idf    = P[i].idf;
        A[N].word   = P[i].word;


        // 4. Update A[dmy]
    //    A[dmy].right  = A[dmy].right  ^ N1 ^ N;
        A[dmy].rightS = A[dmy].rightS ^ N1 ^ N;

        // Update A[N1]
//        if ((N1 != 0))
        if ((N1 != 0)&&(N1 < ArraySIZE)) //why N1< ArraySIZE required ??
            A[N1].left = A[N1].left ^ dmy ^ N;

        // 5. update Previous Node's down pointer
        if(flag == 0){
        	Td[t_a->fkf].ptr = N ^ (t_a->gkf);
            //printf("First file ptr = %u\n", N);
            A[N].down = P[i].H3;
            flag = 1;
        }else if(flag ==1) {
            A[N].down = P[i].H3;
            A[N_].down =  A[N_].down  ^ N ;
        }
        //update N_
        N_ = N;

	}
    newstr = (char *)malloc(FileNameLen*sizeof(char));
	stpcpy(newstr,str);
	C[noOfAddedFiles] = newstr;
    //printf("----------------------  ----------------------\n" );
    //printf("--------%u words of \"%s\" added------ \n", noOfWords,newstr);
    //printf(" fkf = %u \tgkf =  %u\n",t_a->fkf,t_a->gkf );
    //printf("--------------------------------------------\n" );
	return noOfWords;
}


/********************************************************************/
TokenSEARCH *  SearchToken(KEY * K, char * w, DICT_NODE * dic){

    TokenSEARCH * t_s;
    t_s = (TokenSEARCH *)malloc(sizeof(TokenSEARCH));

    changeToLowerCase(w);

    //t_s->fkw  = func_F(K -> k1 , w) % TableSSiZE;//size of table should be defined in other place
    t_s->fkw  = dictionary_search(dic,w);

    t_s->gkw  = func_G(K -> k2 , w);
    t_s->kw   = func_H(K -> k3 , w);

    return t_s;
}


/*********************************************************************/
void Search (TokenSEARCH * t_s, InvINDEX *I , SetofFILES *searchRes){

    unsigned int ptr, dmy, nxt_to_dmy, count = 0;
    char ** Iw;
    CELL * A;
    TabSNODE * Ts;

    A = I-> A;
    Ts = I-> Ts;
    Iw = searchRes->listOfFiles;
    searchRes->noOfFiles =  0;

    if (Ts[t_s->fkw].ifEncrypted == 1 ){// if the node is encrypted
        dmy = Ts[t_s->fkw].ptr ^ t_s->gkw;
        nxt_to_dmy = A[dmy].rightS ^ t_s->gkw ;
        if (nxt_to_dmy == 0){
            return ;
        }else{
            ptr = nxt_to_dmy ;
            do {
                Iw[count] = A[ptr].idf;
                ptr = A[ptr].rightS ^ func_H4 (t_s -> kw , A[ptr].r) ;
                count ++;
            } while(ptr != 0);
            searchRes->noOfFiles =  count ;
            return ;
        }
    } else if(Ts[t_s->fkw].ifEncrypted == 0) {// The entry is not touched before
        return ;
    }
}

/*********************************************************************/
void show_search_result(SetofFILES* setofFiles){
    char ** C  = setofFiles -> listOfFiles;
    unsigned int noOfFiles = setofFiles -> noOfFiles ;

    printf("The Word exits in %u files\n",noOfFiles);
    for (; noOfFiles > 0 ; noOfFiles --){
        printf ("%s\n",C[noOfFiles -1]);
    }
    return;
}

/*********************************************************************/
TokenDELETE * DeleteToken(KEY * K, char * fileId ){
    TokenDELETE * t_d;
    t_d = (TokenDELETE *)malloc(sizeof(TokenDELETE));

    t_d->fkf  = func_F(K -> k11 , fileId) % TableDSiZE;//size of table should be defined in other place
    t_d->gkf  = func_G(K -> k22 , fileId);
    t_d-> kf  = func_H(K -> k3 , fileId);

    //printf("In DeleteToken fkf = %u, gkf = %u %s\n", t_d->fkf, t_d->gkf, fileId);

    return t_d;
}



//-------------------------------------------------------
// Returns 1 on success and 0 on Failure
int Delete(TokenDELETE * t_d, InvINDEX * I){

    unsigned int N, L, R, D, counter = 0 ;
    CELL * A;
    TabDNODE * Td;

    A = I-> A;
    Td = I-> Td;

    N = ( Td[t_d -> fkf].ptr )^ (t_d -> gkf);
    if (N >= ArraySIZE ) {
        //printf("Deletion FAILED: the file is not added earlier\n");
        return 0;
    }

    while(N != 0  ){
        L = A[N].left  ^ func_H1 (t_d -> kf , A[N].r );   //decryption missing
        R = A[N].right ^ func_H2 (t_d -> kf , A[N].r );   //decryption missing
        D = A[N].down  ^ func_H3 (t_d -> kf , A[N].r );   //decryption missing
        //printf("\nL= %u \tN= %u \tR= %u \tD=%u", L, N , R, D );

        //in case wrong file with wrong hash value gives right N
        if (L < 0 || R < 0 || D < 0 || L >= ArraySIZE || R >= ArraySIZE || D >= ArraySIZE){
            //printf("Deletion FAILED: the file is not added earlier\n");
            return 0;
        }

        //Update Right Node
        if (R != 0) {A[R].left = A[R].left ^ N ^ L;}
        //Update left node , consider two cases
        A[L].right  = A[L].right ^ N ^ R ;
        A[L].rightS  = A[L].rightS ^ N ^ R ;
        //printf("deleted %s\n", A[N].word);
        //free_op(A,1,N);
        free_list_insert(A,N);
        N = D ;
        counter ++;
    }
    Td[t_d -> fkf].ptr  = t_d -> gkf;
    return counter;
}


/*********************************************************************/
unsigned int show_array(KEY * K, CELL * A){

    unsigned int i, count=0;
    //unsigned int kf, counter=0;
    for (i = 0 ; i < ArraySIZE ; i++){
        if ( A[i].ifFree == 1)
            continue;
        count++;
        //kf = func_H (K -> k3 , A[i].idf);
        //printf("\n%s\t",A[i].idf );
        //printf("%u \t", A[i].left ^ func_H1(kf, A[i].r) );
        //printf("%u \t", A[i].down ^ func_H3(kf, A[i].r) );
        //printf("%u \t", A[i].right ^ func_H3(kf, A[i].r) );
        //printf("%s  %u",A[i].word, count);
        //if (A[i].ifFirstNode == 1) counter++ ;
    }
    //printf("\n no of cell filled = %u\n",count);
    return count ;
}




/*======== CELL ALLOCATION ========================================*/

/******************************************************************************/
CELL* allocateCell( unsigned int size){
    unsigned int loop_counter;
    CELL * A;

    A = (CELL *) malloc(size* sizeof(CELL ));
    if (A==NULL)
    {
      perror("Index Array hasn't allocated properly\n");
      exit(-1);
    }
    else{ // Initialization Step
        for (loop_counter = 0 ; loop_counter < size ; loop_counter ++){
            //A[loop_counter].left = 0;   //THESE SHOULD BE RANDOM
            //A[loop_counter].right = 0;
            //A[loop_counter].down = 0;
            //A[loop_counter].rightS = 0;
            //A[loop_counter].idf = NULL;
            //A[loop_counter].word = NULL;
            //A[loop_counter].r = 0;
            A[loop_counter].ifFree = 0;
        }
        A[0].rightS = 0; //This will be treated as Stack for free list
		return A;
    }
}

/******************************************************************************/
TabSNODE* allocateSNode( unsigned int size){
	unsigned int loop_counter;
    TabSNODE * Ts;
    Ts = (TabSNODE *) malloc(size* sizeof(TabSNODE ));
    if (Ts==NULL)
    {
      perror("Search Table hasn't allocated properly\n");
      exit(-1);
    }
    else{
		for (loop_counter = 0; loop_counter< size ; loop_counter++){
			Ts[loop_counter].ptr = 0;
            Ts[loop_counter].ifEncrypted = 0;
		}
		return Ts;
    }
}

/**************************************************************************/
TabDNODE* allocateDNode( unsigned int size){
	unsigned int loop_counter;
    TabDNODE * Td;
    Td = (TabDNODE *) malloc(size* sizeof(TabDNODE ));
    if (Td==NULL)
    {
      perror("Delete Table hasn't allocated properly\n");
      exit(-1);
    }
    else{
		for (loop_counter = 0; loop_counter < size ; loop_counter++){
			Td[loop_counter].ptr = 0;
		}
      return Td;
    }
}

/*========= TABLE Accessing Recorders (Temp)=========================*/


/*============ Temp functions used in basic =============*/



/*========== ID Generators ===================================*/

/********************************************************************/
BYTE * generate_idf(char * fileName){

    SHA256_CTX ctx;
    BYTE * buf;
    buf = (BYTE * )malloc(SHA256_BLOCK_SIZE * sizeof(BYTE));

    sha256_init(&ctx);
    sha256_update(&ctx, fileName, strlen(fileName));
    sha256_final(&ctx, buf);

    return buf;
}

/********************************************************************/
BYTE * generate_idw(char * word){
    SHA256_CTX ctx;
    BYTE * buf;
    buf = (BYTE * )malloc(SHA256_BLOCK_SIZE * sizeof(BYTE));

    sha256_init(&ctx);
    sha256_update(&ctx, word, strlen(word));
    sha256_final(&ctx, buf);

    return buf;
}



/*================= HASH FUNCTION SECTION =========================*/


/************************************************************/
/*this is a function difined be me as a keyed hash function
with the help of sha256*/
BYTE * keyed_sha256(unsigned int key, char * str){

    char *tmp_str;

    tmp_str = ( char *)malloc(2*HashValueSize* sizeof(char));
    sprintf(tmp_str, "%u", key);
    strcat(tmp_str, str);

    SHA256_CTX ctx;
    BYTE * buf;
    buf = (BYTE * )malloc(SHA256_BLOCK_SIZE * sizeof(BYTE));

    sha256_init(&ctx);
    sha256_update(&ctx, tmp_str, strlen(tmp_str));
    sha256_final(&ctx, buf);

    return buf;
}

/************************************************************/
unsigned int func_F( unsigned int key, char * word){
    BYTE * buf;
    unsigned int a;

    buf = keyed_sha256(key + (unsigned int ) ('F'), word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_G( unsigned int key, char * word){
    BYTE * buf;
    unsigned int a;

    buf = keyed_sha256(key + (unsigned int ) ('G'), word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_H( unsigned int key, char * word){
    BYTE * buf;
    unsigned int a;

    buf = keyed_sha256(key + (unsigned int ) ('H'), word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_H1( unsigned int key, unsigned int r){
    BYTE * buf;
    unsigned int a;
    char word[HashValueSize];

    sprintf(word, "%u",r);
    buf = keyed_sha256(key+1,word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_H2( unsigned int key, unsigned int r){
    BYTE * buf;
    unsigned int a;
    char word[HashValueSize];

    sprintf(word, "%u",r);
    buf = keyed_sha256(key+2,word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_H3( unsigned int key, unsigned int r){
    BYTE * buf;
    unsigned int a;
    char word[HashValueSize];

    sprintf(word, "%u",r);
    buf = keyed_sha256(key+3,word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}

/************************************************************/
unsigned int func_H4( unsigned int key, unsigned int r){
    BYTE * buf;
    unsigned int a;
    char word[HashValueSize];

    sprintf(word, "%u",r);
    buf = keyed_sha256(key+4,word ) ;
    memcpy(&a , buf , sizeof(unsigned int));

    return a;
}



void free_index(InvINDEX * I){
    free(I-> A);
	free(I-> Ts );
	free(I-> Td );
	free(I-> C );
	return ;
}

/*******************************************/
void changeToLowerCase( char * str){
    int len  = strlen(str);
    while (len > 0){
        if (str[len -1] >= 'A' && str[len -1] <= 'Z' )
            str[len -1] +=  ('a' - 'A');
        len --;
    }
    return ;
}

unsigned int getNoOfKeyFilePair(){
    return no_of_key_file_pair;
}


//------------------------------------------------------------------------------
//This Free list is not ON-THE-GO basis. It build free list while building
int  make_free_list(CELL * A, unsigned int size_of_list){
    if (size_of_list > ArraySIZE || size_of_list<1) {
        printf("Error: While making free list\n");
        exit(0);
    }

    unsigned int next_free_node;
    A[0].rightS=0;

    while (size_of_list>0) {
        do {
            next_free_node = rand () % (ArraySIZE-1) + 1;

        } while(A[next_free_node].ifFree==1);

        A[next_free_node].rightS = A[0].rightS ;
        A[0].rightS = next_free_node;
        A[next_free_node].ifFree = 1;

        size_of_list --;
    }
    return 0;
}

unsigned int free_list_insert(CELL *A, unsigned int removed_cell){
    A[removed_cell].rightS = A[0].rightS ;
    A[0].rightS = removed_cell;
    A[removed_cell].ifFree = 1;
    return 0;
}


unsigned int free_list_remove(CELL *A){
    unsigned int cell_2B_added;
    cell_2B_added = A[0].rightS ;
    if (cell_2B_added != 0){
        A[0].rightS = A[cell_2B_added].rightS;
        A[cell_2B_added].ifFree=0;
    }
    else{
        printf("Free List Error: Memory Full\n");
        return 0;
    }
    return cell_2B_added;
}

//------------------------------------------------------------------------------
//This Free list in ON-THE-GO basis

unsigned int free_op(CELL * A, int flag, unsigned int del_adr){//flag=1 indicates add and flag 0 indicates remove
    unsigned int N;
    if (flag == 0 ){    //take a cell from free list
        if (A[0].right == 0  ){
            do {
                N = rand () % (ArraySIZE-1) + 1;
            } while(A[N].ifFree == 0);

        }
        else {
            N = A[0].rightS ;
            A[0].rightS = A[N].rightS;
        }
        A[N].ifFree = 0;
        return N;
    }
    else if (flag == 1){ //add to free list while deleting a file
        A[del_adr].rightS = A[0].rightS;
        A[0].rightS = del_adr ;
        A[del_adr].ifFree = 1;

        A[del_adr].left = rand(); //THIS SHOULD BE RANDOM
        A[del_adr].right = rand();
        A[del_adr].down = rand();
        A[del_adr].idf = NULL;
        A[del_adr].word = NULL;

        return 0;
    }
    else{

        printf("Invalid Parameters given for free list operations\n");
        return -1;
    }

}
