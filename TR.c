#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


struct thread_data{
    FILE *fp;
    long int offset;
    int start;
    int blockSize;
};

int words=0;  

void *countFrequency(void* data){

    struct thread_data* td=data;
    char *buffer = malloc(td->blockSize);

    int i,c;
    i=0;c=0;
    enum states { WHITESPACE, WORD };
    int state = WHITESPACE;

    fseek(td->fp, td->offset, td->start);

        char last = ' '; 
        while ((fread(buffer, td->blockSize, 1, td->fp))==1){

            if ( buffer[0]== ' ' || buffer[0] == '\t'  ){
            state = WHITESPACE;
            }
            else if (buffer[0]=='\n'){
            //newLine++;
                state = WHITESPACE;
            }
            else {
                if ( state == WHITESPACE ){
                    words++;
                }
                state = WORD;
            }
            last = buffer[0];
    }
    free(buffer);

    pthread_exit(NULL);

    return NULL;
}

int main(int argc, char **argv){

    int nthreads, x, id, blockSize,len;
    //void *state;
    FILE *fp;
    pthread_t *threads;

    struct thread_data data[nthreads];

    if (argc < 2){
        fprintf(stderr, "Usage: ./a.out <file_path>");
        exit(-1);
    }

    if((fp=fopen(argv[1],"r"))==NULL){
        printf("Error opening file");
        exit(-1);
    }  

    printf("Enter the number of threads: ");
    scanf("%d",&nthreads);
    threads = malloc(nthreads*sizeof(pthread_t));

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);  
    printf("len= %d\n",len);

    blockSize=(len+nthreads-1)/nthreads;
    printf("size= %d\n",blockSize);

    for(id = 0; id < nthreads; id++){

        data[id].fp=fp;
        data[id].offset = blockSize;
        data[id].start = id*blockSize+1;

        }
        //LAST THREAD
        data[nthreads-1].start=(nthreads-1)*blockSize+1;

        for(id = 0; id < nthreads; id++)
            pthread_create(&threads[id], NULL, &countFrequency,&data[id]);

    for(id = 0; id < nthreads; id++)
        pthread_join(threads[id],NULL);

    fclose(fp);
    //free(threads);

    //pthread_exit(NULL);

    printf("%d\n",words); 
    return 0;  
}