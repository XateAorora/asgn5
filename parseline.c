#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define PIPELIMIT 10
#define ARGLIMIT 10
#define INPUTLIMIT 512
/*
    errors to deal with: 
    empty: expected error message
    noArgs
    Args
    RedirectFrom
    OnePipe
    OnePipeArgs
    ManyPipes
    ManyPipesArgs
*/
/* Design ideas:
original: contains original string (where we use strtkn to print for stage N)
copy: copying orig then used to put /0 after every word (whitespace->/0)
*cur, *last: keep track for > < and for the last whitespace when cur=NULL
*/
int main(){
    printf("line: ");
    char orig[INPUTLIMIT + 1] = {'\0'}; /*for cmdline print with "|"*/
    //sscanf(stdin,"line: %s",orig);
    fgets(orig, INPUTLIMIT + 1, stdin); /*to get the whitespace*/
    if (orig[INPUTLIMIT] != '\0'){ /*ex:512 limit,get 513,last!=NULL->ERROR*/
        printf("command too long\n");
        perror("failed to parse pipeline\n");
        return 1;
    }
    if (orig[0] == '\0' || orig[0] == '\n'){ /*empty string*/
        printf("invalid null command\n");
        perror("failed to parse pipeline\n");
    }
    char copy[INPUTLIMIT+1] = {'\0'}; /*for printing names*/
    strcpy(copy,orig);
    int argcnumber = 0;
    char *argvlist[ARGLIMIT] = {NULL};
    char *cur = copy;
    char *last = &copy[INPUTLIMIT];
    char *input = "original stdin";
    char *output = "original stdout";
    int stage = 0;    
    char *cmdline = strchr(orig, '|');
    int laststage = 0;
    while (cmdline != NULL){
        laststage++;
        cmdline = strchr(cmdline+1, '|');
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
        (*cur == '<' || *cur == '>'  || *cur == '|')){
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
                last = cur; /*for saving as cur will be ++*/
                cur++;
            }
            if (*cur == '\0' || *cur == '|'){
                /*print routine, then with cur==last==NULL, exit loop if \0*/
                if (cmdline == orig){
                    cmdline = strtok(orig, "|\n"); 
                }
                else{
                    cmdline = strtok(NULL, "|\n");
                }
                printf("\n--------\nStage %d: \"%s\"\n--------\n", 
                stage,cmdline);
                if (input == NULL){
                    printf("     input: pipe from stage %d\n", stage-1);
                }
                else {
                    printf("     input: %s\n", input);
                }
                if ((output == NULL)){
                    printf("    output: pipe to stage %d\n", stage+1);
                }
                else {
                    printf("    output: %s\n", output);
                }
                printf("      argc: %d\n", argcnumber);
                printf("      argv: ");
                for (int i = 0; i <argcnumber; i++){
                    printf("\"%s\"", argvlist[i]);
                    if (i != argcnumber-1){
                        printf(",");
                    }
                }
        
                if (*cur == '|'){
                    last = cur; 
                    cur++;
                    stage++;
                    argcnumber = 0;
                    memset(argvlist, NULL, ARGLIMIT);
                    input = NULL; /*pipeline*/
                    if (stage == laststage){
                        output = "original stdout"; /*pipeline logic*/
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
        /*cur is a name*/
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
            last = cur; 
            if (cur = strchr(cur, ' ')){
                *cur = '\0';
                cur++;
            }
            else { /*no more whitespace, NULL*/
                cur = last;
                cur = strchr(cur, '\n');
                *cur = '\0';
            }
        }
    }    
}
