kdbxmerge
=========
[Keepassx2](https://github.com/keepassx/keepassx) is an awesome password manager, that I use every day. I'm using Qubes OS, which has a seperate VM for it.

I started evaluating the source code of keepassx2, and it's very well written, you can take only the parts that manage the database i/o, structure and it's contents, and seperate it completely from the gui stuff.

This is what I wanted to do for a while now, to make commandline programs that manage the .kdbx password files. Specifically I wanted to be able to merge two .kdbx files.

So what this repo provides is:
* a static library libkeepassio.a, that contains all the Keepassx2 database stuff, so you can create your own kdbx related tools, and/or explore the keepassx logic.
* and a few commandline tools that work with these .kdbx files.

As of now, there are two tools:
* kdbxmerge : merges two or more kdbx files into a new one
* kdbxfgrep : searches the kdbx for entries a search term and display the result as a list. It can optionally print passwords, which could be useful if you don't have X11 available.



Building under fedora
---------------------
You need to install:
```
sudo dnf -y install qt5-devel libgcrypt-devel zlib-devel
```
Fix the qt5 tool filenames:
```
cd /usr/bin
sudo ln -s qmake-qt5 qmake
sudo ln -s moc-qt5 moc
sudo ln -s uic-qt5 uic
sudo ln -s lrelease-qt5 lrelease
sudo ln -s lupdate-qt5 lupdate
cd -
```
Then we proceed as normal:
```
./configure
make
sudo make install
```
Building under debian
---------------------
Install the dependencies:
```
sudo apt-get -y install zlib1g-dev libgcrypt-dev qt5-default
```
And build with:
```
./configure && make && sudo make install
```
