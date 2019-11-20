### 1.1.0
 * Support for the future deprecation of `cmake_force_xxx_compiler`.

### 1.0.1
 * Fix duplicate C++ flag which prevented C++11 support from being correctly
   enabled.

## 1.0.0
 * Switch on C++11 compilation by default. This is technically a breaking
   change, but the mbed-OS modules themselves have been patched so that they
   continue to work correctly.
 * armcc >= 5.05 is now required. An error message is displayed if an older
   version is detected.

## 0.1.3
 * add config for mbed default baud rate

## 0.1.2
 * fix dependency tracking

## 0.1.1
 * update for compatibility with CMake >= 3.4.0

## 0.1.0
 * add `<builddir>/generated/include` to the header saerch paths: this is the
   canonical place to put generated headers (prefixed by module name to avoid
   clashes)
 * suppress duplicate input file warnings from the linker

## 0.0.16
For this and prior releases, no changes were tracked. Please see the git
history

