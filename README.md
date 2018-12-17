kdbxmerge
=========
Keepassx2 is an awesome password manager, that I use every day. I'm using Qubes OS, which has a seperate VM for it.

I started evaluating the source code of keepassx2, and it's very well written, you can take only the parts that manage the database i/o, structure and it's contents, and seperate it completely from the gui stuff.

This is what I wanted to do for a while now, to make commandline programs that manage the .kdbx password files. Specifically I wanted to be able to merge two .kdbx files.

So what this repo provides is:
* a static library libkeepassio.a, that contains all the .kdbx database stuff.
* and a few commandline tools that work with these .kdbx files.
