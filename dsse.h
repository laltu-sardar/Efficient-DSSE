/*================================================
\\	TDL-DSSE Implementation						||
||	dsse.h   									\\
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
/*============================================================================*/
#include<stdbool.h>
#include "sha256.h"

#define FileNameLen 120
#define MAX_NUM_FILE 100000
#define AVG_Fw 200
#define HashValueSize 100 	//this is the word size

extern unsigned int ArraySIZE;
extern unsigned int TableSSiZE;
extern unsigned int TableDSiZE;
extern unsigned int no_of_key_file_pair;
/*============================================================================*/

struct InvIndex;
struct Key;

struct CellNode;
struct TableSNode;
struct TableDNode;

struct TokenAdd;
struct TokenSearch;
struct TokenDelete;

struct SetofWords;
struct SetofFiles;

struct RightSearch;
struct Part;

struct dicNode ;


/*===================== STRUCTURES ===========================================*/

/**************************************************/
typedef struct dicNode {
	unsigned int  end;
	struct dicNode * next[40];
}DICT_NODE;

typedef struct InvIndex
{
  struct CellNode 		* 	A;
  struct TableSNode 	* 	Ts;
  struct TableDNode 	* 	Td;
  char 		** 	C; // C should be kept in diff place
}InvINDEX;

/**************************************************/
typedef struct Key
{
	unsigned int k;
	unsigned int k1;
	unsigned int k11;
	unsigned int k2;
	unsigned int k22;
	unsigned int k3;
}KEY ;
/**************************************************/
typedef struct CellNode
{
   unsigned int left;
   unsigned int right;
   unsigned int down;
   unsigned int rightS;
   char 	*	idf;
   char 	* 	word;
   unsigned int r;
   int ifFree;
} CELL;


/**************************************************/
typedef struct TableSNode
{
    unsigned 	int ptr;
    struct Key *	K;
    unsigned 	int ifEncrypted;
} TabSNODE;


/**************************************************/
typedef struct TableDNode
{
    unsigned int ptr;
	struct Key *	K;
} TabDNODE;

/**************************************************/
typedef struct TokenAdd
{
	unsigned int fkf;
	unsigned int gkf;
	struct  Part * parts;
	unsigned int noOfDistWords;
}TokenADD;


/**************************************************/
typedef struct TokenSearch
{
	unsigned int fkw;
	unsigned int gkw;
	unsigned int kw;
}TokenSEARCH;

/**************************************************/
typedef struct TokenDelete
{
	unsigned int fkf;
	unsigned int gkf;
	unsigned int kf;
}TokenDELETE;

/**************************************************/
typedef struct SetofWords
{
	unsigned int noOfWords;
	char ** list;
}SetofWORDS;

/**************************************************/
typedef struct SetofFiles
{
	unsigned int noOfFiles;
	char ** listOfFiles;
}SetofFILES;


/**************************************************/
typedef struct RightSearch
{
  unsigned int right;
  unsigned int idf;
} RightS;


/**************************************************/
typedef struct  Part
{
	unsigned int fkw;
	unsigned int gkw;
	unsigned int  H1;
	unsigned int  H2;
	unsigned int  H3;
	unsigned int  H4;
	char *  idf;
	char * word;
	unsigned int r;
}PART;


/*======================= FUNCTIONS ==========================================*/

extern void dictionary_free(DICT_NODE *);
extern DICT_NODE * dictionary_init (char * );
extern  unsigned int  dictionary_search(DICT_NODE *, char * );
extern int dictionary_insert(DICT_NODE *, char *, unsigned int );
extern DICT_NODE * createDicNode();
extern int dictionary_findIndex( char ) ;
extern  DICT_NODE * dictionary_init (char * ) ;

extern void initialize_arraySize(unsigned int , unsigned int, unsigned int );
extern KEY * KeyGen(unsigned int );
extern InvINDEX * build(KEY *, char * , unsigned int,DICT_NODE *  );
extern TokenADD * AddToken(KEY *, char * , DICT_NODE * );
extern unsigned int Add(TokenADD *, char *, InvINDEX * );
extern  TokenSEARCH *  SearchToken(KEY * , char * , DICT_NODE * );
extern void Search (TokenSEARCH * , InvINDEX * , SetofFILES* );
extern TokenDELETE * DeleteToken(KEY * , char *  );
extern int Delete(TokenDELETE * t_d, InvINDEX * I);

extern int make_free_list(CELL * , unsigned int );
extern unsigned int free_list_insert(CELL *, unsigned int );
extern unsigned int free_list_remove(CELL *);

extern unsigned int free_op(CELL * , int , unsigned int );
extern unsigned int show_array(KEY * ,CELL * );
extern void show_search_result(SetofFILES *);
extern void free_index(InvINDEX * );

extern CELL * allocateCell( unsigned int );
extern TabSNODE * allocateSNode( unsigned int );
extern TabDNODE * allocateDNode( unsigned int );

extern unsigned int getNoOfKeyFilePair();
extern void changeToLowerCase( char * ) ;
extern BYTE * generate_idw(char * );
extern BYTE * generate_idf(char * );

extern BYTE * keyed_sha256(unsigned int , char * );
extern unsigned int func_F( unsigned int , char * );
extern unsigned int func_G( unsigned int , char * );
extern unsigned int func_H( unsigned int , char * );
extern unsigned int func_H1( unsigned int , unsigned int );
extern unsigned int func_H2( unsigned int , unsigned int );
extern unsigned int func_H3( unsigned int , unsigned int );
extern unsigned int func_H4( unsigned int , unsigned int );


//extern unsigned int sqm( unsigned int , unsigned int , unsigned int );
//extern unsigned int samalgo(unsigned int , unsigned int , unsigned int ,   int *);
//extern int check_build(KEY *, InvINDEX * , char * );
//extern  void searchTrie(DICT_NODE *, char *, int  ) ;
//extern SetofFILES * Generate_fileNames(char * );
//extern unsigned int freeTableS_Access( unsigned int );
//extern unsigned int freeTableD_Access( unsigned int );
//extern unsigned int HashAccess( unsigned int );
//extern char * stringXor(char *  , char * );
//extern unsigned int stringToInt(char * );
//extern unsigned int freeListAccess(CELL * );
//extern void add_dummy_nodes(InvINDEX *);

/**************************************************/
