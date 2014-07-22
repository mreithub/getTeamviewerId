teamviewer-getId
================

Simple program to extract the teamviewer ID of a running instance using the Win32 API.

There's also a bash script that runs the app in the same wine server and should therefore work on linux.

Compilation
-----------

Simply run

    make

That creates the file `teamviewer-getid.exe` which is needed for the `teamviewer-getid.sh` script to
work.

Execution
---------

    $ ./teamviewer-getid.sh
    123 456 789

Requirements
------------

* mingw32 (on the development machine)
* TeamViewer has to be installed and running on the target PC
* While I've only tested this app on Linux, it should also run on Windows (just start the .exe from the command line)
