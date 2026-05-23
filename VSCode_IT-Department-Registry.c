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

#define MAX_STUDENTS 100
#define FILENAME "student_records.txt"

typedef struct Student {
		
		char name[50];    //name of the student
		int id;	      //student ID
		float gwa;	  // general weighted average of the student
		
	} Record;
Record student_list[MAX_STUDENTS]; // array to store student records
int count = 0; // variable to keep track of the number of students in the registry

//function prototypes
void addStudent();
void displayStudents();
void searchByID(int target_id);
void saveToFile();
void loadFromFile();

void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) { printf("Error: could not open file.\n"); return; }

    Record *ptr = student_list;              // pointer arithmetic
    for (int i = 0; i < count; i++, ptr++) {
        fprintf(fp, "%d|%s|%.2f\n", ptr->id, ptr->name, ptr->gwa);
    }
    fclose(fp);
    printf("Records saved.\n");
}

// FILE I/O function to load student records from a file
void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) return;                 

    count = 0;
    while (count < MAX_STUDENTS &&
           fscanf(fp, "%d|49[^|]|%f\n",
                  &student_list[count].id,
                   student_list[count].name,
                  &student_list[count].gwa) == 3) {
        count++;
    }
    fclose(fp);
    printf("Records loaded: %d student(s).\n", count);
}

    for (int i = 0; i < count; i++, ptr++) {
        fprintf(fp, "%d|%s|%.2f\n", ptr->id, ptr->name, ptr->gwa);
    }
    fclose(fp);
    printf("Records saved.\n");
}
void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) return;                 

    count = 0;
    while (count < MAX_STUDENTS &&
           fscanf(fp, "%d|49[^|]|%f\n",
                  &student_list[count].id,
                   student_list[count].name,
                  &student_list[count].gwa) == 3) {
        count++;
    }
    fclose(fp);
    printf("Records loaded: %d student(s).\n", count);
}

//Main function	
int main (){
	
	Record student_list[4] = {
	
		{"Gabriel Philip Shea", 2026001, 1.4},
		{"Mart Rheymor Villanueva", 2026002, 1.5},
		{"James Cedrick Zamudio", 2026003, 1.2},
		{"Vince Gabriel Jalmasco", 2026004, 1.3}
	
	};

	for (int i = 0; i < 4; i++) {
	printf("Name: %s\n", student_list[i].name);
	printf("ID: %d\n", student_list[i].id);
	printf("GWA: %.1f\n", student_list[i].gwa);
	}
	
	Record *ptr = student_list; // pointer to the first element of the array
	for (int i = 0; i < 4; i++) {
		printf("Name: %s\n", ptr->name);
		printf("ID: %d\n", ptr->id);
		printf("GWA: %.1f\n", ptr->gwa);
		ptr++; // move the pointer to the next element
	}
	
}

void searchByID(int target_id, Record *student_list, int size) {
	for (int i = 0; i < size; i++) {
		if (student_list[i].id == target_id) {
			printf("Name: %s\n", student_list[i].name);
			printf("ID: %d\n", student_list[i].id);
			printf("GWA: %.1f\n", student_list[i].gwa);
			return;
		}
	}
	printf("Student with ID %d not found.\n", target_id);
}
