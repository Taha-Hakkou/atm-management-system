#ifndef ATM_H
#define ATM_H

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

/* ─── Color Pair IDs ──────────────────────────────────────────── */
#define CP_NORMAL       1
#define CP_TITLE        2
#define CP_HIGHLIGHT    3
#define CP_SUCCESS      4
#define CP_ERROR        5
#define CP_BORDER       6
#define CP_DIM          7
#define CP_INPUT        8
#define CP_MONEY        9
#define CP_HEADER       10
#define CP_SELECTED     11
#define CP_ACCENT       12

/* ─── ATM Data ────────────────────────────────────────────────── */
#define MAX_ACCOUNTS    50
#define MAX_TRANS       200
#define PIN_LEN         4
#define ACC_NUM_LEN     10

typedef struct {
    char   acc_num[ACC_NUM_LEN + 1];
    char   name[40];
    char   pin[PIN_LEN + 1];
    double balance;
    int    locked;
    int    fail_count;
} Account;

typedef struct {
    char   acc_num[ACC_NUM_LEN + 1];
    char   type[20];
    double amount;
    time_t timestamp;
} Transaction2;

/* ─── Globals (defined in main.c) ────────────────────────────── */
extern Account     accounts[MAX_ACCOUNTS];
extern Transaction2 transactions[MAX_TRANS];
extern int         acc_count;
extern int         trans_count;
extern int         logged_acc;
extern int         is_admin;
extern int         ROWS, COLS;

void screen_reset(void);
void teardown(void);

/* ─── ui.c ────────────────────────────────────────────────────── */
void init_colors(void);
void draw_border(WINDOW *w, const char *title, int attr);
void draw_logo(WINDOW *w, int y, int x);
void statusbar(const char *msg, int is_error);
void animate_loading(const char *msg, int ms);
char *fmt_money(double d, char *buf);
void input_box(WINDOW *w, int y, int x, int width, char *buf, int maxlen, int secret);
void msg_box(const char *title, const char *msg, int is_error);
int  confirm_box(const char *msg);

/* ─── data.c ──────────────────────────────────────────────────── */
int  find_account(const char *acc_num);
void add_transaction(const char *acc_num, const char *type, double amount);
void save_seed_data(void);

/* ─── screens_customer.c ──────────────────────────────────────── */
void screen_welcome(void);
void screen_main_menu(void);
struct User *screen_login(void);
void screen_account_dashboard(struct User u);
void screen_deposit(void);
void screen_withdraw(void);
void screen_transfer(void);
void screen_history(void);
void screen_change_pin(void);
double prompt_amount(const char *label);

/* ─── screens_admin.c ─────────────────────────────────────────── */
void screen_admin_login(void);
void screen_admin_panel(void);
void screen_create_account(void);
void screen_list_accounts(void);
void screen_delete_account(void);

#endif /* ATM_H */