# mcd
Command line executable to make and directory and change into it as a single command - a combo `md` and `cd`.

# What and why?
For me, when I make a directory in a command prompt window, I usually also want to change into it.

Fed up of typing `md x\y\z` and then having to also type `cd x\y\z` straight afterwards, I decided a simple utility should let me do this as a single instruction.

The general idea is that `mcd` should work just like the inbuild command tool `cd`, but that if the requested directory doesn't exist, it does a bit of `md` magic first.

Yes, it would be easy enough to do this in a batch or command file, or by some other method, but it seemed like a nice easy gadget that someone might get some use from.

# Notes
1. This will be a basic C++ application written using Visual Studio
1. This will be written for wide character sets (`wchar_t`, rather than `char`), but should be fairly easy to convert back to a non-wide version
1. Syntactically, the utility will work like `cd` but of course you change that that by copying this code and making you own version
1. Built executables will be provided as a GitHub Release
1. The best usage is to put the appropriate executable into a utilities folder that is on your command prompt path so that you can run it just as simply as you can use the built in `cd` and `md` commands.