#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Maximum buffer size
#define MAX_BUFFER_SIZE 5000

// Function to correct the capitalization in a string
void correctCapitalization(char *str)
{
    int newSentence = 1;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (newSentence && str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] -= 'a' - 'A'; // Convert the first letter of a sentence to uppercase
            newSentence = 0;
        }

        // Set the flag for a new sentence if a period (.) is found followed by a space or end of string,
        // or if a newline character is found
        if ((str[i] == '.' && (str[i + 1] == ' ' || str[i + 1] == '\0')) || str[i] == '\n')
        {
            newSentence = 1;
        }
    }
}

// Function to search
void search(const char *text, const char *term)
{
    const char *p = text;

    while ((p = strstr(p, term)) != NULL)
    {
        // Calculate the position of the found term
        int pos = p - text;

        // Move the cursor to the position of the found term
        move(pos / COLS, pos % COLS);

        // Highlight the found term
        attron(A_REVERSE);
        printw("%s", term);
        attroff(A_REVERSE);

        // Move the pointer past the found term
        p += strlen(term);
    }
}

// Function to open a file and display its content
void openFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printw("Failed to open the file. Check if the file exists and you have read permissions.\n");
        return;
    }

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        addch(c); // Display the content of the file
    }

    fclose(file);
}

// Function to save content to a file
void saveFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printw("Failed to open the file for writing. Check if you have write permissions.\n");
        return;
    }

    fputs(content, file); // Write the content to the file

    fclose(file);
    printw("File saved successfully.\n");
}

// Main function
// Main function
int main(int argc, char *argv[])
{
    initscr();            // Start curses mode
    raw();                // Line buffering disabled
    keypad(stdscr, TRUE); // We get F1, F2, etc.
    noecho();             // Don't echo() while we do getch

    char filename[100]; // Variable to store the filename
    if (argc > 1)
    {
        strncpy(filename, argv[1], sizeof(filename) - 1); // Copy the filename from command line arguments
        filename[sizeof(filename) - 1] = '\0';
    }
    else
    {
        printw("Enter the filename to open (include the extension, e.g., 'file.txt'): ");
        echo();           // Enable echoing of typed characters
        getstr(filename); // Get the filename from user input
        noecho();         // Disable echoing of typed characters
    }

    openFile(filename); // Open and display the content of the file

    printw("\nStart typing. Enter ':wq' on a new line to save and exit, or ':q' to exit without saving:\n");

    char input[MAX_BUFFER_SIZE] = "";
    int input_len = 0;
    int ch;

    // Main loop to get user input and handle commands
    while ((ch = getch()) != EOF)
    {
        if (input_len >= 2 && strncmp(input + input_len - 2, ":/", 2) == 0) // Check if the user entered ':/term' to search for 'term'
        {
            echo(); // Enable echoing of typed characters

            // Get the search term from user input
            char term[MAX_BUFFER_SIZE];
            getstr(term);

            noecho(); // Disable echoing of typed characters

            // Search for the term in the input
            search(input, term);

            continue; // Skip the rest of the loop iteration
        }
        else if (ch == '\n') // If the user enters a newline
        {
            addch(ch);               // Display the newline
            input[input_len++] = ch; // Add the newline to the input

            // Check if the user entered ':wq' to save and quit
            if (input_len >= 4 && strcmp(input + input_len - 4, ":wq\n") == 0)
            {
                input[input_len - 4] = '\0'; // Remove the ':wq\n' from the input
                break;                       // Break the loop to save and quit
            }
            // Check if the user entered ':q' to quit without saving
            else if (input_len >= 3 && strcmp(input + input_len - 3, ":q\n") == 0)
            {
                endwin(); // End curses mode
                return 0; // Exit the program
            }
        }
        else if (ch == erasechar()) // If the user enters the erase character
        {
            // If there is input to erase
            if (input_len > 0)
            {
                input[--input_len] = '\0'; // Erase the last character from the input
                int y, x;
                getyx(stdscr, y, x); // Get the current cursor position
                move(y, x - 1);      // Move the cursor back one position
                delch();             // Delete the character at the cursor position
            }
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) // If the user enters the backspace key
        {
            // If there is input to backspace
            if (input_len > 0)
            {
                input[--input_len] = '\0'; // Backspace the last character from the input
                int y, x;
                getyx(stdscr, y, x); // Get the current cursor position
                move(y, x - 1);      // Move the cursor back one position
                delch();             // Delete the character at the cursor position
            }
        }
        else // If the user enters any other character
        {
            // If there is space in the input buffer
            if (input_len < MAX_BUFFER_SIZE - 1)
            {
                addch(ch);               // Display the character
                input[input_len++] = ch; // Add the character to the input
            }
        }
    }

    endwin(); // End curses mode

    correctCapitalization(input); // Correct the capitalization in the input
    saveFile(filename, input);    // Save the input to the file

    return 0;
}
