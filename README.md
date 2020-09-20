# mcd
Command line utility to make and directory and change into it as a single command - a combo `md` and `cd`.

# What and why?
For me, when I make a directory in a command prompt window, I usually also want to change into it.

Fed up of typing `md x\y\z` and then having to also type `cd x\y\z` straight afterwards, I decided a simple utility should let me do this as a single instruction.

The general idea is that `mcd` should work just like the inbuild command tool `cd`, but that if the requested directory doesn't exist, it does a bit of `md` magic first.

Yes, it would be easy enough to do this in a batch or command file, or by some other method, but it seemed like a nice easy gadget that someone might get some use from.

# Notes
1. Syntactically, the utility will work like `md` and `cd` but of course you change that that by copying this code and making you own version
1. The best usage is to put the command file (batch file) into a utilities folder that is on your command prompt path so that you can run it just as simply as you can use the built in `cd` and `md` commands.

# Information
Displays the name of or changes the current directory. If the directory doesn't exist, it is created.

`MCD [/D] [drive:][path]`<br>
`MCD [..]`

  ..   Specifies that you want to change to the parent directory.

Type `MCD drive:` to display the current directory in the specified drive.
Type `MCD` without parameters to display the current drive and directory.

Use the `/D` switch to change current drive in addition to changing current
directory for a drive.

`MCD` does not treat spaces as delimiters, so it is possible to
create and change into a subdirectory name that contains a space without surrounding
the name with quotes.  For example:

`MCD \winnt\profiles\username\programs\start menu`

is the same as:

`MCD "\winnt\profiles\username\programs\start menu"`

Multiple adjacent spaces will be condensed down to one, which is not precisely
the same behaviour as `CD` with extensions enabled, but close enough for most uses. For
multiple adjecent spaces, surround the path with quotes.

# Implementation note
Initially, this was to be a C++ project delivering an executable, but I overlooked the obvious - that changes to the environment, such as current working directory, do not persist beyond the execution lifetime of the executable.

In fact, my only implementation option is to fall back to using the native CD and MD commands built into the console. Even then, much as I'd like to use SETLOCAL and ENDLOCAL to allow me to make other changes and then have them automatically reverted, using SETLOCAL would again mean that the change of working directory would not persist.

So, here we are. The best way to achieve MCD is to do "MD" and then do "CD", just like we were doing by hand - but at least we now have it was a batch file which, yes, you could easily write yourself, but you didn't, did you, and neither did I, until now.
