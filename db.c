#include <stdio.h>
#include "db.h"

sqlite3 *db;

void db_open(void){
    if (sqlite3_open("leaderboard.db", &db) != SQLITE_OK){
        printf("Error! Database not found: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS leaderboard (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, score INTEGER NOT NULL);", 0, 0, 0);
    //That creates the table if it doesnt't exist.
}
void db_close(void){
    sqlite3_close(db);
}
void db_add_score(const char *name, int score){
    sqlite3_stmt *check_stmt;
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM leaderboard WHERE name = ?;", -1, &check_stmt, 0);
    sqlite3_bind_text(check_stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_step(check_stmt);
    int count = sqlite3_column_int(check_stmt, 0);
    sqlite3_finalize(check_stmt);

    //if the count is greater than 0, notify the user that the name already exists
    //and ask if they want to update the score instead.
    if(count > 0){
        printf("Player with name '%s' already exists. Do you want to update the score instead? (y/n): ", name);
        char input[10];
        fgets(input, sizeof(input), stdin);
        if(input[0] != 'y' && input[0] != 'Y'){
            printf("Score not updated.\n");
            return;
        }
        else{
            printf("Updating score for player '%s'...\n", name);
            sqlite3_stmt *update_stmt;
            sqlite3_prepare_v2(db, "UPDATE leaderboard SET score = ? WHERE name = ?;", -1, &update_stmt, 0);
            sqlite3_bind_int(update_stmt, 1, score);
            sqlite3_bind_text(update_stmt, 2, name, -1, SQLITE_STATIC);
            sqlite3_step(update_stmt);
            sqlite3_finalize(update_stmt);
            return;
        }
    }
    sqlite3_stmt *stmt;
    //Preparing values
    sqlite3_prepare_v2(db, "INSERT INTO leaderboard (name, score) VALUES(?, ?);", -1, &stmt, 0);
    //Binding values
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, score);
    //Executing
    sqlite3_step(stmt);
    //finalizing.
    sqlite3_finalize(stmt);
}
void  db_view_top(int n){
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT id, name, score FROM leaderboard ORDER BY score DESC LIMIT ?;", -1, &stmt, 0);

    sqlite3_bind_int(stmt, 1, n);
    int rank = 1;
    printf("+-----+------+-----------------+---------+\n");
    printf("| #   | ID   | Name            | Score   |\n");
    printf("+-----+------+-----------------+---------+\n");
    while(sqlite3_step(stmt) ==SQLITE_ROW){
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char *) sqlite3_column_text(stmt, 1);
        int score = sqlite3_column_int(stmt, 2);
        printf("| %-3d | %-4d | %-15s | %-7d |\n", rank, id, name, score);
        rank++;
    }
    printf("+-----+------+-----------------+---------+\n");
    sqlite3_finalize(stmt);
}
void db_delete_player(int id){
    printf("Are you sure you want to delete player with ID %d? (y/n): ", id);
    char input[10];
    fgets(input, sizeof(input), stdin);
    if(input[0] != 'y' && input[0] != 'Y'){
        printf("Deletion cancelled.\n");
        return;
    }
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, "DELETE FROM leaderboard WHERE id = ?;", -1, &stmt, 0) == SQLITE_OK){
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_step(stmt);
        printf("Rows deleted: %d\n", sqlite3_changes(db));
        sqlite3_finalize(stmt);
    }
}
void db_reset(void){
    //Confirm reset
    printf("Are you sure you want to reset the leaderboard? This action cannot be undone. (y/n): ");
    char input[10];
    fgets(input, sizeof(input), stdin);
    if(input[0] != 'y' && input[0] != 'Y'){
        printf("Reset cancelled.\n");
        return;
    }
    else{
        printf("Resetting leaderboard...\n");
    }
    sqlite3_exec(db, "DELETE FROM leaderboard;", 0, 0, 0);
}