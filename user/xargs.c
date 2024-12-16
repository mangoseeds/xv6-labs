// arguments describe a command to run
// xargs reads lines from the standard input, 
// and runs the command for each line, appending the line to the command's arguments

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 1024

int
main(int argc, char *argv[])
{
  char line[MAXLINE];
  char *new_argv[MAXARG];
  int n;
  
  if(argc < 2){
    fprintf(2, "usage: xargs command [args...]\n");
    exit(1);
  }
  
  // copy command and its arguments to new_argv
  for(int i = 1; i < argc; i++){
    new_argv[i-1] = argv[i];
  }
  
  // read input by each line
  while(1){
    int i = 0;
    
    // read characters until newline or EOF
    while(i < MAXLINE-1){
      n = read(0, &line[i], 1);
      if(n <= 0 || line[i] == '\n'){
        break;
      }
      i++;
    }
    
    if(n <= 0 && i == 0){
      break;  // EOF with no data
    }
    
    // null terminate the line
    line[i] = 0;
    
    // add line as last argument
    new_argv[argc-1] = line;
    new_argv[argc] = 0;  // null terminate argv
    
    // fork and exec
    int pid = fork();
    if(pid < 0){
      fprintf(2, "xargs: fork failed\n");
      exit(1);
    }
    
    if(pid == 0){
      // child
      exec(new_argv[0], new_argv);
      fprintf(2, "xargs: exec %s failed\n", new_argv[0]);
      exit(1);
    }
    
    // parent process waits for child
    wait(0);
  }
  
  exit(0);
}