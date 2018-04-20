# pfmt

A command line utility and library for setting text attributes.

See `examples/` for examples.

## Build and Install
To install both library and program

`make ; make install`

To install just the dynamic library

`make lib ; make install-lib`

To install just the program

`make pfmt ; make install-exe`

## CLI Usage
`pfmt [-x|--xpos <xpos> -y|--ypos <ypos>] [-f|--fg <fg-color>] [-b|--bg <bg-color>] [-c|--clear] [-k|--kill-line] [strings...]`

If `strings...` is not supplied, input will be read from stdin.

### -x|--xpos -y|--ypos
The x and y position for the start of the text, starting at 0 from
the top left of the screen.

### -f|--fg and -b|--bg
The foreground/background color to be used, in 24-bit hexadecimal format.
For example, `ff0000` is red, `00ff00` is green, and `0000ff` is blue.

### -c|--clear
Clear the screen before printing.

### -k|--kill-line
Kill the line before printing.

### strings...
A list of arguments to be printed with the specified attributes.
