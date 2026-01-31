# wisdomkey

Small accessibility program for quickly moving the mouse to a desired position
on the screen, using only the arrow keys and ESC/RET, optionally even using only
three keys.

[Showcase](https://github.com/user-attachments/assets/0abb010f-fa28-4374-b017-e6eb7f3b5dd0)

## Usage

Use arrow keys to split the screen horizontally or vertically in a specified
direction, and press ENTER or ESCAPE to close the program.

In three key mode, use LEFT and RIGHT to split the window, according to the help
messages (if enabled), and use DOWN to close the program. ESCAPE and ENTER are
still available in three key mode.

The program should automatically capture input.

## Setting up

The program is designed to be opened when needed, and closed immediately after
done, possibly many times in a short period.

Three key mode and it's associated help option can be enabled/disabled by
changing the variables and recompiling. A dotfile configuration will probably be
added at some point.

The expected usage is to bind program startup to some key or key combination.
Furthermore, providing the program with **l**, **r**, **u** or **d** as command
line arguments will start the program with an initial split.

Here is an example i3 configuration:
```
bindsym Shift+Left exec --no-startup-id wisdomkey l
bindsym Shift+Right exec --no-startup-id wisdomkey r
bindsym Shift+Up exec --no-startup-id wisdomkey u
bindsym Shift+Down exec --no-startup-id wisdomkey d
```
(not sure if --no-startup-id should or should not be there)

## Building

The program only has SDL3 as it's dependency. You are expected to have it
installed as a shared library, which can be done on Debian derived systems with:
```bash
sudo apt install libsdl3-dev
```
If you wish to use static libraries, for example on Windows, add SDL3 as a
vendored library, which an example for can be found in the offical SDL3
documentation. 
