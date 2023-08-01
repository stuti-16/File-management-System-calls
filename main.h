void create(char* filename,mode_t permissions);
void readFile(char* filename, int size, int offset, int whence);
void writeFile(char* filename, char* data, int offset, int whence);
void namedpipe(char* pipename, mode_t permission, char* operation);
void copy(char* filename1,char* filename2 );
void statInfo(char* filename);