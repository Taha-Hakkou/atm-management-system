/*
 * screens_admin.c — Admin-facing screens
 */

#include "atm.h"

/* ─── Admin Login ────────────────────────────────────────────── */
void screen_admin_login(void) {
    char pass[30] = {0};

    clear();
    bkgd(COLOR_PAIR(CP_NORMAL));
    attron(COLOR_PAIR(CP_BORDER) | A_BOLD);
    box(stdscr, 0, 0);
    attroff(COLOR_PAIR(CP_BORDER) | A_BOLD);

    attron(COLOR_PAIR(CP_HEADER) | A_BOLD);
    mvhline(1, 1, ' ', COLS - 2);
    mvprintw(1, (COLS - 20) / 2, "  ADMIN LOGIN  ");
    attroff(COLOR_PAIR(CP_HEADER) | A_BOLD);

    int wh = 10, ww = 46;
    int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
    WINDOW *lw = newwin(wh, ww, wy, wx);
    wbkgd(lw, COLOR_PAIR(CP_NORMAL));
    draw_border(lw, "ADMIN ACCESS", COLOR_PAIR(CP_ERROR) | A_BOLD);

    wattron(lw, COLOR_PAIR(CP_ERROR) | A_BOLD);
    mvwprintw(lw, 2, 4, "⚠  Restricted Area — Authorised Only");
    wattroff(lw, COLOR_PAIR(CP_ERROR) | A_BOLD);

    wattron(lw, COLOR_PAIR(CP_NORMAL));
    mvwprintw(lw, 4, 4, "Password :");
    wattroff(lw, COLOR_PAIR(CP_NORMAL));
    wrefresh(lw);
    input_box(lw, 5, 4, 20, pass, 29, 1);
    delwin(lw);

    if (strcmp(pass, "admin123") != 0) {
        msg_box("Access Denied", "Invalid admin password.", 1);
        return;
    }
    is_admin = 1;
    animate_loading("Verifying credentials...", 600);
    screen_admin_panel();
    is_admin = 0;
}

/* ─── Admin Panel ────────────────────────────────────────────── */
void screen_admin_panel(void) {
    const char *items[] = {
        "  ①  Create New Account",
        "  ②  List All Accounts",
        "  ③  Delete Account",
        "  ④  Logout"
    };
    int n = 4, sel = 0, ch;

    while (1) {
        clear();
        bkgd(COLOR_PAIR(CP_NORMAL));
        attron(COLOR_PAIR(CP_ERROR) | A_BOLD);
        box(stdscr, 0, 0);
        attroff(COLOR_PAIR(CP_ERROR) | A_BOLD);

        attron(COLOR_PAIR(CP_HEADER) | A_BOLD);
        mvhline(1, 1, ' ', COLS - 2);
        mvprintw(1, (COLS - 26) / 2, "  ⚙  ADMINISTRATOR PANEL  ⚙  ");
        attroff(COLOR_PAIR(CP_HEADER) | A_BOLD);

        /* stats panel */
        int sw = 46, sh = 5;
        int sy2 = 3, sx2 = (COLS - sw) / 2;
        WINDOW *sw2 = newwin(sh, sw, sy2, sx2);
        wbkgd(sw2, COLOR_PAIR(CP_NORMAL));
        draw_border(sw2, "SYSTEM STATS", COLOR_PAIR(CP_ACCENT) | A_BOLD);
        double total = 0;
        for (int i = 0; i < acc_count; i++) total += accounts[i].balance;
        char mbuf[32];
        wattron(sw2, COLOR_PAIR(CP_MONEY) | A_BOLD);
        mvwprintw(sw2, 2, 3, "Accounts: %3d   |  Total Funds: %s",
                  acc_count, fmt_money(total, mbuf));
        wattroff(sw2, COLOR_PAIR(CP_MONEY) | A_BOLD);
        wattron(sw2, COLOR_PAIR(CP_NORMAL));
        mvwprintw(sw2, 3, 3, "Transactions logged: %d", trans_count);
        wattroff(sw2, COLOR_PAIR(CP_NORMAL));
        wrefresh(sw2);
        delwin(sw2);

        int mh = n + 4, mw = 38;
        int my = sh + 5, mx = (COLS - mw) / 2;
        WINDOW *mwin = newwin(mh, mw, my, mx);
        wbkgd(mwin, COLOR_PAIR(CP_NORMAL));
        draw_border(mwin, "ADMIN MENU", COLOR_PAIR(CP_ERROR) | A_BOLD);
        for (int i = 0; i < n; i++) {
            if (i == sel) {
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
        ch = getch();
        delwin(mwin);

        if (ch == KEY_UP)        sel = (sel - 1 + n) % n;
        else if (ch == KEY_DOWN) sel = (sel + 1) % n;
        else if (ch == '\n' || ch == '\r') {
            if      (sel == 0) screen_create_account();
            else if (sel == 1) screen_list_accounts();
            else if (sel == 2) screen_delete_account();
            else break;
        }
        else if (ch == '1') screen_create_account();
        else if (ch == '2') screen_list_accounts();
        else if (ch == '3') screen_delete_account();
        else if (ch == '4' || ch == 'q') break;
    }
}

/* ─── Create Account ─────────────────────────────────────────── */
void screen_create_account(void) {
    if (acc_count >= MAX_ACCOUNTS) {
        msg_box("Error", "Maximum account limit reached.", 1);
        return;
    }
    char name[40] = {0}, pin[8] = {0}, dep[20] = {0};
    int wh = 16, ww = 52;
    int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
    WINDOW *cw = newwin(wh, ww, wy, wx);
    wbkgd(cw, COLOR_PAIR(CP_NORMAL));
    draw_border(cw, "CREATE ACCOUNT", COLOR_PAIR(CP_SUCCESS) | A_BOLD);
    wattron(cw, COLOR_PAIR(CP_NORMAL));
    mvwprintw(cw, 2, 4, "Full Name         :");
    mvwprintw(cw, 5, 4, "Initial PIN (4 d) :");
    mvwprintw(cw, 8, 4, "Opening Deposit   :");
    wattroff(cw, COLOR_PAIR(CP_NORMAL));
    wrefresh(cw);
    input_box(cw, 3, 4, 28, name, 39, 0);
    input_box(cw, 6, 4, 10, pin,   7, 1);
    input_box(cw, 9, 4, 18, dep,  19, 0);
    delwin(cw);

    if (strlen(name) == 0)      { msg_box("Error", "Name cannot be empty.", 1);   return; }
    if (strlen(pin) != PIN_LEN) { msg_box("Error", "PIN must be 4 digits.", 1);   return; }
    double opening = atof(dep);
    if (opening < 0)            { msg_box("Error", "Invalid deposit amount.", 1); return; }

    Account *na = &accounts[acc_count];
    srand((unsigned)time(NULL) + acc_count);
    /* cap at 8 random digits so total length stays within ACC_NUM_LEN=10 */
    sprintf(na->acc_num, "1%09d", rand() % 100000000);
    strncpy(na->name, name, 39+1);
    strncpy(na->pin,  pin,  PIN_LEN+4); // why 4 ?
    na->balance    = opening;
    na->locked     = 0;
    na->fail_count = 0;
    if (opening > 0) add_transaction(na->acc_num, "Deposit", opening);
    acc_count++;

    char msg[80];
    sprintf(msg, "Account %s created!", na->acc_num);
    animate_loading("Creating account...", 500);
    msg_box("Success", msg, 0);
}

/* ─── List Accounts ──────────────────────────────────────────── */
void screen_list_accounts(void) {
    int wh = ROWS - 6, ww = COLS - 8;
    int wy = 3, wx = 4;
    WINDOW *lw = newwin(wh, ww, wy, wx);
    wbkgd(lw, COLOR_PAIR(CP_NORMAL));
    draw_border(lw, "ALL ACCOUNTS", COLOR_PAIR(CP_TITLE) | A_BOLD);

    wattron(lw, COLOR_PAIR(CP_HEADER) | A_BOLD);
    mvwhline(lw, 2, 1, ' ', ww - 2);
    mvwprintw(lw, 2, 2, "%-12s %-22s %16s %8s",
              "ACC NUMBER", "NAME", "BALANCE", "STATUS");
    wattroff(lw, COLOR_PAIR(CP_HEADER) | A_BOLD);

    int scroll = 0, vis = wh - 6, ch;
    while (1) {
        for (int r = 0; r < vis; r++) {
            int ai = scroll + r;
            wmove(lw, r + 3, 1);
            whline(lw, ' ', ww - 2);
            if (ai >= acc_count) continue;
            Account *a = &accounts[ai];
            char mbuf[24];
            fmt_money(a->balance, mbuf);
            if (a->locked) wattron(lw, COLOR_PAIR(CP_ERROR));
            else           wattron(lw, COLOR_PAIR(CP_NORMAL));
            mvwprintw(lw, r + 3, 2, "%-12s %-22s %16s %8s",
                      a->acc_num, a->name, mbuf,
                      a->locked ? "LOCKED" : "Active");
            if (a->locked) wattroff(lw, COLOR_PAIR(CP_ERROR));
            else           wattroff(lw, COLOR_PAIR(CP_NORMAL));
        }
        wattron(lw, COLOR_PAIR(CP_DIM));
        mvwprintw(lw, wh - 2, 2,
                  "↑↓ Scroll   q Return   (%d accounts)", acc_count);
        wattroff(lw, COLOR_PAIR(CP_DIM));
        wrefresh(lw);
        ch = wgetch(lw);
        if (ch == 'q' || ch == 'Q') break;
        if (ch == KEY_DOWN && scroll + vis < acc_count) scroll++;
        if (ch == KEY_UP   && scroll > 0)               scroll--;
    }
    delwin(lw);
    touchwin(stdscr);
    refresh();
}

/* ─── Delete Account ─────────────────────────────────────────── */
void screen_delete_account(void) {
    char acc[ACC_NUM_LEN + 2] = {0};
    int wh = 8, ww = 46;
    int wy = (ROWS - wh) / 2, wx = (COLS - ww) / 2;
    WINDOW *dw = newwin(wh, ww, wy, wx);
    wbkgd(dw, COLOR_PAIR(CP_NORMAL));
    draw_border(dw, "DELETE ACCOUNT", COLOR_PAIR(CP_ERROR) | A_BOLD);
    wattron(dw, COLOR_PAIR(CP_ERROR) | A_BOLD);
    mvwprintw(dw, 2, 4, "⚠  This action is irreversible!");
    wattroff(dw, COLOR_PAIR(CP_ERROR) | A_BOLD);
    wattron(dw, COLOR_PAIR(CP_NORMAL));
    mvwprintw(dw, 4, 4, "Account Number: ");
    wattroff(dw, COLOR_PAIR(CP_NORMAL));
    wrefresh(dw);
    input_box(dw, 5, 4, 18, acc, ACC_NUM_LEN + 1, 0);
    delwin(dw);

    int idx = find_account(acc);
    if (idx < 0) { msg_box("Error", "Account not found.", 1); return; }

    char conf[60];
    sprintf(conf, "Delete account of %s?", accounts[idx].name);
    if (!confirm_box(conf)) return;

    for (int i = idx; i < acc_count - 1; i++) accounts[i] = accounts[i + 1];
    acc_count--;
    animate_loading("Deleting account...", 400);
    msg_box("Success", "Account deleted.", 0);
}
