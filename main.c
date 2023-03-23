#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <stdarg.h>

// Types
typedef struct {
    int a;
    int b;
    float c;
} Test;

typedef char *string;

// Functions
void createFile() {
    
}

void get_int(char *text, int *var) {
    while(!printf("%s: ", text) || !scanf("%d", &*var)) {}
}

char *get_text(char *text) {
    printf("%s", text);
    static char var[20];
    scanf("%s", &var);
    // printf("%s", var);
    return var;
}

int choices_1(string ch[]) {
    int ln;
    for (ln = 0; ch[ln] != NULL; ln++) printf("%d) %s\n", ln+1, ch[ln]);
    int i, j = 1;
    printf("\r%d", j);
    do {
        i = getch();
        switch (i)
        {
            case 122:
                printf("\r%d", j < ln ? ++j : j);
                break;
            case 115:
                printf("\r%d", j > 1 ? --j : j);
                break;
        }
    } while(i != 13);
    return j;
}

int choices_2(string ch[]) {
    system("cls");
    int ln, i, j = 1;
    do {
        for (ln = 0; ch[ln] != NULL; ln++) printf("%s %d) %s %s\n", ln+1 == j ? "->" : "  ", ln+1, ch[ln], ln+1 == j ? "<-" : "  ");
        printf("press `Z` and `S` to move and `Enter` to select\n");
        i = getch();
        
        if(i == 115 || i == 83) j < ln ? ++j : j;
        else if(i == 122 || i == 90) j > 1 ? --j : j;


        if (i == 13) break;
        system("cls");
    } while(1);
    return j;
}

bool write_data(char *filename, Test *data, int total) {
    FILE *file;
    file = fopen(filename, "wb");
    if (file == NULL) return false;
    if (fwrite(&total, sizeof(int), 1, file) != 1) return false;
    if (fwrite(data, sizeof(Test), total, file) != total) return false;
    if (fclose(file) == EOF) return false;
    return true;
}

Test *read_data(char *filename, int *total) {
    FILE *file;
    file = fopen(filename, "rb");
    if (fread(total, sizeof(int), 1, file) != 1) return NULL;

    Test *data = malloc(sizeof(Test) * *total);

    if (fread(data, sizeof(Test), *total, file) != *total) {
        free(data);
        return NULL;
    }
    if (fclose(file) == EOF) {
        free(data);
        return NULL;
    }
    return data;
}


// Programme
int main() {
    FILE *teams;
    Test *test;
    int total = 0;
    test = read_data("data.bin", &total);
    
    printf("%d", total);
    // test[0].a = 333, test[0].b = 221, test[0].c = 546.22;
    // test[1].a = 99, test[1].b = 567, test[1].c = 12.22;
    // test[2].a = 65656, test[2].b = 123, test[2].c = 6.22;
    // printf("- %d -", write_data("data.bin", test, 3));
    // teams = fopen("teams.bat", "rb");
    // fwrite(&test[0], sizeof(test[0]), 3, teams);
    // fwrite(&test[1], sizeof(test[1]), 3, teams);
    // fwrite(&test[2], sizeof(test[2]), 3, teams);
    // fread(&test, sizeof(test), 3, teams);
    // fread(&test, sizeof(test), 3, teams);
    printf("- %d, %d, %d -- %d", test[0].a, test[1].b, test[2].b, sizeof(test));
    return 0;
}