# wisdomkey

Small accessibility program for quickly moving the mouse to a desired position
on the screen, using only the arrow keys.

## Usage

Use arrow keys to split the screen horizontally or vertically in a specified
direction, and press ENTER or ESCAPE to close the program.

The program should automatically capture input.

## Setting up

The program is designed to be opened when needed, and closed immediately,
possibly many times in a short duration.

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
