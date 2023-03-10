#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
char initialDir[1024]; // initial directory so that if cd command was used doesn't change the path that the log file will be saved in
int checkMultipleExecutions (char*userInput)
{
    int length= strlen(userInput);
    if(userInput[length-1]=='&'||userInput[length-2]=='&')
        {return 1;}
    return 0;
}
void logFile(char*message)
{
//create file if it doesn't exist or append existing file when
    FILE *fp;
    char pathtemp[1024];
    strcpy(pathtemp,initialDir);
    strcat(pathtemp,"/file.log");
    fp=fopen(pathtemp,"a");

    if(fp==NULL)
    {
        printf("Error in file");//log file not created
        exit(1);
    }
    fprintf(fp,"%s",message);
    fclose(fp);
}
void signalHandler(int value)
{
waitpid(0,NULL, WNOHANG);
    if(value ==SIGCHLD)
    {
        logFile("Child process was terminated\n");//appends in logfile when SIGCHLD is found
    }
}

int main()
{
    signal(SIGCHLD,signalHandler); //SIGCHLD refers to child stopped or terminated
    
    
    char userInput[200]; //input taken from user
    char copyUserInput[200]; //copy from what user has entered because we will use tokens that will destroy the string
    char *token;
    char *del=" <>&:;\n\t\0";  //delimeters
    char *arguments[200];      //array of tokens
    int i=0;
    int flagOfMultiples=0;    //indicator(CHECKER) for the presence of &
    char currentDir[1024];
    getcwd(initialDir,sizeof(initialDir)); //copies absolute pathname of the current working directory to the array pointed to,if needed later on for cd
    while(1)
    {

        printf("shell > ");
        fgets(userInput,200,stdin);
        flagOfMultiples=checkMultipleExecutions (userInput);//have the flag that decides later on wait or not
        strcpy(copyUserInput,userInput); //this step is made previous to tokenization as tokenization will destroy the original input
        token=strtok(userInput,del);//first token
        while(token!=NULL)
        {
            arguments[i]=token;
            token=strtok(NULL,del);
            i++;
        }
        arguments[i]=NULL;
//case of exit
        if(strcmp(arguments[0],"exit")==0)
            exit (0);

        else if (arguments[0]==NULL)
            continue;
//cd command       
  
        else if(strcmp(arguments[0],"cd")==0)            
        {
          int dir = chdir(arguments[1]);
          if (dir ==-1)
          {
            printf(" no such file or directory \n"); }
         else
         {
            getcwd(currentDir,sizeof(currentDir));
            printf("current path %s\n",currentDir);
         }
        
        } 
//case of execution of arguments
        else
        {
            pid_t returnedValue=fork();//returned value is  the process ID
            int status; //for the waitpid
      //fork successful:child or parent
            if(returnedValue==0)
            {
                //child code
                if(execvp(arguments[0],arguments)<0)//execvp executes command that is present
                {
                    printf("Error!Command not found\n");
                    exit(1);
                }
            }
            else if(returnedValue>0)
            {
               //parent code
                if(flagOfMultiples==0)
                {
                 //not background
                 waitpid(returnedValue,&status,0); //parent waits for its child to be terminated not any child
                }
            }
            //fork failure
            else
            {
                printf("fork failed unable to create child process");
                return 1;
            }
        }


//reset array
        for(i=0; i<200; i++)
        {
            arguments[i]=""; 
           
        }
         userInput[0]='\0'; // \0 is default value for initiallization for char arrays
//reset variables
        flagOfMultiples=0;
        i=0;
    }
}