/*
 * ui.c — ncurses UI primitives: colors, borders, input, popups
 */

#include "atm.h"

/* ─── Color Setup ─────────────────────────────────────────────── */
void init_colors(void) {
    init_pair(CP_NORMAL,    COLOR_WHITE,   COLOR_BLACK);
    init_pair(CP_TITLE,     COLOR_CYAN,    COLOR_BLACK);
    init_pair(CP_HIGHLIGHT, COLOR_BLACK,   COLOR_CYAN);
    init_pair(CP_SUCCESS,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(CP_ERROR,     COLOR_RED,     COLOR_BLACK);
    init_pair(CP_BORDER,    COLOR_CYAN,    COLOR_BLACK);
    init_pair(CP_DIM,       COLOR_WHITE,   COLOR_BLACK);
    init_pair(CP_INPUT,     COLOR_WHITE,   COLOR_BLUE);
    init_pair(CP_MONEY,     COLOR_GREEN,   COLOR_BLACK);
    init_pair(CP_HEADER,    COLOR_BLACK,   COLOR_BLUE);
    init_pair(CP_SELECTED,  COLOR_BLACK,   COLOR_GREEN);
    init_pair(CP_ACCENT,    COLOR_YELLOW,  COLOR_BLACK);
}

/* ─── ASCII Art Logo ─────────────────────────────────────────── */
void draw_logo(WINDOW *w, int y, int x) {
    // Title
    wattron(w, COLOR_PAIR(CP_TITLE) | A_BOLD);
    mvwprintw(w, y + 0, x, " █████╗ ████████╗███╗   ███╗");
    mvwprintw(w, y + 1, x, "██╔══██╗╚══██╔══╝████╗ ████║");
    mvwprintw(w, y + 2, x, "███████║   ██║   ██╔████╔██║");
    mvwprintw(w, y + 3, x, "██╔══██║   ██║   ██║╚██╔╝██║");
    mvwprintw(w, y + 4, x, "██║  ██║   ██║   ██║ ╚═╝ ██║");
    mvwprintw(w, y + 5, x, "╚═╝  ╚═╝   ╚═╝   ╚═╝     ╚═╝");
    wattroff(w, COLOR_PAIR(CP_TITLE) | A_BOLD);

    // Accent
    wattron(w, COLOR_PAIR(CP_ACCENT) | A_BOLD);
    mvwprintw(w, y + 7, x - 1, "◈  AUTOMATED TELLER MACHINE  ◈");
    wattroff(w, COLOR_PAIR(CP_ACCENT) | A_BOLD);
}

/* ─── Decorative border with title ───────────────────────────── */
void draw_border(WINDOW *w, const char *title, int attr) {
    int rows, cols;
    getmaxyx(w, rows, cols);
    wattron(w, attr);
    box(w, 0, 0);
    if (title && strlen(title) > 0) {
        int tlen = strlen(title) + 4;
        int tx   = (cols - tlen) / 2;
        mvwprintw(w, 0, tx, "[ %s ]", title);
    }
    mvwaddch(w, 0,        0,        ACS_ULCORNER);
    mvwaddch(w, 0,        cols - 1, ACS_URCORNER);
    mvwaddch(w, rows - 1, 0,        ACS_LLCORNER);
    mvwaddch(w, rows - 1, cols - 1, ACS_LRCORNER);
    wattroff(w, attr);
}

/* ─── Status bar at bottom of stdscr ─────────────────────────── */
void statusbar(const char *msg, int is_error) {
    int attr = is_error ? COLOR_PAIR(CP_ERROR) | A_BOLD
                        : COLOR_PAIR(CP_SUCCESS) | A_BOLD;
    attron(attr);
    mvhline(ROWS - 1, 0, ' ', COLS);
    mvprintw(ROWS - 1, 2, " %s ", msg);
    attroff(attr);
    refresh();
}

/* ─── Animated loading bar ───────────────────────────────────── */
void animate_loading(const char *msg, int ms) {
    int w = 30, bx = (COLS - w) / 2, by = ROWS / 2;
    attron(COLOR_PAIR(CP_DIM));
    mvprintw(by - 1, (COLS - (int)strlen(msg)) / 2, "%s", msg);
    attroff(COLOR_PAIR(CP_DIM));
    for (int i = 0; i <= w; i++) {
        attron(COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
        mvhline(by, bx, ACS_BLOCK, i);
        attroff(COLOR_PAIR(CP_HIGHLIGHT) | A_BOLD);
        attron(COLOR_PAIR(CP_BORDER));
        mvhline(by, bx + i, ACS_CKBOARD, w - i);
        attroff(COLOR_PAIR(CP_BORDER));
        attron(COLOR_PAIR(CP_ACCENT) | A_BOLD);
        mvprintw(by, bx + w + 2, "%3d%%", (i * 100) / w);
        attroff(COLOR_PAIR(CP_ACCENT) | A_BOLD);
        refresh();
        napms(ms / w);
    }
}

/* ─── Format money with commas ───────────────────────────────── */
char *fmt_money(double d, char *buf) {
    char tmp[40];
    sprintf(tmp, "%.2f", d);
    char *dot     = strchr(tmp, '.');
    int   int_len = dot ? (int)(dot - tmp) : (int)strlen(tmp);

    buf[0]  = '$';
    int bi  = 1, ti = 0, cnt = 0;
    int rem = int_len % 3;
    if (rem == 0) rem = 3;

    for (; ti < int_len; ti++, bi++) {
        if (ti != 0 && cnt == rem) { buf[bi++] = ','; rem = 3; cnt = 0; }
        buf[bi] = tmp[ti];
        cnt++;
    }
    if (dot) strcpy(buf + bi, dot);
    else     buf[bi] = '\0';

    return buf;
}

/* ─── Text input field inside a window ───────────────────────── */
void input_box(WINDOW *w, int y, int x, int width, char *buf,
               int maxlen, int secret) {
    int pos = 0;
    buf[0] = '\0';
    curs_set(1);
    wattron(w, COLOR_PAIR(CP_INPUT) | A_BOLD);
    mvwhline(w, y, x, ' ', width);
    wmove(w, y, x);
    wrefresh(w);
    int ch;
    while ((ch = wgetch(w)) != '\n' && ch != '\r' && ch != KEY_ENTER) {
        if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (pos > 0) {
                pos--;
                buf[pos] = '\0';
                mvwaddch(w, y, x + pos, ' ');
                wmove(w, y, x + pos);
            }
        } else if (ch >= 32 && ch < 127 && pos < maxlen - 1) {
            buf[pos++] = (char)ch;
            buf[pos]   = '\0';
            waddch(w, secret ? '*' : (chtype)ch);
        }
        wrefresh(w);
    }
    wattroff(w, COLOR_PAIR(CP_INPUT) | A_BOLD);
    curs_set(0);
}

/* ─── Popup message box ──────────────────────────────────────── */
void msg_box(const char *title, const char *msg, int is_error) {
    int mlen = strlen(msg);
    int w    = mlen + 8 < 40 ? 40 : mlen + 8;
    int h    = 7;
    int sy   = (ROWS - h) / 2;
    int sx   = (COLS - w) / 2;
    WINDOW *pop = newwin(h, w, sy, sx);
    int attr = is_error ? COLOR_PAIR(CP_ERROR) | A_BOLD
                        : COLOR_PAIR(CP_SUCCESS) | A_BOLD;
    wbkgd(pop, COLOR_PAIR(CP_NORMAL));
    draw_border(pop, title, attr);
    wmove(pop, 2, (w - mlen) / 2);
    wattron(pop, COLOR_PAIR(CP_NORMAL) | A_BOLD);
    wprintw(pop, "%s", msg);
    wattroff(pop, COLOR_PAIR(CP_NORMAL) | A_BOLD);
    wattron(pop, COLOR_PAIR(CP_HIGHLIGHT));
    mvwprintw(pop, h - 2, (w - 12) / 2, "  [ OK ]  ");
    wattroff(pop, COLOR_PAIR(CP_HIGHLIGHT));
    wrefresh(pop);
    wgetch(pop);
    delwin(pop);
    touchwin(stdscr);
    refresh();
}

/* ─── Yes/No confirm dialog ──────────────────────────────────── */
int confirm_box(const char *msg) {
    int mlen = strlen(msg);
    int w    = mlen + 8 < 44 ? 44 : mlen + 8;
    int h    = 8;
    int sy   = (ROWS - h) / 2;
    int sx   = (COLS - w) / 2;
    WINDOW *pop = newwin(h, w, sy, sx);
    wbkgd(pop, COLOR_PAIR(CP_NORMAL));
    draw_border(pop, "Confirm", COLOR_PAIR(CP_ACCENT) | A_BOLD);
    wattron(pop, COLOR_PAIR(CP_NORMAL) | A_BOLD);
    mvwprintw(pop, 2, (w - mlen) / 2, "%s", msg);
    wattroff(pop, COLOR_PAIR(CP_NORMAL) | A_BOLD);
    int choice = 0, ch;
    while (1) {
        if (choice == 0) wattron(pop, COLOR_PAIR(CP_SELECTED) | A_BOLD);
        else             wattron(pop, COLOR_PAIR(CP_NORMAL));
        mvwprintw(pop, h - 2, w / 2 - 12, "  [ YES ]  ");
        if (choice == 0) wattroff(pop, COLOR_PAIR(CP_SELECTED) | A_BOLD);
        else             wattroff(pop, COLOR_PAIR(CP_NORMAL));

        if (choice == 1) wattron(pop, COLOR_PAIR(CP_ERROR) | A_BOLD);
        else             wattron(pop, COLOR_PAIR(CP_NORMAL));
        mvwprintw(pop, h - 2, w / 2 + 2, "  [ NO ]   ");
        if (choice == 1) wattroff(pop, COLOR_PAIR(CP_ERROR) | A_BOLD);
        else             wattroff(pop, COLOR_PAIR(CP_NORMAL));

        wrefresh(pop);
        ch = wgetch(pop);
        if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == '\t' ||
            ch == 'h'      || ch == 'l') choice ^= 1;
        else if (ch == '\n' || ch == '\r') break;
        else if (ch == 'q') { choice = 1; break; }
    }
    delwin(pop);
    touchwin(stdscr);
    refresh();
    return choice == 0;
}