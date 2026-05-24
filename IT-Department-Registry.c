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


//Function declarations
void addStudent();
void viewStudent();
void updateRecord();
void sortbyGWA();
void savetoFile();
void loadfromFile();
const char* getGradeInterpretation(float gwa);
void displayGradingScale();


typedef struct Student {
    char name[50];           // name of the student
    char block[2];          // student's assigned block
    int year_level;        // student's year level
    int id;                  // student ID
    char contact_number[11];  // contact number 
    float gwa;               // general weighted average 
    int is_inc;
    char email[100];         // email address 
} Record;                    
	
	//Global variables
	Record student_list[100];
	int student_count = 0;

//Main function	

int main (){
	
	loadfromFile();
	
	system("color 1F");
	
	printf("\n");
	printf("%45s\n", "Bicol University College of Science");
	printf("%35s\n", "Legazpi City, Albay");
	printf("%50s\n","Department of Information Technology Registry");
	printf("%35s\n", "===================================================================");
	printf("\n\n%35s\n","STUDENT DATABASE");
	int choose_option;
	do {
		
		printf("\n1. Add Student\n");
		printf("2. View Student\n");
		printf("3. Update Record\n");
		printf("4. Sort by GPA\n");
		printf("5. View Grading Scale\n");
		printf("6. Exit\n\n");
		
		printf("Choose option: ");
		scanf("%d", &choose_option);
		
		switch(choose_option) {
			
			case 1:
				addStudent();
				break;
			case 2:
				viewStudent();
				break;
			
			case 3:
				updateRecord();
				break;
		
			case 4:
				sortbyGWA();
				break;
			
			case 5:
				displayGradingScale();
				break;
				
			case 6:
				printf("You have exited.\n");
				break;
				
			default:
				printf("Invalid choice.\n");
		}
		
	} while (choose_option != 6);
	
	
	
}

//Function for adding student in the records
void addStudent(){
	
	printf("\nEnter name: ");
	scanf(" %[^\n]", student_list[student_count].name);
	printf("Enter ID: ");
	scanf(" %d", &student_list[student_count].id);
	printf("Is this student Incomplete (INC)? (1=Yes, 0=No): ");
	scanf(" %d", &student_list[student_count].is_inc);
	
	if (student_list[student_count].is_inc) {
		student_list[student_count].gwa = 0.0f;
		printf("  [!] GWA skipped, student marked as INC.\n");
	} else {
		do {
			printf("Enter General Weighted Average (GWA) [1.00 - 5.00]: ");
			scanf(" %f", &student_list[student_count].gwa);
			if (student_list[student_count].gwa < 1.00 || student_list[student_count].gwa > 5.00)
				printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
		} while (student_list[student_count].gwa < 1.00 || student_list[student_count].gwa > 5.00);
	}
	
	printf("Enter Block: ");
	scanf(" %s", &student_list[student_count].block);
	printf("Enter Year Level: ");
	scanf(" %d", &student_list[student_count].year_level);
	printf("Enter Contact Number: ");
	scanf(" %s", &student_list[student_count].contact_number);
	printf("Enter Email: ");
	scanf(" %s", &student_list[student_count].email);
	
	printf("\n");
	
	student_count++;
	savetoFile();
}

//Function for viewing student in the records
void viewStudent(){
	
		if (student_count) {
			printf("\n");
			printf("-------STUDENT RECORDS-------\n");
			printf("%-20s %-10s %-8s %-22s %-12s %-15s %-15s %-19s\n",
	       "Name", "ID", "GWA", "Interpretation", "Year Level", "Block", "Contact Number", "Email");
			printf("%-20s %-10s %-8s %-22s %-12s %-15s %-15s %-19s\n",
	       "--------------------", "----------", "--------",
	       "----------------------", "------------", "---------------",
	       "---------------", "-------------------");
		}
		
		for (int i = 0; i < student_count; i++){
			if (student_list[i].is_inc) {
			printf("%-20s %-10d %-8s %-22s %-12d %-15s %-15s %-19s\n",
			       student_list[i].name,
			       student_list[i].id,
			       "INC",
			       "Incomplete",
			       student_list[i].year_level,
			       student_list[i].block,
			       student_list[i].contact_number,
			       student_list[i].email);
		} else {
			printf("%-20s %-10d %-8.2f %-22s %-12d %-15s %-15s %-19s\n",
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
	
	if (student_count == 0) 
		printf("No student in database.\n");
	
}
void updateRecord(){
	
	int locate_id;
	if (student_count != 0){
		printf("\n-------UPDATE GWA-------\n");
		printf("\nInput student's ID:  ");
		scanf("%d", &locate_id);
	}
		
	if (student_count){
		for (int i = 0; i < student_count; i++){
		
		if (locate_id == student_list[i].id){
			
			float *ptr_gwa = &student_list[i].gwa;
			char *ptr_name = student_list[i].name;
			printf("%s", ptr_name);
			printf("\nCurrent GWA: %.1f\n", *ptr_gwa);
			
			int found = 0;
			for (int i = 0; i < student_count; i++){
				if (locate_id == student_list[i].id){
					found = 1;
					float *ptr_gwa = &student_list[i].gwa;
					char  *ptr_name = student_list[i].name;
					printf("Student: %s\n", ptr_name);
		
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
							student_list[i].gwa = 0.0f;
							printf("Student marked as INC.\n");
						} else {
							float new_gwa;
							do {
								printf("Enter new GWA [1.00 - 5.00]: ");
								scanf("%f", &new_gwa);
								if (new_gwa < 1.00 || new_gwa > 5.00)
									printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
							} while (new_gwa < 1.00 || new_gwa > 5.00);
							*ptr_gwa = new_gwa;
							printf("INC cleared. GWA set to %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
						}
					} else if (update_choice == 2) {
						if (student_list[i].is_inc) {
							printf("  [!] Cannot update GWA, student is currently INC.\n");
							printf("  Change INC status first (option 1) to assign a GWA.\n");
						} else {
							float new_gwa;
							do {
								printf("Enter new GWA [1.00 - 5.00]: ");
								scanf("%f", &new_gwa);
								if (new_gwa < 1.00 || new_gwa > 5.00)
									printf("  [!] Invalid GWA. Must be between 1.00 and 5.00.\n");
							} while (new_gwa < 1.00 || new_gwa > 5.00);
							*ptr_gwa = new_gwa;
							printf("GWA updated to %.2f  (%s)\n", *ptr_gwa, getGradeInterpretation(*ptr_gwa));
						}
					} else {
						printf("Invalid choice. No changes made.\n");
					}
					
						if (!found)
							printf("Student with inputted ID does not exist.\n");
						
						break;
					}
				} 	
			}
		}

	}
	
		if (student_count == 0) 
			printf("No student in database.\n");
		
		savetoFile();
		return;
	}

void sortbyGWA(){
	
	if (student_count){
		
		for (int i = 0; i < student_count - 1; i++){
			for (int j = 0; j < student_count - i - 1; j++){
				
				Record *a = &student_list[j];
				Record *b =  &student_list[j+1];
				
				if ((a->is_inc && !b->is_inc) || (!a->is_inc && !b->is_inc && a->gwa > b->gwa)){
					Record temp = *a;
					*a = *b;
					*b = temp;	
				}	
		}
	
	}
	
	printf("\n-------SORT BY GWA-------\n");
	printf("Students sorted by GWA.\n");
	
	printf("\n%-25s %-10s %-8s %-22s\n", "Name", "ID", "GWA", "Interpretation");
	printf("%-25s %-10s %-8s %-22s\n", "-------------------------", "----------", "--------", "----------------------");
		
	for (int i = 0; i < student_count; i++) {
			if (student_list[i].is_inc)
				printf("%-25s %-10d %-8s %-22s\n", student_list[i].name, student_list[i].id, "INC", "Incomplete");
			else
				printf("%-25s %-10d %-8.2f %-22s\n", student_list[i].name, student_list[i].id, student_list[i].gwa, getGradeInterpretation(student_list[i].gwa));
		}
	
	}
	if (student_count == 0) 
		printf("No student in database.\n");	
}

void savetoFile() {
    FILE *fp = fopen("students.txt", "w");
    if (fp == NULL) { printf("Error: could not open file.\n"); return; }

    Record *ptr = student_list;              // pointer arithmetic
    for (int i = 0; i < student_count; i++, ptr++) {
        fprintf(fp, "\nStudent ID: %d\nStudent Name: %s\nYear Level: %d\nBlock: %s\nContact No.: %s\nEmail Address: %s\nGWA: %.1f\nIncomplete Status: %d\n", ptr->id, ptr->name, ptr->year_level, ptr->block, ptr->contact_number, ptr->email, ptr->gwa,  ptr->is_inc);
    }
    fclose(fp);
    
}
void loadfromFile() {
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL) return;                 

    while (student_count < 100 &&
           fscanf(fp, "\nStudent ID: %d\nStudent Name: %s\nYear Level: %d\nBlock: %s\nContact No.: %s\nEmail Address: %s\nGWA: %.1f\n",
                  &student_list[student_count].id,
                   student_list[student_count].name,
				  &student_list[student_count].year_level,
				  &student_list[student_count].block,
				  student_list[student_count].contact_number,
				  student_list[student_count].email,
				  &student_list[student_count].gwa,
				  &student_list[student_count].is_inc) == 8) {
        student_count++;
    }
    fclose(fp);
    
}

// For integration of the Bicol University Grading Scale/Categories
const char* getGradeInterpretation(float gwa) {
    if (gwa >= 1.0f && gwa <= 1.4f) 
		return "Outstanding";
    else if (gwa >= 1.5f && gwa <= 1.9f) 
		return "Superior";
    else if (gwa >= 2.0f && gwa <= 2.5f) 
		return "Very Satisfactory";
    else if (gwa >= 2.6f && gwa <= 2.8f) 
		return "Satisfactory";
    else if (gwa >= 2.9f && gwa <= 3.0f) 
		return "Fair / Passing";
    else if (gwa >= 3.1f && gwa <= 4.0f)
		return "Conditional";
    else if (gwa == 5.0f)                
		return "Failure";
    else                                 
		return "Invalid";
}

void displayGradingScale() {
	
    printf("\n-------BICOL UNIVERSITY GRADING SCALE-------\n");
    printf("%-12s %-26s %-22s\n", "GWA Range", "Percentage Equivalent", "Adjectival Interpretation");
    printf("%-12s %-26s %-22s\n", "------------", "--------------------------", "----------------------");
    printf("%-12s %-26s %-22s\n", "1.0 - 1.4",  "95% - 100%",    "Outstanding");
    printf("%-12s %-26s %-22s\n", "1.5 - 1.9",  "90% - 94%",     "Superior");
    printf("%-12s %-26s %-22s\n", "2.0 - 2.5",  "84% - 89%",     "Very Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.6 - 2.8",  "78% - 83%",     "Satisfactory");
    printf("%-12s %-26s %-22s\n", "2.9 - 3.0",  "75% - 77%",     "Fair / Passing");
    printf("%-12s %-26s %-22s\n", "3.1 - 4.0",  "Below 75%",     "Conditional");
    printf("%-12s %-26s %-22s\n", "5.0",         "Final Failing", "Failure");
    printf("%-12s %-26s %-22s\n", "INC",         "N/A",           "Incomplete");
    printf("\n");
    
}
