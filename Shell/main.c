#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include<time.h>
int TEMPPID;

void ToLowerCase(char* Str)   //convert all string to lower case..
{
   for(int i = 0; i<strlen(Str); i++)
    {
      Str[i] = tolower(Str[i]);
    }
}
void Logger()    //write in  the log and addition on it ..
{
   FILE * log;
   int i;
   log = fopen ("LogFile.txt","a+");
   time_t t;
   time(&t);
   fprintf (log, "Child process was terminated his PID is %d ,the time and date is %s\n",TEMPPID,ctime(&t));

   fclose (log);
}
bool StoreInput(char * x)  //take the input from the user..
{
    gets(x);
    if(strlen(x)==0)
    {
        return false;
    }
    else
        return true ;
}
int SplitInput(char*TotalOrder,char**SeperatedOrder,char*CmdName)   //split the string which take from user and store it in array of string ..,the split done by one white space.
{

    char *temp=strtok(TotalOrder," ");
    int i=0;
    strcpy(CmdName,temp);
    while(temp!=NULL)
    {
        SeperatedOrder[i]=temp;
        temp=strtok(NULL," ");
        i++;
    }
    return i;
}
void ExecuteChild(char**SeperatedOrder,char*CmdName,bool DoBack)     //create the child process..
{
    pid_t PID;
    PID =fork();

    TEMPPID=PID;
    int x;
    if(PID<0)
    {
        perror("Fork Failed");
        //fprintf(stderr,"Fork Failed");
    }
    else if(PID==0)
    {

      execvp(CmdName,SeperatedOrder) ;
    }
    else
    {

        //signal(SIGKILL,Logger);
        if(!DoBack)
        {
            //waitpid(PID,&x,WUNTRACED);
          wait(NULL);
        }
        else
        {
          //kill(getpid(), SIGKILL);

          //exit(0);
        }

    }

}
void control()
{

}
int main()
{
    signal(SIGCHLD,Logger);
    while (true)
    {
        char InstructionsInput[100];
        char CommandName[100];
        char * CommandStructure[100]={NULL};

        char* username = getenv("USER");      //take the user name
        printf ("\033[32;1m%s@%s\033[0m",username,username);    //print in green color and blue ..
        printf(":\033[34;1m~\033[0m$ ");

        bool CheckInput=StoreInput(InstructionsInput);    //if the input if not null the checkinput take the value true..

        if(CheckInput==true)
        {
            bool found=false;
            if(InstructionsInput[strlen(InstructionsInput)-1]=='&')   //delete the & if the input contain it ..
            {
                char t[100];
               // memcpy(t,&InstructionsInput[0],strlen(InstructionsInput)-1);
                /*strncpy(t,InstructionsInput,strlen(InstructionsInput)-1);
                strcpy(InstructionsInput,t);*/
                InstructionsInput[strlen(InstructionsInput)-1]='\0';
                //printf("%s\n",InstructionsInput);
                found=true;
            }
            int TotalSize=SplitInput(InstructionsInput,CommandStructure,CommandName);
            char *temp=CommandName;
            ToLowerCase(temp);
            if(strcmp(temp,"exit")==0)    //handle the exit command
            {
                exit(EXIT_SUCCESS);   //kill(getppid(), SIGKILL);

            }
            else    //handle any command expect the exit command..
            {
                ExecuteChild(CommandStructure,CommandName,found);
            }

        }

       //control();
    }


    return 0;
}
