#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_LINE_LENGTH 1024

int compile_regex(const char *regex_src, regex_t *compiled)
{
    int reti = regcomp(compiled, regex_src, REG_EXTENDED | REG_NOSUB);
    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int match_regex(const char *to_match, const regex_t *compiled)
{
    return !regexec(compiled, to_match, 0, NULL, 0);
}

//Функция замены всех вхождений old_str на new_str
void replace_all(char *line, const char *old_str, const char *new_str)
{
    char *pos = strstr(line, old_str);
    while (pos)
    {
        memmove(pos + strlen(new_str), pos + strlen(old_str), strlen(pos + strlen(old_str)) + 1);
        memcpy(pos, new_str, strlen(new_str));
        pos = strstr(pos, old_str);
    }
}

//Функция удаления строк, содеражщих pattern
void delete_lines(FILE *input, const char *filename, const char *pattern)
{
    rewind(input); //возвращемся в начало файла
    char temp_filename[] = "temp_XXXXXX";
    mkstemp(temp_filename); //создаем временный файл
    FILE *temp = fopen(temp_filename, "w"); //открываем временный файл для записи
    if (!temp)
    {
        perror("Error creating temporary file");
        fclose(input);
        remove(temp_filename);
        exit(EXIT_FAILURE);
    }

    regex_t compiled;
    compile_regex(pattern, &compiled);

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input))
    {
        if (!match_regex(line, &compiled))
        {
            fputs(line, temp); //если строка не соответствует шаблону, то пишем ее во временный файл
        }
    }

    fclose(input);
    fclose(temp);

    rename(temp_filename, filename); // переименовываем временный файл в оригинальный
}

//функция добавления текста в начало строки
void insert_front(char *line, const char *str)
{
    size_t len = strlen(str);
    memmove(line + len, line, strlen(line) + 1);
    memcpy(line, str, len);
}

//функция добавления текста в конец строки
void insert_back(char *line, const char *str)
{
    strcat(line, str);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./sed_simplified input_file command\n");
        return EXIT_FAILURE;
    }

    FILE *input = fopen(argv[1], "r+");
    if (!input)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input))
    {
        if (strncmp(argv[2], "s/", 2) == 0)
        {
            char *old_text= strtok(&argv[2][2], "/");
            char *new_text = strtok(NULL, "/");
            if (old_text && new_text)
            {
                replace_all(line, old_text, new_text);
            } else {
                fprintf(stderr, "Invalid replacement syntax\n");
                fclose(input);
                return EXIT_FAILURE;
            }
        } else if (strncmp(argv[2], "/", 1) == 0 && strncmp(&argv[2][strlen(argv[2]) - 2], "/d", 2) == 0)
        {
            char *pattern = &argv[2][1];
            delete_lines(input, argv[1], pattern);
            continue; //Переход к следующей строке
        } else if (strncmp(argv[2], "s/$/", 3) == 0) //вставка в конец строки
        {
            char *str = &argv[2][3];
            insert_back(line, str);
        } else {
            fprintf(stderr, "Unknown command\n");
            fclose(input);
            return EXIT_FAILURE;
        }

        fputs(line, stdout); //выводим результат на экран

        fclose(input);
        return EXIT_FAILURE;
    }
}