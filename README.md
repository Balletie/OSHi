OSHi, a minimal shell
=====================

Bonus assignment for the Operating Systems course.

Run `make` to compile, or `make install` to compile and install in the $out/bin
directory. $out is by default the current directory, and may be overridden by
an environment variable of the same name.

If you use Nix, run

```
nix-env -i -f default.nix
```

to install, and

```
nix-env -e OSHi
```

to uninstall.
