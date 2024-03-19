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

ssize_t writeToFile(char* path, char* buffer, size_t size){
    int fd;
    if((fd = open(path, O_WRONLY | O_CREAT, 0666)) < 0){
        printf("Can't open file for writing");
        exit(-1);
    }

    ssize_t count_write = write(fd, buffer, size);
    if(count_write != size){
        printf("Can't write string due to size");
        exit(-1);
    }

    if(close(fd) < 0){
        printf("Can't close file");
        exit(-1);
    }
    return count_write;
}

ssize_t findKeyWord(char* source, char* result){
    ssize_t index = 0;
    char *keywords[] = {"hello", "men", "go", "and", "with"};
    int counts[5] = {0};
    char *word = strtok(source, " ,.'");

    while (word != NULL){
        for (int i = 0; i < 5; i++) {
            if (strcmp(word, keywords[i]) == 0) {
                // Если слово совпадает с ключевым словом, увеличиваем счетчик
                counts[i]++;
            }
        }
        word = strtok(NULL, " ,.'");
    }
    char temp[500]; // Буфер для временного хранения результатов
    result[0] = '\0'; // Обнуляем строку dest перед записью
    for (int i = 0; i < 5; i++) {
        sprintf(temp, "Ключевое слово \"%s\" встретилось %d раз(а)\n", keywords[i], counts[i]);
        strcat(result, temp); // Добавляем строку temp к строке dest
    }
    return strlen(result);
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

void HandleProcess(char first_pipe[], char second_pipe[], char* buffer) {

    int first_fd;
    if((first_fd = open(first_pipe, O_RDONLY)) < 0){
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    ssize_t read_pipe = read(first_fd, buffer, 5000);

    if (read_pipe < 0) {
        printf("Can't read from pipe\n");
        exit(-1);
    }

    ssize_t count = findKeyWord(buffer, buffer);

    int second_fd;
    if((second_fd = open(second_pipe, O_WRONLY)) < 0){
        printf("Can't open FIFO for reading\n");
        exit(-1);
    }

    ssize_t write_pipe = write(second_fd, buffer, count);

    if (write_pipe != count) {
        printf("Can't write string\n");
        exit(-1);
    }

    if (close(second_fd) < 0) {
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


    int pid = fork();
    if(pid < 0){
        printf("Can't fork child");
        exit(-1);
    } else if (pid == 0){
        WriteProcess(second_pipe, buffer, output);
    } else {
        pid_t reader_pid = fork();
        if (reader_pid < 0) {
            printf("Can't fork child");
            exit(-1);
        } else if (reader_pid == 0) {
            HandleProcess(first_pipe, second_pipe, buffer);   //ребенок обрабатывает строку
        } else {
            readProcess(first_pipe, buffer, input); //родитель читает файл
        }
    }
}