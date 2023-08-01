#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void create(char* filename,mode_t permissions){
    //creating file
    int fd = open(filename, O_CREAT | O_WRONLY ,permissions);
    if(fd == -1){
        perror("open");
        exit(1);
    }
    printf("File %s created\n", filename);
    close(fd);
}
void readFile(char* filename, int size, int offset, int whence){
    int fd = open(filename, O_RDONLY); //opening file
    char buff[size + 1];
    if(fd == -1){
        perror("open");
        exit(1);
    }
    lseek(fd,offset,whence); //setting offset
    int s = read(fd,buff,size); //reading the data in buffer
    printf("Data read: ");
    printf("\n");
	write(1,buff,s);
    printf("\n");
    close(fd);
}
void writeFile(char* filename, char* data, int offset, int whence){
    int fd = open(filename, O_WRONLY );
    int size = strlen(data);   
    if(fd == -1){
        perror("open");
        exit(1);
    }
    lseek(fd,offset,whence); //setting offset
    int s = write(fd,data,size);
    printf("Data written in file %s: ", filename);
    printf("\n");
    write(1,data,s);     
    printf("\n");
    close(fd);
}
void statInfo(char* filename){
    struct stat r_stat;
    printf("Statistical info about the file \n");
    if (stat(filename, &r_stat) == -1){
        printf("File %s not present\n" , filename);    
        exit(1);
    }
    printf("File name: %s\n" , filename);
    printf("Permissions: %o\n" , r_stat.st_mode);
    printf("Inode number: %lu\n" , (unsigned long)r_stat.st_ino);
    printf("Size: %lu bytes\n" , (unsigned long)r_stat.st_size);
    printf("Last accessed: %s\n" , ctime(&r_stat.st_atime));       
    printf("Last modified: %s\n" , ctime(&r_stat.st_mtime));
    printf("Last status changed: %s\n" , ctime(&r_stat.st_ctime));
}

void namedpipe(char* pipename, mode_t permission, char* operation){
    int result, pfd;  
    //Writing in pipe
    if(strcmp(operation,"write")==0){
        result = mknod(pipename,permission|__S_IFIFO ,0); //it will create a named pipe with the given name
        if (result == 0) {
            printf("%s pipe created successfully\n",pipename);    
        } else {
            perror("mknod");    
            exit(1);
        }
        char data[2056];
        printf("Enter your data: ");
        scanf("%[^\n]s",data);
        data[2055] = '\0';        
        int size = strlen(data);
        pfd = open(pipename, O_WRONLY);
        if (pfd == -1) {            
            perror("open");
            exit(1);
        } 
        int s = write(pfd,data,size);
        if(s<=0){            
            perror("write");
            exit(1);
        }
        printf("Data written in pipe %s successfully \n", pipename);              
        close(pfd);        
    }
    //Reading from pipe
    else if(strcmp(operation,"read")==0) {
        pfd = open(pipename, O_RDONLY); 
        if (pfd == -1) {         
            perror("open");
            exit(1);
        }
        char buff[2056];
        int s = read(pfd,buff,2055);
        buff[s] = '\0';
        if(s<=0){
            printf("Erorr in reading from the pipe!");
            exit(1);
        }
        printf("Received message: %s\n", buff);             
        close(pfd);
    }
    else{
        printf("Check the parameters\n");
        close(pfd);
    }
    
}
void copy(char* filename1,char* filename2 ){
    int pipefd[2];
    pid_t pid;
    char* sourceFile = filename1;
    char* destinationFile = filename2;

    //if the destination file exist then return error
    int fd = open(destinationFile, O_WRONLY );
    if(fd == -1){

        if (pipe(pipefd) == -1) { // Creating pipe
            perror("pipe");
            exit(1);
        }        
        pid = fork(); // Fork a child process
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        // Child process
        if (pid == 0) {
            close(pipefd[0]);  // Close the read end of the pipe
            
            // Open the source file for reading
            int sourceFd = open(sourceFile, O_RDONLY);
            if (sourceFd == -1) {
                perror("open");
                exit(1);
            }
            // Read from the source file and write to the pipe
            char buff[2056];
            ssize_t s;
            while ((s = read(sourceFd, buff, 2055)) > 0) {
                write(pipefd[1], buff, s);
            }
            close(sourceFd);
            close(pipefd[1]);  // Close the write end of the pipe
            exit(0);
        }
        // Parent process
        else if (pid > 0){
            close(pipefd[1]);  // Close the write end of the pipe
            // create the destination file for writing
            int destFd = open(destinationFile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (destFd == -1) {
                perror("open");
                exit(1);
            }

            // Read from the pipe and write to the destination file
            char buff[2056];
            ssize_t s;
            while ((s = read(pipefd[0], buff, 2055)) > 0) {
                write(destFd, buff, s);
            }

            close(destFd);
            close(pipefd[0]);  // Close the read end of the pipe
            printf("File copied successfully\n");
            exit(0);
        }
    }
    else{
        printf("%s already exits\n", filename2);
    }
}
