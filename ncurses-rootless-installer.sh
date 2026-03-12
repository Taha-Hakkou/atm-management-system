# ===============================
# c ncurses rootless installation
# ===============================


# 1. Download ncurses source (latest)
mkdir ~/src
cd ~/src
wget https://invisible-island.net/datafiles/release/ncurses.tar.gz
tar -xzf ncurses.tar.gz
cd ncurses-*

# 2. configure local installation (rootless)
mkdir -p $HOME/local # everything installs into it
# mkdir -p $HOME/src
./configure --prefix=$HOME/local \
            --with-shared \
            --without-debug \
            --without-ada \
            --enable-widec \
            --enable-pc-files \
            --with-pkg-config-libdir=$HOME/local/lib/pkgconfig
# What this gives you:
#    shared libs (.so)
#    UTF-8 support (ncursesw)
#    pkg-config files in your prefix










# 5. env variables
export PATH=$HOME/local/bin:$PATH
export LD_LIBRARY_PATH=$HOME/local/lib:$HOME/local/lib64 # :$LD_LIBRARY_PATH -> only if has previous value!
export PKG_CONFIG_PATH=$HOME/local/lib/pkgconfig # :$PKG_CONFIG_PATH
# export CPATH=$CPATH:$HOME/local/include




# 4. Build & Install
make -j$(nproc) # flag not necessary ?!
make install



# Some systems need this symlink:

# cd $HOME/local/lib
# ln -s libncursesw.so libncurses.so




# 6. Reload
source ~/.zshrc    # or ~/.bashrc


# 7. Verify installation
## Check pkg-config
pkg-config --modversion ncursesw
pkg-config --cflags ncursesw
pkg-config --libs ncursesw

## Check library presence
ls $HOME/local/lib | grep ncurses
	# You should see:
	#    libncursesw.so
	#    libncursesw.a


# 8. Cleanup