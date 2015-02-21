OSHi, a minimal shell
=====================

Bonus assignment for the Operating Systems course.

##Features

Currently supports:

 * Execution of commands (optionally in background)
 * Two builtins: history and exit
 * Command substitution: use !! and !N shorthand for executing the last or N-th
   command.

##Compiling and Installation

Run `make` to compile, or `make install` to compile and install in the $out/bin
directory. If $out is not defined, it installs in the current directory. Use
`make uninstall` to uninstall the program (only works if you installed in a
directory other than the current directory).

If you use Nix, run

```
nix-env -i -f default.nix
```

to install, and

```
nix-env -e OSHi
```

to uninstall.
