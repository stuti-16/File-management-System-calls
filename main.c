#include<unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include "main.h"


void main(int argc, char* argv[])
{ 
    if(argc == 1){
        printf("Type: ./.exe <operation> \n\t <operation> : create, read, write, stat, namedpipe, copy\n");
        exit(1);
    }
    if(strcmp(argv[1],"create")==0 ){
        if(argc != 4){printf("To create type : \n\t ./.exe create <filename> <permissions> \n");}
        if(argc == 4){
            char* f =  argv[2];
            long p =  strtol(argv[3],NULL,8);            
            create(f,p);
        }        
    }
    else if(strcmp(argv[1],"read")==0){
        if(argc != 6){printf("To read type in the format: \n\t read <filename> <no. of bytes to read> <offset> <whence> \n\t <To set the whence>: \n\t\t Type 0 to read from starting \n\t\t Type 1 to read from current position \n\t\t Type 2 to read from end\n");}
        if(argc == 6){
            char* f =  argv[2];
            int s =  atoi(argv[3]);
            int o =  atoi(argv[4]);
            int w =  atoi(argv[5]);
            readFile(f,s,o,w);
        }        
    }
    else if(strcmp(argv[1],"write")==0 ){
        if(argc != 6){printf("To write type: \n\t write <filename> <data you want to write> <offset> <whence> \n\t <To set the whence>: \n\t\t Type 0 to write from starting \n\t\t Type 1 to write from current position \n\t\t Type 2 to write from end\n");}
        if(argc == 6){
            char* f =  argv[2];
            char* d =  argv[3];
            int o =  atoi(argv[4]);
            int w =  atoi(argv[5]);
            writeFile(f,d,o,w);
        }        
    }
    else if(strcmp(argv[1],"stat")==0){
        if(argc != 3){printf("To display the statical information of the file type: stat <filename> \n");}
        if(argc == 3){
            char* f =  argv[2];        
            statInfo(f);
        }        
    }
    else if(strcmp(argv[1],"namedpipe")==0){
        if(argc != 5){printf("To create a named pipe type: namedpipe <pipename> <permissions> <operation> \n\t <operation> : write, read \n");}
        if(argc == 5){
            char* f =  argv[2];
            long p =  strtol(argv[3],NULL,8);
            char* o =  argv[4];        
            namedpipe(f,p,o);
        }
    }
    else if(strcmp(argv[1],"copy")==0){
        if(argc != 4){printf("To copy the data from one file to other using unamed pipes, type:\n\t copy <Source_filename>  <destination_filename>\n");}
        if(argc == 4){
            char* s =  argv[2];
            char* d =  argv[3];        
            copy(s,d);
        }        
    }
    else{
        printf("Invalid Arguments\n");
    }
    

}
