#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "db.h"
//Main function with menu
int main(void){
db_open();
    //Menu loop
    int choice;
    do {
        printf("====Welcome to my database menu!====\n");
        printf("====1. Add new score====\n");
        printf("====2. View top N players====\n");
        printf("====3. Delete a player====\n");
        printf("====4. Reset leaderboard====\n");
        printf("====0. Exit====\n");
        printf("Enter your choice: ");
        char input[10];
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);
        switch(choice){
            //Add new score
        case 1: {
            char input_name[50];
            int input_score;
            printf("Enter player name: ");
            fgets(input_name, sizeof(input_name), stdin);
            input_name[strcspn(input_name, "\n")] = 0; // Remove newline
            printf("Enter player score: ");
            fgets(input, sizeof(input), stdin);
            input_score = atoi(input);
            //Checks for valid input
            if(input_score < 0){
                printf("Score cannot be negative!\n");
            }
            else{
                db_add_score(input_name, input_score);
            }
            break;
        }
        //View top n players
        case 2:{
            int n;
            printf("Enter number of top players to view: ");
            fgets(input, sizeof(input), stdin);
            n = atoi(input);
            db_view_top(n);
            break;
        }
        //Delete a player by id
        case 3:{
            char id_input[10];
            int del_id;
            printf("Enter player ID to delete: ");
            fgets(id_input, sizeof(id_input), stdin);
            del_id = atoi(id_input);
            printf("DEBUG: del_id: %d\n", del_id);
            db_delete_player(del_id);
            break;
        }
        //Reset leaderboard with confirmation
        case 4:{
            db_reset();
            break;
        }
        //Exit
        case 0:{
            printf("Goodbye!\n");
            break;
        }
        default: {
            printf("Invalid choice!\n");
            break;
        }
    }
    } while (choice != 0);
    //Close database connection before exiting
    db_close();
    return 0;
}