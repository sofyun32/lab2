#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"

// Функция для вывода справки по использованию программы
void print() 
{
    printf("Usage:\n");
    printf("./sed_simplified input.txt -r 'old_regex' 'new_text'\n"); 
    printf("./sed_simplified input.txt -d 'regex'\n"); 
    printf("./sed_simplified input.txt -i 'text_to_add_start'\n"); 
    printf("./sed_simplified input.txt -b 'text_to_add_end'\n"); 
}

int main(int argc, char *argv[]) 
{
    // Проверяем, достаточно ли аргументов для обработки
    if (argc < 4) 
    {
        print(); 
        return 1;
    }

    const char *filename = argv[1]; 
    const char *options = argv[2]; 

    // Обработка параметров для замены текста с помощью регулярного выражения
    if (argc == 5 && strcmp(options, "-r") == 0 ) 
    {
        replace_text_regex(filename, argv[3], argv[4]); 
    } 
    // Обработка параметров для удаления строк
    else if (argc == 4 && strcmp(options, "-d") == 0) 
    {
        delete_lines_regex(filename, argv[3]); 
    } 
    // Обработка параметров для добавления текста в начало строк
    else if (argc == 4 && strcmp(options, "-i") == 0) 
    {
        text_start(filename, argv[3]); 
    } 
    // Обработка параметров для добавления текста в конец строк
    else if (argc == 4 && strcmp(options, "-b") == 0) 
    {
        text_end(filename, argv[3]); 
    } 
    // Если переданы некорректные аргументы, выводим справку
    else 
    {
        print();
        return 1;
    }

    return 0; 
}
