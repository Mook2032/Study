#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"

#define TO_NOCAP 32
#define PLIST state->IList
#define LIST state.IList

void decaption(char* str)
{
	int i;
	int len=strlen(str);
	for (i=0;i<len;i++)
	{
		if ((str[i]<='Z')&&(str[i]>='A'))
			str[i]+=TO_NOCAP;
	}
}

int NewIndexes(IndexesState *state)
{
	int i;
	state->word_count=0;
	for (i=0;i<MAX_WORD;i++)
    {
		PLIST[i].wrdentry=NULL;
		PLIST[i].wrdcounter=0;
		PLIST[i].wrdoccurence=NULL;
    }
  return 1;
}

DynamicWord forbidden[30];					//Dear me, I will have these guys as globals.
int unneeded_word;

int unneeded_word_load(FILE *f)
{
  int i=0,k;
  char ch;
  while (!feof(f))
    {
      k=0;
      do{
	ch=fgetc(f);
	forbidden[i][k++]=ch;
      }while ((ch!='\n') && (ch!=EOF));
      forbidden[i][k-1]='\0';
      i++;
    }
  return i-1;
}

int index_search(String newword,IndexesState *state,int *position)
{
	int i,cmp_result,low=0,high=state->word_count-1;
	*position=0;
	while (low<=high)
	{
		*position=(low+high)/2;
		cmp_result=strcmp(newword,PLIST[*position].wrdentry);
		if (cmp_result==0)
		{
			return UPDATE;
		}
		else if (cmp_result<0)
		{
			high=(*position)-1;
		}
		else
		{
			low=(*position)+1;
		}
	}
	
	(*position)=low;
	return INSERT;
}

/*
int index_search(String newword,IndexesState *state,int *position)
{
  int i;
  if (state->word_count!=0)
  for (i=0;i<state->word_count;i++)
    {
		if (strcmp(newword,PLIST[i].wrdentry)==0)
		{
			*position=i;
			return UPDATE;//
		}
		if (strcmp(newword,PLIST[i].wrdentry)<0)
		{
			*position=i;
			return INSERT;
		}
    }
  *position=state->word_count;
  return INSERT;
}
*/

inline void indexes_copy(IndexesState *state,int from,int to)
{
	PLIST[to]=PLIST[from];
}

void pusher(IndexesState *state,int position)
{
	int i;
	for (i=state->word_count;i>position;i--)
		indexes_copy(state,i-1,i);
}

int insert_wordlist(String newword,IndexesState *state,int position,int linenum)
{
	state->word_count++;
	if (state->word_count>MAX_WORD)
	{
		printf("Word limit exceeded!\n");
		state->word_count--;
		return -1;
	}

	pusher(state,position);
	PLIST[position].wrdentry=(char*)malloc(sizeof(char)*(strlen(newword)+1));
	PLIST[position].wrdoccurence=(char*)malloc(sizeof(char)*MAX_LINE);
	if ((PLIST[position].wrdentry==NULL)||(PLIST[position].wrdoccurence==NULL))
	{
		printf("Memory Allocation Failed.\n");
		exit(1);
	}
	strcpy(PLIST[position].wrdentry,newword);
	PLIST[position].wrdcounter=1;
	sprintf(PLIST[position].wrdoccurence,"%d",linenum);
	return 0;
}

void update_wordlist(String newword,IndexesState *state,int position,int linenum)
{
  PLIST[position].wrdcounter++;
  char tmp[10];
  sprintf(tmp,",%d",linenum);
  strcat(PLIST[position].wrdoccurence,tmp);

}

void upsert_wordlist(String newword,IndexesState *state,int linenum)
{
  int position;

  if (index_search(newword,state,&position)==UPDATE)
    {
		update_wordlist(newword,state,position,linenum);
    }
  else
    {
		if (!word_invalid(newword))
		insert_wordlist(newword,state,position,linenum);
    }
}

inline void fgetchar_count_line(FILE *f,char *ch,int *linenum)
{
	*ch=fgetc(f);
	if (*ch=='\n')
		(*linenum)++;
}

void word_detector(FILE *f,int *finisher,char tmp[],int *linenum)
{
	int k=0;
	char ch;

	do{
		fgetchar_count_line(f,&ch,linenum);
		if (ch==EOF)
		{
			tmp[0]=EOF;
			return;
		}
	}while ((ch<'A')||((ch>'Z')&&(ch<'a'))||(ch>'z'));	//Skip non-character
	
	tmp[k++]=ch;
	do{
		fgetchar_count_line(f,&ch,linenum);
		tmp[k++]=ch;
	}while (((ch<='z') && (ch>='a')) || ((ch<='Z')&&(ch>='A')));
	
	if ((ch=='.')||(ch=='?')||(ch=='!')) *finisher=CAPTION_OK;
	else *finisher=CAPTION_NOT_OK;
	tmp[k-1]='\0';
}

int word_invalid(String newword)
{
	int i;
	for (i=0;i<unneeded_word;i++)
	{
		if (strcmp(newword,forbidden[i])==0)
			return 1;
	}
	
	return 0;
}

void word_reader(FILE *f,IndexesState *state)
{
	char tmp[40];
	int finisher,linenum=1,capcheck=CAPTION_OK;
	while (!feof(f))
	{
		word_detector(f,&finisher,tmp,&linenum);
		if (tmp[0]==EOF) return;
		if (capcheck==CAPTION_NOT_OK)
		{
			if ((tmp[0]<='Z')&&(tmp[0]>='A'))
			{
				capcheck=finisher;
				continue;
			}
		}
		decaption(tmp);
		upsert_wordlist(tmp,state,linenum);
		capcheck=finisher;
	}

}

int main()
{
  FILE *f;
  int i;
  IndexesState state;

  if ((f=fopen("stopw.txt","r"))==NULL)
    {
      printf("Can't open file stopw.txt!\n");
      exit(1);
    }

  unneeded_word=unneeded_word_load(f);
  fclose(f);

  NewIndexes(&state);
  //  for (i=0;i<unneeded_word;i++)
  //  printf("%s\n",forbidden[i]);

  if ((f=fopen("vanban.txt","r"))==NULL)
    {
      printf("Can't open file stopw.txt!\n");
      exit(1);
    }
	
	word_reader(f,&state);
	fclose(f);

	for (i=0;i<state.word_count;i++)
	{
		printf("%s [%d times]:  %s\n",LIST[i].wrdentry,LIST[i].wrdcounter,LIST[i].wrdoccurence);
	}

}
