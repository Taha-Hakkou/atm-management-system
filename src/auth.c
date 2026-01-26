#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

void registerMenu(char a[50], char pass[50])
{
    // 1. The Registration feature, you must be able to register new users, users with the same name can not exist (names must be unique). They must be saved in the right file.

    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Name:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    // resolved first line not readable by adding %d and reading id
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

bool doesUserExist(struct User u) // const ? + better to take only username and not all user
{
    FILE *fp;
    struct User tu; // temporary user

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &tu.id, tu.name, tu.password) != EOF)
    {
        if (strcmp(tu.name, u.name) == 0)
        {
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    return false;
}

void addUserToFile(struct User u)
{
    FILE *fp;
    int id;

    if ((fp = fopen("./data/users.txt", "a")) == NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    // create id
    id = countUsers();

    while (fprintf(fp, "%d %s %s\n", id, u.name, u.password) < 0)
    {
        printf("Error opening file"); // perror instead ?
        fclose(fp);
        exit(1);
    }

    fclose(fp);
}

int countUsers()
{
    // used only for addUserToFile (for now)
    // need another way to handle it instead of reading the whole file
    FILE *fp;
    int count = 0;
    char* line = NULL;
    size_t len = 0;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    while (getline(&line, &len, fp) != -1) {
        count++;
    }

    fclose(fp);
    return count;
}