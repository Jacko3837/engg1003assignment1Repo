/**
==========================================================================================

ENGG1003 - Programming Assignment 1 English Text Ciphers
Submission for Student (ID): c3355432

This program was made to fulfill the following tasks:
- Encryption of a message using the classical rail-fence cipher
- Encryption of a message using the 2-level rail-fence cipher
- Decryption of a message using the 2-level rail-fence cipher
- Partial decryption of text encrypted with a substitution cipher without a key
Read text block above main() function for details on use.

==========================================================================================
**/

//==INCLUDED LIBRARIES==
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
//======================

//==FUNCTION PROTOTYPES==
void railFence(char *message, char *cipherText, int length, int A);
void railFence2(char *message, char *cipherText, int length, int A, int B, int dir);
void substitution(char *message, char *cipherText, char *key);
void fetchUserMessage(char *userInput);
void menu(int option, char *input, int A, int B);
int fLength(int length, int A, int B);
//=======================


/*
==========================================================================================
The main function manages the menu and allows the user to select their desired process,
    in which it then calls the respective cipher function to perform said task.

///In the case of command-line arguments being used, the syntax is as follows:
*//**

<EXECUTABLE> <INPUT FILE> <OUTPUT FILE>

**//*
Syntax: <> - Necessary; [] - Optional.
EXECUTABLE - This program
INPUT FILE - The input file path, formatted correctly
OUTPUT FILE - The output file path (does not need to exist)

File Format:
*//**

<OPERATION><\n>
[A [B]]<\n>
<CONTENT>

**//*
Syntax: <> - Necessary; [] - Optional.
OPERATION - Number from 1-4, where:
        1 = Classical Rail-Fence Encryption
        2 = 2-Level Rail-Fence Encryption
        3 = 2-Level Rail-Fence Decryption
        4 = Substitution Decryption (No key)
A - Used in operations 1, 2 & 3. Denotes the number of "rails"
B - Used in operation 2 & 3. Denotes peak of the point midway between top rail points
CONTENT - The content to be encrypted or decrypted. NOTE: The character '~' should not be
            present in the text.

///In the case of no command-line arguments being used, a user interface including a menu
    will appear (Note: Files are not supported in this format)

------------------------------------------------------------------------------------------
DETAILS:
int     {argc}          number of arguements provided during execution
char*[] {argv}          arguements provided during execution
int     {return}        returns status of execution (0 = good; 1 = bad)
==========================================================================================
*/
int main(int argc, char *argv[]) {
    if (argc == 3) {
        ///COMMAND LINE ARGUMENT INPUT (USES FILES)
        //initialise operation, a and b
        int operation, A, B;
        //opens files; prepares for [message] and [cipherText] initialisation
        FILE *input;
        FILE *output;
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
        //Checks if input file exists:
        if (!input) {
            printf("INPUT FILE DOES NOT EXIST!\n");
            exit(0);
        }
        //sets the length of the input and output variables to file input size
        fseek(input, 0, SEEK_END);
        int fileLength = ftell(input);
        char inputMsg[fileLength];
        fseek(input, 0, SEEK_SET);
        //variables for reading the input file
        char c;
        int count = 0; //used to collect file characters
        int flag = -1; //to keep track of file format area
        int ab = 1; //flag to tell if B exists
        //reads from file input, cycles through everything to find <operation>,<key>,&<content>
        while (!feof(input)) {
            c = fgetc(input);
            if (flag > 0) {
                //reads <content>
                inputMsg[count++] = c;
            } else if (c == '\n') {
                //starts reading new section <>s
                flag++;
                count = 0;
            } else if (flag == -1) {
                //reads operation
                inputMsg[count++] = c;
                inputMsg[count] = '\0';
                operation = atoi(inputMsg);
            } else if (c != ' ' && ab) {
                //reads A
                inputMsg[count++] = c;
                inputMsg[count] = '\0';
                A = atoi(inputMsg);
            } else if (ab && c == ' ') {
                //determines if B exists
                ab = 0;
                count = 0;
            } else {
                //reads B
                inputMsg[count++] = c;
                inputMsg[count] = '\0';
                B = atoi(inputMsg);
            }
        };
        inputMsg[count - 1] = '\0';
        printf("\nInput:\n%s\n", inputMsg);
        //set output size now that we know the length needed:
        char outputMsg[fileLength + A];
        //calls the user selected function
        switch (operation) {
            case 1:
                //check validity of a
                if (A < 2) {
                    system("cls");
                    printf("Invalid arguement(s). Try again...\n");
                    exit(0);
                }
                //inputMsg is the message to be encrypted to cipherText, outputMsg
                railFence(&inputMsg, &outputMsg, strlen(inputMsg), A);
                break;
            case 2:
                //check validity of a and b
                if (B < 2 || A <= B) {
                    system("cls");
                    printf("Invalid arguement(s). Try again...\n");
                    exit(0);
                }
                //inputMsg is the message to be encrypted to cipherText, outputMsg
                railFence2(&inputMsg, &outputMsg, strlen(inputMsg), A, B, 0);
                break;
            case 3:
                //check validity of a and b
                if (B < 2 || A <= B) {
                    system("cls");
                    printf("Invalid arguement(s). Try again...\n");
                    exit(0);
                }
                //inputMsg is the message to be encrypted to cipherText, outputMsg
                railFence2(&outputMsg, &inputMsg, strlen(inputMsg), A, B, 1);
                break;
            case 4: ;
                //inputMsg is the message to be encrypted to cipherText, outputMsg
                char key[27]; //key to print
                key[26] = '\0';
                char *alphabet = "abcdefghijklmnopqrstuvwxyz";
                substitution(&outputMsg, &inputMsg, &key);
                //print key
                printf("\nKEY:\n%s\nTO\n%s\n", alphabet, key);
                break;
            default:
                printf("Invalid first argument. Enter int <1-4>.\nRead documentation for function information.");
        }
        //writes to file output
        fputs(outputMsg, output);
        printf("\nOutput:\n%s\n", outputMsg);
        //closes files cause that's a good idea
        int fclose(FILE *input);
        int fclose(FILE *output);
        ///========================================
    } else if (argc == 1) {
        ///USER INTERFACE (NO FILES)
        //variables that menu will use
        char input[8193];//, output[8193]; -> Declare output[] after A has been fetched
        int A = 0, B = 0, userInput, userInput2;
        //Loops to make sure the user stays in the program until they want to exit
        menu(1, &input, A, B);
        do {
            fflush(stdin); //makes sure a previous choice doesn't accidentally get carried over
            scanf("%d", &userInput);
            switch (userInput) {
                case 0:
                    //QUITS
                    system("cls");
                    printf("Exiting...\n");
                    break;
                case 1:
                    //CLASSICAL RAIL-FENCE ENCRYPTION
                    menu(2, &input, A, B);
                    do {
                        fflush(stdin); //makes sure a previous choice doesn't accidentally get carried over
                        scanf("%d", &userInput2);
                        switch (userInput2) {
                            case 0:
                                //Goes back to main menu
                                menu(1, &input, A, B);
                                userInput2 = 0;
                                break;
                            case 1:
                                //Gets the input message, terminating character '~'
                                system("cls");
                                printf("Message to encrypt (End with '~'): ");
                                fetchUserMessage(&input);
                                menu(2, &input, A, B);
                                break;
                            case 2:
                                //Gets the value of A
                                system("cls");
                                printf("Value of A (A > 1): ");
                                scanf("%d", &A);
                                menu(2, &input, A, B);
                                break;
                            case 9:
                                //Attempts to run the cipher if
                                //  all arguments have been provided
                                if (A > 1 && strlen(input) > 0) {
                                    //Declares output since it now has the potential needed buffer length (A):
                                    char output[8192 + A];
                                    //Calls function to encrypt input into output
                                    railFence(&input, &output, strlen(input), A);
                                    //Prints output
                                    system("cls");
                                    printf("Input Message: %s\n\n", input);
                                    printf("Input A: %d\n\n", A);
                                    printf("Output Message: %s\n\n", output);
                                    //Pause, then return to main menu
                                    system("pause");
                                    menu(1, &input, A, B);
                                    //Allows loop to exit to main menu
                                    userInput2 = 0;
                                } else {
                                    printf(">> Invalid arguement(s). Try again...\n>");
                                }
                                break;
                            default:
                                printf(">> Invalid input. Try again...\n>");
                        }
                    } while (userInput2 != 0);
                    break;
                case 2:
                    //2-LEVEL RAIL-FENCE ENCRYPTION
                    menu(3, &input, A, B);
                    do {
                        fflush(stdin); //makes sure a previous choice doesn't accidentally get carried over
                        scanf("%d", &userInput2);
                        switch (userInput2) {
                            case 0:
                                //Goes back to main menu
                                menu(1, &input, A, B);
                                userInput2 = 0;
                                break;
                            case 1:
                                //Gets the input message, terminating character '~'
                                system("cls");
                                printf("Message to encrypt (End with '~'): ");
                                fetchUserMessage(&input);
                                menu(3, &input, A, B);
                                break;
                            case 2:
                                //Gets the value of A
                                system("cls");
                                printf("Value of A (A > 2): ");
                                scanf("%d", &A);
                                menu(3, &input, A, B);
                                break;
                            case 3:
                                //Gets the value of A
                                system("cls");
                                printf("Value of B (A > B and B > 1): ");
                                scanf("%d", &B);
                                menu(3, &input, A, B);
                                break;
                            case 9:
                                //Attempts to run the cipher if
                                //  all arguments have been provided
                                if (A > B && B > 1 && strlen(input) > 0) {
                                    //Declares output since it now has the potential needed buffer length (A):
                                    char output[8192 + A];
                                    //Calls function to encrypt input into output
                                    railFence2(&input, &output, strlen(input), A, B, 0);
                                    //Prints output
                                    system("cls");
                                    printf("Input Message: %s\n\n", input);
                                    printf("Input A: %d\n\n", A);
                                    printf("Input B: %d\n\n", B);
                                    printf("Output Message: %s\n\n", output);
                                    //Pause, then return to main menu
                                    system("pause");
                                    menu(1, &input, A, B);
                                    //Allows loop to exit to main menu
                                    userInput2 = 0;
                                } else {
                                    printf(">> Invalid arguement(s). Try again...\n>");
                                }
                                break;
                            default:
                                printf(">> Invalid input. Try again...\n>");
                        }
                    } while (userInput2 != 0);
                    break;
                case 3:
                    //2-LEVEL RAIL-FENCE DECRYPTION
                    menu(4, &input, A, B);
                    do {
                        fflush(stdin); //makes sure a previous choice doesn't accidentally get carried over
                        scanf("%d", &userInput2);
                        switch (userInput2) {
                            case 0:
                                //Goes back to main menu
                                menu(1, &input, A, B);
                                userInput2 = 0;
                                break;
                            case 1:
                                //Gets the input message, terminating character '~'
                                system("cls");
                                printf("Message to decrypt (End with '~'): ");
                                fetchUserMessage(&input);
                                menu(4, &input, A, B);
                                break;
                            case 2:
                                //Gets the value of A
                                system("cls");
                                printf("Value of A (A > 2): ");
                                scanf("%d", &A);
                                menu(4, &input, A, B);
                                break;
                            case 3:
                                //Gets the value of A
                                system("cls");
                                printf("Value of B (A > B and B > 1): ");
                                scanf("%d", &B);
                                menu(4, &input, A, B);
                                break;
                            case 9:
                                //Attempts to run the cipher if
                                //  all arguments have been provided
                                if (A > B && B > 1 && strlen(input) > 0) {
                                    //Declares output since we now know it's needed length:
                                    char output[8192];
                                    //Calls function to decrypt input into output
                                    railFence2(&output, &input, strlen(input), A, B, 1);
                                    //Prints output
                                    system("cls");
                                    printf("Input Message: %s\n\n", input);
                                    printf("Input A: %d\n\n", A);
                                    printf("Input B: %d\n\n", B);
                                    printf("Output Message: %s\n\n", output);
                                    //Pause, then return to main menu
                                    system("pause");
                                    menu(1, &input, A, B);
                                    //Allows loop to exit to main menu
                                    userInput2 = 0;
                                } else {
                                    printf(">> Invalid arguement(s). Try again...\n>");
                                }
                                break;
                            default:
                                printf(">> Invalid input. Try again...\n>");
                        }
                    } while (userInput2 != 0);
                    break;
                case 4:
                    //SUSBTITUTION DECRYPTION
                    menu(5, &input, A, B);
                    do {
                        fflush(stdin); //makes sure a previous choice doesn't accidentally get carried over
                        scanf("%d", &userInput2);
                        switch (userInput2) {
                            case 0:
                                //Goes back to main menu
                                menu(1, &input, A, B);
                                userInput2 = 0;
                                break;
                            case 1:
                                //Gets the input message, terminating character '~'
                                system("cls");
                                printf("Message to decrypt (End with '~'): ");
                                fetchUserMessage(&input);
                                menu(5, &input, A, B);
                                break;
                            case 9:
                                //Attempts to run the cipher if
                                //  all arguments have been provided
                                if (strlen(input) > 0) {
                                    //Declares output since we now know no buffer length is needed:
                                    char output[8192];
                                    //The key, to be printed
                                    char key[27];
                                    key[26] = '\0';
                                    char *alphabet = "abcdefghijklmnopqrstuvwxyz";
                                    //Calls function to decrypt input into output
                                    substitution(&output, &input, &key);
                                    //Prints output
                                    system("cls");
                                    printf("Input Message: %s\n\n\n", input);
                                    printf("KEY:\n%s\nTO\n%s\n\n\n", alphabet, key);
                                    printf("Output Message: %s\n\n\n", output);
                                    //Pause, then return to main menu
                                    system("pause");
                                    menu(1, &input, A, B);
                                    //Allows loop to exit to main menu
                                    userInput2 = 0;
                                } else {
                                    printf(">> Invalid arguement(s). Try again...\n>");
                                }
                                break;
                            default:
                                printf(">> Invalid input. Try again...\n>");
                        }
                    } while (userInput2 != 0);
                    break;
                default:
                    printf(">> Invalid input. Try again...\n>");
            }
        } while (userInput != 0);
        ///=========================
    } else {
        printf("There was an error executing the program, inavild number of arguements. Read documentation for correct syntax.");
    }

    return 0;
}


/*
==========================================================================================
This generates the "fake length" of the cipher text.
This length is needed to ensure that the cipher always ends on the top rail,
    in the case where [length] does not need extra characters, [fakeLength] = [length]

[fakeLength] is calculated by finding how many [waves] length covers and adding an
    extra wave if it does not end on the top rail.
[waves] is calculated by dividing [length] by length of a wave (from top rail to top).
    NB: The length of a wave in this context is 1 less then the actual length of the wave
        [(A - 1) * 2]. This is because waves share the same start and end points.
        Therefore, at the end we add 1 to the number of points (see return line).

///EXAMPLE OF WAVE LENGTH:
 |0|1|2|3|4|5|6|7|8|
-
0 o       o       o
-
~   o   o   o   o
-
A     o       o
-
///The first wave is from 0-4 (x-axis). Therefore a length of 5?
///The second wave is from 4-8 (x-axis). There a length of 5?
///THEREFORE, the length of two waves must be 10? HOWEVER, we can see this is false.
///The reason for this is the two waves share a point. So, the actual length is 10-1=9.
///We can also use [A] to calculate the length of a wave. 1 Wave equals (A - 1) * 2 + 1.
///The length of two waves is (A - 1) * 2 * 2 + 1.
>THEREFORE, the length of N waves is [2*N*(A - 1) + 1], where N is the number of waves.

///SIMILARLY, for any [B] we have (A - 1)*2 + (B - 1)*2 + 1.
>THEREFORE, [fakeLength] = [N*2*((A - 1) + (B - 1)) + 1], where N is the number of waves.

*Don't like the mathematical proof? Deal with it, it's easier to explain like this :P

------------------------------------------------------------------------------------------
DETAILS:
int     {length}        length of message
int     {A}             number of rails ("rows") needed for the cipher
int     {B}             extra value that forms "bumps" in the cipher
int     {return}        returns the length for a "complete" set of waves
==========================================================================================
*/
int fLength(int length, int A, int B) {
    if (A == B) {
        //[waves] is the number of times the cipher should hit the top rail (including buffer)
        int waves = length / (2*(A - 1));
        if (length % ((A - 1)*2) != 0 && length % ((A - 1)*2) != 1) {
            //if text does not end on the line before, or on, the top rail (going up)
            waves += 1;
        }
        return ((A - 1)*2*waves + 1); //this is [fakeLength]
    } else {
        //[waves] is the number of times the cipher should hit the top rail (including buffer)
        int waves = length / (2*(A - 1) + 2*(B - 1));
        if (length % (2*(A - 1) + 2*(B - 1)) != 0 && length % (2*(A - 1) + 2*(B - 1)) != 1) {
            //if text does not end on the line before, or on, the top rail (going up)
            waves += 1;
        }
        return ((2*(A - 1) + 2*(B - 1))*waves + 1); //this is [fakeLength]
    }
}


/*
==========================================================================================
Prints all the different menus

------------------------------------------------------------------------------------------
DETAILS:
int     {option}        used to determine which menu to print
char*   {input}         printed as user feedback
int     {A}             printed as user feedback
int     {B}             printed as user feedback
void    {return}        returns nothing
==========================================================================================
*/
void menu(int option, char *input, int A, int B) {
    ///PRINTS THE DIFFERENT MENUS
    system("cls");
    switch (option) {
        case 1:
            //Prints main menu
            printf("Menu - Enter your choice:\n\n");
            printf("1) Classical Rail-Fence Cipher Encryption\n");
            printf("2) 2-Level Rail-Fence Cipher Encryption\n");
            printf("3) 2-Level Rail-Fence Cipher Decryption\n");
            printf("4) Substitution Cipher Decryption (No key)\n");
            printf("0) Exit\n\n\n> ");
            break;
        case 2:
            //Prints the menu for classical rail-fence encryption
            printf("Menu - Classical Rail-Fence Encryption Cipher Options:\n\n");
            printf("1) Message to Encrypt: %s\n", input);
            printf("2) A: %d\n", A);
            printf("9) Encrypt!\n");
            printf("0) Back to Main Menu\n\n\n> ");
            break;
        case 3:
            //Prints the menu for 2-level rail-fence encryption
            printf("Menu - 2-Level Rail-Fence Encryption Cipher Options:\n\n");
            printf("1) Message to Encrypt: %s\n", input);
            printf("2) A: %d\n", A);
            printf("3) B: %d\n", B);
            printf("9) Encrypt!\n");
            printf("0) Back to Main Menu\n\n\n> ");
            break;
        case 4:
            //Prints the menu for 2-level rail-fence decryption
            printf("Menu - 2-Level Rail-Fence Decryption Cipher Options:\n\n");
            printf("1) Message to Decrypt: %s\n", input);
            printf("2) A: %d\n", A);
            printf("3) B: %d\n", B);
            printf("9) Decrypt!\n");
            printf("0) Back to Main Menu\n\n\n> ");
            break;
        case 5:
            //Prints the menu for substitution decryption (no key)
            printf("Menu - Substitution Decryption Cipher Options:\n\n");
            printf("1) Message to Decrypt: %s\n", input);
            printf("9) Decrypt!\n");
            printf("0) Back to Main Menu\n\n\n> ");
            break;
    }
    ///==========================
}


/*
==========================================================================================
Fetches all characters until '~' has been typed and stores it in the [userInput] array

------------------------------------------------------------------------------------------
DETAILS:
int     {userInput}     where the message typed is stored
void    {return}        returns nothing
==========================================================================================
*/
void fetchUserMessage(char *userInput) {
    char c;
    int count = 0;
    //Loops until ~ and enter has been pressed
    c = fgetc(stdin);
    while (c != '~' && count < 8193) {
        c = fgetc(stdin);
        userInput[count++] = c;
    }
    userInput[count - 1] = '\0';
}


/*
==========================================================================================
This function performs an encryption of a message using the classical rail-fence cipher.

The encryption of [message] is stored in [cipherText], [length] dictates the length of
    [message], and [A] represents the number of vertical "rails".

------------------------------------------------------------------------------------------
DETAILS:
int     {message}       message to be encrypted
char*   {cipherText}    encrypted message
int     {length}        length of message
int     {A}             value of rails ("rows") for grid
void    {return}        returns nothing
==========================================================================================
*/
void railFence(char *message, char *cipherText, int length, int A) {
    ///INITIALISE ARRAY
    //[fakeLength] is length + any needed buffer to ensure the message ends on top rail
    int fakeLength = fLength(length, A, A);
    char *grid[A][fakeLength];
    //set all elements to '~'
    for (int y = 0; y < A; y++) {
        for (int x = 0; x < fakeLength; x++) {
            grid[y][x] = '~';
        }
    }
    ///================
    //------------------------------------------------------------------------------------
    ///ENCODE [MESSAGE] INTO GRID[]
    srand(time(0));
    int y = 0, direction = 1;
    for (int x = 0; x < fakeLength; x++) {
        //places character in correct position on grid
        if (x < length) {
            grid[y][x] = message[x];
        } else {
            grid[y][x] = message[rand() % strlen(message)];
        }
        //moves y into next position it should be in
        if (direction) {
            y++;
        } else {
            y--;
        }
        //flips direction when y hits top or bottom rail
        if (y == 0 || y == (A - 1)) {
            direction = !direction;
        }
    }
    ///============================
    //------------------------------------------------------------------------------------
    ///PRINT AND TRANSFER GRID[] TO [CIPHERTEXT]
    int count = 0;
    for (int y = 0; y < A; y++) {
        for (int x = 0; x < fakeLength; x++) {
            if (grid[y][x] != '~') {
                cipherText[count++] = grid[y][x];
            }
        }
    }
    cipherText[count] = '\0';
    ///=========================================
}


/*
==========================================================================================
This function performs an encryption of a message using the 2-level rail-fence cipher.

When encrypting, [message] is encrypted and stored in [cipherText], [length] is the
    length of [message], [A] reprsents the number of rails, [B] represents how far
    the message "bounces" from the bottom up in the middle of two "crests", and [dir]
    dictates whether this function decrypts or encrypts.

When decrypting, [cipherText] is decrypted and stored in [message], [length] is the
    length of [cipherText], [A] reprsents the number of rails, [B] represents how far
    the message "bounces" from the bottom up in the middle of two "crests", and [dir]
    dictates whether this function decrypts or encrypts.

... like, i could shave off five lines here by combining the array initialisation but ...

------------------------------------------------------------------------------------------
DETAILS:
int     {message}       unencrypted message
char*   {cipherText}    encrypted message
int     {length}        length of message
int     {A}             value of rails ("rows") for grid
int     {B}             extra value that forms "bumps" in the cipher
int     {dir}           to determine if to encrypt or decrypt
void    {return}        returns nothing
==========================================================================================
*/
void railFence2(char *message, char *cipherText, int length, int A, int B, int dir) {
    if (dir) {
        ///===================================================================================
        /*
                                                DECRYPTION
        */
        ///===================================================================================
        ///INITIALISE ARRAY
        char *grid[A][length];
        //set all elements to '~'
        for (int y = 0; y < A; y++) {
            for (int x = 0; x < length; x++) {
                grid[y][x] = '~';
            }
        }
        ///================
        //------------------------------------------------------------------------------------
        ///TRANSFER [CIPHERTEXT] INTO GRID[]
        int y = 0, direction = 1, bHappen = 1;
        //this finds and marks all the positions the characters can go in [grid]
        for (int x = 0; x < length; x++) {
            //places character in correct position on grid
            grid[y][x] = '+';
            //moves y into next position it should be in
            if (direction) {
                y++;
            } else {
                y--;
            }
            //flips direction when y hits top or bottom rail
            if (y == 0 || y == (A - 1)) {
                direction = !direction;
            }
            //handles the [B] aspect by testing if y == b once everytime y == 0
            if (y == 0) {
                bHappen = 1;
            } else if (y == A - B && !direction && bHappen) {
                bHappen = 0;
                direction = !direction;
            }
        }
        //this runs through [grid] left-to-right, top-to-bottom, transfering [cipherText]
        int count = 0;
        for (int y = 0; y < A; y++) {
            for (int x = 0; x < length; x++) {
                if (grid[y][x] != '~') {
                    grid[y][x] = cipherText[count];
                    count++;
                }
            }
        }
        ///=================================
        //------------------------------------------------------------------------------------
        ///PRINT AND TRANSFER GRID[] TO [MESSAGE]
        count = 0;
        for (int x = 0; x < length; x++) {
            for (int y = 0; y < A; y++) {
                if (grid[y][x] != '~') {
                    message[count++] = grid[y][x];
                }
            }
        }
        message[count] = '\0';
        ///======================================
    } else {
        ///===================================================================================
        /*
                                                ENCRYPTION
        */
        ///===================================================================================
        ///INITIALISE ARRAY
        //[fakeLength] is length + any needed buffer to ensure the message ends on top rail
        int fakeLength = fLength(length, A, B);
        char *grid[A][fakeLength];
        //set all elements to '~'
        for (int y = 0; y < A; y++) {
            for (int x = 0; x < fakeLength; x++) {
                grid[y][x] = '~';
            }
        }
        ///================
        //------------------------------------------------------------------------------------
        ///ENCODE [MESSAGE] INTO GRID[]
        srand(time(0));
        int y = 0, direction = 1, bHappen = 1;
        for (int x = 0; x < fakeLength; x++) {
            //places character in correct position on grid
            if (x < length) {
                grid[y][x] = message[x];
            } else {
                grid[y][x] = message[rand() % strlen(message)];
            }
            //moves y into next position it should be in
            if (direction) {
                y++;
            } else {
                y--;
            }
            //flips direction when y hits top or bottom rail
            if (y == 0 || y == (A - 1)) {
                direction = !direction;
            }
            //handles the [B] aspect by testing if y == b once everytime y == 0
            if (y == 0) {
                bHappen = 1;
            } else if (y == A - B && !direction && bHappen) {
                bHappen = 0;
                direction = !direction;
            }
        }
        ///============================
        //------------------------------------------------------------------------------------
        ///PRINT AND TRANSFER GRID[] TO [CIPHERTEXT]
        int count = 0;
        for (int y = 0; y < A; y++) {
            for (int x = 0; x < fakeLength; x++) {
                if (grid[y][x] != '~') {
                    cipherText[count++] = grid[y][x];
                }
            }
        }
        cipherText[count] = '\0';
        ///=========================================
    }
}


/*
==========================================================================================
This function attempts to decrypt a substitution cipher without a key.

The way that this decryption works in general is as follows:
    - It finds any words from the encrypted message that match the same format as words
        from the wordbank (same length and matching "patterns". Eg: 12272 | e55&5).
    - It then increases the "frequency" (a value dictating probability) of that pair of
        character being a match.
    - Then it finds the highest probability match, out of 26*26, it determines them a key
        and removes them from being a match for any other character. Then it repeats.
    - Boom. Key I guess.


... this whole idea is a clusterfuck ... i have no clue how i thought of this ...

------------------------------------------------------------------------------------------
DETAILS:
int     {message}       unencrypted message
char*   {cipherText}    encrypted message
void    {return}        returns nothing
==========================================================================================
*/
void substitution(char *message, char *cipherText, char *key) {
    cipherText[strlen(cipherText) + 1] = '\0'; //This is needed to make sure the last word is noticed
    cipherText[strlen(cipherText)] = '\n'; // ^
    //Convert input to all lowercase:
    for (int i = 0; i < strlen(cipherText); i++) {
        cipherText[i] = tolower(cipherText[i]);
    }
    ///INITIALISE STARTING AND CONSTANT VARIABLES
    char* alphabet = "abcdefghijklmnopqrstuvwxyz";
    int frequencies[26][26];
    //initialise key[] to '?'
    for (int i = 0; i < 26; i++) {
        key[i] = '?';
    }
    //initialise frequencies to 0
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            frequencies[i][j] = 0;
        }
    }
    ///==========================================
    ///FIND LETTER FREQUINCIES OF CIPHER.TXT
    //open the wordlist
    FILE *wordlist;
    wordlist = fopen("wordlist.txt", "r");
    //Checks if wordlist exists:
    if (!wordlist) {
        system("cls");
        printf("MISSING WORDLIST.TXT FILE IN CURRENT DIRECTORY!\n\n");
        system("pause");
        exit(0);
    }
    //loop through all words in cipherText
    char word[50]; //holds words
    char listWord[50]; //holds words from wordlist
    for (int i = 0; i < 50; i++) {
        //initialise word and listWord to '~':
        word[i] = '~';
        listWord[i] = '~';
    }
    int index = 0; //index of word[]
    int index2 = 0; //index of listWord[]
    int count = 0; //index of cipherText
    char c; //used to read through characters of wordlist
    //start reading words in cipherText and wordlist:
    do {
        if (isalpha(cipherText[count])) {
            //character is part of the word in word[]
            word[index++] = tolower(cipherText[count]);
        } else if (!ispunct(cipherText[count])) {
            //word[] has ended, prepare to start again
            word[index] = '\0';
            index = 0;
        }
        //if whole word has been read:
        if (!index) {
            //loop until end of wordlist file:
            while (!feof(wordlist)) {
                c = fgetc(wordlist);
                if (!isalpha(c)) {
                    //starts next wordlist word
                    listWord[index2] = '\0';
                    index2 = 0;
                } else {
                    //character is part of the word in listWord[]
                    listWord[index2++] = c;
                }
                //if whole word has been read:
                if (!index2) {
                    //compare word cipherText and wordlist word
                    //if words same length:
                    if (strlen(word) == strlen(listWord)) {
                        //if words have same characters in same places:
                        int value = 1;
                        int comparison[50]; //hold the "format" of words (same number = same letter)
                        //initialise comparison to 0
                        for (int i = 0; i < 50; i++) {
                            comparison[i] = 0;
                        }
                        //sets format of word
                        for (int i = 0; i < strlen(word); i++) {
                            //if character not yet formatted
                            if (comparison[i] == 0) {
                                //scans all characters including and after this one:
                                for (int j = i; j < strlen(word); j++) {
                                    //sets them to new format
                                    comparison[j] = value;
                                }
                                value++;
                            }
                        }
                        //more variables
                        value = 1;
                        int comparison2[50]; //hold the "format" of words (same number = same letter)
                        //initialise comparison to 0
                        for (int i = 0; i < 50; i++) {
                            comparison2[i] = 0;
                        }
                        //sets format of listWord
                        for (int i = 0; i < strlen(listWord); i++) {
                            //if character not yet formatted
                            if (comparison2[i] == 0) {
                                //scans all characters including and after this one:
                                for (int j = i; j < strlen(listWord); j++) {
                                    //sets them to new format
                                    comparison2[j] = value;
                                }
                                value++;
                            }
                        }
                        //checks format of listWord against word
                        int flag = 1;
                        for (int i = 0; i < strlen(word); i++) {
                            if (comparison[i] != comparison2[i]) {
                                flag = 0;
                            }
                        }
                        //adds to possible substitutes
                        char *e; //to help search *alphabet
                        char *ee; //^
                        if (flag) {
                            for (int i = 0; i < strlen(listWord); i++) {
                                //increases necessary frequency count:
                                e = strchr(alphabet, listWord[i]);
                                ee = strchr(alphabet, word[i]);
                                frequencies[(int)(e - alphabet)][(int)(ee - alphabet)] += 1;
                            }
                        }
                    }
                }
            }
            fseek(wordlist, 0, SEEK_SET); //starts from beginning of wordlist
        }
        count++;
    } while (count < strlen(cipherText)); //until every cipherText word has been read
    fclose(wordlist);
    ///=====================================
    ///GENERATE KEY FROM COLLECTED FREQUENCIES
    for (int i = 0; i < 26; i++) {
        int highest = 0;
        int highx = -1, highy = -1;
        //finds the highest value match out of all match possibilities
        for (int x = 0; x < 26; x++) {
            for (int y = 0; y < 26; y++) {
                if (highest < frequencies[x][y]) {
                    highest = frequencies[x][y];
                    highx = x;
                    highy = y;
                }
            }
        }
        //makes sure these letters cannot be used again
        if (highest > 0) {
            for (int j = 0; j < 26; j++) {
                frequencies[j][highy] = 0;
                frequencies[highx][j] = 0;
            }
            //sets another character of key
            key[highx] = alphabet[highy];
        }
    }
    ///=======================================
    ///PRINT KEY AND SUBSTITUTE CIPHERTEXT
    //susbstitues the "known" values into cipherText to make message:
    cipherText[strlen(cipherText) - 1] = '\0'; //Undoing what I did at start
    //initialise message to all unknown:
    for (int i = 0; i < strlen(cipherText); i++) {
        message[i] = '~';
    }
    for (int i = 0; i < strlen(cipherText); i++) {
        for (int j = 0; j < 26; j++) {
            //substitutes:
            if (cipherText[i] == key[j]) {
                message[i] = alphabet[j];
            } else if(!isalpha(cipherText[i])) {
                message[i] = cipherText[i];
            }
        }
    }
    message[strlen(cipherText)] = '\0';
    ///===================================
}



///TODO:
/**

**/

/*
DEPRECIATED COMMAND-LINE FORMAT:
*//**

<EXECUTABLE> <OPERATION> <DECRYPTED FILE> <ENCRYPTED FILE> [<A> [<B>]]

**//*
Syntax: <> - Necessary; [] - Optional.
EXECUTABLE - This program
OPERATION - Number from 1-4, where:
        1 = Classical Rail-Fence Encryption
        2 = 2-Level Rail-Fence Encryption
        3 = 2-Level Rail-Fence Decryption
        4 = Substitution Decryption (No key)
DECRYPTED FILE - Input file location (No spaces in file directory)
ENCRYPTED FILE - Output file location (No spaces in file directory)
A - Used in operations 1, 2 & 3. Denotes the number of "rails"
B - Used in operation 2 & 3. Denotes peak of the point midway between top rail points
*/
