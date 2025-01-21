#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_PERSONS 1000
#define MAX_UNITS 100

// Define the person structure
typedef struct person {
    char name[31];
    char id[10];
    char genes[5][22];
} person;

// Function prototypes
void createDatabase(FILE** units, int numberOfUnits, char* filename);
person* getPotentialDonors(char* database, person patient, int min_match, int* size);
void printPotentialDonorsList(person* potentialDonors, int size);

// Helper functions




/**
 * @brief Compares two persons lexicographically by their names.
 * 
 * This function uses `strcmp` to compare the names of two `person` structs.
 * It is primarily used to determine the order of persons during sorting or merging operations.
 * 
 * @param a Pointer to the first person.
 * @param b Pointer to the second person.
 * 
 * @return An integer less than, equal to, or greater than zero if the name of `a`
 *         is found, respectively, to be less than, equal to, or greater than the name of `b`.
 */
int comparePersons(const person* a, const person* b) {
    return strcmp(a->name, b->name);
}





/**
 * @brief Counts the number of gene matches between a donor and a patient.
 * 
 * This function iterates through the genes of both the donor and patient,
 * comparing them one by one. If a gene matches, the `matchCount` is incremented.
 * 
 * @param donor Pointer to the donor `person`.
 * @param patient Pointer to the patient `person`.
 * 
 * @return The total number of matching genes between the donor and patient.
 * 
 * @note The genes are stored as strings, and a match is determined using `strcmp`.
 */
int countGeneMatches(const person* donor, const person* patient) {
    int matchCount = 0;
    // Calculate how many genes match between the donor and the patient
    for (int i = 0; i < 5; i++) {
        if (strcmp(donor->genes[i], patient->genes[i]) == 0) {
            matchCount++;
        }
    }
    return matchCount;
}




/**
 * @brief Counts the number of character mismatches between two gene strings.
 * 
 * This function compares two strings character by character to determine the
 * number of positions where the characters differ. It stops counting when the
 * end of either string is reached.
 * 
 * @param donorGene Pointer to the donor's gene string.
 * @param patientGene Pointer to the patient's gene string.
 * 
 * @return The total number of character mismatches between the two gene strings.
 * 
 * @note This function assumes that both input strings are null-terminated.
 */
int countMismatches(const char* donorGene, const char* patientGene) {
    int mismatches = 0;
    // Calculate how many genes mismatch between the donor and the patient
    for (int i = 0; donorGene[i] != '\0'; i++) {
        if (donorGene[i] != patientGene[i]) {
            mismatches++;
        }
    }
    return mismatches;
}




/**
 * @brief Cleans a person's name by trimming any part after the first digit.
 * 
 * This function iterates through the provided name and trims off everything 
 * starting from the first digit encountered. It stops processing as soon as 
 * a digit is found and null-terminates the string just before that point.
 * 
 * @param name Pointer to the person's name string to be cleaned.
 * 
 * @note This function assumes that the name string is null-terminated.
 */
void cleanName(char* name) {
    int i = 0;
    int lastCharIndex = -1;

    // Iterate through the string to find the first digit or meaningful part
    while (name[i] != '\0') {
        if (isdigit(name[i])) {
            break;  // Stop at the first digit
        }
        if (!isspace(name[i])) {
            lastCharIndex = i;  // Track the last non-space character
        }
        i++;
    }

    // Null-terminate the string after the last meaningful character
    name[lastCharIndex + 1] = '\0';
}





/**
 * @brief Checks if a given ID is a duplicate in a list of processed IDs.
 * 
 * This function compares the provided ID against a list of previously processed
 * IDs. If a match is found, it indicates that the ID is a duplicate and returns
 * 1. If no match is found, it returns 0, indicating that the ID is unique.
 * 
 * @param id The ID to be checked for duplication.
 * @param processedIDs A 2D array containing the list of already processed IDs.
 * @param numProcessed The number of processed IDs in the array.
 * 
 * @return 1 if the ID is a duplicate, 0 if the ID is unique.
 * 
 * @note This function assumes that the provided `processedIDs` array is valid
 *       and the strings are null-terminated.
 */
// Function to check if the ID is already in the list of processed IDs
int isDuplicate(char* id, char processedIDs[][10], int numProcessed) {
    for (int i = 0; i < numProcessed; i++) {
        if (strcmp(id, processedIDs[i]) == 0) {
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicate
}




int main() {
    int choice;
    char rootName[50];              // For the root name of the units
    int numUnits;                   // Number of collection units
    char dbName[50];                // For the database name
    person* potentialDonors = NULL; // Pointer to potential donors array
    int potentialDonorSize = 0;     // Size of potential donors array
    int minMatch;                   // Minimum number of matching genes

    do {
        printf("\n******* Main Menu *******\n");
        printf("1. Unify Database\n");
        printf("2. Find Potential Donors\n");
        printf("3. Print The List of Potential Donors\n");
        printf("4. Exit\n");
        printf("Enter Your Selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                printf("Enter units root name: ");
                scanf("%s", rootName);
                printf("Enter the number of units: ");
                scanf("%d", &numUnits);

                FILE* unitFiles[numUnits];
                for (int i = 0; i < numUnits; i++) {
                    char fileName[50];
                    sprintf(fileName, "%s%d.txt", rootName, i + 1);
                    unitFiles[i] = fopen(fileName, "r");
                    if (unitFiles[i] == NULL) {
                        printf("Error: Could not open file %s\n", fileName);
                        return 1;
                    }
                }

                printf("Enter the new database name: ");
                scanf("%s", dbName);

                createDatabase(unitFiles, numUnits, dbName);

                // Close all files after processing
                for (int i = 0; i < numUnits; i++) {
                    fclose(unitFiles[i]);
                }
                break;
            }
            case 2: {
                person patient;
                printf("Enter Genes DNA Sequences:\n");
                for (int i = 0; i < 5; i++) {
                    printf("Gene %d: ", i + 1);
                    scanf("%s", patient.genes[i]);
                }
                printf("Enter Minimal Match: ");
                scanf("%d", &minMatch);
                printf("Enter The Database Filename: ");
                scanf("%s", dbName);


                potentialDonors = getPotentialDonors(dbName, patient, minMatch, &potentialDonorSize);
                break;
            }
            case 3: {
                if (potentialDonors != NULL && potentialDonorSize > 0) {
                    printPotentialDonorsList(potentialDonors, potentialDonorSize);
                } else {
                    printf("No potential donors found or the list is empty.\n");
                }
                break;
            }
            case 4: {
                printf("Exiting program.\n");
                break;
            }
            default: {
                printf("Invalid selection. Try again.\n");
                break;
            }
        }
    } while (choice != 4);

    // Free dynamically allocated memory for potential donors
    if (potentialDonors != NULL) {
        free(potentialDonors);
    }

    return 0;
}




/**
 * @brief Merges records from multiple input files into a single output file, eliminating duplicates.
 * 
 * This function reads records from multiple input files, merges them into a single output file, and ensures
 * that no duplicate records are written. It performs the merging in lexicographical order based on the names,
 * and skips any records that have already been processed (checked by unique ID).
 * 
 * The function handles the records by keeping track of processed IDs and adding a newline between records from
 * different files. The smallest lexicographically ordered record is selected and written to the output file at
 * each iteration.
 * 
 * @param units Array of file pointers to the input files to read records from.
 * @param numberOfUnits The number of input files to process.
 * @param filename The name of the output file to write the merged records to.
 * 
 * @note This function assumes that each input file contains records in a specific format, with each record
 * consisting of a name, ID, and multiple gene sequences.
 */
void createDatabase(FILE** units, int numberOfUnits, char* filename) {
    // Open the output file for writing; exit if unable to open
    FILE* outFile = fopen(filename, "w");
    if (!outFile) {
        perror("Error creating database file");
        exit(1);
    }

    // Array to store the current records being read from each input file
    person currentPersons[numberOfUnits];
    int activeFiles = 0;

    char processedIDs[MAX_UNITS][10]; // To store processed IDs
    int numProcessed = 0; // Counter for processed IDs

    // Initialize the array with the first record from each input file
    for (int i = 0; i < numberOfUnits; i++) {
        // Read the name (first and last name), id, and genes
        if (fscanf(units[i], "%30[^0-9]%9s %21s %21s %21s %21s %21s", 
           currentPersons[i].name, currentPersons[i].id,
           currentPersons[i].genes[0], currentPersons[i].genes[1],
           currentPersons[i].genes[2], currentPersons[i].genes[3],
           currentPersons[i].genes[4]) == 7) {

            cleanName(currentPersons[i].name);
            
            activeFiles++;
        }

    }

    // Process records until all active files are exhausted
    // Process records until all active files are exhausted

    int lastFileIndex = -1; // Keep track of the last file processed
    
    while (activeFiles > 0) {
    int smallestIndex = -1;  // Index of the lexicographically smallest record

    // Find the smallest record among current records
    for (int i = 0; i < numberOfUnits; i++) {
        if (currentPersons[i].name[0] != '\0' && 
            (smallestIndex == -1 || comparePersons(&currentPersons[i], &currentPersons[smallestIndex]) < 0)) {
            smallestIndex = i;
        }
    }

    if (smallestIndex == -1) break; // No valid records left

    int newLine = -1;
    // Check if switching to a new file
        if (smallestIndex != lastFileIndex) {
            if (lastFileIndex != -1) {
                if (!isDuplicate(currentPersons[smallestIndex].id, processedIDs, numProcessed))
                {
                    fprintf(outFile, "\n"); // Add a new line before starting a new file
                }
                
                newLine = 1;
            }
            lastFileIndex = smallestIndex; // Update the last file index
        }
    
    // Write the smallest record to the output file
    if (!isDuplicate(currentPersons[smallestIndex].id, processedIDs, numProcessed)) {
        if(newLine == 1)
        {
                fprintf(outFile, "%-30s%-9s %-21s %-21s %-21s %-21s %-21s",
                currentPersons[smallestIndex].name, currentPersons[smallestIndex].id,
                currentPersons[smallestIndex].genes[0], currentPersons[smallestIndex].genes[1],
                currentPersons[smallestIndex].genes[2], currentPersons[smallestIndex].genes[3],
                currentPersons[smallestIndex].genes[4]);
                newLine = -1;
        }
        else 
        {
            
                fprintf(outFile, "%-30s %-9s %-21s %-21s %-21s %-21s %-21s",
                currentPersons[smallestIndex].name, currentPersons[smallestIndex].id,
                currentPersons[smallestIndex].genes[0], currentPersons[smallestIndex].genes[1],
                currentPersons[smallestIndex].genes[2], currentPersons[smallestIndex].genes[3],
                currentPersons[smallestIndex].genes[4]);
                newLine = -1;
        }
        // Add the current ID to the list of processed IDs
        strcpy(processedIDs[numProcessed], currentPersons[smallestIndex].id);
        numProcessed++;
    }
    

    

    // Read the next record from the file that contained the smallest record
    if (fscanf(units[smallestIndex], "%30[^0-9] %9s %21s %21s %21s %21s %21s",
               currentPersons[smallestIndex].name, currentPersons[smallestIndex].id,
               currentPersons[smallestIndex].genes[0], currentPersons[smallestIndex].genes[1],
               currentPersons[smallestIndex].genes[2], currentPersons[smallestIndex].genes[3],
               currentPersons[smallestIndex].genes[4]) != 7) {

                cleanName(currentPersons[smallestIndex].name);

        // Mark as invalid if the file has no more records
        memset(&currentPersons[smallestIndex], 0, sizeof(person));
        currentPersons[smallestIndex].name[0] = '\0';
        activeFiles--;  // Decrement the count of active files
        
    }
}


    // Close the output file to free resources
    fclose(outFile);
}




/**
 * @brief Identifies potential bone marrow donors based on genetic compatibility.
 * 
 * This function reads a donor database file, compares each donor's genetic data with
 * the patient's, and returns an array of donors meeting the minimum gene match criteria.
 * 
 * @param database A string containing the file name of the donor database.
 * @param patient A person structure containing the patient's genetic data.
 * @param min_match The minimum number of matching genes required for a donor to be considered.
 * @param size A pointer to an integer where the function will store the number of potential donors found.
 * 
 * @return A dynamically allocated array of person structures containing the potential donors.
 *         The caller is responsible for freeing the allocated memory.
 * 
 * @note If the database file cannot be opened, the function prints an error message and exits the program.
 * 
 * Example Usage:
 * person* donors = getPotentialDonors("database.txt", patient, 3, &size);
 * for (int i = 0; i < size; i++) {
 *     printf("Donor Name: %s, ID: %s\n", donors[i].name, donors[i].id);
 * }
 * free(donors); // Free the memory allocated for the donors array.
 */
person* getPotentialDonors(char* database, person patient, int min_match, int* size) {
    FILE* dbFile = fopen(database, "r");
    if (!dbFile) {
        perror("Error opening database file");
        exit(1); // Handle file opening failure
    }

    // Initialize an array to store potential donors
    person* donors = malloc(MAX_PERSONS * sizeof(person));
    int donorCount = 0;

    person current;

    // Read each donor from the database
    while (fscanf(dbFile, "%30[^0-9] %9s %21s %21s %21s %21s %21s",
                  current.name, current.id,
                  current.genes[0], current.genes[1],
                  current.genes[2], current.genes[3],
                  current.genes[4]) == 7) {

        // Count the number of gene matches between donor and patient
        int matches = countGeneMatches(&current, &patient);

         // Include the donor only if the match count is above the threshold
        if (matches >= min_match) {
            donors[donorCount++] = current;
        }
    }

    fclose(dbFile); // Close the database file to free resources and avoid resource leaks.
    *size = donorCount; // Update the size variable with the total number of potential donors found.
    return donors; // Return the dynamically allocated array of potential donors to the caller.
}





/**
 * @brief Prints the list of potential bone marrow donors.
 * 
 * This function displays the details of potential donors from a given array
 * of donor structures. If no donors are found (size is 0), it notifies the user.
 * 
 * @param potentialDonors Pointer to an array of person structures representing potential donors.
 * @param size The number of potential donors in the array.
 * 
 * Example Output:
 * Potential Donors Details
 * ------------------------
 * 1. John Doe                     123456789
 * 2. Jane Smith                   987654321
 */
void printPotentialDonorsList(person* potentialDonors, int size) {
    if (size == 0) {
        printf("No potential donors found.\n");
        return; // Exit if no donors are available
    }

    printf("Potential Donors Details\n------------------------\n");

    // Loop through each potential donor and print their details
    for (int i = 0; i < size; i++) {
        printf("%d. %-30s %s\n", i + 1, potentialDonors[i].name, potentialDonors[i].id);
    }
}
