#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define INPUTLIMIT 512

int scanline(char *line);
void pipePrint(int pipeNum, char *input, char *output,
                char *pipeLine);
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
            printf("     input: pipe from stage %i\n", pipeNum - 1):
            printf("     input: %s\n", input);
        (output == NULL) ?
            printf("    output: pipe to stage %i\n", pipeNum + 1):
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
            (i == countSpace - 1) ? printf("\n") : printf(",");
	    /*Tetiraty operand to check for end of line*/
        }
}

char *copyToSpace(char *source, char *copy){
    while(*source != '<' && *source != '>' && !(isspace(*source)) 
            && *source != '\0'){
	    *(copy++) = *(source++);
    }
    *copy = '\0';
    return source;
}

int correctString(char *string){
    int countPipes = 0, i = 0; /*number of pipes, for looping*/
    char *pipesList[10] = { NULL };
    char *cursor = string, *cur = string;
    char program[257] = "";
    char input[257] = "original stdin";
    char output[257] = "original stdout";
    cursor = copyToSpace(cursor, program);
    
    /*check for input and output*/
    while(*(++cursor) != '\0'){
        if(*cursor == '<'){
            cursor++;
            while(*(cursor++) == ' ');
    	    cursor = copyToSpace(--cursor, input);
    	    while(*(cursor++) == ' ');
    	    cursor--;
        }
        if(*cursor == '>'){
            cursor++;
            while(*(cursor++) == ' ');
    	    cursor = copyToSpace(--cursor, output);
    	    while(*(cursor++) == ' ');
    	    cursor--;
        }
    }
    
    /*Pipes*/
    pipesList[countPipes++] = string;
    cur = string;
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

int scanline(char *line){
    int paramater = 0;
    int input = 1;
    int output = 1;
    int arguments = 0;
    char *scan = line;
    char program[INPUTLIMIT] = { '\0' };
    char *pointProg = program;
    
    while(((strchr(" |<>\n", *scan)) == NULL) && *scan != '\0'){
        *(pointProg++) = *(scan++);
    }
    scan = line;
    
    while(*scan != '|' && *scan != '\0'){
        if(*scan == '<' || *scan == '>'){
            if(paramater){
                paramater = 0;
            }else{
                /*NULL error*/
                printf("%s: invalid null command\n", program);
                return 0;
            }
            
            if(*scan == '<'){
                if(input){
                    input--;
                    arguments--;
                }else{
                    /*input ambiguous*/
                    printf("%s: bad input redirection\n", program);
                    return 0;
                }
            }
            
            if(*scan == '>'){
                if(output){
                    output--;
                    arguments--;
                }else{
                    /*output ambiguous*/
                    printf("%s: bad output redirection\n", program);
                    return 0;
                }
                char *scan2 = scan;
                while(*(scan2++) != '\0'){
                    if(*scan2 == '|'){
                        printf("%s: ambiguous output\n", program);
                        return 0;
                    }
                }
            }
        }else if(*scan != ' '){
            paramater = 1;
            if(++arguments > 10){
                /*too many arguments*/
                printf("%s: too many arguments\n", program);
                return 0;
            }
            while(*(++scan) != ' ' && *scan != '\0');
            scan--;
        }
        scan++;
    }
    if(paramater == 0){
        printf("%s: invalid null command\n", program);
        return 0;
    }
    
    int pipe = 0;
    int numPipes = 0;
    arguments = 0;
    paramater = 0;
    int noPipes = 0;
    if(*scan == '|'){
        scan++;
    }
    /*after pipes*/
    while(*scan != '\0'){
        noPipes = 1;
        if(*scan == '<' || *scan == '>' || *scan == '|'){
            if(*scan == '<'){
                /*input ambiguous*/
                printf("%s: ambiguous input\n", program);
                return 0;
            }
            if(*scan == '>'){
                if(paramater){
                    paramater = 0;
                }else{
                    /*NULL error*/
                    printf("%s: invalid null command\n", program);
                    return 0;
                }
            
                if(output){
                    output--;
                    arguments--;
                    char *scan2 = scan;
                    while(*(scan2++) != '\0'){
                        if(*scan2 == '|'){
                            printf("%s: ambiguous output\n", program);
                            return 0;
                        }
                    }
                }else{
                    /*output ambiguous*/
                    printf("%s: bad output redirection\n", program);
		    return 0;
                }
            }
            if(*scan == '|'){
                if(++numPipes > 10){
                    /*Too many pipes*/
                    printf("pipeline too deep");
                    return 0;
                }
                arguments = 0;
                if(paramater){
                    paramater = 0;
                }else{
                    /*NULL error*/
                    printf("%s: invalid null command\n", program);
                    return 0;
                }
                if(pipe){
                    /*NULL error*/
                    printf("%s: invalid null command\n", program);
                    return 0;
                }
                pipe++;
            }
        }else if(*scan != ' '){
            paramater = 1;
            pipe = 0;
            if(++arguments > 10){
                /*too many arguments*/
                printf("%s: too many arguments\n", program);
                return 0;
            }
            while(*(++scan) != ' ' && *scan != '\0');
            scan--;
        }
        scan++;
    }
    if(paramater == 0 && noPipes){
        printf("%s: invalid null command\n", program);
        return 0;
    }
    
    return 1;
}

int main()
{
    printf("line: ");
    char orig[INPUTLIMIT + 1] = {'\0'};
    //string butchered for cmdline print with "|"
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
    char *scan = orig;
    while(*scan != '\0'){
        if(*scan == '\n'){
            *scan = '\0';
        }
        scan++;
    }
    
    if(scanline(orig)){
        printf("\n");
        correctString(orig);
        return 1;
    }
    perror("failed to parse pipeline\n");
    return 0;
}
