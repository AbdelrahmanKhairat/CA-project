#include <stdio.h>
#include <string.h>

int memInstructions[1024];
int memData[2048];
int GPRS[66]; // 64 R0-R63  and 1 PC and 1 status Register
int opcode;
int R1;
int R2;
int imm;
int getOpcode(char *token)
{

    if (strcmp(token, "ADD") == 0)
    {
        return 0;
    }
    else if (strcmp(token, "SUB") == 0)
    {
        return 1;
    }
    else if (strcmp(token, "MUL") == 0)
    {
        return 2;
    }
    else if (strcmp(token, "LDI") == 0)
    {
        return 3;
    }
    else if (strcmp(token, "BEQZ") == 0)
    {
        return 4;
    }
    else if (strcmp(token, "AND") == 0)
    {
        return 5;
    }
    else if (strcmp(token, "OR") == 0)
    {
        return 6;
    }
    else if (strcmp(token, "JR") == 0)
    {
        return 7;
    }
    else if (strcmp(token, "SLC") == 0)
    {
        return 8;
    }
    else if (strcmp(token, "SRC") == 0)
    {
        return 9;
    }
    else if (strcmp(token, "LB") == 0)
    {
        return 10;
    }
    else if (strcmp(token, "SB") == 0)
    {
        return 11;
    }
}
int getFirstValue(char *token)
{

    int len = strlen(token);
    int val;

    if (len == 2)
    {
        char arr[3];
        char substring[2];

        strcpy(arr, token);
        strncpy(substring, arr + 1, 2);
        val = atoi(substring); // parse the string into integer
    }
    else if (len == 3)
    {
        char arr[4];
        char substring[3];

        strcpy(arr, token);
        strncpy(substring, arr + 1, 3);
        val = atoi(substring); // parse the string into integer
    }
}
int getSecondValue(char *token)
{
    int len = strlen(token);
    char arr[len + 1];

    strcpy(arr, token);

    if (arr[0] == 'R')
    {
        return getFirstValue(token);
    }
    else
    {
        return atoi(arr);
    }
}

void printBinary(int num)
{
    // Size of an integer in bits
    int size = sizeof(int) * 8;

    // Loop through each bit position
    for (int i = size - 1; i >= 0; i--)
    {
        // Extract the i-th bit using bitwise AND with a bitmask
        int bit = (num >> i) & 1;

        // Print the bit
        printf("%d", bit);
    }
    printf("\n");
}
void showInstructMem()
{

    for (int i = 0; i < 1024; i++)
    {
        if (memInstructions[i] != 0)
        {
            printf("%d ", memInstructions[i]);
        }
    }
}

void readAndParse()
{

    FILE *filePointer;
    char buffer[100]; // Buffer to store each line

    // Open the file in read mode
    filePointer = fopen("test.txt", "r");

    // Check if file opened successfully
    if (filePointer == NULL)
    {
        printf("File could not be opened.");
        return;
    }

    int i = 0;

    // Read and split each line
    while (fgets(buffer, sizeof(buffer), filePointer) != NULL)
    {
        // Split the line based on spaces

        char *token = strtok(buffer, " ");
        int opcode = getOpcode(token);

        token = strtok(NULL, " ");
        int val1 = getFirstValue(token);

        token = strtok(NULL, " ");
        int val2 = getSecondValue(token);

        opcode = opcode << 12;
        val1 = val1 << 6;

        int res = 0;

        res = res | opcode;
        res = res | val1;
        res = res | val2;

        memInstructions[i++] = res;
    }

    // Close the file
    fclose(filePointer);
}
void decode(int instruction)
{

    opcode = instruction & 0b1111000000000000; // bits31:26
    R1 = instruction & 0b0000111111000000;
    R2 = instruction & 0b0000000000111111;
    imm = instruction & 0b0000000000111111;
    opcode = opcode >> 12;
    R1 = R1 >> 6;
    // Printings
    printf("opcode = %i\n", opcode);
    printf("R1 = %i\n", R1);
    printf("R2 = %i\n", R2);
    printf("imm = %i\n", imm);
}
void excute(int op, int R1, int R2)
{
    if (op == 0) // if it is ADD
    {
        R1 += R2;
    }
    if (op == 1)    // if it is SUB
    {
        R1 -= R2;   
    }
    if (op == 2)
    {
        R1 = R1 * R2;   // if it is MUL
    }
    if (op == 5)
    {
        R1 = R1 & R2;   // if it is AND
    }
    if (op == 6)
    {
        R1 = R1 | R2;   // if it is OR
    }
    if (op == 7)
    {
        pc = R1 || R2;  // if it is JR
    }
}

int main()
{

    readAndParse();
    showInstructMem();
    return 0;
}
