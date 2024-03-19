#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

ssize_t readFromFile(char* path, char* buffer, int size){
    int fd;
    if((fd = open(path, O_RDONLY)) < 0){
        printf("Can't open file");
        exit(-1);
    }
    ssize_t read_bytes = read(fd, buffer, size);
    if(read_bytes == -1){
        printf("Can't read from the file");
        exit(-1);
    }
    buffer[read_bytes] = '\0';
    if(close(fd) < 0){
        printf("Can't close file");
        exit(-1);
    }
    return read_bytes;
}

ssize_t writeToFile(char* path, char* buffer, size_t size) {
    int fd;
    if ((fd = open(path, O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can't open file for writing");
        exit(-1);
    }

    ssize_t count_write = write(fd, buffer, size);
    if (count_write != size) {
        printf("Can't write string due to size");
        exit(-1);
    }

    if (close(fd) < 0) {
        printf("Can't close file");
        exit(-1);
    }
    return count_write;
}

void readProcess(char first_pipe[], char* buffer, char* input) {

    ssize_t read_file = readFromFile(input, buffer, 5000);

    int fd;
    if((fd = open(first_pipe, O_WRONLY)) < 0){
        printf("Can\'t open FIFO for writing\n");
        exit(-1);
    }
    ssize_t write_pipe = write(fd, buffer, read_file);

    if (write_pipe != read_file) {
        printf("Can't write to pipe\n");
        exit(-1);
    }

    if (close(fd) < 0) {
        printf("Can't close pipe\n");
        exit(-1);
    }
}

void WriteProcess(char second_pipe[], char* buffer, char* output_name) {
    int fd;
    if((fd = open(second_pipe, O_RDONLY)) < 0){
        printf("Can't open FIFO for reading\n");
        exit(-1);
    }

    ssize_t read_pipe = read(fd, buffer, 5000);
    ssize_t write_count = writeToFile(output_name, buffer, read_pipe);

    if(write_count != read_pipe){
        printf("Can't write string\n");
        exit(-1);
    }

    if(close(fd) < 0){
        printf("Can't close pipe\n");
        exit(-1);
    }
}


int main(int argc, char** argv){
    if(argc < 3){
        printf("Not enough arguments");
        exit(-1);
    }

    char* input = argv[1];
    char* output = argv[2];
    char buffer[5000];


    char first_pipe[] = "first.fifo";
    char second_pipe[] = "second.fifo";

    mknod(first_pipe, S_IFIFO | 0666, 0);
    mknod(second_pipe, S_IFIFO | 0666, 0);

    readProcess(first_pipe, buffer, input); //родитель читает файл
    WriteProcess(second_pipe, buffer, output);

}