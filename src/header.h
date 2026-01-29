#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit; // should i consider it as date of creation ?
    struct Date withdraw; // ignored ?
};

struct User
{
    int id;
    char name[50];
    char password[50];
};


// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);
//
bool doesUserExist(struct User u);
void saveUserToFile(struct User u);
int countUsers();


// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
//
void updateAccount(struct User u);
void checkAccount(struct User u);


//
struct AccType {
    char type[8];
    int interest; // percent
};