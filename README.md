# pfmt

A command line utility and library for setting text attributes.

## Build and Install
To install both library and program

`make ; make install`

To install just the dynamic library

`make lib ; make install-lib`

To install just program
`make pfmt ; make install-exe`

## CLI Usage
`pfmt [-x <xpos> -y <ypos>] [-f <fg-color>] [-b <bg-color>] [-c] [strings...]`

### -x <xpos> -y <ypos>
The x and y position for the start of the text, starting at 0 from
the top left of the screen.

### -f <fg-color> and -b <bg-color>
The foreground/background color to be used, in 24-bit hexadecimal format.
For example, `ff0000` is red, `00ff00` is blue, and `0000ff` is red.

### -c
Clear the screen before printing.

### strings...
A list of arguments to be printed with the specified attributes.
