## Base yotta Target Description for Compiling with armcc 5

This is a base [yotta target
description](http://docs.yottabuild.org/tutorial/targets.html) for compiling
mbed OS with the arm compiler version 5. Other target descriptions can inherit
from it and add or override things as necessary (such as the link script, or
preprocessor definitions).

You should not select this target to compile with directly (compilation will
probably not succeed without target-specific startup code).

See [CHANGELOG.md](CHANGELOG.md) for the changes associated with
each version.
