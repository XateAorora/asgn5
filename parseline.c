#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define PIPELIMIT 10
#define ARGLIMIT 10
#define INPUTLIMIT 512
//and more

//goal: parse line and print accordingly
//question: memory management for calloc for 
/* somehow?
Command line length: at least 512 bytes
Commands in a pipeline: at least 10
Arguments to a command: at least 10
*/
//original: contains original string (where we use strchr to increment with "< >" until it's false)
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
    char orig[INPUTLIMIT + 1] = {'\0'}; //string butchered for cmdline print with "|"
    //sscanf(stdin,"line: %s",orig);
    fgets(orig, INPUTLIMIT + 1, stdin);
    if (orig[INPUTLIMIT] != '\0'){
        printf("command too long\n");
        perror("failed to parse pipeline\n");
        return 1;
    }
    if (orig[0] == '\0'){
        printf("invalid null command\n");
        perror("failed to parse pipeline\n");
    }
    char copy[INPUTLIMIT+1] = {'\0'}; //string butchered with \0 after each name
    strcpy(copy,orig);
    int argcnumber = 0;
    char *argvlist[ARGLIMIT] = {NULL};
    char *cur = copy;
    char *last = &orig[INPUTLIMIT];
    char *input = "original stdin";
    char *output = "original stdout";
    int stage = 0;    
    char *cmdline = orig;
    int laststage = 0;
    while (cmdline = strchr(cmdline, '|')){
        laststage++;
    }
    cmdline = orig;
    if (laststage >= PIPELIMIT){
        printf("pipeline too deep\n");
        perror("failed to parse pipeline\n");
        return 1;
    }
    while (*cur != '\0' || *last != '\0'){
        while (isspace(*cur)){
            cur++;
        }
        if (argcnumber == 0 && 
        (*last == '<' || *last == '>'  || *last == '|')){
            printf("invalid null command\n");
            perror("failed to parse pipeline\n");
            return 1;
        }
        if (*cur == '<' || *cur == '>' || *cur == '|' || *cur == '\0'){
            if (*last == '<'){ 
                printf("%s: bad input redirection\n", argvlist[0]);
                perror("failed to parse pipeline\n");
                return 1;
            }
            if (*last == '>'){ 
                printf("%s: bad output redirection\n", argvlist[0]);
                perror("failed to parse pipeline\n");
                return 1;
            }
            if (*cur == '>' || *cur == '<'){
                last = cur; //for saving as cur will be ++
                cur++;
            }
            if (*cur == '\0' || *cur == '|'){
                //print routine: checks if output has pipe for ambiguous, and this is where output is changed based on pipe
                //print routine, then with cur==last==NULL, exit loop if \0
                if (cmdline == orig){
                    cmdline = strtok(orig, "|\n"); 
                }
                else{
                    cmdline = strtok(NULL, "|\n");
                }
                printf("\n--------\nStage %d: \"%s\"\n--------\n", stage,cmdline);

                if (input != NULL){
                    printf("     input: %s\n", input);
                }
                else {
                    printf("     input: pipe from stage %d\n", stage-1);
                }
                if (output != NULL){
                    printf("    output: %s\n", output);
                }
                else {
                    printf("    output: pipe to stage %d\n", stage+1);
                }
                printf("      argc: %d\n", argcnumber);
                printf("      argv: ");
                for (int i = 0; i <argcnumber; i++){
                    printf("%s", argvlist[i]);
                    if (i != argcnumber-1){
                        printf(",");
                    }
                }
        
                if (*cur == '|'){
                    last = cur; //for saving as cur will be ++
                    cur++;
                    stage++;
                    argcnumber = 0;
                    memset(argvlist, NULL, ARGLIMIT);
                    input = NULL;
                    if (stage == laststage){
                        output = "original stdout";
                    }
                    else{
                        output = NULL;
                    }
                }
                else{ //cur == '\0'
                    last = cur;
                }
            }
        }
        //cur is a name
        //checks the last pointer to decide cur's function as name
        else{
            if (*last == '<'){
                if (stage != 0){
                    printf("%s: ambiguous input\n", argvlist[0]);
                    perror("failed to parse pipeline\n");
                    return 1;         
                }
                if (input == "original stdin") {
                    input = cur;
                }
            }
            if (*last == '>'){
                if (stage < laststage){
                    printf("%s: ambiguous output\n", argvlist[0]);
                    perror("failed to parse pipeline\n");
                    return 1;         
                }
                if (output == "original stdout") {
                    output = cur;
                }
            }
            if (argcnumber == ARGLIMIT){
                printf("%s: too many arguments\n", argvlist[0]);
                perror("failed to parse pipeline\n");
                return 1;
            }
            if (*last != '>' && *last != '<'){
                argvlist[argcnumber] = cur;
                argcnumber++;
            }
            last = cur; //for saving as cur will be ++
            if (cur = strchr(cur, ' ')){
                *cur == '\0';
                cur++;
            }
            else { //no more whitespace, NULL
                cur = last;
                cur = strchr(cur, '\0');
            }
        }
        //checks if it has existing input/output that's not in/out 
    }
    //end of line, repeat everything 1 last time
    
}
//if return 1: failed to parse input (put this into a v
//if first thing is not name, throw null error. If it is, increment argc and put it into argv with pointer and isspace loop

//if redirection, check next one if it's name, NULL, or redirection
