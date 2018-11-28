#include <stdio.h>
#include <string.h>
#include <ctype.h>
int correctString(char *string);
char *copyToSpace(char *source, char *copy);

/*takes string source and copies everything up to a space
  assumes that *copy can handle it*/

void pipePrint(int pipeNum, char *input, char *output,
                char *pipeLine){
        int i = 0, countSpace = 0;
        char *argumentsList[10] = { NULL};
        printf("--------\nStage %i: \"%s\"\n--------\n", pipeNum, pipeLine);
        /*check if or if not a pipe*/
        (input == NULL) ?
            printf("     input: Pipe from stage %i\n", pipeNum - 1):
            printf("     input: %s\n", input);
        (output == NULL) ?
            printf("    output: Pipe to stage %i\n", pipeNum + 1):
            printf("    output: %s\n", output);
        
        /*checks for argumentsList*/
        char *cur = pipeLine;
        do{
            while(*cur == ' '){
                *(cur++) = '\0'; 
            }
            if(*cur == '<' || *cur == '>'){
                if(*(++cur) == ' '){
                    while(*cur == ' '){
                        *(cur++) = '\0'; 
                    }
                }
                while(*cur != ' ' && *cur != '\0'){
                    *(cur++) = '\0'; 
                }
            }else if(*cur != '\0'){
                argumentsList[countSpace++] = cur;
            }
        }while((cur = strchr(cur, ' ')) != NULL);
        
        printf("      argc: %i\n", countSpace);
        printf("      argv: ");
        for(; i < countSpace; i++){
            printf("\"%s\"", argumentsList[i]);
            (i == countSpace - 1) ? printf("\n") : printf(","); /*Tetiraty operand to check for end of line*/
        }
        printf("--------\n");
}

char *copyToSpace(char *source, char *copy){
    while(*source != '<' && *source != '>' && !(isspace(*source)) 
            && *source != 0)){
	    *(copy++) = *(source++);
    }
    *copy = '\0';
    return source;
}

int correctString(char *string){
    int countPipes = 0, i = 0; /*number of pipes, for looping*/
    char *pipesList[10] = { NULL };
    char *argumentsList[10] = { NULL };
    char *cursor = string, *cur = string;
    char program[257] = "";
    char input[257] = "original stdin";
    char output[257] = "original stdout";
    cursor = copyToSpace(cursor, program);
    
    /*check for input and output*/
    while(*(++cursor) == ' ');
    if(*cursor == '<'){
        while(*(cursor++) == ' ');
	    cursor = copyToSpace(cursor, input);
	    while(*(cursor++) == ' ');
    }
    if(*cursor == '<'){
        while(*(cursor++) == ' ');
	    cursor = copyToSpace(cursor, output);
	    while(*(cursor++) == ' ');
    }
    if(*cursor == '<'){
        cursor++;
        while(*(cursor++) == ' ');
	    cursor = copyToSpace(cursor, input);
    }
    
    /*Pipes*/
    pipesList[countPipes++] = string;
    while((cur = strchr(cur, '|')) != NULL){
        pipesList[countPipes++] = cur + 1;
        *(cur++) = '\0';
    }
    for(i = 0; i < countPipes; i++){
        pipePrint(i, ((i == 0) ? input : NULL),
                    ((i == countPipes - 1) ? output : NULL), pipesList[i]);
    }
    return 1;
}

int main()
{
    char str[256] = "ls< a";
    correctString(str);
    return 0;
}
