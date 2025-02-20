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

        // Trim leading whitespace
        char *trimmedLine = line;
        while (*trimmedLine == ' ' || *trimmedLine == '\t') {
            trimmedLine++;
        }

        // Check for function call in the form of 'FunctionName();'
        char *functionCall = strstr(trimmedLine, "();");
        if (functionCall != NULL) {
            // Extract function name by removing parentheses
            char *functionCallName = strtok(trimmedLine, " (");
            // Translate to assembly call without printing the original line
            fprintf(outputFile, "    jsr %s\n", functionCallName);
            continue;
        }
        
        

        // Check for function definition, i.e., starting with 'void' and followed by function name
        if (strstr(trimmedLine, "void ") == trimmedLine) {
            // Look for function name, assuming function signature is simple
            char *funcName = strtok(trimmedLine + 5, " ("); // Split on ' ' or '(' to remove '()'
            strcpy(functionName, funcName);  // Store function name
            fprintf(outputFile, "%s:\n", functionName); // Print function name directly followed by ':'
            inFunction = 1;
            continue;
        }

        // If inside a function, check for function calls and assembly code
        if (inFunction) {
            // Check for 'return;' and convert to 'rts'
            if (strstr(trimmedLine, "return;") != NULL) {
                fprintf(outputFile, "    rts\n");
                continue;
            }

			if (strstr(trimmedLine, "while(1)") != NULL) 
			{
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
                    continue;
                } else {
                    // Skip any function calls or unwanted code
                    if (strstr(trimmedLine, "();") == NULL) {
                        fprintf(outputFile, "    %s\n", line);
                    }
                }
            }
            

            // Check for function call in the form of 'FunctionName();'
            char *functionCall = strstr(trimmedLine, "();");
            if (functionCall != NULL) {
                // Extract function name by removing parentheses
                char *functionCallName = strtok(trimmedLine, " (");
                fprintf(outputFile, "    jsr %s\n", functionCallName); // Generate assembly call 'jsr FunctionName'
                continue;
            }

            // Check for inline assembly and print it
            if (strstr(trimmedLine, "asm volatile") != NULL) {
                // Extract assembly code after "asm volatile"
                char *assemblyCode = strchr(trimmedLine, '"') + 1;
                char *endQuote = strchr(assemblyCode, '"');
                if (endQuote) {
                    *endQuote = '\0';
                    fprintf(outputFile, "    %s\n", assemblyCode);
                }
            }

            if (strstr(trimmedLine, "#WRITE") == trimmedLine) {
                int value, address;
                if (sscanf(trimmedLine, "#WRITE %x %x", &value, &address) == 2) {
                    fprintf(outputFile, "    lda #$%02X\n", value);
                    fprintf(outputFile, "    sta $%04X\n", address);
                }
                continue;
            }

            // Look for the closing brace of the function
            if (strchr(trimmedLine, '}')) {
                inFunction = 0;
                continue;
            }

            // Additional handling for specific cases like while loops, etc.
        }
    }
}

int main() {
    printf("\nRiscom 6502 CC v1.5\n");
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
