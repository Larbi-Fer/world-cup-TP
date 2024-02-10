#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

char* get_text(char *text) {
    int i = 0;
    printf("%s", text);
    char *var = malloc(100);
    char c;
    while ((c=getch()) != '\r') {
        if (c == 27 && i == 0) break;
        if (c == 8) {
            if (i == 0) {
                printf("%s", strlen(text) ? " " : "");
                continue;
            }
            for (int j = 0; j <= i; j++) {
                printf(" ");
            }
            var[--i] = '\0';
            printf("\r%s%s", text, var);
            continue;
        }
        if (c >= 97 && c <= 122) c -= 32;
        var[i++] = c;
        printf("%c", c);
    }
    var[i] = '\0';
    printf("\n");
    if (i == 0 && c == '\r') return get_text("");
    return var;
}

typedef struct Account {
    char username[20];
    char password[20];
    bool isAdmin;
} Account;

int main() {
    char *name = "a2";
    // fclose(fopen(name, "w"));
    getch();
    remove(name);
    return 0;
    FILE *faccount;
    faccount = fopen("accounts.bin", "a+b");
    fclose(faccount);
    faccount = fopen("accounts.bin", "r+b");
    Account user;

    return 0;

    strcpy(user.username, get_text("username: "));
    strcpy(user.password, get_text("password: "));
    user.isAdmin = true;

    fwrite(&user, sizeof(Account), 1, faccount);

    return 0;
}