#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define BYTE_CODE_SIZE 512

#define MOV_AH 0xB4
#define MOV_AL 0xB0
#define INT 0xCD
#define JMP 0xEA
#define HLT 0xF4

typedef struct label_st
{
    char name[32];
    char address;
} LABEL;

void assemble(const char *source_filename, const char *bin_filename);
void write_to_binary(FILE *file, uint8_t *code, int size);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <source_filename> <bin_filename>\n", argv[0]);
        return 1;
    }
    assemble(argv[1], argv[2]);

    return 0;
}

void trim(char *str, char *str1)
{
    int idx = 0, j, k = 0;

    while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n')
    {
        idx++;
    }

    for (j = idx; str[j] != '\0'; j++)
    {
        str1[k] = str[j];
        k++;
    }

    str1[k] = '\0';
}

void assemble(const char *source_filename, const char *bin_filename)
{
    FILE *src_file = fopen(source_filename, "r");
    if (!src_file)
    {
        printf("Error opening source file: %s.\n", source_filename);
        return;
    }

    FILE *bin_file = fopen(bin_filename, "wb");
    if (!bin_file)
    {
        printf("Error creating binary file: %s.\n", bin_filename);
        fclose(src_file);
        return;
    }

    uint8_t code[BYTE_CODE_SIZE];
    int index = 0;
    char line_space[256];
    char line[256];

    int n_labels = 0;
    LABEL labels[16];

    while (fgets(line_space, sizeof(line), src_file))
    {
        trim(line_space, line);

        if (line[0] == ';' || line[0] == '\n')
        {
            continue;
        }

        char *token = strtok(line, " ");
        if (token)
        {
            /* Move */
            if (strcmp(token, "mov") == 0)
            {
                token = strtok(NULL, " ");
                char value;
                /* ASCII */
                if (token[1] == '\'')
                {
                    value = token[2];
                }
                /* Hex */
                else
                {
                    sscanf(&token[3], "%hhx", &value);
                }

                token = strtok(NULL, " ");
                if (token[2] == 'h' || token[2] == 'H')
                {
                    code[index++] = MOV_AH;
                }
                else if (token[2] == 'l' || token[2] == 'L')
                {
                    code[index++] = MOV_AL;
                }
                code[index++] = value;
            }
            /* Interruption */
            else if (strcmp(token, "int") == 0)
            {
                token = strtok(NULL, " ");
                code[index++] = INT;

                char value;
                sscanf(&token[3], "%hhx", &value);

                code[index++] = value;
            }
            /* Jump */
            else if (strcmp(token, "jmp") == 0)
            {
                token = strtok(NULL, " ");
                code[index++] = JMP;

                int i;
                for (i = 0; i < n_labels; i++)
                {
                    if (token[strlen(token) - 1] == '\n')
                    {
                        token[strlen(token) - 1] = '\0';
                    }

                    if (strcmp(labels[i].name, token) == 0)
                    {
                        code[index++] = labels[i].address;
                        break;
                    }
                }
            }
            /* Save label */
            else if (strstr(token, ":"))
            {
                char name[32];
                strcpy(name, token);

                name[strlen(name) - 2] = '\0';

                strcpy(labels[n_labels].name, name);
                labels[n_labels++].address = index;
            }
            /* Fill */
            else if (strcmp(token, ".fill") == 0)
            {
                token = strtok(NULL, " ");
                int fill_size = strtol(token, NULL, 0);
                token = strtok(NULL, " ");
                uint8_t fill_byte = strtol(token, NULL, 0);

                int n_fill = fill_size - index;
                
                int i;
                for (i = 0; i < n_fill; i++)
                {
                    /* printf("%d\n", i); */
                    code[index++] = fill_byte;
                }
            }
            /* Halt */
            else if (strcmp(token, "hlt\n") == 0)
            {
                code[index++] = HLT;
            }
            /* Word */
            else if (strcmp(token, ".word") == 0)
            {
                token = strtok(NULL, token);
                short value;
                sscanf("aa55", "%hx", &value);

                code[index++] = value & 0xFF;
                code[index++] = (value >> 8) & 0xFF;
            }
        }
    }

    write_to_binary(bin_file, code, BYTE_CODE_SIZE);
    fclose(src_file);
    fclose(bin_file);
}

void write_to_binary(FILE *file, uint8_t *code, int size)
{
    fwrite(code, sizeof(uint8_t), size, file);
}