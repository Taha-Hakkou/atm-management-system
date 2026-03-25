#!/bin/bash
# ===============================
# ncurses rootless installation
# ===============================
set -e  # stop immediately on any error

# ── Step 1: Auto-detect shell rc file ─────────────────────────────
# NOT VALID CONDITIONS
# if [ -n "$ZSH_VERSION" ]; then
#     RC_FILE="$HOME/.zshrc"
# elif [ -n "$BASH_VERSION" ]; then
#     RC_FILE="$HOME/.bashrc"
# else
#     RC_FILE="$HOME/.profile"   # fallback for sh/other shells
# fi
RC_FILE="$HOME/.zshrc"
touch "$RC_FILE"   # create if it doesn't exist
echo "Detected shell rc file: $RC_FILE"

# ── Step 2: Export env vars FIRST (needed during build) ───────────
export PATH="$HOME/local/bin:$PATH"
export LD_LIBRARY_PATH="$HOME/local/lib:$HOME/local/lib64"
export PKG_CONFIG_PATH="$HOME/local/lib/pkgconfig"

# ── Step 3: Save env vars to rc file so they persist ──────────────
# $RC_FILE is unquoted-tilde-safe — uses $HOME instead of ~
grep -qxF 'export PATH="$HOME/local/bin:$PATH"' "$RC_FILE" || \
    echo 'export PATH="$HOME/local/bin:$PATH"' >> "$RC_FILE"

grep -qxF 'export LD_LIBRARY_PATH="$HOME/local/lib:$HOME/local/lib64"' "$RC_FILE" || \
    echo 'export LD_LIBRARY_PATH="$HOME/local/lib:$HOME/local/lib64"' >> "$RC_FILE"

grep -qxF 'export PKG_CONFIG_PATH="$HOME/local/lib/pkgconfig"' "$RC_FILE" || \
    echo 'export PKG_CONFIG_PATH="$HOME/local/lib/pkgconfig"' >> "$RC_FILE"

# ── Step 4: Download ncurses source ───────────────────────────────
mkdir -p "$HOME/src"
cd "$HOME/src"
wget -N https://invisible-island.net/datafiles/release/ncurses.tar.gz
tar -xzf ncurses.tar.gz
cd ncurses-*/

# ── Step 5: Configure ─────────────────────────────────────────────
mkdir -p "$HOME/local"
./configure \
    --prefix="$HOME/local" \
    --with-shared \
    --without-debug \
    --without-ada \
    --enable-widec \
    --enable-pc-files \
    --with-pkg-config-libdir="$HOME/local/lib/pkgconfig"
# --with-shared        → build shared libs (.so)
# --enable-widec       → UTF-8 / wide character support (ncursesw)
# --enable-pc-files    → generate pkg-config .pc files
# --without-ada        → skip Ada bindings (not needed)
# --without-debug      → skip debug build

# ── Step 6: Build & Install ───────────────────────────────────────
make -j"$(nproc)"
make install

# ── Step 7: Create symlinks ───────────────────────────────────────
# needed so -lncurses resolves to the wide lib when compiling
cd "$HOME/local/lib"
[ ! -f libncurses.so ] && ln -s libncursesw.so libncurses.so
[ ! -f libncurses.a  ] && ln -s libncursesw.a  libncurses.a

# ── Step 8: Verify ────────────────────────────────────────────────
echo ""
echo "=== Verify installation ==="
pkg-config --modversion ncursesw && echo "pkg-config  : OK"
ls "$HOME/local/lib" | grep ncurses
echo ""
echo "=== Done! ==="
echo "Run: source $RC_FILE"
# you must run source yourself — sourcing inside a script
# only affects the script process, not your terminal session