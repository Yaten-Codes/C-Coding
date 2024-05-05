# Shell (Phase 1)

In this project, you'll build a simple shell. The shell is the heart of
the command-line interface, and thus is central to the Linux/MacOS/C programming
environment. Mastering use of the shell is necessary to become proficient in
that world; knowing how the shell itself is built is the focus of this
project.

## Overview

You will implement a *command line interpreter (CLI)* or,
as it is more commonly known, a *shell*. The shell should operate in this
basic way: when you type in a command (in response to its prompt), the shell
creates a child process that executes the command you entered; the parent process
waits for the child process to finish, then prompts
for the next command.

The shell you will implement will be similar to, but much simpler than, the one you run
every day in Linux/MacOS. If you don't know what shell you are running, it's probably
`bash` (though Apple has recently migrated to `zsh` on MacOS).
One thing you should do on your own time is learn more about your
shell, by reading the man pages or other online materials.

## Program Specifications

### Basic Operation

Your shell, called `wish` (short for _Whittier Shell_, naturally), is
an interactive loop: it repeatedly prints a prompt `wish> ` (note
the space after the greater-than sign), parses the input, executes the command
specified by the input, and waits for the command to finish. This is
repeated until the user types `exit`.  The name of your executable
should be `wish`.

The shell can be invoked with either no arguments or a single argument;
anything else is an error. Here is the zero-argument way:

```sh
$ ./wish
wish>
```

At this point, `wish` is running, and ready to accept commands. Type away!

The mode above is called *interactive* mode, and allows the user to type
commands directly. Your shell must also support a *batch mode*, which instead reads
input from a batch file and executes the commands found therein. Here is how you
run the shell with a batch file named `batch.txt`:

```sh
$ ./wish batch.txt
```

One difference between batch and interactive modes: in interactive mode, a
prompt is printed (`wish> `) when the shell is ready to read the next
command. In batch mode, no prompt should be printed.

You should structure your shell such that it creates a child process for each new
command (the exceptions are *built-in commands*, discussed below).  Your
shell should be able to parse a command and run the program corresponding to
the command.  For example, if the user types `ls -la /tmp`, your shell should
run the program `/bin/ls` with the given arguments `-la` and `/tmp`. How does
the shell know to run `/bin/ls`? It uses something called the shell **path**;
more on this below.

### Details

The basic structure of the shell is simple (conceptually): it runs in a loop, repeatedly
asking for a command. It then executes that
command. The loop continues indefinitely until the user types the built-in
command `exit`, at which point it exits. That's it!

#### Reading Input

To read a line of input, you should use `fgets()`.
Generally, the shell will be
run in *interactive mode*, where the user types a command and
the shell acts on it. However, your shell will also support *batch mode*, in
which the shell is given an input file of commands; in this case, the shell
should not read user input from `stdin`; instead, it should read the next line
of the file to get the next command to execute.

In either mode, if you hit the end-of-file marker, you should call
`exit(0)` to exit gracefully. Note that `fgets()` returns `NULL` to signal
an end-of-file condition.

To parse the input line into constituent pieces, you should use
`strtok()`. Read the man page (carefully) for more details.

#### Executing Commands

To execute commands, you will need to use `fork()`, `execv()`, and `wait()/waitpid()`.
See the man pages for these functions, and also read the relevant [book
chapter](http://www.ostep.org/cpu-api.pdf) for a brief overview.

You will note that there are a variety of commands in the `exec` family;
**for this project, you must use `execv`**. You may not use `execvp()`, nor
may you use the `system()`
library function call to run a command.  Remember that if `execv()` is
successful, it will not return; if it does return, there was an error (e.g.,
the command you tried to run does not exist).

#### The Search Path

In our example above, the user typed `ls` but the shell knew to execute the
program `/bin/ls`. How does your shell know this?

Your shell must maintain a **path** variable that describes a list
of directories to search for executables. This list of directories
is called the *search path* of the shell. The
`path` variable holds the list of all directories to search, in order, when
the user types a command.

**Important:** Note that the shell itself does not *implement* `ls` or other
commands (except built-ins). All it does is find the given executable in one of
the directories specified by `path` and create a new process to run it.

To run a program (i.e., not a built-in command), your shell should do the following:
```
call fork() to create a child process

in the child:

    for each directory in path:
        if the program exists in the directory and is executable:
            call execv() to run the program

    if we reach this point in the child, it is an error, because the
    program was not found on the search path

in the parent:

    wait for the child process to finish
```

To check if a file exists in a directory and is executable,
you should use the `access()` system call. For example, to check whether
`ls` exists in `/bin` and is executable, call `access("/bin/ls", X_OK)`.

##### Default Path
Your initial shell path should contain one directory: `/bin`.

##### Note
Most shells allow you to specify a binary specifically without using a
search path, using either **absolute paths** or **relative paths**. For
example, a user could type the **absolute path** `/bin/ls` to execute the
`ls` binary without a search path being needed. A user could also specify a
**relative path** which starts with the current working directory and
specifies the executable directly, e.g., `./main`. In this project, you **do
not** have to worry about these features.

#### Built-in Commands

Whenever your shell accepts a command, it should check whether the command is
a **built-in command** or not. If it is, it should not be executed like other
programs. Instead, your shell will invoke your implementation of the built-in
command. For example, to implement the `exit` built-in command, you simply
call `exit(0);` in your wish source code, which then will exit the shell.

In this project, you should implement three built-in comands: `exit`, `cd`, and `path`.

##### exit
When the user types `exit`, your shell should simply call the `exit()` system call
with 0 as a parameter. It is an error to pass any arguments to `exit`.

##### cd
The `cd` built-in command always takes one argument (anything else is an error).
To change directories, use the `chdir()` system call with the argument supplied
by the user; if `chdir` fails, that is also an error.

##### path
The `path` built-in command takes zero or more arguments. For example,
```sh
wish> path /bin /usr/bin . /usr/games
```
should set the shell's search path to `[/bin, /usr/bin, ., /usr/games]`.  The `path`
command should overwrite the current path with the newly specified path.  When run
with zero arguments, the `path` command should simply print the current path, with
elements separated by colons, like this:
  ```sh
  wish> path
  /bin:/usr/bin:.:/usr/games
  ```

### Program Errors

**The one and only error message.** You should print this error
message whenever you encounter an error of any type:

```c
    fprintf(stderr, "An error has occured\n");
```

The error message should be printed to `stderr` (standard error), as shown
above.

After most errors, your shell should simply *continue processing* after printing the
error message. However, if the shell is invoked with more than one file, or if
the shell is invoked with a file that does not exist or cannot be opened, it
should exit by calling `exit(1)`.

There is a difference between errors that your shell catches and those that are
caught by a program run by your shell. Your shell should catch all the syntax
errors specified in this README file. If the syntax of the command looks
perfect, you simply run the specified program. If there are any program-related
errors (e.g., invalid arguments to `ls`, for example), your shell
does not have to worry about it, because that program will print its own error
messages and exit.

### Miscellaneous Hints

First, get batch mode working. The automated tests all use batch mode to
supply input to your shell, so you will not be able to run the automated
tests until batch mode is working.

Second, try to get the basic functionality of your shell working before worrying
about all of the error conditions and edge cases. For example, first get a
single command with no arguments running (such as `ls` or `date`).

Next, add the ability to parse command lines so you can run commands
with arguments (such as `ls -l -S /usr/bin` or `hexdump -C some_file`)
At some point, you must make sure your code is robust in handling whitespace of
various kinds, including spaces (` `) and tabs (`\t`). In general, the user
must be able to surround commands and arguments with any amount of whitespace.

Finally, add built-in commands.

Check the return codes of all system calls from the very beginning of your
work. This will often catch errors in how you are invoking these new system
calls. It's also just good programming practice.

Beat up your own code!  Throw lots of different inputs at your shell and make
sure it behaves well. Good code comes through testing; you must run many
different tests to make sure things work as desired.

As in previous projects, we have provided you with a suite of automated tests that you can
run in the usual way:

```c
$ ./test-wish.sh
```

Finally, keep versions of your code. More advanced programmers will use a
source control system such as git. Minimally, when you get a piece of
functionality working, make a copy of your .c file (perhaps in a subdirectory
with a version number, such as v1, v2, etc.). By keeping older, working
versions around, you can comfortably work on adding new functionality, safe in
the knowledge you can always go back to an older, working version if necessary.
