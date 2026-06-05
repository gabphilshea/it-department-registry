The IT Department's Registry

A mini system program created in C language that simulates a database for the registry of the IT Department of Bicol University. 
In this program, a designated personnel can add students and view information about that student, such as the Year Level, ID, Contact Number,
and their General Weighted Average (GWA).


Instructions for compiling
Open the .c file in Dev-C++
Under 'Execute' in the navigation bar, look for compile and run
Or, press F11 (or Fn + 11) to compile and run easily


Launching the Program after compiling
Upon compiling, a terminal log or CMD will pop up on your screen, then a main menu will greet you.

Main Menu:

--------------------------------------------------------------------------------

	       Bicol University College of Science
                Legazpi City, Albay
     Department of Information Technology Registry

                   STUDENT DATABASE

1. Add Student
2. View Student
3. Update Record
4. Sort Records
5. View Grading Scale
6. Exit

Choose option: 

--------------------------------------------------------------------------------

On the choose option:

Input 1 if you want to add a student on the database
Input 2 if you want to view all records of student in the database
Input 3 if you want to update an existing student's record in the database
Input 4 if you want to sort records (either by GWA or by ID) 
Input 5 to view the Bicol University Grading Scale
Input 6 to exit the program




PROGRAM WALKTHROUGH


Add student

------------------------------------------------------------------------------

Enter first name: Juan
Enter middle name (press Enter to skip): Miguel
Enter last name: Dela Cruz
Enter Student ID (format: YYYY-XX-XXXXX): 2025-01-12345
Enter General Weighted Average (GWA) [1.00 - 5.00]: 1.2
Enter Block: A
Enter Contact Number (11 digits ex: 09xxxxxxxxx): 09112345678
[+] Student 'Juan Miguel Dela Cruz' added successfully.

------------------------------------------------------------------------------


2. View Student

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-------STUDENT RECORDS-------

  [ Batch 2025 ]
  First Name                     Middle Name          Last Name            ID                   GWA    Interpretation         Block    Contact
  ------------------------------ -------------------- -------------------- -------------------- ------ ---------------------- -------- ----------------
  Gabriel Philip                 Matubis              Shea                 2025-01-05589        1.30   Outstanding            A        09682448172
  Juan                           Miguel               Dela Cruz            2025-01-12345        1.20   Outstanding            A        09112345678

  [ Batch 2026 ]
  First Name                     Middle Name          Last Name            ID                   GWA    Interpretation         Block    Contact
  ------------------------------ -------------------- -------------------- -------------------- ------ ---------------------- -------- ----------------
  Juan                                                Dela Cruz            2026-01-20275        1.30   Outstanding            1        A
  Maria                          Santos               Reyes                2026-01-20276        1.50   Superior               1        A
  Carlo                                               Bautista             2026-01-20277        2.10   Very Satisfactory      1        B


-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


3. Update Record

// Attempt to update record (Clear INC status) of non-incomplete student

-------UPDATE RECORD-------

Input student's ID: 2025-01-12345
Student: Juan Miguel Dela Cruz
Current GWA: 1.20  (Outstanding)

What would you like to update?
  1. Clear INC status
  2. Update GWA
Choose: 1
  [!] Student is not currently INC.

// Attempt to update record (Clear INC status) of incomplete student

-------UPDATE RECORD-------

Input student's ID: 2025-01-02234
Student: Mart Rheymor Nalzaro Villanueva
Current Status: INC (Incomplete)
Reason: No defense

What would you like to update?
  1. Clear INC status
  2. Update GWA
Choose: 1
Enter OR Number for completion form: 060526001
Enter new GWA after completion [1.00 - 3.00]: 1.2
INC cleared. OR Number recorded. GWA set to 1.20  (Outstanding)


// Attempt to update GWA of student

-------UPDATE RECORD-------

Input student's ID: 2025-01-05589
Student: Gabriel Philip Matubis Shea
Current GWA: 1.30  (Outstanding)

What would you like to update?
  1. Clear INC status
  2. Update GWA
Choose: 2
Enter new GWA [1.00 - 5.00]: 1.5
GWA updated to 1.50  (Superior)


4. Sort Records


Sortation of records by GWA

-------SORT RECORDS-------
1. Sort by GWA
2. Sort by ID
Choose sort option: 1

-------SORT BY GWA-------
Students grouped by batch (ID year), sorted by GWA (INC at bottom of each group).


  [ Batch 2025 ]
  First Name                     Middle Name          Last Name            ID                   GWA      Interpretation
  ------------------------------ -------------------- -------------------- -------------------- -------- ----------------------
  Juan                           Miguel               Dela Cruz            2025-01-12345        1.20     Outstanding
  Mart Rheymor                   Nalzaro              Villanueva           2025-01-02234        1.20     Outstanding
  Gabriel Philip                 Matubis              Shea                 2025-01-05589        1.50     Superior

  [ Batch 2026 ]
  First Name                     Middle Name          Last Name            ID                   GWA      Interpretation
  ------------------------------ -------------------- -------------------- -------------------- -------- ----------------------
  Juan                                                Dela Cruz            2026-01-20275        1.30     Outstanding
  Maria                          Santos               Reyes                2026-01-20276        1.50     Superior
  Carlo                                               Bautista             2026-01-20277        2.10     Very Satisfactory


Sortation of records by ID

-------SORT RECORDS-------
1. Sort by GWA
2. Sort by ID
Choose sort option: 2

-------SORT BY ID-------
Students sorted by ID (ascending), grouped by batch year.


  [ Batch 2025 ]
  ID                   First Name                     Middle Name          Last Name            GWA    Block
  -------------------- ------------------------------ -------------------- -------------------- ------ --------
  2025-01-02234        Mart Rheymor                   Nalzaro              Villanueva           1.20   B
  2025-01-05589        Gabriel Philip                 Matubis              Shea                 1.50   A
  2025-01-12345        Juan                           Miguel               Dela Cruz            1.20   A

  [ Batch 2026 ]
  ID                   First Name                     Middle Name          Last Name            GWA    Block
  -------------------- ------------------------------ -------------------- -------------------- ------ --------
  2026-01-20275        Juan                                                Dela Cruz            1.30   1
  2026-01-20276        Maria                          Santos               Reyes                1.50   1
  2026-01-20277        Carlo                                               Bautista             2.10   1


5. View Grading Scale

-------BICOL UNIVERSITY GRADING SCALE-------
GWA Range    Percentage Equivalent      Adjectival Interpretation
------------ -------------------------- ----------------------
1.0 - 1.4    95% - 100%                 Outstanding
1.5 - 1.9    90% - 94%                  Superior
2.0 - 2.5    84% - 89%                  Very Satisfactory
2.6 - 2.8    78% - 83%                  Satisfactory
2.9 - 3.0    75% - 77%                  Fair / Passing
3.1 - 4.0    Below 75%                  Conditional / INC
5.0          Final Failing              Failure
INC          N/A                        Incomplete


------------------------------------------------- E N D  O F  W A L K T H R O U G H ----------------------------------------------------------



File Structure

IT-Department-Registry/
├── IT-Department-Registry.c    — main source code
├── students.txt                — auto-generated database file
└── README.md                   — this file


Limitations

Program is set to a maximum of 1000 students
Contact number is based on the Philippines that uses 11-digit formatting
ID must follow the Bicol University ID format YYYY-XX-XXXXX


Library utilization
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

** No need for an external library









