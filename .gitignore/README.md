# Player leaderboard using SQLite3 and C

## Description
This project uses both SQLite3 for database management and C for defining logic, it creates a player leaderboard
with the features of adding, showing top players, delete,  and resetting the table, and the data is persistently stored in a local SQLite database.

## How to compile
To compile the project type in the terminal: gcc main.c db.c sqlite3.c -o leaderboard -I.

## How to run
To run this project type in the terminal: ./leaderboard

## Features
- Add a new player with a score
- View top N players sorted by score in a formatted table
- Delete a player by ID with confirmation
- Reset the leaderboard with confirmation
- Duplicate name detection with option to update score
- Input validation for negative scoresb