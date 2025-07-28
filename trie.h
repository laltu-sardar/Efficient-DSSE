/*================================================
\\	DSSE Implementation							||
||	trie.h										\\
\\----------------------------------------------||
||	Author: Laltu Sardar						\\
\\	Junior Research Fellow						||
||	Cryptology and Security Research Unit		\\
\\	Indian Statistical Institute				||
||	Kolkata, India								\\
===============================================*/

struct dicNode ;

typedef struct dicNode {
	bool end;
	struct dicNode * next[43];
}DICT_NODE;

extern void dictionary_free(DICT_NODE *);
extern DICT_NODE * createDicNode();
extern int dictionary_findIndex( char ) ;
extern void dictionary_insert(DICT_NODE **, char *) ;
extern  void searchTrie(DICT_NODE *, char *, int  ) ;
extern bool dictionary_search(DICT_NODE *, char * ) ;
extern  DICT_NODE * dictionary_init (char * ) ;
