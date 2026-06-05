/*
	C O M P U T E R  P R O G R A M M I N G  2  F I N A L  P R O J E C T
	
	Program Title: The IT Department's Registry
	Program Description: 
		A C-based program that manages database of IT students' academic records for 
		easy monitoring, accessing, and reviewing of students. Developed using Arrays, Structs, Pointer Arithmetic,
		and File I/O.
	Program Version by: Jalmasco, Vince Gabriel P. - Special features (such as incomplete GWA) and Integration of BU grading scale
						Shea, Gabriel Philip M. - Basic code functions (prototyping) before integration of complex functions. Functions and Interface
						Villanueva, Mart Rheymor N. - Pointers and Troubleshooting 
						Zamudio, James Cedrick F. - Filehandling and Troubleshooting
						
	BS INFORMATION TECHNOLOGY 1A
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 1000
#define DB_FILENAME  "students.txt"

// Builds the full absolute path to the database file: $HOME/students.txt
// Falls back to "./students.txt" if HOME is not set.
void getDBPath(char *buf, int bufsize) {
    const char *home = getenv("HOME");
    if (home != NULL)
        snprintf(buf, bufsize, "%s/%s", home, DB_FILENAME);
    else
        snprintf(buf, bufsize, "%s", DB_FILENAME);
}

// Nested struct for student name, split into parts for sorting by last name
typedef struct {
    char firstName[75];   // supports long compound first names
    char middleName[50];  // bumped up from 25 for safety
    char lastName[50];    // bumped up from 25 for safety
} Name;

// Struct to hold everything about a student
typedef struct Student {
    Name name;
    char block[50];
    char id[20];
    char contact_number[12];
    float gwa;
    int  is_inc;
    char inc_reason[100];
    char or_number[20];
} Record;

typedef struct {
    Record student_list[MAX_STUDENTS];
    int student_count;
} Registry;

void addStudent(Registry *reg);
void viewStudent(Registry *reg);
void updateRecord(Registry *reg);
void sortMenu(Registry *reg);
void sortbyGWA(Registry *reg);
void sortbyID(Registry *reg);
void savetoFile(Registry *reg);
void loadfromFile(Registry *reg);
void getValidName(const char *prompt, char *dest, int size, int allowEmpty);
int isDuplicateName(Registry *reg, const char *firstName, const char *middleName, const char *lastName);
void getValidBlock(const char *prompt, char *dest, int size);
void getValidContact(const char *prompt, char *dest, int size);
void fixNameCase(char *str);
void fixUpperCase(char *str);
char toUpperChar(char c);
char toLowerChar(char c);
void clearInputBuffer();
const char* getGradeInterpretation(float gwa);
void displayGradingScale();
void pressEnterToContinue();


// converts a single character to uppercase
char toUpperChar(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 'A';
    return c;
}

// converts a single character to lowercase
char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

// capitalizes the first letter of each word in a string, rest lowercase
void fixNameCase(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (i == 0 || str[i - 1] == ' ')
            str[i] = toUpperChar(str[i]);
        else
            str[i] = toLowerChar(str[i]);
        i++;
    }
}

// converts an entire string to uppercase
void fixUpperCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = toUpperChar(str[i]);
}

// ---------------------------------------------------------------

int main() {
    Registry reg;
    reg.student_count = 0;
    loadfromFile(&reg);

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
        printf("4. Sort Records\n");
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
            case 1: addStudent(&reg); break;
            case 2: viewStudent(&reg); break;
            case 3: updateRecord(&reg); break;
            case 4: sortMenu(&reg); break;
            case 5: displayGradingScale(); break;
            case 6: printf("You have exited.\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choose_option != 6);

    return 0;
}

// extracts the first 4 digits (year) from a student ID like "2025-01-01720"
int getIDYear(const char *id) {
    int year = 0;
    sscanf(id, "%4d", &year);
    return year;
}

// Adds a new student; INC is automatically flagged if GWA is 3.1 - 4.0
void addStudent(Registry *reg) {
    if (reg->student_count >= MAX_STUDENTS) {
        printf("[!] Student list is full.\n");
        return;
    }

    Record *s = &reg->student_list[reg->student_count];

    s->inc_reason[0] = '\0';
    s->or_number[0]  = '\0';

    getValidName("\nEnter first name: ", s->name.firstName, sizeof(s->name.firstName), 0);
    getValidName("Enter middle name (press Enter to skip): ", s->name.middleName, sizeof(s->name.middleName), 1);
    getValidName("Enter last name: ", s->name.lastName, sizeof(s->name.lastName), 0);
	
	if (isDuplicateName(reg, s->name.firstName, s->name.middleName, s->name.lastName)) {

    // find and show the existing match so the user can compare
    for (int i = 0; i < reg->student_count; i++) {
        Record *existing = &reg->student_list[i];
        if (strcmp(existing->name.firstName,  s->name.firstName)  == 0 &&
            strcmp(existing->name.middleName, s->name.middleName) == 0 &&
            strcmp(existing->name.lastName,   s->name.lastName)   == 0) {

            printf("\n  [!] A student with this name already exists:\n");
            printf("      Name : %s %s %s\n",
                   existing->name.firstName,
                   existing->name.middleName,
                   existing->name.lastName);
            printf("      ID   : %s\n", existing->id);
            printf("      Block: %s\n", existing->block);
            break;
        }
    }

    char confirm[5];
    printf("\n  Is this a different person? (y/n): ");
    fgets(confirm, sizeof(confirm), stdin);

    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("  [!] Add student cancelled.\n");
        pressEnterToContinue();
        return;   // abort
    }
    // if 'y', fall through and continue adding normally
}
    int id_valid = 0;
    do {
        printf("Enter Student ID (format: YYYY-XX-XXXXX): ");
        fgets(s->id, sizeof(s->id), stdin);
        s->id[strcspn(s->id, "\n")] = '\0';

        id_valid = 1;
        for (int i = 0; i < reg->student_count; i++) {
            if (strcmp(s->id, reg->student_list[i].id) == 0) {
                printf("  [!] ID already exists. Please enter a unique ID.\n");
                id_valid = 0;
                break;
            }
        }

        if (strlen(s->id) < 12 || s->id[4] != '-' || s->id[7] != '-') {
            printf("  [!] Invalid ID format. Use YYYY-XX-XXXXX.\n");
            id_valid = 0;
        }
    } while (!id_valid);

    // GWA input — INC is automatically determined by the range
    int valid;
    do {
        char buffer[20];
        printf("Enter General Weighted Average (GWA) [1.00 - 5.00]: ");
        fgets(buffer, sizeof(buffer), stdin);
        valid = sscanf(buffer, "%f", &s->gwa);
        if (valid != 1) {
            printf("  [!] Invalid input. Please enter a numeric value.\n");
        } else if (s->gwa < 1.00f || s->gwa > 5.00f) {
            printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
            valid = 0;
        }
    } while (valid != 1 || s->gwa < 1.00f || s->gwa > 5.00f);

    // automatically flag INC if GWA falls in the conditional range
    if (s->gwa >= 3.10f && s->gwa <= 4.00f) {
        s->is_inc = 1;
        printf("  [!] GWA falls under Conditional range (3.1 - 4.0). Student flagged as INC.\n");
        printf("  Enter reason for INC: ");
        fgets(s->inc_reason, sizeof(s->inc_reason), stdin);
        s->inc_reason[strcspn(s->inc_reason, "\n")] = '\0';
    } else {
        s->is_inc = 0;
    }

    getValidBlock("Enter Block: ", s->block, sizeof(s->block));
    getValidContact("Enter Contact Number (11 digits ex: 09xxxxxxxxx): ", s->contact_number, sizeof(s->contact_number));

    reg->student_count++;
    savetoFile(reg);
    printf("[+] Student '%s %s %s' added successfully.\n",
        s->name.firstName, s->name.middleName, s->name.lastName);
    printf("\n");
    pressEnterToContinue();
}

// Displays all students, grouped by ID year (first 4 digits of ID)
void viewStudent(Registry *reg) {
    if (reg->student_count == 0) {
        printf("No student in database.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n-------STUDENT RECORDS-------\n");

    // collect unique years from IDs
    int years[MAX_STUDENTS];
    int year_count = 0;
    for (int i = 0; i < reg->student_count; i++) {
        int y = getIDYear(reg->student_list[i].id);
        int already = 0;
        for (int j = 0; j < year_count; j++) {
            if (years[j] == y) { already = 1; break; }
        }
        if (!already)
            years[year_count++] = y;
    }

    // simple sort of the unique years ascending
    for (int i = 0; i < year_count - 1; i++)
        for (int j = 0; j < year_count - i - 1; j++)
            if (years[j] > years[j + 1]) {
                int tmp = years[j];
                years[j] = years[j + 1];
                years[j + 1] = tmp;
            }

    for (int yi = 0; yi < year_count; yi++) {
        int found = 0;
        for (int i = 0; i < reg->student_count; i++) {
            if (getIDYear(reg->student_list[i].id) == years[yi]) {
                if (!found) {
                    found = 1;
                    printf("\n  [ Batch %d ]\n", years[yi]);
                    // FIX: widened first name column to 30 chars
                    printf("  %-30s %-20s %-20s %-20s %-6s %-22s %-8s %-16s\n",
                           "First Name", "Middle Name", "Last Name", "ID",
                           "GWA", "Interpretation", "Block", "Contact");
                    printf("  %-30s %-20s %-20s %-20s %-6s %-22s %-8s %-16s\n",
                           "------------------------------", "--------------------",
                           "--------------------", "--------------------",
                           "------", "----------------------", "--------", "----------------");
                }

                if (reg->student_list[i].is_inc) {
                    printf("  %-30s %-20s %-20s %-20s %-6s %-22s %-8s %-16s\n",
                           reg->student_list[i].name.firstName,
                           reg->student_list[i].name.middleName,
                           reg->student_list[i].name.lastName,
                           reg->student_list[i].id,
                           "INC", "Incomplete",
                           reg->student_list[i].block,
                           reg->student_list[i].contact_number);
                    if (strlen(reg->student_list[i].inc_reason) > 0)
                        printf("    Reason    : %s\n", reg->student_list[i].inc_reason);
                    if (strlen(reg->student_list[i].or_number) > 0)
                        printf("    OR Number : %s\n", reg->student_list[i].or_number);
                } else {
                    printf("  %-30s %-20s %-20s %-20s %-6.2f %-22s %-8s %-16s\n",
                           reg->student_list[i].name.firstName,
                           reg->student_list[i].name.middleName,
                           reg->student_list[i].name.lastName,
                           reg->student_list[i].id,
                           reg->student_list[i].gwa,
                           getGradeInterpretation(reg->student_list[i].gwa),
                           reg->student_list[i].block,
                           reg->student_list[i].contact_number);
                }
            }
        }
    }

    printf("\n");
    pressEnterToContinue();
}

// Find student by ID, then modify data
void updateRecord(Registry *reg) {
    if (reg->student_count == 0) {
        printf("No student in database.\n");
        pressEnterToContinue();
        return;
    }

    char locate_id[20];

    printf("\n-------UPDATE RECORD-------\n");
    printf("\nInput student's ID: ");
    scanf(" %19s", locate_id);
    clearInputBuffer();

    int found = 0;
    for (int i = 0; i < reg->student_count; i++) {
        if (strcmp(locate_id, reg->student_list[i].id) == 0) {
            found = 1;

            float *ptr_gwa  = &reg->student_list[i].gwa;
            Name  *ptr_name = &reg->student_list[i].name;

            printf("Student: %s %s %s\n",
                   ptr_name->firstName, ptr_name->middleName, ptr_name->lastName);

            if (reg->student_list[i].is_inc) {
                printf("Current Status: INC (Incomplete)\n");
                printf("Reason: %s\n", reg->student_list[i].inc_reason);
                if (strlen(reg->student_list[i].or_number) > 0)
                    printf("OR Number: %s\n", reg->student_list[i].or_number);
            } else {
                printf("Current GWA: %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
            }

            int update_choice;
            printf("\nWhat would you like to update?\n");
            printf("  1. Clear INC status\n");
            printf("  2. Update GWA\n");
            printf("Choose: ");
            scanf("%d", &update_choice);
            clearInputBuffer();

            if (update_choice == 1) {
                if (!reg->student_list[i].is_inc) {
                    printf("  [!] Student is not currently INC.\n");
                } else {
                    printf("Enter OR Number for completion form: ");
                    fgets(reg->student_list[i].or_number, sizeof(reg->student_list[i].or_number), stdin);
                    reg->student_list[i].or_number[strcspn(reg->student_list[i].or_number, "\n")] = '\0';

                    float new_gwa;
                    do {
                        printf("Enter new GWA after completion [1.00 - 3.00]: ");
                        scanf("%f", &new_gwa);
                        clearInputBuffer();
                        if (new_gwa < 1.00f || new_gwa > 3.00f)
                            printf("  [!] Completed grade must be between 1.00 and 3.00.\n");
                    } while (new_gwa < 1.00f || new_gwa > 3.00f);

                    *ptr_gwa = new_gwa;
                    reg->student_list[i].is_inc = 0;
                    printf("INC cleared. OR Number recorded. GWA set to %.2f  (%s)\n",
                           *ptr_gwa, getGradeInterpretation(*ptr_gwa));
                }
            } else if (update_choice == 2) {
                if (reg->student_list[i].is_inc) {
                    printf("  [!] Cannot update GWA, student is currently INC.\n");
                    printf("  Clear INC status first (option 1) to assign a new GWA.\n");
                } else {
                    float new_gwa;
                    do {
                        printf("Enter new GWA [1.00 - 5.00]: ");
                        scanf("%f", &new_gwa);
                        clearInputBuffer();
                        if (new_gwa < 1.00f || new_gwa > 5.00f)
                            printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
                    } while (new_gwa < 1.00f || new_gwa > 5.00f);
                    *ptr_gwa = new_gwa;
                    printf("GWA updated to %.2f  (%s)\n",
                           *ptr_gwa, getGradeInterpretation(*ptr_gwa));
                }
            } else {
                printf("Invalid choice. No changes made.\n");
            }
            break;
        }
    }

    if (!found)
        printf("Student with ID '%s' does not exist.\n", locate_id);

    savetoFile(reg);
    pressEnterToContinue();
}

// User chooses sorting criteria
void sortMenu(Registry *reg) {
    if (reg->student_count == 0) {
        printf("No student in database.\n");
        pressEnterToContinue();
        return;
    }

    char input[10];
    int sort_choice = 0;

    printf("\n-------SORT RECORDS-------\n");
    printf("1. Sort by GWA\n");
    printf("2. Sort by ID\n");
    printf("Choose sort option: ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) return;
    if (sscanf(input, "%d", &sort_choice) != 1) {
        printf("Invalid choice.\n");
        return;
    }

    switch (sort_choice) {
        case 1: sortbyGWA(reg); break;
        case 2: sortbyID(reg); break;
        default: printf("Invalid choice.\n");
    }
}

// Sorts by GWA within each ID year group, INC at bottom of each group
void sortbyGWA(Registry *reg) {
    for (int i = 0; i < reg->student_count - 1; i++) {
        for (int j = 0; j < reg->student_count - i - 1; j++) {
            Record *a = &reg->student_list[j];
            Record *b = &reg->student_list[j + 1];

            int year_a = getIDYear(a->id);
            int year_b = getIDYear(b->id);

            int swap = 0;
            if (year_a != year_b) {
                swap = (year_a > year_b);
            } else {
                if (a->is_inc && !b->is_inc) {
                    swap = 1;
                } else if (!a->is_inc && !b->is_inc) {
                    if (a->gwa > b->gwa)
                        swap = 1;
                    else if (a->gwa == b->gwa)
                        swap = (strcmp(a->name.lastName, b->name.lastName) > 0);
                } else if (a->is_inc && b->is_inc) {
                    swap = (strcmp(a->name.lastName, b->name.lastName) > 0);
                }
            }

            if (swap) {
                Record temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    printf("\n-------SORT BY GWA-------\n");
    printf("Students grouped by batch (ID year), sorted by GWA (INC at bottom of each group).\n\n");

    int current_year = -1;
    for (int i = 0; i < reg->student_count; i++) {
        int y = getIDYear(reg->student_list[i].id);
        if (y != current_year) {
            current_year = y;
            printf("\n  [ Batch %d ]\n", current_year);
            // FIX: widened first name column to 30 chars
            printf("  %-30s %-20s %-20s %-20s %-8s %-22s\n",
                   "First Name", "Middle Name", "Last Name", "ID", "GWA", "Interpretation");
            printf("  %-30s %-20s %-20s %-20s %-8s %-22s\n",
                   "------------------------------", "--------------------",
                   "--------------------", "--------------------",
                   "--------", "----------------------");
        }

        if (reg->student_list[i].is_inc)
            printf("  %-30s %-20s %-20s %-20s %-8s %-22s\n",
                   reg->student_list[i].name.firstName,
                   reg->student_list[i].name.middleName,
                   reg->student_list[i].name.lastName,
                   reg->student_list[i].id,
                   "INC", "Incomplete");
        else
            printf("  %-30s %-20s %-20s %-20s %-8.2f %-22s\n",
                   reg->student_list[i].name.firstName,
                   reg->student_list[i].name.middleName,
                   reg->student_list[i].name.lastName,
                   reg->student_list[i].id,
                   reg->student_list[i].gwa,
                   getGradeInterpretation(reg->student_list[i].gwa));
    }
    printf("\n");
    pressEnterToContinue();
}

// Sorts by full ID ascending, grouped by ID year
void sortbyID(Registry *reg) {
    for (int i = 0; i < reg->student_count - 1; i++) {
        for (int j = 0; j < reg->student_count - i - 1; j++) {
            Record *a = &reg->student_list[j];
            Record *b = &reg->student_list[j + 1];

            int id_cmp   = strcmp(a->id, b->id);
            int name_cmp = strcmp(a->name.lastName, b->name.lastName);
            if (id_cmp > 0 || (id_cmp == 0 && name_cmp > 0)) {
                Record temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    printf("\n-------SORT BY ID-------\n");
    printf("Students sorted by ID (ascending), grouped by batch year.\n\n");

    int current_year = -1;
    for (int i = 0; i < reg->student_count; i++) {
        Record *p = &reg->student_list[i];
        int y = getIDYear(p->id);

        if (y != current_year) {
            current_year = y;
            printf("\n  [ Batch %d ]\n", current_year);
            // FIX: widened first name column to 30 chars
            printf("  %-20s %-30s %-20s %-20s %-6s %-8s\n",
                   "ID", "First Name", "Middle Name", "Last Name", "GWA", "Block");
            printf("  %-20s %-30s %-20s %-20s %-6s %-8s\n",
                   "--------------------", "------------------------------",
                   "--------------------", "--------------------",
                   "------", "--------");
        }

        if (p->is_inc)
            printf("  %-20s %-30s %-20s %-20s %-6s %-8s\n",
                   p->id,
                   p->name.firstName, p->name.middleName, p->name.lastName,
                   "INC", p->block);
        else
            printf("  %-20s %-30s %-20s %-20s %-6.2f %-8s\n",
                   p->id,
                   p->name.firstName, p->name.middleName, p->name.lastName,
                   p->gwa, p->block);
    }
    printf("\n");
    pressEnterToContinue();
}

// Clears leftover characters in input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// validates name input and auto-fixes casing using fixNameCase
void getValidName(const char *prompt, char *dest, int size, int allowEmpty) {
    int valid;
    do {
        valid = 1;
        printf("%s", prompt);
        fgets(dest, size, stdin);
        dest[strcspn(dest, "\n")] = '\0';

        if (strlen(dest) == 0 && !allowEmpty) {
            valid = 0;
            printf("  [!] This field cannot be empty.\n");
            continue;
        }

        if (strlen(dest) == 0 && allowEmpty)
            return;

        for (int i = 0; dest[i] != '\0'; i++) {
            if (dest[i] >= '0' && dest[i] <= '9') {
                valid = 0;
                printf("  [!] Invalid name. Names cannot contain numbers.\n");
                break;
            }
        }

        if (valid)
            fixNameCase(dest);

    } while (!valid);
}
// updated fix where the program checks if there is an exact duplicate 
int isDuplicateName(Registry *reg, const char *firstName, const char *middleName, const char *lastName) {
    for (int i = 0; i < reg->student_count; i++) {
        Record *s = &reg->student_list[i];

        int firstMatch  = strcmp(s->name.firstName,  firstName)  == 0;
        int middleMatch = strcmp(s->name.middleName, middleName) == 0;
        int lastMatch   = strcmp(s->name.lastName,   lastName)   == 0;

        if (firstMatch && middleMatch && lastMatch)
            return 1;   // duplicate found
    }
    return 0;   // no duplicate
}
// validates block input and auto-fixes to uppercase using fixUpperCase
void getValidBlock(const char *prompt, char *dest, int size) {
    int valid;
    do {
        valid = 1;
        printf("%s", prompt);
        fgets(dest, size, stdin);
        dest[strcspn(dest, "\n")] = '\0';

        if (strlen(dest) == 0) {
            valid = 0;
            printf("  [!] Block cannot be empty.\n");
            continue;
        }

        for (int i = 0; dest[i] != '\0'; i++) {
            if ((dest[i] < 'A' || dest[i] > 'Z') && (dest[i] < 'a' || dest[i] > 'z')) {
                valid = 0;
                printf("  [!] Invalid block. Only letters allowed.\n");
                break;
            }
        }

        if (valid)
            fixUpperCase(dest);

    } while (!valid);
}

void getValidContact(const char *prompt, char *dest, int size) {
    int valid;
    do {
        valid = 1;
        printf("%s", prompt);
        fgets(dest, size, stdin);
        dest[strcspn(dest, "\n")] = '\0';

        if (strlen(dest) == 0) {
            valid = 0;
            printf("  [!] Contact number cannot be empty.\n");
        } else if (strlen(dest) != 11) {
            valid = 0;
            printf("  [!] Contact number must be exactly 11 digits.\n");
        }

        for (int i = 0; dest[i] != '\0'; i++) {
            if (dest[i] < '0' || dest[i] > '9') {
                valid = 0;
                printf("  [!] Invalid contact number. Only digits allowed.\n");
                break;
            }
        }
    } while (!valid);
}

void displayGradingScale() {
    printf("\n-------BICOL UNIVERSITY GRADING SCALE-------\n");
    printf("%-12s %-26s %-22s\n", "GWA Range", "Percentage Equivalent", "Adjectival Interpretation");
    printf("%-12s %-26s %-22s\n", "------------", "--------------------------", "----------------------");
    printf("%-12s %-26s %-22s\n", "1.0 - 1.4",  "95% - 100%",   "Outstanding");
    printf("%-12s %-26s %-22s\n", "1.5 - 1.9",  "90% - 94%",    "Superior");
    printf("%-12s %-26s %-22s\n", "2.0 - 2.5",  "84% - 89%",    "Very Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.6 - 2.8",  "78% - 83%",    "Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.9 - 3.0",  "75% - 77%",    "Fair / Passing");
    printf("%-12s %-26s %-22s\n", "3.1 - 4.0",  "Below 75%",    "Conditional / INC");
    printf("%-12s %-26s %-22s\n", "5.0",         "Final Failing","Failure");
    printf("%-12s %-26s %-22s\n", "INC",         "N/A",          "Incomplete");
    printf("\n");
}

// savetoFile includes inc_reason and or_number
void savetoFile(Registry *reg) {
    char path[512];
    getDBPath(path, sizeof(path));
    FILE *fp = fopen(path, "w");
    if (fp == NULL) { printf("Error: could not open file.\n"); return; }

    for (int i = 0; i < reg->student_count; i++) {
        Record *p = &reg->student_list[i];
        fprintf(fp, "%s|%s|%s|%s|%.2f|%d|%s|%s|%s|%s\n",
                p->id,
                p->name.firstName,
                p->name.middleName,
                p->name.lastName,
                p->gwa,
                p->is_inc,
                p->block,
                p->contact_number,
                p->inc_reason,
                p->or_number);
    }
    fclose(fp);
}

// Safely copies a token into dest, or empty string if token is NULL
static void safeToken(char *dest, int destSize, char *token) {
    if (token != NULL)
        strncpy(dest, token, destSize - 1);
    else
        dest[0] = '\0';
    dest[destSize - 1] = '\0';
}

// loadfromFile — uses fgets + manual pipe splitting so empty fields (e.g.
// missing middle name stored as "||") are handled correctly.
// fscanf's %[^|] requires at least one character and silently fails on "||",
// which caused the entire database to stop loading whenever a student had
// no middle name.
void loadfromFile(Registry *reg) {
    char path[512];
    getDBPath(path, sizeof(path));
    FILE *fp = fopen(path, "r");
    if (fp == NULL) return;

    reg->student_count = 0;
    char line[512];

    while (reg->student_count < MAX_STUDENTS && fgets(line, sizeof(line), fp) != NULL) {
        // strip trailing newline
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        Record *s = &reg->student_list[reg->student_count];
        s->inc_reason[0] = '\0';
        s->or_number[0]  = '\0';

        // Split on '|' — strtok collapses empty tokens, so we walk manually
        // Fields: id|firstName|middleName|lastName|gwa|is_inc|block|contact|inc_reason|or_number
        char *fields[10];
        int   field_count = 0;
        char *p = line;

        while (field_count < 10) {
            fields[field_count++] = p;
            char *pipe = strchr(p, '|');
            if (pipe == NULL) break;
            *pipe = '\0';   // terminate this field
            p = pipe + 1;   // advance past the '|'
        }

        // Need at least 8 fields to be a valid record
        if (field_count < 8) continue;

        safeToken(s->id,              sizeof(s->id),              fields[0]);
        safeToken(s->name.firstName,  sizeof(s->name.firstName),  fields[1]);
        safeToken(s->name.middleName, sizeof(s->name.middleName), fields[2]);
        safeToken(s->name.lastName,   sizeof(s->name.lastName),   fields[3]);
        s->gwa    = (field_count > 4) ? (float)atof(fields[4]) : 0.0f;
        s->is_inc = (field_count > 5) ? atoi(fields[5])        : 0;
        safeToken(s->block,           sizeof(s->block),           field_count > 6 ? fields[6] : NULL);
        safeToken(s->contact_number,  sizeof(s->contact_number),  field_count > 7 ? fields[7] : NULL);
        safeToken(s->inc_reason,      sizeof(s->inc_reason),      field_count > 8 ? fields[8] : NULL);
        safeToken(s->or_number,       sizeof(s->or_number),       field_count > 9 ? fields[9] : NULL);

        fixNameCase(s->name.firstName);
        fixNameCase(s->name.middleName);
        fixNameCase(s->name.lastName);
        fixUpperCase(s->block);

        reg->student_count++;
    }
    fclose(fp);
}

// Returns a description for GWA
const char* getGradeInterpretation(float gwa) {
    if (gwa >= 1.00f && gwa <= 1.49f)      return "Outstanding";
    else if (gwa >= 1.50f && gwa <= 1.99f) return "Superior";
    else if (gwa >= 2.00f && gwa <= 2.49f) return "Very Satisfactory";
    else if (gwa >= 2.50f && gwa <= 2.99f) return "Satisfactory";
    else if (gwa >= 3.00f && gwa <= 3.09f) return "Fair / Passing";
    else if (gwa >= 3.10f && gwa <= 4.00f) return "Conditional";
    else if (gwa >= 4.10f && gwa <= 5.00f) return "Failure";
    else                                   return "Invalid";
}

// Waits for user to press Enter
void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    getchar();
}
