/*
	Program Title: The IT Department's Registry
	Program Description: 
		A C-based program that manages database of IT students' academic records for 
		easy monitoring, accessing, and reviewing of students. Developed using Arrays, Structs, Pointer Arithmetic,
		and File I/O.
	Program Version by: Jalmasco, Vince Gabriel
						Shea, Gabriel Philip
						Villanueva, Mart Rheymor
						Zamudio, James Cedrick
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 100 students should be more than enough for a single block registry
#define MAX_STUDENTS 100

// Forward declarations so the compiler doesn't complain about call order
void addStudent();
void viewStudent();
void updateRecord();
void sortbyGWA();
void savetoFile();
void loadfromFile();
const char* getGradeInterpretation(float gwa);
void displayGradingScale();

// One struct to hold everything about a student. Keeps things tidy instead of
// juggling a dozen separate arrays
typedef struct Student {
    char name[50];
    char block[50];
    int  year_level;
    char id[20];             // stored as a string because IDs like "2025-01-01720" have dashes
    char contact_number[16]; // 16 chars to be safe; PH numbers are 11 digits but formatting varies
    float gwa;
    int  is_inc;             // acts like a boolean: 1 means the student has an INC, 0 means they don't
    char email[100];
} Record;

// Global array and counter. Every function needs access to these,
// so keeping them global saves a lot of passing around
Record student_list[MAX_STUDENTS];
int student_count = 0;

int main() {

    loadfromFile();

    system("color 1F");

    printf("%45s\n", "Bicol University College of Science");
    printf("%35s\n", "Legazpi City, Albay");
    printf("%50s\n", "Department of Information Technology Registry");
    printf("%35s\n", "===================================================================");
    printf("\n\n%35s\n", "STUDENT DATABASE");

    char input[10];
    int choose_option = 0;

    do {
        printf("\n1. Add Student\n");
        printf("2. View Student\n");
        printf("3. Update Record\n");
        printf("4. Sort by GWA\n");
        printf("5. View Grading Scale\n");
        printf("6. Exit\n\n");

        printf("Choose option: ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;

        if (sscanf(input, "%d", &choose_option) != 1) {
            printf("Invalid choice.\n");
            choose_option = 0;
            continue;
        }

        switch (choose_option) {
            case 1: addStudent();          break;
            case 2: viewStudent();         break;
            case 3: updateRecord();        break;
            case 4: sortbyGWA();           break;
            case 5: displayGradingScale(); break;
            case 6: printf("You have exited.\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choose_option != 6);

    return 0;
}

void addStudent() {

    // Stop early if we've already hit the cap, better than overwriting memory
    if (student_count >= MAX_STUDENTS) {
        printf("[!] Student list is full. Cannot add more students.\n");
        return;
    }

    // Point directly to the next empty slot instead of copying later
    Record *s = &student_list[student_count];

    // The [^\n] lets us capture full names with spaces. Just using %s would cut off at the first space
    printf("\nEnter name: ");
    scanf(" %49[^\n]", s->name);

    // Keep asking for an ID until the user gives one that isn't already in the list
    int id_valid = 0;
    do {
        printf("Enter ID: ");
        scanf(" %19s", s->id);

        int duplicate = 0;
        for (int i = 0; i < student_count; i++) {
            if (strcmp(student_list[i].id, s->id) == 0) {
                printf("  [!] ID %s already exists. Enter a unique ID.\n", s->id);
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) id_valid = 1;

    } while (!id_valid);

    // Validate INC status. Only 1 or 0 are accepted, anything else loops again
    do {
        printf("Is this student Incomplete (INC)? (1=Yes, 0=No): ");
        scanf(" %d", &s->is_inc);
        if (s->is_inc != 0 && s->is_inc != 1)
            printf("  [!] Please enter 1 for Yes or 0 for No.\n");
    } while (s->is_inc != 0 && s->is_inc != 1);

    // INC students don't have a GWA yet, so we skip that field entirely
    if (s->is_inc) {
        s->gwa = 0.0f;
        printf("  [!] GWA skipped, student marked as INC.\n");
    } else {
        // BU's grading scale runs from 1.00 (best) to 5.00 (failing),
        // so anything outside that range is clearly a typo
        do {
            printf("Enter General Weighted Average (GWA) [1.00 - 5.00]: ");
            scanf(" %f", &s->gwa);
            if (s->gwa < 1.00f || s->gwa > 5.00f)
                printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
        } while (s->gwa < 1.00f || s->gwa > 5.00f);
    }

    printf("Enter Block: ");
    scanf(" %49s", s->block);

    // BU has year levels 1 through 5, including a 5th year for some programs
    do {
        printf("Enter Year Level (1-5): ");
        scanf(" %d", &s->year_level);
        if (s->year_level < 1 || s->year_level > 5)
            printf("  [!] Invalid year level. Must be between 1 and 5.\n");
    } while (s->year_level < 1 || s->year_level > 5);

    printf("Enter Contact Number: ");
    scanf(" %15s", s->contact_number);

    printf("Enter Email: ");
    scanf(" %99s", s->email);

    printf("\n");

    // Only increment after everything is filled in successfully
    student_count++;
    savetoFile();
    printf("[+] Student '%s' added successfully.\n", s->name);
}

void viewStudent() {

    if (student_count == 0) {
        printf("\n[!] No student records found.\n");
        return;
    }

    // Column headers. Widths are set to match the printf formats below
    printf("\n");
    printf("-------STUDENT RECORDS-------\n");
    printf("%-20s %-20s %-8s %-22s %-12s %-15s %-16s %-30s\n",
           "Name", "ID", "GWA", "Interpretation", "Year Level", "Block", "Contact Number", "Email");
    printf("%-20s %-20s %-8s %-22s %-12s %-15s %-16s %-30s\n",
           "--------------------", "--------------------", "--------",
           "----------------------", "------------", "---------------",
           "----------------", "------------------------------");

    for (int i = 0; i < student_count; i++) {
        // INC students get a special row since they have no valid GWA to print
        if (student_list[i].is_inc) {
            printf("%-20s %-20s %-8s %-22s %-12d %-15s %-16s %-30s\n",
                   student_list[i].name,
                   student_list[i].id,
                   "INC",
                   "Incomplete",
                   student_list[i].year_level,
                   student_list[i].block,
                   student_list[i].contact_number,
                   student_list[i].email);
        } else {
            // %.2f keeps the GWA display clean, e.g. 1.75 instead of 1.750000
            printf("%-20s %-20s %-8.2f %-22s %-12d %-15s %-16s %-30s\n",
                   student_list[i].name,
                   student_list[i].id,
                   student_list[i].gwa,
                   getGradeInterpretation(student_list[i].gwa),
                   student_list[i].year_level,
                   student_list[i].block,
                   student_list[i].contact_number,
                   student_list[i].email);
        }
    }
}

void updateRecord() {

    char locate_id[20];

    printf("\n-------UPDATE RECORD-------\n");
    printf("\nInput student's ID: ");
    scanf(" %19s", locate_id);

    int found = 0;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(locate_id, student_list[i].id) == 0) {
            found = 1;

            // Using pointers here to avoid retyping student_list[i] everywhere
            float *ptr_gwa  = &student_list[i].gwa;
            char  *ptr_name =  student_list[i].name;
            printf("Student: %s\n", ptr_name);

            // Show current status so the user knows what they're changing
            if (student_list[i].is_inc) {
                printf("Current Status: INC (Incomplete)\n");
            } else {
                printf("Current GWA: %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
            }

            int update_choice;
            printf("\nWhat would you like to update?\n");
            printf("  1. Change INC status\n");
            printf("  2. Update GWA\n");
            printf("Choose: ");
            scanf("%d", &update_choice);

            if (update_choice == 1) {
                printf("Mark as Incomplete? (1=Yes, 0=No): ");
                scanf("%d", &student_list[i].is_inc);
                if (student_list[i].is_inc) {
                    // Wipe the GWA when marking as INC so no stale value stays behind
                    student_list[i].gwa = 0.0f;
                    printf("Student marked as INC.\n");
                } else {
                    // Clearing INC means we need to ask for a real GWA now
                    float new_gwa;
                    do {
                        printf("Enter new GWA [1.00 - 5.00]: ");
                        scanf("%f", &new_gwa);
                        if (new_gwa < 1.00f || new_gwa > 5.00f)
                            printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
                    } while (new_gwa < 1.00f || new_gwa > 5.00f);
                    *ptr_gwa = new_gwa;
                    printf("INC cleared. GWA set to %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
                }
            } else if (update_choice == 2) {
                // Block GWA edits if the student is still tagged as INC.
                // The INC flag has to be cleared first or the data won't make sense
                if (student_list[i].is_inc) {
                    printf("  [!] Cannot update GWA, student is currently INC.\n");
                    printf("  Change INC status first (option 1) to assign a GWA.\n");
                } else {
                    float new_gwa;
                    do {
                        printf("Enter new GWA [1.00 - 5.00]: ");
                        scanf("%f", &new_gwa);
                        if (new_gwa < 1.00f || new_gwa > 5.00f)
                            printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
                    } while (new_gwa < 1.00f || new_gwa > 5.00f);
                    *ptr_gwa = new_gwa;
                    printf("GWA updated to %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
                }
            } else {
                printf("Invalid choice. No changes made.\n");
            }
            break; // no point continuing the loop once we found the student
        }
    }

    if (!found)
        printf("Student with ID '%s' does not exist.\n", locate_id);

    savetoFile();
}

void sortbyGWA() {

    // Bubble sort. Not the fastest, but simple and the list size is small so it's fine
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = 0; j < student_count - i - 1; j++) {
            Record *a = &student_list[j];
            Record *b = &student_list[j + 1];

            // INC students always get pushed to the bottom since they have no valid GWA.
            // Among non-INC students, lower GWA value means better standing, so sort ascending
            if ((a->is_inc && !b->is_inc) ||
                (!a->is_inc && !b->is_inc && a->gwa > b->gwa)) {
                Record temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    printf("\n-------SORT BY GWA-------\n");
    printf("Students sorted by GWA.\n\n");

    // Compact view, just the grade-related columns since that's the point of this feature
    printf("%-25s %-20s %-8s %-22s\n", "Name", "ID", "GWA", "Interpretation");
    printf("%-25s %-20s %-8s %-22s\n",
           "-------------------------", "--------------------", "--------", "----------------------");

    for (int i = 0; i < student_count; i++) {
        if (student_list[i].is_inc)
            printf("%-25s %-20s %-8s %-22s\n",
                   student_list[i].name, student_list[i].id, "INC", "Incomplete");
        else
            printf("%-25s %-20s %-8.2f %-22s\n",
                   student_list[i].name, student_list[i].id,
                   student_list[i].gwa, getGradeInterpretation(student_list[i].gwa));
    }
}

void savetoFile() {
    FILE *fp = fopen("students.txt", "w");
    if (fp == NULL) { printf("Error: could not open file.\n"); return; }

    // Walk through the array using a pointer instead of indexing each time
    Record *ptr = student_list;
    for (int i = 0; i < student_count; i++, ptr++) {
        // Pipe-delimited format so fscanf can reliably split fields on load
        fprintf(fp, "%s|%s|%.2f|%d|%d|%s|%s|%s\n",
                ptr->id, ptr->name, ptr->gwa, ptr->is_inc,
                ptr->year_level, ptr->block, ptr->contact_number, ptr->email);
    }
    fclose(fp);
    printf("Records saved.\n");
}

void loadfromFile() {
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL) return; // file just doesn't exist yet on first run, that's normal
    
    // [^|] reads up to the next pipe, [^\n] reads to end of line.
    // fscanf returns the number of fields it matched, so checking for 8 confirms a full row was read
    while (student_count < MAX_STUDENTS &&
           fscanf(fp, "%19[^|]|%49[^|]|%f|%d|%d|%49[^|]|%15[^|]|%99[^\n]\n",
                   student_list[student_count].id,
                   student_list[student_count].name,
                  &student_list[student_count].gwa,
                  &student_list[student_count].is_inc,
                  &student_list[student_count].year_level,
                   student_list[student_count].block,
                   student_list[student_count].contact_number,
                   student_list[student_count].email) == 8) {
        student_count++;
    }
    fclose(fp);
    printf("Records loaded: %d student(s).\n", student_count);
}

// Mirrors the official BU grading scale. The ranges follow the school's actual rubric,
// not something we made up
const char* getGradeInterpretation(float gwa) {
    if      (gwa >= 1.0f && gwa <= 1.4f) return "Outstanding";
    else if (gwa >= 1.5f && gwa <= 1.9f) return "Superior";
    else if (gwa >= 2.0f && gwa <= 2.5f) return "Very Satisfactory";
    else if (gwa >= 2.6f && gwa <= 2.8f) return "Satisfactory";
    else if (gwa >= 2.9f && gwa <= 3.0f) return "Fair / Passing";
    else if (gwa >= 3.1f && gwa <= 4.0f) return "Conditional";
    else if (gwa >  4.0f && gwa <  5.0f) return "Conditional / Failing";
    else if (gwa == 5.0f)                return "Failure";
    else                                 return "Invalid";
}

void displayGradingScale() {
    // Reference table so users don't have to guess what a 2.3 GWA actually means
    printf("\n-------BICOL UNIVERSITY GRADING SCALE-------\n");
    printf("%-12s %-26s %-22s\n", "GWA Range", "Percentage Equivalent", "Adjectival Interpretation");
    printf("%-12s %-26s %-22s\n", "------------", "--------------------------", "----------------------");
    printf("%-12s %-26s %-22s\n", "1.0 - 1.4", "95% - 100%",    "Outstanding");
    printf("%-12s %-26s %-22s\n", "1.5 - 1.9", "90% - 94%",     "Superior");
    printf("%-12s %-26s %-22s\n", "2.0 - 2.5", "84% - 89%",     "Very Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.6 - 2.8", "78% - 83%",     "Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.9 - 3.0", "75% - 77%",     "Fair / Passing");
    printf("%-12s %-26s %-22s\n", "3.1 - 4.0", "Below 75%",     "Conditional");
    printf("%-12s %-26s %-22s\n", "5.0",        "Final Failing", "Failure");
    printf("%-12s %-26s %-22s\n", "INC",        "N/A",           "Incomplete");
    printf("\n");
}