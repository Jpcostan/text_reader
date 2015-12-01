/*Joshua Costanza*/
/*CSCI 423*/
/*textReader.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define MAX_WORD 32
#define MAX_TEXT_LENGTH 10000
#define NUM_THREADS 2

struct fSize {
	
	long int offset;
	int start;
	int chunkSize;
};

typedef struct word {
/*str to store the word*/	
char *str;              
/*fred to store the frequency of the word*/
int freq;              
/*word for the pointer to the next word counter in the list*/
struct word *Next;   
} Word;

/*addWord will add a word to the list and update it*/
void addWord(char *pWord);    
/*printWords will print the words and its frequency*/             
void printWords(Word *pWordcounter);
/*getWords will grab the words from the file*/
int getWords(FILE *file, char *buf, int bufsize);
/*createWordCounter will create a new word counter struc*/
Word* makeWordCounter(char *word); 
/*point will point to the 1st counter within the list*/
Word *point = NULL;                 
/*total will store total amount of words in the file*/
int total = 0;          
/*individual will store the total amount of individual words*/
int individual = 0;              

/*addWord*/
void addWord(char *word){
  Word *pCount = NULL;
  Word *pLast = NULL;

  if(point == NULL){
    point = makeWordCounter(word);
    return;
  }

  /* Incrament word in the list */
  pCount = point;
  while(pCount != NULL){
    if(strcmp(word, pCount->str) == 0){
      ++pCount->freq;

      return;
    }
    pLast = pCount;            
    pCount = pCount->Next;  
  }

  pLast->Next = makeWordCounter(word);
}
/*printWords*/
void printWords(Word *pWordcounter){
	printf("\n%-30s   %5d", pWordcounter->str,pWordcounter->freq);

}
/*getWords*/
int getWords(FILE *file, char *buf, int bufsize){
    char *p = buf;
    char c;


    do {
        c = fgetc(file);
        if (c == EOF) 
            return 0;
        } while (!isalpha(c));

    //read word chars

    do {
        if (p - buf < bufsize - 1)
        *p++ = tolower(c);
        c = fgetc(file);
        } while (isalpha(c));

        //finalize word
        *p = '\0';
        return 1;
}
	
Word* makeWordCounter(char *word){
  Word *pCount = NULL;
  pCount = (Word*)malloc(sizeof(Word));
  pCount->str = (char*)malloc(strlen(word)+1);
  strcpy(pCount->str, word);
  pCount->freq = 1;
  pCount->Next = NULL;
  return pCount;
}

	//pthread_exit(NULL);


int main(){
	
	
	/* Thread */
	pthread_t thread1;
    /* File pointer */
    FILE * file;
	printf("Searching for file...\n");
    /* .txt file name goes here */
    file = fopen("myEssay.txt","r");

    /* buf to hold the words */
    char buf[MAX_WORD];

    /* Size being instantiated  */
    int size = MAX_TEXT_LENGTH;
	/*Status variable */
	void *status;

    /* Pointer to Word counter */
    Word *pCount = NULL;
	
	

    	while (getWords(file, buf, size)){

        	/* Create thread and add the word to the list */
			pthread_create(&thread1, NULL, addWord,buf);
        	//addWord(buf); 
        	total++;
    	}
		pthread_join(thread1,&status);

		//}	
    /* Loop through list and figure out the number of  words */
    pCount = point;
    while(pCount != NULL){
        individual++;
        pCount = pCount->Next;
    }

	/*This while loop will list the total number of words and their frequency shown in the doc*/
    pCount = point;
    while(pCount != NULL){
        printWords(pCount);
        pCount = pCount->Next;
    }
    printf("\n");

    pCount = point;
    while(pCount != NULL){
        free(pCount->str);        
        point = pCount;           
        pCount = pCount->Next;  
        free(point);                  
    }
	
    printf("The following is the total number of words\n");
	/*Print the total number of words in the file*/
    printf("There are currently:  %d words total\n", total); 
	/*Print the total number of individual words*/
    printf("There are currently:  %d individual words total\n", individual);

    fclose(file);

	pthread_exit(NULL);

    return 0;

}