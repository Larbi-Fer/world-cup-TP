#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

// KNOCKOUT
FILE *fknout;
FILE *fgrp;
FILE *fmtch;
FILE *fteams;
FILE *faccount;
FILE *fpoll;
FILE *ftour;

int tour;

int kout[8][2][2] = { { { 0, 0 }, { 1, 1 } }, { { 0, 2 }, { 1, 3 } },
                      { { 0, 4 }, { 1, 5 } }, { { 0, 6 }, { 1, 7 } },
                      { { 0, 7 }, { 1, 6 } }, { { 0, 5 }, { 1, 4 } },
                      { { 0, 3 }, { 1, 2 } }, { { 0, 1 }, { 1, 0 } }, };

// Types
typedef struct {
    int month;
    int day;
    int year;

    int hour;
    int min;
} Date;
// Gls_Con: Goals Conceded
typedef struct {
    int id;
    char name[20];
    int Gls;
    int Gls_Con;
} Team;

typedef struct Group {
    int id;
    int points;
    int index;
    int Gls;
    int Gls_Con;
} Group;

typedef struct {
    int team1;
    int team2;
    int rlt1;
    int rlt2;
    bool isplay;
    Date date;
} Matche;

typedef struct {
    int position, team1, team2, rlt1, rlt2, plt1, plt2; bool isplay; Date date;
} Kout;

typedef struct Account {
    char username[20];
    char password[20];
    bool isAdmin;
} Account;

typedef struct Poll {
    char username[20];
    int match;
    int type;
} Poll;

Account user;

// Functions
void clr(char *user) {
    system("cls");
    if (user[0]) printf("- Logged in by `%s` -\n", user);
    if(tour) printf("- FIFA CUP WORLD 20%d -\n", tour);
}

bool dateIsTrue(int day, int month, int year, int h, int min) {
    if (day <= 0 || day > 31 || month <= 0 || month > 12) return false;
    if (year % 4 != 0 && month == 2 && day >= 29) return false;
    if (day == 31 && !( (month <= 7 && month % 2 == 1) || (month > 7 && month % 2 == 0)) ) return false;

    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;
    return true;
}

Date get_date() {
    Date date;
    do {
        scanf("%d/%d/%d  %d:%d", &date.month, &date.day, &date.year, &date.hour, &date.min);
        if (dateIsTrue(date.day, date.month, date.year, date.hour, date.min)) break;
        printf("Date is wrong, Repete:  ");
    } while (1);
    return date;
}

int get_int(char *text) {
    printf("%s", text);
    int var;
    scanf("%d", &var);
    return var;
}

char* get_text(char *text) {
    int i = 0;
    printf("%s", text);
    char *var = malloc(100);
    char c;
    while ((c=getch()) != '\r') {
        if (c == 27 && i == 0) break;
        if (c == 8) {
            if (i == 0) continue;
            i--;
            printf("\b \b");
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

char* input(char *text) {
    int i = 0;
    printf("%s", text);
    char *var = malloc(100);
    char c;
    while ((c=getch()) != '\r') {
        if (c == 27 && i == 0) break;
        if (c == 8) {
            if (i == 0) continue;
            i--;
            printf("\b \b");
            continue;
        }
        var[i++] = c;
        printf("%c", c);
    }
    var[i] = '\0';
    printf("\n");
    if (i == 0 && c == '\r') return get_text("");
    return var;
}

char *get_pass(char *text) {
    printf("%s", text);
    char *var = malloc(100);
    char p;
    int i = 0;
    do {
        p = getch();
        if ((p == 13 && i != 0) || (p == 27 && i == 0)) break;
        if (p == 8 && i != 0) {
            i--;
            printf("\b \b");
            continue;
        }
        if (!((p >= 65 && p <= 90 ) || (p >= 97 && p <= 122) || (p >= 48 && p <= 57))) continue;
        var[i] = p;
        i++;
        printf("*");
    } while (1);
    var[i] = '\0';
    printf("\n");
    return var;
}

int choices_1(char *ch[]) {
    int ln;
    for (ln = 0; ch[ln] != NULL; ln++) printf("%d) %s\n", ln+1, ch[ln]);
    int i, j = 1;
    printf("\r%d", j);
    do {
        i = getch();
        if (i == 122) printf("\r%d", j < ln ? ++j : j);
        else if (i == 115) printf("\r%d", j > 1 ? --j : j);
    } while(i != 13);
    return j;
}

int choices_2(char *ch[], char *user) {
    clr(user);
    int ln, i, j = 1;
    do {
        for (ln = 0; ch[ln] != NULL; ln++) printf("%s %d) %s %s\n", ln+1 == j ? "->" : "  ", ln+1, ch[ln], ln+1 == j ? "<-" : "  ");
        printf("press `Z` and `S` to move and `Enter` to select\n");
        i = getch();

        if(i == 115 || i == 83) j = j < ln ? ++j : 1;
        else if(i == 122 || i == 90) j = j > 1 ? --j : ln;
        else if (i == 27) return -1;

        if (i == 13) break;
        clr(user);
    } while(1);
    return j;
}

bool sure() {
    char *an = input("\nAre you sure ? y/n: ");
    return an[0] == 'y' || an[0] == 'Y';
}

FILE* fcreat(char *name, bool istour) {
    if(istour) {
        char *name2 = malloc(sizeof(char) * 25);
        sprintf(name2, "%d\\%s.bin", tour, name);
        name = name2;
    }
    FILE* file = fopen(name, "r+b");
    if (!file) {
        file = fopen(name, "wb");
        fclose(file);
        file = fopen(name, "r+b");
    }
    return file;
}

// Account
Account acc_ser(char username[]) {
    Account user;
    while( fread(&user, sizeof(Account), 1, faccount) == 1 ) {
        if (!strcmp(user.username, username)) break;
    }
    if (strcmp(user.username, username)) user.username[0] = '\0';
    rewind(faccount);
    return user;
}

void acc_cre(Account *acc) {
    Account user;
    while (fread(&user, sizeof(user), 1, faccount) == 1) {}
    fwrite(acc, sizeof(Account), 1, faccount);
    rewind(faccount);
}


Team get_teamed(int id) {
    Team team;
    if (id == -1) {
        team.id = -1;
        strcpy(team.name, " --- ");
        return team;
    }
    fseek(fteams, sizeof(Team) * id, SEEK_SET);
    fread(&team, sizeof(team), 1, fteams);
    rewind(fteams);
    return team;
}

void Show_group(int gr) {
    Group group;
    fseek(fgrp, sizeof(group) * gr * 4, SEEK_SET);
    printf("\n %-13s %-2s  %-2s  %-2s  %-3s \n", "Name", "P", "+", "-", "+/-");
    for(int i = 0; i < 4; i++) {
        fread(&group, sizeof(group), 1, fgrp);
        printf("%s%-2d %-10s %-2d  %-2d  %-2d  %-3d\n", i == 0 || i == 1 ? "|" : " ", i+1, get_teamed(group.id).name, group.points, group.Gls, group.Gls_Con, group.Gls - group.Gls_Con);
    }

    rewind(fteams);
    rewind(fgrp);
}

void gen_date(Date date) {
    printf("%d/%d/%d  %d:%d", date.month, date.day, date.year, date.hour, date.min);
}

// User
void sgroup() {
    // Search a group by name
    printf("Enter group name: ");
    int gr = getch();
    if (gr >= 97 && gr <= 104) gr -= 32;
    printf("%c\n", gr);
    gr -= 65;
    Show_group(gr);

    fseek(fmtch, sizeof(Matche) * gr * 6, SEEK_SET);
    Team team1, team2;
    Matche mtch;
    int j = 1;
    for (int i = 0; i < 6; i++) {
        fread(&mtch, sizeof(Matche), 1, fmtch);
        if(i % 2 == 0) printf("\n\nRound %d:", j++);
        team1 = get_teamed(mtch.team1);
        team2 = get_teamed(mtch.team2);
        printf("\n%-10s %d - %d %10s || ", team1.name, mtch.rlt1, mtch.rlt2, team2.name);
        gen_date(mtch.date);
    }
    printf("\n");
    rewind(fmtch);
    getch();
}

/* char* num(int n) {
    return n > 9 ? n : 
} */

Matche* get_fix(Team team) {
    static Matche mtch[3];
    int gr = team.id / 4;
    fseek(fmtch, sizeof(Matche) * gr * 6, SEEK_SET);
    int j = 0;
    for (int i = 0; i < 6; i++) {
        fread(&mtch[j], sizeof(Matche), 1, fmtch);
        if (mtch[j].team1 == team.id || mtch[j].team2 == team.id) j++;
    }
    rewind(fmtch);
    return mtch;
}

bool isWin(Kout kt, int id) {
    if (!kt.isplay) return false;
    if (kt.team1 == id) return kt.rlt1 > kt.rlt2 || kt.plt1 > kt.plt2;
    else return kt.rlt2 > kt.rlt1 || kt.plt2 > kt.plt1;
}

Kout* get_knout(Team team) {
    static Kout knout[4];
    int p = team.id / 4;
    p /= 2;
    int i = 0;
    fseek(fknout, sizeof(Kout) * p, 0);
    fread(&knout[i], sizeof(Kout), 1, fknout);
    if(knout[i].team1 != team.id && knout[i].team2 != team.id) {
        p = 7 - p;
        fseek(fknout, sizeof(Kout) * p, 0);
        fread(&knout[i], sizeof(Kout), 1, fknout);
    }
    if (!(knout[i].team1 != team.id && knout[i].team2 != team.id)) i++;
    for (; (i < 4 && isWin(knout[i-1], team.id)) || (i == 3 && knout[i-1].isplay); i++) {
        p = (p != 13 && p != 12) ? (p - get_round(i-1)) / 2 + get_round(i) : ( isWin(knout[i-1], team.id) ? 15 : 14 );
        fseek(fknout, sizeof(Kout) * p, 0);
        fread(&knout[i], sizeof(Kout), 1, fknout);
    }
    if (i != 4) knout[i].rlt1 = -1;
    rewind(fknout);
    return knout;
}

void matches() {
    // Search a matches of team
    char* name = get_text("Enter the team name: ");
    Team team, vs;
    while(fread(&team, sizeof(Team), 1, fteams) == 1 && strcmp(team.name, name));
    rewind(fteams);
    if (strcmp(team.name, name)) {
        printf("Team not found\n");
        getch();
        return;
    }
    Matche* mtch = get_fix(team);
    Kout* knout = get_knout(team);
    for (int i = 0; i < 3; i++) {
        if(mtch[i].team1 == team.id) {
            vs = get_teamed(mtch[i].team2);
            if (mtch[i].isplay) printf("\n%-10s %d - %d %10s || ", team.name, mtch[i].rlt1, mtch[i].rlt2, vs.name);
            else printf("\n%-10s   -   %10s || ", team.name, vs.name);
        }
        else {
            vs = get_teamed(mtch[i].team1);
            if (mtch[i].isplay) printf("\n%-10s %d - %d %10s || ", vs.name, mtch[i].rlt1, mtch[i].rlt2, team.name);
            else printf("\n%-10s   -   %10s || ", vs.name, team.name);
        }
        gen_date(mtch[i].date);
    }
    for (int i = 0; i < 4 && knout[i].rlt1 != -1; i++) {
        if(knout[i].team1 == team.id) {
            vs = get_teamed(knout[i].team2);
            if (knout[i].isplay) {
                if (knout[i].rlt1 == knout[i].rlt2) printf("\n%-10s %d (%d) - (%d) %d %10s || ", team.name, knout[i].rlt1, knout[i].plt1, knout[i].plt2, knout[i].rlt2, vs.name);
                else printf("\n%-10s %d - %d %10s || ", team.name, knout[i].rlt1, knout[i].rlt2, vs.name);
            }
            else printf("\n%-10s   -   %10s || ", team.name, vs.name);
        }
        else {
            vs = get_teamed(knout[i].team1);
            if (knout[i].isplay) {
                if (knout[i].rlt1 == knout[i].rlt2) printf("\n%-10s %d (%d) - (%d) %d %10s || ", vs.name, knout[i].rlt1, knout[i].plt1, knout[i].plt2, knout[i].rlt2, team.name);
                else printf("\n%-10s %d - %d %10s || ", vs.name, knout[i].rlt1, knout[i].rlt2, team.name);
            }
            else printf("\n%-10s   -   %10s || ", vs.name, team.name);
        }
        gen_date(knout[i].date);
    }
    printf("\n");
    rewind(fmtch);
    getch();
}

void round() {
    // Round information
    
}

void seagroup() {
    // Search a group by team
    // Par Accès séquentiel
    char *name = get_text("Enter the team name: ");
    Team team;
    while(fread(&team, sizeof(Team), 1, fteams) == 1 && strcmp(team.name, name));
    rewind(fteams);
    if (strcmp(team.name, name)) {
        printf("Team not found\n");
        getch();
        return;
    }
    int g = team.id / 4;
    printf("\nGroup %c:", g + 65);
    Show_group(g);
    getch();
}

void bestatk() {
    Team teams[8];
    // fread(&teams, sizeof(teams), 8, fteams);
    for (int i = 0; i < 8; i++) {
        fread(&teams[i], sizeof(Team), 1, fteams);
    }
    int num = 8;
    for(int i = 0; i < num - 1; i++){       
        for(int j = 0; j < num - i - 1; j++){          
            if(teams[j].Gls < teams[j + 1].Gls){    
                Team tmp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = tmp;
            }
        }
    }



    printf("   %-10s %s\n", "Team Name", "Goals");
    for (int i = 0; i < 8; i++) {
        printf("%2d) %-10s %d\n", i+1, teams[i].name, teams[i].Gls);
    }
    rewind(fteams);
    getch();
}

void bestdef() {
    // Best defense
    Team teams[8];
    // fread(&teams, sizeof(teams), 8, fteams);
    for (int i = 0; i < 8; i++) {
        fread(&teams[i], sizeof(Team), 1, fteams);
    }

    int num = 8;
    bool isort = false;
    for(int i = 0; i < num - 1 && !isort; i++){
        isort = true;
        for(int j = 0; j < num - i - 1; j++){
            if(teams[j].Gls_Con > teams[j + 1].Gls_Con){    
                isort = false;
                Team tmp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = tmp;
            }
        }
    }

    printf("   %-10s %s\n", "Team Name", "Goals");
    for (int i = 0; i < 8; i++) {
        printf("%2d) %-10s %d\n", i+1, teams[i].name, teams[i].Gls_Con);
    }
    rewind(fteams);

    getch();
}

Account login() {
    Account user;
    do {
        printf("Enter your information:\n");
        char *username = input("\tusername: ");
        if (username[0] == '\0') {
            user.username[0] = '\0';
            break;
        }
        char *pass = get_pass("\tpassword: ");
        if (pass[0] == '\0') {
            user.username[0] = '\0';
            break;
        }
        user = acc_ser(username);
        if (strcmp(user.username, username) || strcmp(user.password, pass)) {
            clr("\0");
            printf("The username or password is incorrect\n");
        } else break;
    } while (1);
    return user;
}

void polling() {
    char *team_name = get_text("Enter the team name: ");
    Team team, vs;
    while(fread(&team, sizeof(team), 1, fteams) == 1) {
        if (!strcmp(team.name, team_name)) break;
    }
    rewind(fteams);
    if (strcmp(team.name, team_name)) {
        printf("Team not found\n");
        getch();
        return;
    }
    Matche* mtch = get_fix(team);
    Kout* knout = get_knout(team);
    char *ch[8];
    for (int i = 0; i < 7; i++) {
        ch[i] = malloc(sizeof(char) * 15);
        if (ch[i] == NULL) exit(1);
    }

    int i = 0, j, n = 0, kn;
    // get group matches
    for (i = 0, j = 0; j < 3 && mtch[j].rlt1 != -1; i++, j++) {
        if (mtch[j].isplay) {
            i--;
            continue;
        }
        n++;
        if(mtch[j].team1 == team.id) {
            vs = get_teamed(mtch[j].team2);
            sprintf(ch[i], "%-10s - %10s", team.name, vs.name);
        }
        else {
            vs = get_teamed(mtch[j].team1);
            sprintf(ch[i], "%-10s - %10s", vs.name, team.name);
        }
    }
    if (!n) {
        // get knockout matches
        for (j = 0; j < 4 && knout[j].rlt1 != -1; i++, j++) {
            if (knout[j].isplay) {
                i--;
                continue;    
            }
            kn = j;
            if(knout[j].team1 == -1 || knout[j].team2 == -1) break;
            if(knout[j].team1 == team.id) {
                vs = get_teamed(knout[j].team2);
                sprintf(ch[i], "%-10s - %10s", team.name, vs.name);
            }
            else {
                vs = get_teamed(knout[j].team1);
                sprintf(ch[i], "%-10s - %10s", vs.name, team.name);
            }
            break;
        }
    }
    ch[i] = NULL;
    int chi = choices_2(ch, &user);
    chi--;

    printf("\n\n1) %s\n2) %s\n", chi < n ? get_teamed(mtch[chi].team1).name : get_teamed(knout[chi - n].team1).name, chi < n ? get_teamed(mtch[chi].team2).name : get_teamed(knout[chi - n].team2).name);
    do {
        scanf("%d", &chi);
    } while (chi < 1 || chi > 2);
    getch();

    for (int i = 0; i < 7; i++) {
        free(ch[i]);
    }
    
}

void account_sett(Account *user) {
    char *ch[] = { "Login", "Create account", NULL };
    char *ch2[] = { "Change username", "Change password", "Delete your account", "Logout", NULL };
    int chi;
    clr(user->username);
    Account user2;
    char *username;
    if (user->username[0] != '\0') {
        // Logged in
        chi = choices_2(ch2, user->username);
        switch(chi) {
            case 1:
                if (strcmp(user->password, get_pass("\tEnter your password: "))) {
                    printf("\tpassword incorrect ");
                    getch();
                    break;
                }
                username = input("\tEnter the new username: ");
                user2 = acc_ser(username);
                printf("username: %s\n", user2.username);
                if (!strcmp(username, user2.username)) {
                    printf("\tThis username already exists");
                    getch();
                    break;
                }
                strcpy(user->username, username);
                while( fread(&user2, sizeof(Account), 1, faccount) == 1 ) {
                    if (!strcmp(user->username, user2.username)) break;
                }
                fseek(faccount, sizeof(Account) * -1, SEEK_CUR);
                fwrite(user, sizeof(Account), 1, faccount);
                rewind(faccount);
                break;
            case 2:
                if (strcmp(user->password, get_pass("\tEnter the old password: "))) {
                    printf("\tpassword incorrect ");
                    getch();
                    break;
                }
                while( fread(&user2, sizeof(Account), 1, faccount) == 1 ) {
                    if (!strcmp(user->username, user2.username)) break;
                }
                strcpy(user->password, get_pass("\tEnter the new password: "));
                fseek(faccount, sizeof(Account) * -1, SEEK_CUR);
                fwrite(user, sizeof(Account), 1, faccount);
                rewind(faccount);
                break;
            case 3:
                if (sure()) {
                    // Remove Account
                    FILE *ftemp = fopen("temp.bin", "wb");
                    getch();
                    while (fread(&user2, sizeof(Account), 1, faccount)) {
                        if (strcmp(user2.username, user->username)) {
                            fwrite(&user2, sizeof(Account), 1, ftemp);
                        }
                    }
                    fclose(ftemp);
                    fclose(faccount);
                    remove("accounts.bin");
                    getch();
                    rename("temp.bin", "accounts.bin");
                    user->username[0] = '\0';
                    user->password[0] = '\0';
                    user->isAdmin = false;
                    printf("Your account has been deleted successfully ");
                    getch();
                    faccount = fcreat("accounts.bin", false);
                }
                break;
            case 4:
                if (!sure()) break;
                user->username[0] = '\0';
                break;
        }
    } else {
        chi = choices_2(ch, user->username);
        switch(chi) {
            case 1:
                *user = login();
                if (user->username[0] != '\0') {
                    printf("Successful login\n");
                    getch();
                }
                break;
            case 2:
                printf("Enter your information:\n");
                bool b = true;
                do {
                    strcpy(user->username, input("\tusername: "));
                    user2 = acc_ser(user->username);
                    if (!strcmp(user->username, user2.username)) {
                        printf("This username already exists\n");
                        if(getch() == 27) b = false;
                    } else break;
                } while(b);
                if (!b) {
                    user->username[0] = '\0';
                    break;
                }
                strcpy(user->password, get_pass("\tpassword: "));
                user->isAdmin = false;
                acc_cre(user);
                printf("\tSuccessful creating account");
                getch();
                break;
        }
    }
}

void edit_gls(Team team, int Gls, int Gls_Con) {
    fseek(fteams, sizeof(team) * team.id, 0);
    team.Gls += Gls;
    team.Gls_Con += Gls_Con;
    fwrite(&team, sizeof(team), 1, fteams);
    rewind(fteams);
}

int get_round(int rnd) {
    if (rnd == 0) return 0;
    if (rnd == 1) return 8;
    if (rnd == 2) return 12;
    if (rnd == 3) return 14;
}

// Admin
void mod_mtch(char *user) {
    // Enter the match result
    char *name = get_text("Enter the team name: ");
    Team team, vs;
    rewind(fteams);
    while(fread(&team, sizeof(Team), 1, fteams) && strcmp(team.name, name)) {}
    rewind(fteams);
    if (strcmp(team.name, name)) {
        printf("Team not found\n");
        getch();
        return;
    }
    Matche* mtch = get_fix(team);
    Kout* knout = get_knout(team);
    char *ch[8];
    int i;
    for (int i = 0; i < 8; i++) {
        ch[i] = malloc(50);  // Allocate memory for each string
    }
    // get group matches
    for (i = 0; i < 3 && mtch[i].rlt1 != -1; i++) {
        if(mtch[i].team1 == team.id) {
            vs = get_teamed(mtch[i].team2);
            if (mtch[i].isplay) sprintf(ch[i], "%-10s %d - %d %10s", team.name, mtch[i].rlt1, mtch[i].rlt2, vs.name);
            else sprintf(ch[i], "%-10s - %10s", team.name, vs.name);
        }
        else {
            vs = get_teamed(mtch[i].team1);
            if (mtch[i].isplay) sprintf(ch[i], "%-10s %d - %d %10s", vs.name, mtch[i].rlt1, mtch[i].rlt2, team.name);
            else sprintf(ch[i], "%-10s - %10s", vs.name, team.name);
        }
    }
    // get knockout matches
    for (i = 0; i < 4 && knout[i].rlt1 != -1; i++) {
        if(knout[i].team1 == -1 || knout[i].team2 == -1) break;
        if(knout[i].team1 == team.id) {
            vs = get_teamed(knout[i].team2);
            if (knout[i].isplay) sprintf(ch[i + 3], "%-10s %d - %d %10s", team.name, knout[i].rlt1, knout[i].rlt2, vs.name);
            else sprintf(ch[i + 3], "%-10s - %10s", team.name, vs.name);
        }
        else {
            vs = get_teamed(knout[i].team1);
            if (knout[i].isplay) sprintf(ch[i + 3], "%-10s %d - %d %10s", vs.name, knout[i].rlt1, knout[i].rlt2, team.name);
            else sprintf(ch[i + 3], "%-10s - %10s", vs.name, team.name);
        }
    }
    // for (int ln = 0; ch[ln] != NULL; ln++) printf("\n%s", ch[ln]);
    ch[i + 3] = NULL;
    printf("\n%d\n", mtch[0].team1);
    // printf("\n%s", user);
    int cho = choices_2(ch, user);
    cho--;

    for (int i = 0; i < 8; i++) {
        free(ch[i]);
    }

    int g = team.id / 4;
    Matche mth;

    if (cho < 3) {
        for (i = 0; i < 2; i++) {
            fseek(fmtch, sizeof(Matche) * (6 * g + i + cho * 2), SEEK_SET);
            fread(&mth, sizeof(Matche), 1, fmtch);
            if (mth.team1 == mtch[cho].team1) {
                mth.rlt1 = get_int("Enter the number of goals of team 1: ");
                mth.rlt2 = get_int("Enter the number of goals of team 2: ");
                bool isp = mth.isplay;
                mth.isplay = true;
                fseek(fmtch, sizeof(Matche) * 6 * g + i * sizeof(Matche) + cho * 2 * sizeof(Matche), SEEK_SET);
                fwrite(&mth, sizeof(Matche), 1, fmtch);

                rewind(fmtch);

                // if (!isp) {
                if (1) {
                    // Add goals
                    edit_gls(get_teamed(mth.team1), mth.rlt1, mth.rlt2);
                    edit_gls(get_teamed(mth.team2), mth.rlt2, mth.rlt1);
                    // sort the group
                    int pnt = 1, id;
                    if (mth.rlt1 > mth.rlt2) {
                        pnt = 3;
                        id = mth.team1;
                    } else if (mth.rlt1 < mth.rlt2) {
                        pnt = 3;
                        id = mth.team2;
                    }
                    Group group[4];
                    fseek(fgrp, sizeof(Group) * g * 4, 0);
                    for (i = 0; i < 4; i++) {
                        fread(&group[i], sizeof(group[i]), 1, fgrp);
                    }
                    for (int j = 0; j < 4; j++) {
                        if ((pnt == 1 && (group[j].id == mth.team1 || group[j].id == mth.team2)) || (pnt != 1 && group[j].id == id)) {
                            group[j].points += pnt;
                        }
                        if ( group[j].id == mth.team1 ) {
                            group[j].Gls += mth.rlt1;
                            group[j].Gls_Con += mth.rlt2;
                        } else if ( group[j].id == mth.team2 ) {
                            group[j].Gls += mth.rlt2;
                            group[j].Gls_Con += mth.rlt1;
                        }
                    }

                    void revers(int j, int max) {
                        Group tmp = group[j];
                        group[j] = group[max];
                        group[max] = tmp;
                    }
                    int max;
                    for (i = 0; i < 3; i++) {
                        max = i;
                        for (int j = i+1; j < 4; j++) {
                            if(group[j].points > group[max].points) {
                                revers(j, max);
                            } else if(group[j].points == group[max].points) {
                                if((group[j].Gls - group[j].Gls_Con) > (group[max].Gls - group[max].Gls_Con)) {
                                    revers(j, max);
                                }
                            }
                        }
                    }
                    fseek(fgrp, sizeof(Group) * g * 4, 0);
                    for (i = 0; i < 4; i++) {
                        fwrite(&group[i], sizeof(group[i]), 1, fgrp);
                    }
                    rewind(fgrp);

                    if (cho == 2) {
                        // if fixturs of group finale
                        fseek(fmtch, sizeof(Matche) * g * 6 + sizeof(Matche) * 4, 0);
                        fread(&mtch[0], sizeof(Matche), 1, fmtch);
                        if (!mtch[0].isplay) break;
                        fread(&mtch[1], sizeof(Matche), 1, fmtch);
                        if (!mtch[1].isplay) break;
                        // Add to knockout
                        // The First Team
                        int p = g/2;
                        printf("\n%d - %d\n", p, g);
                        int s = sizeof(Kout);
                        Kout matche;
                        fseek(fknout, s * p, 0);
                        fread(&matche, s, 1, fknout);
                        if(g % 2 == 0) matche.team1 = group[0].id;
                        else matche.team2 = group[0].id;
                        printf("%d ", group[0].id);
                        fseek(fknout, s * p, 0);
                        fwrite(&matche, s, 1, fknout);
                        // The Seconde Team
                        p = 7 - p;
                        fseek(fknout, s * p, 0);
                        fread(&matche, s, 1, fknout);
                        if(g % 2 == 0) matche.team2 = group[1].id;
                        else matche.team1 = group[1].id;
                        fseek(fknout, s * p, 0);
                        fwrite(&matche, s, 1, fknout);
                        rewind(fknout);
                    }

                    break;
                }
            }
        }
        
    }
    else {
        // Knockout
        cho -= 3;
        int p = knout[cho].position;
        knout[cho].rlt1 = get_int("Enter the number of goals of team 1: ");
        knout[cho].rlt2 = get_int("Enter the number of goals of team 2: ");
        if (knout[cho].rlt1 == knout[cho].rlt2) {
            knout[cho].plt1 = get_int("Enter the number of goals of team 1: ");
            knout[cho].plt2 = get_int("Enter the number of goals of team 2: ");
        }
        knout[cho].isplay = true;
        fseek(fknout, sizeof(Kout) * p, 0);
        fwrite(&knout[cho], sizeof(Kout), 1, fknout);
        
        edit_gls(get_teamed(knout[cho].team1), knout[cho].rlt1, knout[cho].rlt2);
        edit_gls(get_teamed(knout[cho].team2), knout[cho].rlt2, knout[cho].rlt1);
        if (cho < 2) {
            // create in new round
            int id = knout[cho].rlt1 > knout[cho].rlt2 || knout[cho].plt1 > knout[cho].plt2 ? knout[cho].team1 : knout[cho].team2;
            // printf("%d");
            Kout newk;
            p -= get_round(cho);
            int j = p % 2;
            p = p / 2 + get_round(++cho);
            fseek(fknout, sizeof(Kout) * p, 0);
            fread(&newk, sizeof(Kout), 1, fknout);
            if (j == 0) newk.team1 = id;
            else newk.team2 = id;
            fseek(fknout, sizeof(Kout) * p, 0);
            fwrite(&newk, sizeof(Kout), 1, fknout);
            rewind(fknout);
        } else if (cho == 2) {
            int idw, idl;
            if (knout[cho].rlt1 > knout[cho].rlt2 || knout[cho].plt1 > knout[cho].plt2) {
                idw = knout[cho].team1;
                idl = knout[cho].team2;
            } else {
                idl = knout[cho].team1;
                idw = knout[cho].team2;
            }
            Kout newk;
            int j = p % 2;
            // Add loser
            fseek(fknout, sizeof(Kout) * 14, 0);
            fread(&newk, sizeof(Kout), 1, fknout);
            if (j == 0) newk.team1 = idl;
            else newk.team2 = idl;
            fseek(fknout, sizeof(Kout) * 14, 0);
            fwrite(&newk, sizeof(Kout), 1, fknout);
            // Add winner
            fseek(fknout, sizeof(Kout) * 15, 0);
            fread(&newk, sizeof(Kout), 1, fknout);
            if (j == 0) newk.team1 = idw;
            else newk.team2 = idw;
            fseek(fknout, sizeof(Kout) * 15, 0);
            fwrite(&newk, sizeof(Kout), 1, fknout);
            rewind(fknout);
        }
    }
}

void closeFiles() {
    fclose(faccount);
    fclose(fteams);
    fclose(fmtch);
    fclose(fgrp);
    fclose(fknout);
    fclose(fpoll);
}

// Programme
int main() {
    ftour = fcreat("tour.bin", false);
    
    while(!feof(ftour)) fread(&tour, sizeof(int), 1, ftour);

    Team team[4];
    user.username[0] = '\0';
    start:
    if (tour) {
        faccount = fcreat("accounts.bin", false);
        fteams = fcreat("teams", true);
        fmtch = fcreat("matches", true);
        fgrp = fcreat("groups", true);
        fknout = fcreat("knockout", true);
        fpoll = fcreat("polls", true);
    }

    rewind(ftour);


    const int mchingr[3][2][2] = { { { 0, 2 }, {1, 3} }, { {0, 1}, {2, 3} }, { {0, 3}, {1, 2}} };
    int chi;
    do {
        char *ch[4] = { "Admin", "User", "Change the tournament", NULL };
        char *chuser[10] = { "Show groupe", "Fixtures of one team", "Round information", "Search for a group by team", "Best attack", "Best defense", "Poll", "Account", "Exit", NULL };
        char *chadmin[10] = { "Enter the matche result", "Account", "Exit", NULL };
        chi = choices_2(ch, user.username);
        if (chi == -1) exit(0);
        clr(user.username);
        switch (chi)
        {
        case 1:
            if (user.username[0] == '\0') {
                do {
                    user = login();
                    if (!user.isAdmin) {
                        clr("\0");
                        printf("You do not have authorization\n");
                        if (getch() == 27) break;
                    }
                } while(!user.isAdmin);
                if (!user.isAdmin) break;
            }
            if (!user.isAdmin) {
                printf("You do not have authorization\n");
                getch();
                break;
            }
            if (user.username[0] == '\0') break;
            do
            {
                // admin menu
                if (!tour) {
                    printf("Enter this tour: ");
                    scanf("%d", &tour);
                    fwrite(&tour, sizeof(int), 1, ftour);
                    rewind(ftour);
                    goto start;
                }
                if (feof(fteams)) {
                    clr(user.username);
                    printf("Participating teams have not been entered yet\nDo you want to enter it now (y/n)? ");
                    if (getche() != 'y') break;
                    int k = 0;
                    int id = 0;
                    while (k < 2) {
                        printf("\nGroup %c:\n", k+65);
                        Group grp[4];
                        for (int i = 0; i < 4; i++) {
                            printf("The team N %d: ", i+1);
                            strcpy(team[i].name, get_text(""));
                            team[i].id = id;
                            team[i].Gls = 0, team[i].Gls_Con = 0;
                            grp[i].id = id++;
                            grp[i].index = i;
                            grp[i].points = 0, grp[i].Gls = 0, grp[i].Gls_Con = 0;
                            fwrite(&grp[i], sizeof(Group), 1, fgrp);
                            fwrite(&team[i], sizeof(Team), 1, fteams);
                        }
                        printf("\n");
                        Matche mch;
                        for (int i = 0; i < 3; i++) {
                            printf("\nRound %d\n", i+1);
                            for (int j = 0; j < 2; j++) {
                                printf("%s - %s  | ", team[grp[mchingr[i][j][0]].index].name, team[grp[mchingr[i][j][1]].index].name);
                                if (i != 2 || j != 0) {
                                    printf("date: ");
                                    mch.date = get_date();
                                    mch.team1 = grp[mchingr[i][j][0]].id;
                                    mch.team2 = grp[mchingr[i][j][1]].id;
                                    mch.rlt1 = 0, mch.rlt2 = 0;
                                    mch.isplay = false;
                                    fwrite(&mch, sizeof(Matche), 1, fmtch);
                                }
                            }
                        }
                        mch.team1 = grp[mchingr[2][0][0]].id;
                        mch.team2 = grp[mchingr[2][0][1]].id;
                        fwrite(&mch, sizeof(Matche), 1, fmtch);
                        k++;
                    }
                    printf("\nEnter knockout time: \n");
                    Kout knout;
                    knout.rlt1 = 0, knout.rlt2 = 0, knout.plt1 = 0, knout.plt2 = 0;
                    knout.isplay = false;
                    knout.team1 = -1, knout.team2 = -1;
                    for (int i = 0; i < 8; i += 2) {
                        printf("%c1 - %c2 | date: ", i + 65, i + 66);
                        knout.date = get_date();
                        int p = i/2;
                        knout.position = p;
                        fseek(fknout, sizeof(Kout) * p, 0);
                        fwrite(&knout, sizeof(Kout), 1, fknout);

                        printf("%c1 - %c2 | date: ", i + 66, i + 65 );
                        knout.date = get_date();
                        p = 7 - p;
                        knout.position = p;
                        fseek(fknout, sizeof(Kout) * p, 0);
                        fwrite(&knout, sizeof(Kout), 1, fknout);
                    }
                    int m = 4;
                    int p = 8;
                    char kn_names[4][25] = { "Quarter-final", "Semi-final", "Play-off for third place", "Final" };
                    for (int j = 0; j < 4; j++) {
                        printf("\n%s:\n", kn_names[j]);
                        for (int i = 0; i < m || m == 0; i++) {
                            printf("Match N %d | date: ", i+1);
                            knout.date = get_date();
                            knout.position = p;
                            fseek(fknout, sizeof(Kout) * (p++), 0);
                            fwrite(&knout, sizeof(Kout), 1, fknout);
                            if (m == 0) break;
                        }
                        m /= 2;
                    }
                    
                    rewind(fknout);
                }
                chi = choices_2(chadmin, user.username);
                if (chi != -1) printf("\n-> %s\n", chadmin[chi-1]);
                printf("\n\n");
                switch(chi) {
                    case 1:
                        mod_mtch(user.username);
                        break;
                    case 2:
                        account_sett(&user);
                        break;
                 }
            } while (chi != -1 && chi != 3);
            break;
        case 2:
            if (!tour || feof(fteams)) {
                printf("The information has not been entered yet");
                getch();
                break;
            }
            do {
                chi = choices_2(chuser, user.username);
                if (chi != -1) printf("\n-> %s\n", chuser[chi-1]);
                printf("\n\n");
                switch (chi) {
                    case 1:
                        sgroup();
                        break;
                    case 2:
                        matches();
                        break;
                    case 3:
                        round();
                        Kout k2;
                        while( fread(&k2, sizeof(k2), 1, fknout) ) printf("%-10s %d - %d   %10s\n", get_teamed(k2.team1).name, k2.team1, k2.team2, get_teamed(k2.team2).name);
                        rewind(fknout);
                        getch();
                        break;
                    case 4:
                        seagroup();
                        break;
                    case 5 :
                        bestatk();
                        break;
                    case 6 :
                         bestdef();
                        break;
                    case 7:
                        polling();
                        break;
                    case 8 :
                         account_sett(&user);
                         break;
                }
            } while (chi != -1 && chi != 9);
            break;
        case 3:
            clr(user.username);
            if (!tour) {
                printf("The information has not been entered yet");
                getch();
                break;
            }
            int ln, i, j = 1, tours;
            do {
                for (ln = 0; fread(&tours, sizeof(int), 1, ftour) == 1; ln++) {
                    if (tours == tour) {
                        ln--;
                        continue;
                    }
                    printf("%s %d) 20%d %s\n", ln+1 == j ? "->" : "  ", ln+1, tours, ln+1 == j ? "<-" : "  ");
                }
                if (user.isAdmin) {
                    printf("%s %d) Creat 20%d ? %s\n", ln+1 == j ? "->" : "  ", ln+1, tours + 4, ln+1 == j ? "<-" : "  ");
                    ln++;
                }
                if (ln == 0) {
                    printf("\n\tThere id no other tournament\n");
                    getch();
                    break;
                }
                printf("press `Z` and `S` to move and `Enter` to select\n");
                i = getch();

                if(i == 115 || i == 83) j = j < ln ? ++j : 1;
                else if(i == 122 || i == 90) j = j > 1 ? --j : ln;
                else if (i == 27) {
                    rewind(ftour);
                    break;
                }

                if (i == 13) {
                    if (j != ln || !user.isAdmin) {
                        rewind(ftour);
                        for (ln = 0; ln < j && fread(&tours, sizeof(int), 1, ftour) == 1; ln++) {
                            if (tours == tour) ln--;
                        }
                        rewind(ftour);
                        tour = tours;
                        closeFiles();
                        goto start;
                    } else {
                        if (sure()) {
                            tours += 4;
                            tour = tours;
                            fwrite(&tour, sizeof(int), 1, ftour);
                            rewind(ftour);
                            char *a = malloc(100);
                            sprintf(a, "mkdir %d", tour);
                            system(a);
                            free(a);
                            clr(user.username);
                            printf("\nSuccess !!");
                            getch();
                            closeFiles();
                            goto start;
                        }
                    }
                }
                clr(user.username);
                rewind(ftour);
            } while(1);
            break;
        }
    } while (chi != -1);
    return 0;
}


/* bool write_data(char *filename, Test *data, int total) {
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
} */