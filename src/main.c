/*
 * ╔══════════════════════════════════════════════════════════════╗
 *   ATM MANAGEMENT SYSTEM — main.c
 *   Build: make   (or see Makefile)
 * ╚══════════════════════════════════════════════════════════════╝
 */

#include "atm.h"
#include "header.h"

/* ─── Global definitions ──────────────────────────────────────── */
Account accounts[MAX_ACCOUNTS];
Transaction2 transactions[MAX_TRANS];
int acc_count = 0;
int trans_count = 0;
int logged_acc = -1;
int is_admin = 0;
int ROWS, COLS;

/* ═══════════════════════════════════════════════════════════════ */
int main(void) {
    setlocale(LC_ALL, ""); // UTF-8
    initscr();
    cbreak();
    noecho(); // disable all these properties to not corrupt terminal after finish
    keypad(stdscr, TRUE);
    curs_set(0); // hides cursor
    // curs_set(1) restores the cursor to normal. curs_set(0) hides it, curs_set(1) is the default visible cursor, curs_set(2)
    getmaxyx(stdscr, ROWS, COLS);

    if (!has_colors() || !can_change_color())
    {
        // endwin();
        teardown();
        fprintf(stderr, "Terminal does not support colors.\n");
        return 1;
    }
    start_color();
    init_colors();
    // save_seed_data();
    screen_welcome();

    // endwin();
    teardown();
    return 0;
}

void teardown(void) {
    // how to call it even in unintended exit (program crash, segfault ...) ?
    keypad(stdscr, FALSE); // disable special key translation
    nocbreak();            // restore normal line-buffered input
    echo();                // re-enable terminal echo
    curs_set(1);           // restore cursor visibility
    endwin();              // hand terminal control back to shell
}