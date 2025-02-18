#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertToAssembly(FILE *inputFile, FILE *outputFile) {
    char line[1024];
    int inFunction = 0;
    int insideWhileLoop = 0;
    char functionName[256]; // Store function name
    
    while (fgets(line, sizeof(line), inputFile)) {
        // Remove single-line comments (//)
        char *commentStart = strstr(line, "//");
        if (commentStart != NULL) {
            *commentStart = '\0';  // Truncate the line at the comment
        }

        // Check for function definition, i.e., starting with 'void' and followed by function name
        if (strstr(line, "void ") == line) {
            // Look for function name, assuming function signature is simple
            char *funcName = strtok(line + 5, " ("); // Split on ' ' or '(' to remove '()'
            strcpy(functionName, funcName);  // Store function name
            fprintf(outputFile, "%s:\n", functionName); // Print function name directly followed by ':'
            inFunction = 1;
            continue;
        }

        // If inside a function, check for function calls and assembly code
        if (inFunction) {
            // Check for 'return;' and convert to 'rts'
            if (strstr(line, "return;") != NULL) {
                fprintf(outputFile, "    rts\n");
                continue;
            }

            // Check for function call in the form of 'FunctionName();'
            char *functionCall = strstr(line, "();");
            if (functionCall != NULL) {
                // Extract function name by removing parentheses
                char *functionCallName = strtok(line, " (");
                fprintf(outputFile, "    jsr %s\n", functionCallName); // Generate assembly call 'jsr FunctionName'
                continue;
            }

            // Check for inline assembly and print it
            if (strstr(line, "asm volatile") != NULL) {
                // Extract assembly code after "asm volatile"
                char *assemblyCode = strchr(line, '"') + 1;
                char *endQuote = strchr(assemblyCode, '"');
                if (endQuote) {
                    *endQuote = '\0';
                    fprintf(outputFile, "    %s\n", assemblyCode);
                }
            }

            // Look for a while(1) loop and convert it to infinite loop with 'jmp'
            if (strstr(line, "while(1)") != NULL) {
                insideWhileLoop = 1;
                continue;  // Skip the rest of the loop body and handle it below
            }

            if (insideWhileLoop) {
                // Handle the body of the while(1) loop
                if (strchr(line, '}')) {
                    // Add jump to the start of the loop
                    fprintf(outputFile, "    jmp %s\n", functionName); // Jump to function start
                    insideWhileLoop = 0;  // Exit the while loop handling
                } else if (strchr(line, '{')) {
                    // Skip printing the '{' in assembly code
                    continue;  // Just ignore the '{'
                } else {
                    // Print the code inside the while loop
                    fprintf(outputFile, "    %s\n", line);
                }
            }

            // Look for the closing brace of the function
            if (strchr(line, '}')) {
                inFunction = 0;
            }
        }
    }
}

int main() {
    printf("\nRISCOM E6502 CC V1.0\n");
    FILE *inputFile = fopen("OS.C", "r");
    if (inputFile == NULL) {
        perror("Error opening file OS.C");
        return 1;
    }

    FILE *outputFile = fopen("OUT.ASM", "w");
    if (outputFile == NULL) {
        perror("Error opening OUT.ASM");
        fclose(inputFile);
        return 1;
    }

    convertToAssembly(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    printf("BUILD SUCCESS\n");
    return 0;
}
