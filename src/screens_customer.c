/*
 * screens_customer.c — Customer-facing screens
 */

#include "header.h"
#include "atm.h" // order matters because some functions need 'struct User'

/* ─── Welcome / Splash ───────────────────────────────────────── */
void screen_welcome(void) {
    struct User *u;

    screen_reset();

    // Logo
    draw_logo(stdscr, (ROWS / 2) - 10, (COLS - 28) / 2); // 33 -> 28 = logo row length

    // Dim
    attron(COLOR_PAIR(CP_DIM));
    mvprintw((ROWS / 2) + 0, (COLS - 38) / 2,
             "Secure ▸ Reliable ▸ Fast ▸ Modern Banking");
    attroff(COLOR_PAIR(CP_DIM));

    // ...
    attron(COLOR_PAIR(CP_ACCENT) | A_BLINK);
    mvprintw((ROWS / 2) + 3, (COLS - 22) / 2, "▶  Press any key to begin");
    attroff(COLOR_PAIR(CP_ACCENT) | A_BLINK);

    // time !!!!!!!!!! (previous place)

    refresh();
    getch();

    // initMenu(&u);
    u = initMenu();

    // mainMenu(u);
    // screen_main_menu();
    screen_account_dashboard(*u);
    return; // 0
}

void screen_reset(void) {
    clear();
    // background
    bkgd(COLOR_PAIR(CP_NORMAL));
    // borders
    attron(COLOR_PAIR(CP_BORDER) | A_BOLD);
    box(stdscr, 0, 0);
    attroff(COLOR_PAIR(CP_BORDER) | A_BOLD);
    // time
    time_t now = time(NULL);
    char tbuf[64];
    strftime(tbuf, sizeof(tbuf), "%A, %B %d %Y  |  %H:%M:%S", localtime(&now));
    attron(COLOR_PAIR(CP_DIM));
    // mvhline(1, 1, ' ', COLS - 2);
    // mvprintw(1, (COLS - 28) / 2, "  ATM MANAGEMENT SYSTEM  v1.0  ");
    mvprintw(1, (COLS - (int)strlen(tbuf)) / 2, "%s", tbuf); // previous row: ROWS-2
    attroff(COLOR_PAIR(CP_DIM));

    refresh();
}

struct User *initMenu(void)
{
    //  In C, uninitialized pointers contain garbage values — whatever random bytes happen to be at that memory location.
    // The only cases where pointers are automatically NULL/zero: global vars and static vars
    struct User *u = NULL;

    const char *items[] = {
        "  [1]  Login",
        "  [2]  Register",
        "  [3]  Exit"
    };
    int n = 3, selected = 0, c;

    screen_reset();

    // Logo
    // draw_logo(stdscr, 3, (COLS - 33) / 2);

    attron(COLOR_PAIR(CP_DIM));
    mvprintw(ROWS - 2, 2, "↑↓ Navigate   Enter Select   q Quit");
    attroff(COLOR_PAIR(CP_DIM));
    refresh();

    int mh = n + 4, mw = 36;
    int my = (ROWS / 2) + 2, mx = (COLS - mw) / 2;
    WINDOW *mw_win = newwin(mh, mw, my, mx);
    // if (!mw_win) return;  --> null check
    
    // + keypad(mw_win, TRUE)
    
    while (1) {
        wbkgd(mw_win, COLOR_PAIR(CP_NORMAL));
        draw_border(mw_win, "MAIN MENU", COLOR_PAIR(CP_TITLE) | A_BOLD);
        // window background and borders should be redrawn inside loop
        
        for (int i = 0; i < n; i++) {
            if (i == selected) {
                wattron(mw_win, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
                mvwhline(mw_win, i + 2, 1, ' ', mw - 2);
                mvwprintw(mw_win, i + 2, 2, "%s", items[i]);
                wattroff(mw_win, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
            } else {
                wattron(mw_win, COLOR_PAIR(CP_NORMAL));
                mvwhline(mw_win, i + 2, 1, ' ', mw - 2); // used instead of deleting window
                mvwprintw(mw_win, i + 2, 2, "%s", items[i]);
                wattroff(mw_win, COLOR_PAIR(CP_NORMAL));
            }
        }
        wrefresh(mw_win);

        c = getch();
        // Bug 4 — getch() on stdscr instead of the window. You call getch()(reads from stdscr) but your active window is mw_win.Use wgetch(mw_win) to be consistent.
        

        // delwin(mw_win); // ?

        // struct User *tu;

        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + n) % n;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % n;
            break;
        case '\n':
        case '\r':
            if (selected == 0) {
                u = screen_login();
            } else if (selected == 1) {
                // screen_admin_login();
                registerMenu(u->name, u->password);
                if (doesUserExist(*u)) {
                    // check if username exists
                    printf("\n\nusername exists!");
                    exit(1);
                }
                // + check if password is valid
                else {
                    saveUserToFile(*u);
                    printf("\nUser created successfully!\n");
                }
            }
            // delwin
            break;
        case '1':
            u = screen_login();
            // redo same login
            // + delwin
            break;
        case '2':
            // screen_admin_login();
            // redo same register
            // + delwin
            break;
        case '3':
        case 'q':
        case 'Q':
            // clear(); --> has no effect
            teardown();
            exit(1);
            break;
        default:
            // skip: not a valid option
            break;
        }

        // if (u != NULL) {
            return u;
        // }
    }
    // return null not recheable
};

// /* ─── Account Dashboard ──────────────────────────────────────── */
void screen_account_dashboard(struct User u) {
    const char *items[] = {
        // "  ①  Deposit",
        // "  ②  Withdraw",
        // "  ③  Transfer Funds",
        // "  ④  Transaction History",
        // "  ⑤  Change PIN",
        // "  ⑥  Logout"
        "  [1]  Create a new account",
        "  [2]  Update account information",
        "  [3]  Check accounts",
        "  [4]  Check list of owned account",
        "  [5]  Make Transaction",
        "  [6]  Remove existing account",
        "  [7]  Transfer ownership",
        "  [8]  Exit"
    };
    int n = 8, selected = 0, c;
    screen_reset();

    // Account *a = &accounts[logged_acc];
    // char mbuf[32];

    while (1) {
        // clear();
        // bkgd(COLOR_PAIR(CP_NORMAL));
        // attron(COLOR_PAIR(CP_BORDER) | A_BOLD);
        // box(stdscr, 0, 0);
        // attroff(COLOR_PAIR(CP_BORDER) | A_BOLD);

        // 1... welcome username
        attron(COLOR_PAIR(CP_HEADER) | A_BOLD);
        mvhline(2, 1, ' ', COLS - 2); // why ?
        // mvprintw(1, 3, " Welcome, %s ", a->name);
        mvprintw(2, 3, " Welcome, %s ", u.name);
        // mvprintw(1, COLS - 26, " Acc: %s ", a->acc_num);
        attroff(COLOR_PAIR(CP_HEADER) | A_BOLD);

        /* balance panel */
        int bpw = 36, bph = 6;
        int bpx = (COLS - bpw) / 2, bpy = 3;
        // WINDOW *bpan = newwin(bph, bpw, bpy, bpx);
        // wbkgd(bpan, COLOR_PAIR(CP_NORMAL));
        // draw_border(bpan, "ACCOUNT BALANCE", COLOR_PAIR(CP_MONEY) | A_BOLD);
        // wattron(bpan, COLOR_PAIR(CP_MONEY) | A_BOLD);
        // char *ms = fmt_money(a->balance, mbuf);
        // mvwprintw(bpan, 2, (bpw - (int)strlen(ms) - 2) / 2, "▶  %s  ◀", ms);
        // wattroff(bpan, COLOR_PAIR(CP_MONEY) | A_BOLD);
        // wattron(bpan, COLOR_PAIR(CP_DIM));
        // mvwprintw(bpan, 4, 3, "Available Balance — Savings Account");
        // wattroff(bpan, COLOR_PAIR(CP_DIM));
        // wrefresh(bpan);
        // delwin(bpan);

        /* menu */
        int mh = n + 4, mw = 38;
        int my = bpy + bph + 1, mx = (COLS - mw) / 2;
        WINDOW *mwin = newwin(mh, mw, my, mx);
        wbkgd(mwin, COLOR_PAIR(CP_NORMAL));
        draw_border(mwin, "OPERATIONS", COLOR_PAIR(CP_TITLE) | A_BOLD);
        for (int i = 0; i < n; i++) {
            if (i == selected) {
                wattron(mwin, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
                mvwhline(mwin, i + 2, 1, ' ', mw - 2);
                mvwprintw(mwin, i + 2, 2, "%s", items[i]);
                wattroff(mwin, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
            } else {
                wattron(mwin, COLOR_PAIR(CP_NORMAL));
                mvwprintw(mwin, i + 2, 2, "%s", items[i]);
                wattroff(mwin, COLOR_PAIR(CP_NORMAL));
            }
        }
        wrefresh(mwin);

        attron(COLOR_PAIR(CP_DIM));
        mvprintw(ROWS - 2, 2, "↑↓ Navigate   Enter Select");
        attroff(COLOR_PAIR(CP_DIM));
        refresh();
        c = getch();
        delwin(mwin);

        switch (c) {
        case KEY_UP:
            selected = (selected - 1 + n) % n;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % n;
            break;
        case '\n':
        case '\r':
            // need to try merging conditions of c and selected !
            // if      (selected == 0) screen_deposit();
            // else if (selected == 1) screen_withdraw();
            // else if (selected == 2) screen_transfer();
            // else if (selected == 3) screen_history();
            // else if (selected == 4) screen_change_pin();
            // else break;
            // ...
            break;
        case '1':
            // screen_deposit();
            // ...
            createNewAcc(u);
            break;
        case '2':
            // screen_withdraw();
            // ...
            // student TODO : add your **Update account information** function
            // updateAccount(u);
            break;
        case '3':
            // screen_transfer();
            // ...
            // student TODO : add your **Check the details of existing accounts** function
            // checkAccount(u);
            break;
        case '4':
            // screen_history();
            // ...
            // checkAllAccounts(u);
            break;
        case '5':
            // screen_change_pin();
            // ...
            // student TODO : add your **Make transaction** function
            // makeTransaction(u);
            break;
        case '6':
            // ...
            // student TODO : add your **Remove existing account** function
            // removeAccount(u);
            break;
        case '7':
            // ...
            // student TODO : add your **Transfer owner** function
            // transferOwnership(u);
            break;
        case '8':
        case 'q':
        case 'Q':
            // ...
            // exit(1);
            break;
        default:
            // printf("Invalid operation!\n");
        }
    }
}

void mainMenu(struct User u)
{
//     int option;
//     system("clear");
//     printf("\n\n\t\t======= ATM =======\n\n");
//     printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
//     printf("\n\t\t[1]- Create a new account\n");
//     printf("\n\t\t[2]- Update account information\n");
//     printf("\n\t\t[3]- Check accounts\n");
//     printf("\n\t\t[4]- Check list of owned account\n");
//     printf("\n\t\t[5]- Make Transaction\n");
//     printf("\n\t\t[6]- Remove existing account\n");
//     printf("\n\t\t[7]- Transfer ownership\n");
//     printf("\n\t\t[8]- Exit\n");
//     scanf("%d", &option);

//     switch (option)
//     {
//     case 1:
//         createNewAcc(u);
//         break;
//     case 2:
//         // student TODO : add your **Update account information** function
//         updateAccount(u);
//         // ********************************
//         break;
//     case 3:
//         // student TODO : add your **Check the details of existing accounts** function
//         checkAccount(u);
//         // ******************************************************
//         break;
//     case 4:
//         checkAllAccounts(u);
//         break;
//     case 5:
//         // student TODO : add your **Make transaction** function
//         makeTransaction(u);
//         // ********************
//         break;
//     case 6:
//         // student TODO : add your **Remove existing account** function
//         removeAccount(u);
//         // here
//         break;
//     case 7:
//         // student TODO : add your **Transfer owner** function
//         transferOwnership(u);
//         // here
//         break;
//     case 8:
//         exit(1);
//         break;
//     default:
//         printf("Invalid operation!\n");
//     }
};


/* ─── Amount prompt helper ───────────────────────────────────── */
double prompt_amount(const char *label)
{
    char buf[20] = {0};
    int wh = 9, ww = 44;
    int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
    WINDOW *pw = newwin(wh, ww, wy, wx);
    wbkgd(pw, COLOR_PAIR(CP_NORMAL));
    draw_border(pw, label, COLOR_PAIR(CP_ACCENT) | A_BOLD);
    wattron(pw, COLOR_PAIR(CP_NORMAL));
    mvwprintw(pw, 3, 4, "Enter amount ($): ");
    wattroff(pw, COLOR_PAIR(CP_NORMAL));
    wrefresh(pw);
    input_box(pw, 4, 4, 20, buf, 19, 0);
    delwin(pw);
    touchwin(stdscr);
    refresh();
    return atof(buf);
}

/* ─── Main Menu ──────────────────────────────────────────────── */
// void screen_main_menu(void) {
//     const char *items[] = {
//         "  ①  Customer Login",
//         "  ②  Admin Panel",
//         "  ③  Exit"
//     };
//     int n = 3, sel = 0, ch;
    
//     screen_reset();
    
//         // Header
//         attron(COLOR_PAIR(CP_HEADER) | A_BOLD);
//         mvhline(1, 1, ' ', COLS - 2);
//         mvprintw(1, (COLS - 28) / 2, "  ATM MANAGEMENT SYSTEM  v1.0  ");
//         attroff(COLOR_PAIR(CP_HEADER) | A_BOLD);
        
//         // Logo
//         draw_logo(stdscr, 3, (COLS - 33) / 2);

//         attron(COLOR_PAIR(CP_DIM));
//         mvprintw(ROWS - 2, 2, "↑↓ Navigate   Enter Select   q Quit");
//         attroff(COLOR_PAIR(CP_DIM));
//         refresh();

//         int mh = n + 4, mw = 36;
//         int my = (ROWS / 2) + 2, mx = (COLS - mw) / 2;
//         WINDOW *mw_win = newwin(mh, mw, my, mx);
//         wbkgd(mw_win, COLOR_PAIR(CP_NORMAL));
//         draw_border(mw_win, "MAIN MENU", COLOR_PAIR(CP_TITLE) | A_BOLD);
        
//     while (1) {
//         for (int i = 0; i < n; i++) {
//             if (i == sel) {
//                 wattron(mw_win, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
//                 mvwhline(mw_win, i + 2, 1, ' ', mw - 2);
//                 mvwprintw(mw_win, i + 2, 2, "%s", items[i]);
//                 wattroff(mw_win, COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
//             } else {
//                 wattron(mw_win, COLOR_PAIR(CP_NORMAL));
//                 mvwprintw(mw_win, i + 2, 2, "%s", items[i]);
//                 wattroff(mw_win, COLOR_PAIR(CP_NORMAL));
//             }
//         }
//         wrefresh(mw_win);

        

//         ch = getch();
//         delwin(mw_win);

//         switch (ch) {
//         case KEY_UP:
//             sel = (sel - 1 + n) % n;
//             break;
//         case KEY_DOWN:
//             sel = (sel + 1) % n;
//             break;
//         case '\n': // same instructions
//         case '\r':
//             if (sel == 0) screen_login();
//             else if (sel == 1) screen_admin_login();
//             break;
//         case '1':
//             screen_login();
//             break;
//         case '2':
//             screen_admin_login();
//             break;
//         case '3':
//         case 'q':
//         case 'Q':
//             break;
//         default: // there is no default
//             break;
//         }
//         // if (ch == KEY_UP)             sel = (sel - 1 + n) % n;
//         // else if (ch == KEY_DOWN)      sel = (sel + 1) % n;
//         // else if (ch == '\n' || ch == '\r') {
//         //     if (sel == 0)      screen_login();
//         //     else if (sel == 1) screen_admin_login();
//         //     else break;
//         // }
//         // else if (ch == '1') screen_login();
//         // else if (ch == '2') screen_admin_login();
//         // else if (ch == '3') break;
//         // else if (ch == 'q' || ch == 'Q') break;
//     }
// }





// /* ─── Deposit ────────────────────────────────────────────────── */
// void screen_deposit(void) {
//     double amt = prompt_amount("DEPOSIT");
//     if (amt <= 0) { msg_box("Error", "Invalid amount entered.", 1); return; }
//     Account *a = &accounts[logged_acc];
//     a->balance += amt;
//     add_transaction(a->acc_num, "Deposit", amt);
//     char mbuf[32], msg[64];
//     sprintf(msg, "Deposited %s successfully.", fmt_money(amt, mbuf));
//     animate_loading("Processing deposit...", 500);
//     msg_box("Success", msg, 0);
// }

// /* ─── Withdraw ───────────────────────────────────────────────── */
// void screen_withdraw(void) {
//     double amt = prompt_amount("WITHDRAW");
//     if (amt <= 0) { msg_box("Error", "Invalid amount.", 1); return; }
//     Account *a = &accounts[logged_acc];
//     if (amt > a->balance) { msg_box("Error", "Insufficient funds.", 1); return; }
//     a->balance -= amt;
//     add_transaction(a->acc_num, "Withdraw", amt);
//     char mbuf[32], msg[64];
//     sprintf(msg, "Withdrawn %s. Please collect cash.", fmt_money(amt, mbuf));
//     animate_loading("Dispensing cash...", 700);
//     msg_box("Success", msg, 0);
// }

// /* ─── Transfer ───────────────────────────────────────────────── */
// void screen_transfer(void) {
//     char to_acc[ACC_NUM_LEN + 2] = {0};
//     int wh = 12, ww = 50;
//     int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
//     WINDOW *tw = newwin(wh, ww, wy, wx);
//     wbkgd(tw, COLOR_PAIR(CP_NORMAL));
//     draw_border(tw, "FUND TRANSFER", COLOR_PAIR(CP_TITLE) | A_BOLD);
//     wattron(tw, COLOR_PAIR(CP_NORMAL));
//     mvwprintw(tw, 2, 4, "Destination Account Number:");
//     wattroff(tw, COLOR_PAIR(CP_NORMAL));
//     wrefresh(tw);
//     input_box(tw, 3, 4, 20, to_acc, ACC_NUM_LEN + 1, 0);
//     delwin(tw);

//     int to_idx = find_account(to_acc);
//     if (to_idx < 0)           { msg_box("Error", "Destination account not found.", 1); return; }
//     if (to_idx == logged_acc) { msg_box("Error", "Cannot transfer to yourself.", 1);   return; }

//     double amt = prompt_amount("TRANSFER AMOUNT");
//     if (amt <= 0) { msg_box("Error", "Invalid amount.", 1); return; }

//     Account *from = &accounts[logged_acc];
//     Account *to   = &accounts[to_idx];
//     if (amt > from->balance) { msg_box("Error", "Insufficient funds.", 1); return; }

//     char conf[80], mbuf[32];
//     sprintf(conf, "Transfer %s to %s?", fmt_money(amt, mbuf), to->name);
//     if (!confirm_box(conf)) return;

//     from->balance -= amt;
//     to->balance   += amt;
//     add_transaction(from->acc_num, "Transfer Out", amt);
//     add_transaction(to->acc_num,   "Transfer In",  amt);
//     animate_loading("Transferring funds...", 800);
//     msg_box("Success", "Transfer completed successfully.", 0);
// }

// /* ─── Transaction History ────────────────────────────────────── */
// void screen_history(void) {
//     Account *a = &accounts[logged_acc];
//     int idx[MAX_TRANS], cnt = 0;
//     for (int i = 0; i < trans_count; i++)
//         if (strcmp(transactions[i].acc_num, a->acc_num) == 0)
//             idx[cnt++] = i;

//     int wh = ROWS - 6, ww = COLS - 10;
//     int wy = 3, wx = 5;
//     WINDOW *hw = newwin(wh, ww, wy, wx);
//     wbkgd(hw, COLOR_PAIR(CP_NORMAL));
//     draw_border(hw, "TRANSACTION HISTORY", COLOR_PAIR(CP_TITLE) | A_BOLD);

//     wattron(hw, COLOR_PAIR(CP_HEADER) | A_BOLD);
//     mvwhline(hw, 2, 1, ' ', ww - 2);
//     mvwprintw(hw, 2, 2, "%-22s %-16s %14s", "DATE & TIME", "TYPE", "AMOUNT");
//     wattroff(hw, COLOR_PAIR(CP_HEADER) | A_BOLD);

//     int scroll = 0, visible = wh - 6, ch;
//     while (1) {
//         for (int row = 0; row < visible; row++) {
//             int ti = scroll + row;
//             wmove(hw, row + 3, 1);
//             whline(hw, ' ', ww - 2);
//             if (ti >= cnt) continue;
//             Transaction2 *t = &transactions[idx[ti]];
//             char tbuf[24], mbuf[24];
//             strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S",
//                      localtime(&t->timestamp));
//             fmt_money(t->amount, mbuf);
//             int is_in = (strstr(t->type, "Deposit") || strstr(t->type, "In"));
//             if (is_in) wattron(hw, COLOR_PAIR(CP_MONEY));
//             else       wattron(hw, COLOR_PAIR(CP_ERROR));
//             mvwprintw(hw, row + 3, 2, "%-22s %-16s %14s", tbuf, t->type, mbuf);
//             if (is_in) wattroff(hw, COLOR_PAIR(CP_MONEY));
//             else       wattroff(hw, COLOR_PAIR(CP_ERROR));
//         }
//         wattron(hw, COLOR_PAIR(CP_DIM));
//         mvwprintw(hw, wh - 2, 2,
//                   "↑↓ Scroll   q Return   (%d/%d records)", cnt, cnt);
//         wattroff(hw, COLOR_PAIR(CP_DIM));
//         wrefresh(hw);
//         ch = wgetch(hw);
//         if (ch == 'q' || ch == 'Q' || ch == KEY_F(10)) break;
//         if (ch == KEY_DOWN && scroll + visible < cnt) scroll++;
//         if (ch == KEY_UP   && scroll > 0)             scroll--;
//     }
//     delwin(hw);
//     touchwin(stdscr);
//     refresh();
// }

// /* ─── Change PIN ─────────────────────────────────────────────── */
// void screen_change_pin(void) {
//     char old_pin[8] = {0}, new_pin[8] = {0}, confirm[8] = {0};
//     int wh = 14, ww = 46;
//     int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
//     WINDOW *pw = newwin(wh, ww, wy, wx);
//     wbkgd(pw, COLOR_PAIR(CP_NORMAL));
//     draw_border(pw, "CHANGE PIN", COLOR_PAIR(CP_ACCENT) | A_BOLD);
//     wattron(pw, COLOR_PAIR(CP_NORMAL));
//     mvwprintw(pw, 2, 4, "Current PIN   :");
//     mvwprintw(pw, 5, 4, "New PIN (4 d.) :");
//     mvwprintw(pw, 8, 4, "Confirm PIN   :");
//     wattroff(pw, COLOR_PAIR(CP_NORMAL));
//     wrefresh(pw);
//     input_box(pw, 3, 4, 12, old_pin, PIN_LEN + 1, 1);
//     input_box(pw, 6, 4, 12, new_pin, PIN_LEN + 1, 1);
//     input_box(pw, 9, 4, 12, confirm, PIN_LEN + 1, 1);
//     delwin(pw);

//     Account *a = &accounts[logged_acc];
//     if (strcmp(a->pin, old_pin) != 0)  { msg_box("Error", "Current PIN is incorrect.", 1);   return; }
//     if (strlen(new_pin) != PIN_LEN)    { msg_box("Error", "PIN must be exactly 4 digits.", 1); return; }
//     if (strcmp(new_pin, confirm) != 0) { msg_box("Error", "PINs do not match.", 1);            return; }
//     strcpy(a->pin, new_pin);
//     msg_box("Success", "PIN changed successfully.", 0);
// }