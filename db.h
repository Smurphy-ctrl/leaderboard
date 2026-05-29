#ifndef DB_H
#define DB_H

#include <sqlite3.h>

//Defining the struct

typedef struct{
    int id;
    char name[50];
    int score;
} Player;
//Function prototypes
void db_open(void);
void db_close(void);
void db_add_score(const char *name, int score);
void db_view_top(int n);
void db_delete_player(int id);
void db_reset(void);

#endif