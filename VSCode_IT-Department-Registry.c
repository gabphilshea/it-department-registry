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

typedef struct Student {
		
		char name[20];    //name of the student
		int id;	      //student ID
		float gwa;	  // general weighted average of the student
		
	} Record;

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
	
	
	
	
}