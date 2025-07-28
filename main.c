/*================================================
\\	TDL-DSSE Implementation						||
||	main.c										\\
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

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include "dsse.h"

int  main(int argc, char const *argv[]) {
	unsigned int lmbda;
    double  query_token_time , query_time ;

	SetofFILES  *searchRes;
	KEY * K;
	InvINDEX *I;
    TokenADD *t_a;
	TokenSEARCH * t_s;
	TokenDELETE * t_d;
    DICT_NODE *dic ;
    struct timeval  tv1, tv2;

    unsigned int total_no_of_words_in_W, max_no_of_files_supported,
    max_no_of_files_2b_uploaded, no_of_files_for_build, no_of_add_queries,
    no_of_search_queries, no_of_del_queries, avg_keywords_in_a_file,
    added_cell,searched_cell,deleted_cell;


    FILE * output_file_ptr, *query_file_ptr;
    char path[6][FileNameLen], word[HashValueSize], fileName[FileNameLen];
    unsigned int  query_counter=0,no_of_key_file_pair=0, temp_un_int;

    srand(time(NULL));   // should only be called once

    //Assumptions
    lmbda = 5;      //Not used
    //----------------- Arguments checkings ----------------------------------
    //-------------------------------------------------------------------
    if (argc<8) {
        printf("Give proper Arguments\n");
        exit(0);
    }

    total_no_of_words_in_W = strtoul(argv[1], NULL,10); // Ts will be is of this size
    max_no_of_files_supported = strtoul(argv[2], NULL,10); //Td will be is of this size
    max_no_of_files_2b_uploaded = strtoul(argv[3], NULL,10);//search result will contain this no of files at max
    no_of_files_for_build = strtoul(argv[4], NULL,10);  //for build functions
    no_of_add_queries = strtoul(argv[5], NULL,10);
    no_of_search_queries = strtoul(argv[6], NULL,10);
    no_of_del_queries = strtoul(argv[7], NULL,10);


    strcpy(path[0], "pre-processing/pathFilev2");   //Paths of the File that contains names of the files to be added in build
    strcpy(path[1], "pre-processing/DistWordsV8");  //path of the keyword-file for dictionary
    strcpy(path[2], "queries/addQueries");      //path of the add-Query file//
    strcpy(path[3], "pre-processing/DistWordsV8");   //path of the search-Query file
    strcpy(path[4], "queries/deleteQueries");   //path of the  delete-Query file
    strcpy(path[5], "results");   //path of the  result file


    //----------------- Initializations ----------------------------------
    //-------------------------------------------------------------------
	initialize_arraySize(total_no_of_words_in_W, max_no_of_files_supported,max_no_of_files_2b_uploaded);
	K = KeyGen(lmbda);
    dic = dictionary_init(path[1]);

    //---------------- Building InvINDEX -------------------------------
	//-------------------------------------------------------------------
    printf("\n----------Building Started...............\n");
    gettimeofday(&tv1, NULL);
	I = build(K, path[0], no_of_files_for_build, dic);
	gettimeofday(&tv2, NULL);
    query_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

    output_file_ptr = fopen(path[5],"a+");
    if (output_file_ptr== NULL) { printf("Error: in opening results file\n" ); exit(0); }
    fprintf (output_file_ptr,"0  %lf\n",query_time);
    //query_counter = show_array(K, I->A);
    no_of_key_file_pair = getNoOfKeyFilePair();
    fprintf(output_file_ptr, "5 %u %u\n", no_of_files_for_build, no_of_key_file_pair );
    fclose(output_file_ptr);
    printf ("0  %lf\n",query_time);
    printf( "5 No of files added =%u \tNo of cell filled=%u\tNo of key-file pair added = %u\n", no_of_files_for_build,query_counter,no_of_key_file_pair);
	printf("-----------Building Complete.........\n");
    //-------------------------------------------------------------------

    //----------------- Add Queries--------------------------------------
    //-------------------------------------------------------------------
    printf("\n----------Add Query Started...............\n");
    query_file_ptr = fopen(path[2],"r");
    if (query_file_ptr== NULL) { printf("Error: in opening %s\n",path[2] ); exit(0); }
    query_time =0; query_token_time= 0; query_counter=0; added_cell=0;
    while(1){
        if(fscanf(query_file_ptr, "%[^'\n']\n",fileName) == EOF )   break;

        gettimeofday(&tv1, NULL);
        t_a = AddToken(K, fileName, dic);
        gettimeofday(&tv2, NULL);
        query_token_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

        gettimeofday(&tv1, NULL);
        added_cell += Add(t_a, fileName, I);
        gettimeofday(&tv2, NULL);
        query_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

        query_counter++;
        if (query_counter >no_of_add_queries){
            break;
        }
    }
    fclose(query_file_ptr);

    output_file_ptr = fopen(path[5],"a+");
    if (output_file_ptr== NULL) { printf("Error: in opening result file\n" ); exit(0); }
    fprintf(output_file_ptr, "1 %u %u %lf %lf %lf\n", query_counter,added_cell,query_token_time ,query_time,query_token_time+query_time);
    fclose(output_file_ptr);
    printf("1 %u %u %lf %lf %lf\n", query_counter,added_cell,query_token_time ,query_time,query_token_time+query_time);
    printf("\n----------Add Query Completed...............\n");
    //-------------------------------------------------------------------


        //----------------- Search Queries-----------------------------------
        //-------------------------------------------------------------------
        printf("\n----------Search Query Started...............\n");

        searchRes = (SetofFILES *) malloc(sizeof (SetofFILES ));
        searchRes -> noOfFiles = 0;
        searchRes -> listOfFiles = (char **) malloc( (max_no_of_files_2b_uploaded) * sizeof (char * ));
        //check this initialization
        query_file_ptr = fopen(path[3],"r");
        if (query_file_ptr== NULL) { printf("Error: in opening %s\n",path[3]); exit(0); }
        query_time =0; query_token_time= 0; query_counter=0; searched_cell=0;
        while(1){
    		if(fscanf(query_file_ptr,"%s",word) == EOF) break;

            gettimeofday(&tv1, NULL);
            t_s = SearchToken(K, word, dic);
            gettimeofday(&tv2, NULL);
            query_token_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

            gettimeofday(&tv1, NULL);
            Search(t_s, I, searchRes);
            gettimeofday(&tv2, NULL);
            query_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

            searched_cell +=  searchRes->noOfFiles;
    		query_counter++;
            if (query_counter >no_of_search_queries){
                break;
            }
    	}
        fclose(query_file_ptr);

        output_file_ptr = fopen(path[5],"a+");
        if (output_file_ptr== NULL) { printf("Error: in opening results file\n" ); exit(0); }
        fprintf(output_file_ptr, "3 %u %u %lf %lf %lf\n", query_counter,searched_cell,query_token_time ,query_time,query_token_time+query_time);
        fclose(output_file_ptr);
        printf("3 %u %u %lf %lf %lf\n", query_counter,searched_cell,query_token_time ,query_time,query_token_time+query_time);
        printf("\n----------Search Query Completed...............\n");
        //-------------------------------------------------------------------


    //----------------- Delete Queries-----------------------------------
    //-------------------------------------------------------------------
    printf("\n----------Delete Query Started...............\n");
    query_file_ptr = fopen(path[4],"r");
    if (query_file_ptr== NULL) { printf("Error: in opening %s\n",path[4]); exit(0); }
    query_time =0; query_token_time= 0; query_counter=0; deleted_cell=0;
    while(1){
        if(fscanf(query_file_ptr, "%[^'\n']\n",fileName) == EOF ) break;

        gettimeofday(&tv1, NULL);
        t_d = DeleteToken(K,fileName);
        gettimeofday(&tv2, NULL);
        query_token_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);

        gettimeofday(&tv1, NULL);
        temp_un_int = Delete(t_d, I);
        gettimeofday(&tv2, NULL);
        query_time += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec	);
        //if (temp_un_int==0) printf(" Deletion Failed: %s\n", fileName);
        //else
        deleted_cell += temp_un_int;
        query_counter++;
        if (query_counter >no_of_del_queries){
            break;
        }
    }
    fclose(query_file_ptr);

    output_file_ptr = fopen(path[5],"a+");
    if (output_file_ptr== NULL) { printf("Error: in opening result file\n" ); exit(0); }
    fprintf(output_file_ptr, "2 %u %u %lf %lf %lf\n", query_counter,deleted_cell,query_token_time ,query_time,query_token_time+query_time);
    fclose(output_file_ptr);
    printf("2 %u %u %lf %lf %lf\n", query_counter,deleted_cell,query_token_time ,query_time,query_token_time+query_time);
    printf("\n----------Delete Query Completed...............\n");
    //-------------------------------------------------------------------


	free_index(I);
	printf("\n-------Program Finished---------------\n\n");
	return 0;
}
