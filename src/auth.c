#include <termios.h>
#include "header.h"
#include "atm.h"

char *USERS = "./data/users.txt";

/* ─── Customer Login ─────────────────────────────────────────── */
struct User *screen_login(void)
{
    // struct User tu = (struct User){
    //     .name = "",
    //     .password = ""};
    struct User *tu = calloc(1, sizeof(struct User));
    // malloc does not initialize memory — it contains garbage, same as uninitialized local variables.
    // to zero everything, use:
    // 1. calloc
    // 2. malloc + memset(0)
    // 3. stack: struct User u = {0};

    // char acc[ACC_NUM_LEN + 2] = {0};
    // char pin[PIN_LEN  + 2]    = {0};

    screen_reset(); // maybe not needed, just delwin ?!

    int wh = 14, ww = 50;
    int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
    WINDOW *lw = newwin(wh, ww, wy, wx);
    // check null
    wbkgd(lw, COLOR_PAIR(CP_NORMAL));
    draw_border(lw, "SECURE LOGIN", COLOR_PAIR(CP_TITLE) | A_BOLD);

    wattron(lw, COLOR_PAIR(CP_ACCENT) | A_BOLD);
    mvwprintw(lw, 2, 4, "🔒  Enter your credentials below");
    wattroff(lw, COLOR_PAIR(CP_ACCENT) | A_BOLD);

    wattron(lw, COLOR_PAIR(CP_NORMAL));
    mvwprintw(lw, 4, 4, "Account Number :");
    mvwprintw(lw, 7, 4, "PIN            :");
    wattroff(lw, COLOR_PAIR(CP_NORMAL));
    wrefresh(lw); // 1

    input_box(lw, 5, 4, 20, tu->name, ACC_NUM_LEN + 1, 0); // secret ?
    input_box(lw, 8, 4, 10, tu->password, PIN_LEN    + 1, 1);

    wrefresh(lw); // why 2 refreshes ?
    delwin(lw);
    // lw = NULL;   /* good habit: null the pointer so accidental use crashes loudly */
    // delwin is not deferred. ncurses has two separate concepts:
    // 1. The WINDOW struct  → lives in RAM  → managed by newwin / delwin
    // 2. The screen buffer  → lives in the terminal  → managed by refresh / clear
    // other functions: wclear or werase

    // 1... locking account
    // int idx = -1; // find_account(acc);
    // if (idx < 0) {
    //     msg_box("Login Failed", "Account not found.", 1);
    //     return;
    // }
    // if (accounts[idx].locked) {
    //     msg_box("Account Locked", "Too many failed attempts. Contact admin.", 1);
    //     return;
    // }


    // 2... not valid credentials
    // if (strcmp(accounts[idx].pin, pin) != 0) {
    // accounts[idx].fail_count++;
    // if (accounts[idx].fail_count >= 3) {
    //     accounts[idx].locked = 1;
    //     msg_box("Account Locked", "3 failed attempts — account locked.", 1);
    // } else {
    //     char emsg[60];
    //     sprintf(emsg, "Wrong PIN. Attempt %d/3.", accounts[idx].fail_count);
    //     msg_box("Login Failed", emsg, 1);
    // }
    //     return;
    // }
    char *pw = getPassword(*tu);
    // strcmp is not safe with null values
    if (pw == NULL || strcmp(tu->password, pw) != 0)
    {
        msg_box("Login Failed", "Wrong password!! or User Name.", 1);
        return NULL;
    }


    // 3... valid credentials
    // accounts[idx].fail_count = 0;
    // logged_acc = idx;
    // is_admin   = 0;

    // animate_loading("Authenticating...", 800);
    // screen_account_dashboard();
    // logged_acc = -1;
    free((void *)pw); /* must free — was malloc'd inside getPassword */
    // if reached here, so pw is not null 
    msg_box("Login Successful", "Password Match!", 0);
    // u = tu; // need to free user at logout !!!
    // u->id = tu->id;
    // memcpy(u->name, tu->name, 50);
    // memcpy(u->password, tu->password, 50);
    // free(tu);
    return tu;
}

// void loginMenu(char a[50], char pass[50])
// {
//     struct termios oflags, nflags;

//     system("clear");
//     printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
//     scanf("%s", a);

//     // disabling echo
//     tcgetattr(fileno(stdin), &oflags);
//     nflags = oflags;
//     nflags.c_lflag &= ~ECHO;
//     nflags.c_lflag |= ECHONL;

//     if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
//     {
//         perror("tcsetattr");
//         return exit(1);
//     }
//     printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
//     scanf("%s", pass);

//     // restore terminal
//     if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
//     {
//         perror("tcsetattr");
//         return exit(1);
//     }
// };

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
            // char *buff = userChecker.password;
            /* SEGFAULT — userChecker is a local
                variable on the stack, destroyed
                when function returns. caller gets
                a dangling pointer */
            char *buff = malloc(strlen(userChecker.password) + 1); /* heap allocated — survives return */
            if (!buff) return NULL;
            strcpy(buff, userChecker.password);
            return buff;
        }
    }

    fclose(fp);
    return NULL; // "no user found"
}

///////////////////////////////////////// My functions

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

void saveUserToFile(struct User u)
{
    // can i do it like save account ?
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