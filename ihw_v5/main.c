#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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

int main(int argc, char** argv){
    char buffer[5000];
    char first_pipe[] = "first.fifo";
    char second_pipe[] = "second.fifo";
    HandleProcess(first_pipe, second_pipe, buffer);   //ребенок обрабатывает строку
}