For basic gameloop in C, and also basic game code.

There are two files, and "program.c" is code for the game snake.

It's to be compiled in Linux with gcc program.c -lncursesw

The library lncurses can be installed on:

Debian/Ubuntu Linux
CentOS/RHEL/Scientific Linux 6.x/7.x+ and Fedora Linux 21 or older Fedora
Linux 22.x+
...And maybe other distros

If you don't have the library, here is how it can be installed

Debian/Ubuntu Linux
You need to install the following two packages: libncurses5-dev : Developer’s libraries for ncurses libncursesw5-dev : Developer’s libraries for ncursesw Open the Terminal application. Type the following apt-get command to install ncurses header and libs: sudo apt-get install libncurses5-dev libncursesw5-dev

CentOS/RHEL/Scientific Linux 6.x/7.x+ and Fedora Linux 21 or older
You need to install the following package: ncurses-devel : Developer's libraries for ncurses Open the Terminal application. Type the following yum command to install ncurses header and libs: sudo yum install ncurses-devel

Fedora Linux 22.x+
You need to install the following package: ncurses-devel : Developer's libraries for ncurses Open the Terminal application. Type the following dnf command to install ncurses header and libs: sudo dnf install ncurses-devel

Source of text: https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/
