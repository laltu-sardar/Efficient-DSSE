


/*=========== Functions Not Required Anymore =================*/

/*

/*


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




searchRes = (SetofFILES *) malloc(sizeof (SetofFILES ));
searchRes -> noOfFiles = 0;
searchRes -> listOfFiles = (char **) malloc( (2 * 100) * sizeof (char * ));
//size of this sould be no of existing files in the system
flag=1;
while(flag){
    printf("\n=======================================================" );
    printf("\nPress\n 1---->SEARCH \n 2---->DELETE \n 3---->Show Array \n 0---->end\n Your Choice:\t");
    scanf("%u",&choice);

    switch (choice) {
        case 1:
            printf("\nEnter a keyword:\t");
            scanf("%s",w);
            printf("\nSearching...............");

            t_s = SearchToken(K, w, dic);
            Search(t_s, I, searchRes);

            printf("\nSearching Complete.........");
            printf("\nResult given Bellow:");
            printf("\n The Word exits in %u files",searchRes->noOfFiles);
            //show_search_result(searchRes);
            break;

        case 2:
            printf("\nEnter file Name with proper Path:\t");
            scanf("%s",fileName);
            strcpy(fileName, "/home/laltu/Desktop/dataFilter/extractedDataSet/Louise Kitchen/Sent Items/32-out");
            printf("\nDeleting.............");

            t_d = DeleteToken(K,fileName);
            Delete(t_d, I);

            printf("\nDeletion Complete.........");
            break;
        case 3:
            show_array(K, I->A);
            break;
        case 0:
            flag=0;
            break;

        default:
            printf("\nMake a correct choice\n");
            break;
    }

}
*/



//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void add_dummy_nodes(InvINDEX *I){
    unsigned int i, N;
    CELL * A;
    TabSNODE * Ts;
    char  dummyStr[HashValueSize]= "Dummy Node";

    A   = I-> A;
    Ts  = I-> Ts;

    for ( i = 0 ; i < TableSSiZE ; i++){
        N = freeListAccess(0);  // 1. select a random node
        //printf("%u %u\n",i, N);
        A[N].r = rand();
        A[N].left =  0;          // 2. update A[N]
        A[N].right = 0;
        A[N].rightS = 0;
        A[N].idf = dummyStr;
        A[N].word = dummyStr;
        Ts[i].ptr = N ;          // 3. Update Ts[]
        Ts[i].ifEncrypted = 0;
    }//In actual case these will be encrypted

}


//-----------------------------------------------------------------------------
unsigned int freeListAccess(CELL * A){//flag 0 will take a node from free list and 1 will add a node
    unsigned int f ;
    do {
        f = rand () % (ArraySIZE-1) + 1;
    } while(A[f].ifFree == 0);

    A[f].ifFree = 0;
    return f;

}

/**************************************************************
//-----------------------------------------------------------------------------
SetofFILES * Generate_fileNames(char *fileNameSourceFile ){
    char * temp_str;
    FILE * file_ptr;

    unsigned int loop_counter, noOfFiles ;
    char ** fileNames;
    SetofFILES * setofFiles ;

    setofFiles = (SetofFILES *)malloc (sizeof(SetofFILES));
    fileNames = (char **)malloc (MAX_NUM_FILE*sizeof(char*));

    if ((file_ptr = fopen(fileNameSourceFile, "r")) == NULL){
        printf("\nError! in Opening %s\n",fileNameSourceFile);
        perror("\nOpening File Error! \n");
        return (NULL);
    }
    else{
        fscanf(file_ptr,"%u",&noOfFiles); //error
        for (loop_counter = 0 ; loop_counter < noOfFiles ; loop_counter++){
            temp_str = (char *) +malloc (FileNameLen*sizeof(char));
            fscanf(file_ptr,"%s",temp_str);
            fileNames[loop_counter] = temp_str;
        }
        fclose(file_ptr);
    }
    setofFiles -> noOfFiles = noOfFiles;
    setofFiles -> listOfFiles = fileNames ;

    return setofFiles;
}

//-----------------------------------------------------------------------------
 unsigned int sqm(unsigned int x, unsigned int n, unsigned int m){
	unsigned  int c,k,i=0;
    int r[100];
	c=n;
	do
	{
		r[i]=c%2;
		c=c/2;
		i++;
	}while(c!=0);
	k=--i;
    return samalgo(k,x,m,r);
}

//-----------------------------------------------------------------------------
unsigned int samalgo( unsigned int k, unsigned int x, unsigned int m,   int *r)
{
	unsigned int a=1;
    int i;
    for(i=k;i>=0;i--)
    {
    	a=(a*a)%m;
    	if(r[i]==1)
    	a=(a*x)%m;
    }
    return(a);
}


//-----------------------------------------------------------------------------
int check_build(KEY *K, InvINDEX * I, char * fileName){
    TokenDELETE * t_d ;
    CELL * A;
    TabDNODE * Td;
    //char ** C;
    unsigned int N , counter = 0;

    A = I-> A;
    Td = I-> Td;
    //C = I-> C;
    t_d = DeleteToken(K,fileName );
    //printf(" fkf = %u ,gkf =  %u \n",t_d ->fkf, t_d ->gkf );

    N =  Td[t_d -> fkf].ptr ^ t_d ->gkf;
    while (N!=0 && counter < 50){
        //printf("%u\t",N );
        if (N > ArraySIZE) {
            //printf("Failed\n" );
            return 0;
        }
        N = A[N].down ^ func_H3 (func_H(K -> k3 , fileName), A[N].r);
        counter ++;
    }
    //printf("\ncount  = %u\n", counter);
    return 1;
}

//-----------------------------------------------------------------------------
char * stringXor(char * str1 , char * str2){
	char *str;

	unsigned int i, len1, len2;
	str = (char *)malloc(HashValueSize*sizeof(char));

	len1 = strlen(str1);
	len2 = strlen(str2);

	for(i=0; i<HashValueSize; ++i){
		if ( i < len1 && i < len2){
			str[i] = (char)(str1[i] ^ str2[i]);
		}else if (i < len1 && i >= len2 ){
			str[i] = str1[i];
		}else if (i >= len1 && i < len2 ){
			str[i] = str2[i];
		}else{
			str[i] = '0'^'0';
		}
	}
	return str;
}

//-----------------------------------------------------------------------------
unsigned int stringToInt(char * str){
    unsigned int k=0, len;
    len  = strlen(str);
    while(len > 0){
        k = k ^ ((unsigned int)str[len-1]);
        len--;
    }
     return k;
}


//-----------------------------------------------------------------------------

unsigned int freeTableS_Access( unsigned int flag){//flag 0 will take a node from free list and 1 will add a node

	if (flag == 1){ //push a node
		return (211);
	}else if(flag ==0){// pop a node
		freeTabSCounter++;
		return (freeTabSCounter*107 %211);
	}
}

//-----------------------------------------------------------------------------

unsigned int freeTableD_Access( unsigned int flag){              //flag 0 will take a node from free list and 1 will add a node

	if (flag == 1){                            //push a node
		return (97);
	}else if(flag ==0){                        // pop a node
		freeTabDCounter++;
		return (freeTabDCounter*47 %97);
	}
}

//-----------------------------------------------------------------------------

unsigned int HashAccess( unsigned int flag){//flag 0 will take a node from free list and 1 will add a node

	if (flag == 1){ //push a node
		return (65535);
	}else if(flag ==0){// pop a node
		hashCounter++;
		return (hashCounter*35011 %65011);
	}
}





*/
