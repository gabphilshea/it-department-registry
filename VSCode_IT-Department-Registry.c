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

//Function declarations
void addStudent();
void viewStudent();
void updateRecord();
void sortbyGPA();

typedef struct Student {
		
		char name[50];    //name of the student
		int id;	      //student ID
		float gwa;	  // general weighted average of the student
		
	} Record;
	

	Record student_list[100];
	int student_count = 0;

//Main function	
int main (){
	

	int choose_option;
	do {
		
		printf("\n1. Add Student\n");
		printf("2. View Student\n");
		printf("3. Update Record\n");
		printf("4. Sort by GPA\n");
		printf("5. Exit\n\n");
		
		printf("Choose option: ");
		scanf("%d", &choose_option);
		
		switch(choose_option) {
			
			case 1:
				addStudent();
				break;
			case 2:
				viewStudent();
				break;
			/*
			case 3:
				updateRecord();
				break;
			case 4:
				sortbyGPA();
				break;
			*/
			case 5:
				printf("You have exited.\n");
				break;
			default: 
				printf("Invalid choice.\n");
		}
		
	} while (choose_option != 5);
	
	
	
}

//Function for adding student in the records
void addStudent(){
	
	printf("\nEnter name: ");
	scanf(" %[^\n]", &student_list[student_count].name);
	printf("Enter ID: ");
	scanf(" %d", &student_list[student_count].id);
	printf("Enter General Weighted Average (GWA): ");
	scanf(" %f", &student_list[student_count].gwa);
	
	printf("\n");
	
	student_count++;
}

//Function for viewing student in the records
void viewStudent(){
	
	printf("\n");
	printf("-------STUDENT RECORDS-------\n");
	printf("Name    ID       GWA\n");
	for (int i = 0; i < student_count; i++){
		printf("%4s   %4d   %4.1f\n", student_list[i].name, student_list[i].id, student_list[i].gwa);
	}
}
void updateRecord();
void sortbyGPA();
