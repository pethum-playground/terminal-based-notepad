# Text Editor with ncurses

This project is a simple text editor implemented in C using the ncurses library. It provides basic text editing features, as well as a search function and automatic sentence capitalization correction.

## Features

* Open a file and display its content in a terminal-based user interface
* Edit the content of the file
* Save the changes back to the file
* Search for a term in the file and highlight all occurrences
* Correct the capitalization of sentences in the file

## Requirements

* A Unix-like environment
* The ncurses library
* A standard C compiler

## Compilation

The project can be compiled with the following command:

`gcc -o editor main.c -lncurses`

## Usage

Run the program with the following command:

`./editor [filename]`

If no filename is provided, the program will prompt for one.

Once the program is running, you can start typing to edit the file. The following commands are available:

* :wq - Save the changes and exit the program
* :q - Exit the program without saving
* :s - Search for a term in the file

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.txt) file for details.
