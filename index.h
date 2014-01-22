//Week 1's homework: Index

#define UPDATE 1
#define INSERT 0

//MAX_LINE is the maximum number of character can be stored in wrdoccurence.
//MAX_WORD is the maximum number of different words.

#define MAX_LINE 10000
#define MAX_WORD 20000

#define CAPTION_OK 1
#define CAPTION_NOT_OK 0


typedef char *String;
typedef char *pos_holder;
typedef char DynamicWord[20];

typedef struct _indexes
{
  String wrdentry;
  int wrdcounter;
  pos_holder wrdoccurence;
} Indexes;

typedef struct _indexes_state
{
	Indexes IList[MAX_WORD];
	int word_count;
} IndexesState;

void decaption(char* str);
int NewIndexes(IndexesState *state);
int unneeded_word_load(FILE *f);
int index_search(String newword,IndexesState *state,int *position);	//Binary search, and if not found, return the position for insertion sort
inline void indexes_copy(IndexesState *state,int from,int to);
void pusher(IndexesState *state,int position);
int insert_wordlist(String newword,IndexesState *state,int position,int linenum);	//Using insertion sort
void update_wordlist(String newword,IndexesState *state,int position,int linenum);
void upsert_wordlist(String newword,IndexesState *state,int linenum);
inline void fgetchar_count_line(FILE *f,char *ch,int *linenum);
void word_detector(FILE *f,int *finisher,char tmp[],int *linenum);
int word_invalid(String newword);
void word_reader(FILE *f,IndexesState *state);
