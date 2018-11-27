#include <studio.h>
#include <string.h>
//and more

//goal: parse line and print accordingly
//question: memory management for calloc for 
/* somehow?
Command line length: at least 512 bytes
Commands in a pipeline: at least 10
Arguments to a command: at least 10
*/
//original: contains original string (where we use index to increment with "< >" until it's false)
//stage: string butchered by "|" token
//stuff: string butchered by " "
//use strtoken to find the very first command, ignoring all things before it ("< >")
//compare strtoken and stroken (NULL, "|") to see if it found the end of the stage
//if said strtoken above ^ is not NULL, repeat (to find the extent of the user input relevant to the stage)
//OR use strbreak to find the start of the stage n (+1 then strtoken)
//have a second strtoken that uses " " to break line into elements

//implementation: use the last token to expect the next one

//char *pch=NULL;
//while (pch=strtok) //if pch==NULL, abort
//if lazy, isspace() is there to help you with finding the end of a name :D without relying on strtoken
//limits: 512 bytes for line length (?)
//pointers to pointer for location of the next | or next argument/ ><


int main(){
    printf("line: ");
    char orig[257] = {NULL};
    sscanf(stdin,"line: %s",orig);
/*    int a=parseline(orig);
    if a==1{
        printf("failed to parse line");
    }
    else{
        //the loop
    }*/
    if (orig[257] != NULL){
        perror("command too long");
        return 1;
    }
    int argc=0;
    char *argv[10]={NULL};
    char *cur=orig;
    char *input=stdin;
    char *output=stdout;
    while (*cur != NULL){
        while (isspace(*cur)){
            cur++;
        }
        if (argc==0){
            if (*cur == '<'){ 
                perror("bad input redirection\n");
                printf("failed to yada yada\n");
                return 1;
            }
            if (*cur == '>'){ 
                perror("bad output redirection");
                printf("failed to yada yada\n");
                return 1;
            }
            if (*cur == '|'){ 
                perror("invalid null command");
                printf("failed to yada yada\n");
                return 1;
            }
        }
        //checks if it has existing input/output that's not in/out 
    }
}
//if return 1: failed to parse input (put this into a v
//if first thing is not name, throw null error. If it is, increment argc and put it into argv with pointer and isspace loop

//if redirection, check next one if it's name, NULL, or redirection
int isredirection(char c){
    
}