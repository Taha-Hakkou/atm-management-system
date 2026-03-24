// /*
//  * data.c — Account & transaction data helpers
//  */

// #include "atm.h"

// /* ─── Seed Demo Data ──────────────────────────────────────────── */
// void save_seed_data(void)
// {
//     strcpy(accounts[0].acc_num, "1000000001");
//     strcpy(accounts[0].name, "Alice Martin");
//     strcpy(accounts[0].pin, "1234");
//     accounts[0].balance = 12450.75;
//     accounts[0].locked = 0;
//     accounts[0].fail_count = 0;

//     strcpy(accounts[1].acc_num, "1000000002");
//     strcpy(accounts[1].name, "Bob Johnson");
//     strcpy(accounts[1].pin, "5678");
//     accounts[1].balance = 3200.00;
//     accounts[1].locked = 0;
//     accounts[1].fail_count = 0;

//     strcpy(accounts[2].acc_num, "1000000003");
//     strcpy(accounts[2].name, "Carol White");
//     strcpy(accounts[2].pin, "9999");
//     accounts[2].balance = 87600.50;
//     accounts[2].locked = 0;
//     accounts[2].fail_count = 0;

//     acc_count = 3;

//     add_transaction("1000000001", "Deposit", 5000.00);
//     add_transaction("1000000001", "Withdraw", 200.00);
//     add_transaction("1000000002", "Deposit", 3200.00);
//     add_transaction("1000000003", "Deposit", 90000.00);
//     add_transaction("1000000003", "Withdraw", 2399.50);
// }

// /* ─── Find account by number ──────────────────────────────────── */
// int find_account(const char *acc_num)
// {
//     for (int i = 0; i < acc_count; i++)
//         if (strcmp(accounts[i].acc_num, acc_num) == 0)
//             return i;
//     return -1;
// }

// /* ─── Record a transaction ────────────────────────────────────── */
// void add_transaction(const char *acc_num, const char *type, double amount)
// {
//     if (trans_count >= MAX_TRANS)
//         return;
//     Transaction2 *t = &transactions[trans_count++];
//     strncpy(t->acc_num, acc_num, ACC_NUM_LEN);
//     strncpy(t->type, type, 19);
//     t->amount = amount;
//     t->timestamp = time(NULL);
// }