#include "header.h"

const char *RECORDS = "./data/records.txt";
const char *TRANSACTIONS = "./data/transactions.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		          &r->userId,
		          name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id, /*&r->id,*/
	        u.id, /*&u->id*/
	        u.name, /*&u->name,*/
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

///////////////////////////////////////// My functions

// The Update information of existing account feature, users must be able to update their country or phone number.
// 2.1. You must ask users to input the account id they want to change, followed by a prompt asking which field they want to also change (the only fields that are permitted to update is the phone number and the country).
// 2.2. Whenever users update an account, it must be saved into the corresponding file.
void updateAccount(struct User u) {
    int accountId, option;
    char username[100];
    struct Record r;

    FILE *ofp = fopen(RECORDS, "r");
    FILE *nfp = fopen("./data/.records.txt.tmp", "w");

    system("clear");
    printf("\t\t====== Update account info =====\n\n\t\tAccount ID:");
    scanf("%d", &accountId); // check if not a number OR if id not found
    while (getAccountFromFile(ofp, username, &r))
    {
        if (r.id == accountId && strcmp(username, u.name) == 0) {
            // process only one because records are accounts, unless it means transactions
            // prompt to choose either phone number or country
            printf("\n\t\t[1]- Phone Number\n");
            printf("\n\t\t[2]- Country\n");
            scanf("%d", &option);
            switch (option)
            {
            case 1:
                printf("\n\t\tPhone Number:");
                scanf("%d", &r.phone); // check if number
                break;
            case 2:
                printf("\n\t\tCountry:");
                scanf("%s", r.country); // check if number
                break;
            default:
                printf("Invalid choice!\n");
            }
        }
        saveAccountToFile(nfp, u, r);
        // if it stops, the temp file must be deleted !!!
    }
    fclose(ofp);
    fclose(nfp);
    rename("./data/.records.txt.tmp", RECORDS); // check return value
    success(u);
}


// The Checking the details of existing accounts feature, users must be able to check just one account at a time.
// 3.1. For this they must be asked to input the account id they want to see
// 3.2. If the account is either savings, fixed01, fixed02 and fixed03 the system will display the information of that account and the interest you will acquire depending on the account:
// savings: interest rate 7%
// fixed01(1 year account): interest rate 4%
// fixed02(2 year account): interest rate 5%
// fixed03(3 year account): interest rate 8%
// If the account is current you must display You will not get interests because the account is of type current
// For example: for an account of type savings with a deposit date of 10/10/2002 and an amount of $1023.20 the system will show "You will get $5.97 as interest on day 10 of every month".
void checkAccount(struct User u) {
    int accountId;
    char username[100];
    struct Record r;

    struct AccType accountTypes[] = {
        {"savings", 7},
        {"fixed01", 4},
        {"fixed02", 5},
        {"fixed03", 8}//,
        //{"current"}
    };

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== Check account info =====\n\n\t\tAccount ID:");
    scanf("%d", &accountId); // check if not a number OR if id not found !!!!!!!!!!!!!!!!!!!!!!!!!
    bool found;
    while (getAccountFromFile(pf, username, &r))
    {
        if (r.id == accountId && strcmp(username, u.name) == 0)
        {
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            printf("_____________________\n");
            if (strcmp(r.accountType, "current") == 0) {
                printf("You will not get interests because the account is of type current\n");
                break;
            }
            int len = sizeof(accountTypes) / sizeof(accountTypes[0]); // be causious! not always working. why ?
            int i;
            for (i = 0; i < len; i++) {
                if (strcmp(r.accountType, accountTypes[i].type) == 0) {
                    float mi = r.amount * accountTypes[i].interest / 100 / 12;
                    printf("\"You will get $%.2f as interest on day %d of every month\"\n", mi, r.deposit.day); // display 2 floating points + round
                    break;
                }
            }
            if (i == len) {
                printf("Not a valid account type\n"); // needs better handling
            }
            found = true;
            break;
        }
    }
    if (found == false) {
        printf("Account ID not found !\n"); // even if found and not permitted user
    }
    fclose(pf);
    success(u);
}






// 4. The Make transaction feature, users must be able to create transactions, withdrawing or depositing money to a certain account. All transactions must be updated and saved into the corresponding file. Accounts of type fixed01, fixed02 and fixed03 are not allowed to make transactions and an error message should be displayed if transactions are attempted with these accounts.
void makeTransaction(struct User u) {
    int accountId, option;
    char username[100];
    struct Record r;
    float amout;

    FILE *ofp = fopen(RECORDS, "r");
    FILE *nfp = fopen("./data/.records.txt.tmp", "w");

    FILE *tfp = fopen(TRANSACTIONS, "a");

    struct tm *t;
    time_t now;

    system("clear");
    printf("\t\t====== Make Transaction =====\n\n\t\tAccount ID:");
    scanf("%d", &accountId); // check if not a number OR if id not found
    bool found;
    while (getAccountFromFile(ofp, username, &r))
    {
        if (r.id == accountId && strcmp(username, u.name) == 0) {
            found = true;
            if (strcmp(r.accountType, "fixed01") == 0 ||
                strcmp(r.accountType, "fixed02") == 0 ||
                strcmp(r.accountType, "fixed03") == 0) {
                printf("Transactions are not permitted on this type of account !");
                break;
            }
            printf("\n\t\t[1]- Withdraw\n");
            printf("\n\t\t[2]- Deposit\n");
            scanf("%d", &option);
            switch (option)
            {
            case 1:
                printf("\n\t\tAmout:");
                scanf("%f", &amout); // check if float
                // withdraw amount from account
                r.amount -= amout;
                //add transaction
                now = time(NULL); 
                t = localtime(&now);
                fprintf(tfp, "%d %s %d/%d/%d\n",
                    accountId,
                    "withdraw",
                    t->tm_mon+1,
                    t->tm_mday,
                    t->tm_year+1900);
                break;
            case 2:
                printf("\n\t\tAmout:");
                scanf("%f", &amout); // check if float
                // deposit amount to account
                r.amount += amout;
                //add transaction
                now = time(NULL); 
                t = localtime(&now);
                fprintf(tfp, "%d %s %d/%d/%d\n",
                    accountId,
                    "deposit",
                    t->tm_mon+1,
                    t->tm_mday,
                    t->tm_year+1900);
                break;
            default:
                printf("Invalid Operation!\n");
            }
        }
        saveAccountToFile(nfp, u, r);
        // if it stops, the temp file must be deleted !!!
    }
    if (found == false) {
        printf("Account ID not found !\n"); // even if found and not permitted user
    }
    fclose(ofp);
    fclose(nfp);
    fclose(tfp);
    rename("./data/.records.txt.tmp", RECORDS); // check return value
    success(u);
}