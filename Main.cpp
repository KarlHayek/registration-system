#include <iostream>
#include <iomanip>
#include "Registrar.h"
using namespace std;

void StudentRegistrationSystem(Student* student, Registrar& sis);
void InstructorInformationSystem(Instructor* instructor, Registrar& sis);
void AdministratorInformationSystem(Registrar& sis);

int main()
{
//  Administrator ID: 0

	Registrar sis;
	string studentID, instructorID, administratorID; Student* student;
	Instructor* instructor; int choice;
	string init = "The semester has been initialized and the add/drop period has begun.\n";

	while (true)
	{
		cout<<">>>>>> Welcome to the University's Information System! "<<init<<"You are a:\n\t\t1 - Student\n\t\t2 - Instructor\n\t\t3 - Administrator\n\t\t4 - Quit\n";
		cout<<">>>>>> "; cin>>choice; init = "";
		if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');} // Prevents errors when the input is wrong
		
		if (choice == 1)		// Student
		{
			cout<<"\n>>> Please enter your student ID to log in: "; cin>>studentID;
			if (sis.getStudent(studentID, student)){
				cout<<"\n>>> Welcome to the student registration system, "<<student->getSName()<<"!\n\n";
				StudentRegistrationSystem(student, sis);
			}
			else cout<<"\n|!| The ID you entered could not be found. Please try again.\n\n"<<endl;
		}
		
		
		else if (choice == 2)	// Instructor
		{
			cout<<"\n>>> Please enter your instructor ID to log in: "; cin>>instructorID;
			if (sis.getInstructor(instructorID, instructor)){
				cout<<"\n>>> Welcome to the instructor information system, "<<instructor->getName()<<"!\n\n";
				InstructorInformationSystem(instructor, sis);
			}
			else cout<<"\n|!| The ID you entered could not be found. Please try again.\n\n"<<endl;
		}
		
		else if (choice == 3)	// Registrar
		{
			cout<<"\n>>> Please enter your adminstrator ID to log in: "; cin>>administratorID;	
			if (administratorID == "0"){
				cout<<"\n>>> Welcome to the administrator information system!\n\n";
				AdministratorInformationSystem(sis);
			}
			else cout<<"\n|!| The ID you entered could not be found. Please try again.\n\n"<<endl;
		}
		
		else if (choice == 4) break; 	// Quit
		
		else cout<<"|!| Error: Invalid input. Please enter a number between 1 and 4.\n\n"<<endl;
	}
}




string getSectionOfACourse_input(string courseCode, Registrar& sis)
{
	string sectionNumber = "1"; int nbOfSections = sis.getNbOfSections(courseCode);
	if (nbOfSections > 1)	// if the course has multiple sections
	{
		cout<<">>> There are "<<nbOfSections<<" sections for this course. Please enter the desired section number of the course: ";
		while (true) {
			cin>>sectionNumber;
			if (atoi(sectionNumber.c_str()) >= 1 && atoi(sectionNumber.c_str()) <= nbOfSections) 	break;
			else cout<<"|!| Error: Please enter a number between 1 and "<<nbOfSections<<": ";
		}
	}
	return sectionNumber;
}




void AdministratorInformationSystem(Registrar& sis)
{
	string studentID, studentName, studentProgram, instructorID, courseCode; bool found;
	Student* student; CourseOffering* course; Instructor* instructor; Course* courseGeneral;
	int choice, nbOfPrograms = 4; string Programs[4] = { "IT","SE","BT","SA"}, sectionNumber;
	
	while (true)
	{
		cout<<">>> Please enter what you would like to do:\n\t1 - Perform end-of-semester procedures\n\t2 - Admit a student\n\t3 - Assign a course to an instructor\n\t4 - End add/drop period\n\t5 - Add student grades from courses-grades file\n\t6 - View the information of a student\n\t7 - View the information of an instructor\n\t8 - View the information of a course\n\t9 - View all students\n\t10 - View all instructors\n\t11 - View all courses currently offered\n\t12 - View all courses\n\t13 - Log out\n";
		cout<<">>> "; cin>>choice;
		if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');} // Prevents errors when the input is wrong
		
		
		if (choice == 1)			// Perform end-of-semester procedures
		{
			sis.endOfSemesterProcedures();
			cout<<"\nMoved students' registered courses to taken courses/failed courses and updated student statuses.\nEmptied instructors' course assignments.\nEmptied offered courses' students and instructors.\nNew semester initialized."<<endl;
		}
		
		
		else if (choice == 2)		// Admit a student
		{
			cout<<">>> Please enter the ID of the student you would like to admit: "; cin>>studentID;
			if (!(studentID.length() == 9 && studentID.substr(0,2) == "20" && atoi(studentID.c_str())))
				{ cout<<"\n|!| Error: Please enter an ID of this format: 20XXXXXX (9 digits in total).\n\n"; continue; }
			if (sis.getStudent(studentID, student))
				{ cout<<"\n|!| Error: This ID already exists.\n\n"; continue; }
			
			cout<<">>> Please enter the Name of the student you would like to admit: "; cin.ignore(1); getline(cin, studentName);
			
			cout<<">>> Please enter the Program of the student you would like to admit: "; cin>>studentProgram;
			found = false;
			for (int i = 0; i < nbOfPrograms; i++)
				if (studentProgram == Programs[i])	found = true;
				
			if (!found) {
				cout<<"\n|!| Error: Please enter one of these programs: ";
				for (int i = 0; i < nbOfPrograms; i++) {
					cout<<Programs[i];
					if (i < nbOfPrograms-1) cout<<", ";
				}
				cout<<".\n\n"; continue;
			}
	 		sis.AdmitStudent(new Student(studentID, studentName, studentProgram, "Clear"));
			cout<<"\nThe student '"<<studentName<<"' with ID "<<studentID<<" was admitted to the "<<studentProgram<<" program.";	
		}
		
		
		else if (choice == 3)		// Assign a course to an instructor
		{
			cout<<"Instructors who can give a course:\n\n"<< "Instructor ID |    Instructor Name     |      Title      |    Specialty   |   Courses Given";
			sis.print_instructorsWhoCanGiveACourse();

			cout<<"\nCourse sections that haven't been assigned an instructor yet:\n\n"<<"Course Code   |    Date/Time   |   Available slots  |  Section\n";
			sis.print_courseSectionsWithNoInstructor();

			cout<<"\n>>> Please enter the code of the course would like to assign: "; cin>>courseCode; cout<<endl;
			if (!sis.getCourse(courseCode)) { cout<<"|!| Error: The course whose code you entered could not be found.\n\n"; continue; }
			
			string sectionNumber = getSectionOfACourse_input(courseCode, sis); course = sis.getCourse(courseCode, sectionNumber);
			if (course->getInstructorID() != "") 
				{ cout<<"|!| Error: The section of the course whose code you entered is already being given by an instructor.\n\n"; continue; }
			
			cout<<">>> Please enter the ID of the instructor you would like to assign the course to: "; cin>>instructorID; cout<<endl;
			
			if (!sis.getInstructor(instructorID, instructor)) cout<<"|!| Error: The instructor whose ID you entered could not be found.";
			else if (instructor->getNbCoursesGivingNow() >= 2)	cout<<"|!| Error: The instructor whose ID you entered is already giving two courses and cannot give one more course.";

			else if (sis.registerCourse_Instructor(instructorID, courseCode, sectionNumber))
				cout<<"The section "<<sectionNumber<<" of the course "<<courseCode<<" was assigned to the instructor with ID "<<instructorID<<".";
		}
		
		else if (choice == 4)		// End add/drop period
		{
			if (!sis.EndAddDropPeriod()) cout<<"The Add/Drop period has already ended.\n";
			else cout<<"The Add/Drop period has been ended.\n";
		}
		
		else if (choice == 5)		// Add student grades from courses-grades file
		{
			sis.buildStudentsCurrentCourseGrades("StudentsCourseGrades.txt");
			cout<<"The grades from the file 'StudentsCourseGrades.txt' were added to the students.\n";
		}
		
		else if (choice == 6)		// View the information of a student
		{
			cout<<">>> Please enter the ID or the name of the student you would like to search for: ";
			cin.ignore(1); getline(cin, studentID); cout<<endl;
			
			if (sis.getStudent(studentID, student) || sis.getStudentByName(studentID, student)) 
			{			
				cout<<"- Student information:\n";
				student->printInformation();
				cout<<"\n- Courses currently registered in: ";
				student->printRegisteredCourses();
				cout<<"\n- Previously taken courses: ";
				student->printTakenCourses();
			}
			
			else cout<<"|!| A student with the ID/Name you entered could not be found.";
		}
		
		else if (choice == 7)		// View the information of an instructor
		{
			cout<<">>> Please enter the ID or the name of the instructor you would like to search for: ";
			cin.ignore(1); getline(cin, instructorID); cout<<endl;
			
			if (sis.getInstructor(instructorID, instructor) || sis.getInstructorByName(instructorID, instructor)) 
			{			
				cout<<"Instructor information:\n";
				instructor->printInformation();
			}
			
			else cout<<"|!| An instructor with the ID/Name you entered could not be found.";
		}
		
		else if (choice == 8)	// View the information of a course
		{
			cout<<">>> Please enter the code of the course you would like to search for: ";
			cin.ignore(1); getline(cin, courseCode); cout<<endl;
			
			if (sis.getCourseGeneral(courseCode, courseGeneral))
			{
				cout<<"Course information:\n";
				courseGeneral->printInformation();
				cout<<"Course offered this semester: ";
				
				course = sis.getCourse(courseCode);
				if (!course) { cout<<"No\n\n"; continue; }				

				cout<<"Yes\n\n";
				course->printInformation(); course->printStudents();
				
				Link <CourseOffering>* courseLink = sis.getCourseLink(courseCode);
				if (!courseLink || !(courseLink->next)) { cout<<"\n\n"; continue; }
				
				while (courseLink->next->element.getSection() != "1") {	// print other sections
					cout<<endl;
					courseLink->next->element.printInformation(); course->printStudents();
					courseLink = courseLink->next;
					if (courseLink->next == NULL) break;
				}
			}
			else cout<<"|!| A course with the code you entered could not be found.";
		}
		
		else if (choice == 9)		// View all students
		{
			cout<<"All students:\n\n"<<"   Student ID   |      Student Name             |    Program    |    Status\n";
			sis.print_Students();
		}
		
		else if (choice == 10)		// View all instructors
		{
			cout<<"All instructors:\n\n"<< "Instructor ID |    Instructor Name     |      Title      |    Specialty   |   Courses Given\n";
			sis.print_instructors();
		}
		
		else if (choice == 11)		// View all currently offered courses
		{
			cout<<"All currently offered courses:\n\n"<<"Course Code   |    Date/Time   |   Available slots  |  Section   |   Instructor Name\n";
			sis.print_coursesOffered();
		}
		
		else if (choice == 12)		// View all courses
		{
			cout<<"All courses:\n\n"<< "Course Code   |      Title      |   Status   |   Year";
			sis.print_courses();		
		}

		else if (choice == 13) { cout<<endl<<endl; break; } 	// Log out
		
		else cout<<"|!| Error: Invalid input, please try again.\n";
		cout<<endl<<endl;
	}
}








void StudentRegistrationSystem(Student* student, Registrar& sis)
{	
	string studentID = student->getSID(), courseCode, sectionNumber; char option;
	CourseOffering* course; Course* courseGeneral; int choice, nbOfSections;
	while (true)
	{
		cout<<">>> Please enter what you would like to do:\n\t1 - Add a course\n\t2 - Drop a course\n\t3 - View offered courses\n\t4 - View the information of a course\n\t5 - View registered courses\n\t6 - View current schedule\n\t7 - View previously taken courses\n\t8 - View remaining required courses to graduate\n\t9 - View your information\n\t10 - Log out\n";
		cout<<">>> "; cin>>choice;
		if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');} // Prevents errors when the input is wrong
		
		
		if (choice == 1)			// add a course
		{
			if (student->registeredCoursesCount() >= 5) { cout<<"|!| Error: You cannot take more than 5 courses.\n\n"; continue; }
			cout<<">>> Please enter the code of the course you would like to add: ";
			cin>>courseCode; cout<<endl;
			
			if (student->findInResgisteredCourses(courseCode)) cout<<"|!| Error: The course whose code you entered is already registered.";
			else if (!sis.getCourse(courseCode)) cout<<"|!| Error: The course whose code you entered could not be found in the current offered courses.";
			else if (!sis.getCourseByProgram(courseCode, student->getProgram())) cout<<"|!| Error: You cannot take this course because it is not a course for your program.";
			
			else
			{
				sectionNumber = getSectionOfACourse_input(courseCode, sis);
				course = sis.getCourse(courseCode, sectionNumber);
				
				if (course->getSlots() <= 0)
				{
					if (course->getSlots() == 0)	  // add student to course waiting list
					{
						cout<<"|!| The course whose code you entered has full capacity. ";
						while (true)
						{
							cout<<"Would you like to add your name to the waiting list for this course (Y/N)? "; cin>>option; cout<<endl;
							if (option == 'y' || option == 'Y') {
								sis.addStudentToWaitingList(student, courseCode, sectionNumber); 
								cout<<"Your name was added to the waiting list for the course "<<courseCode<<" section "<<sectionNumber<<"."; break; }
							else if (option == 'n' || option == 'N') {
								cout<<"Your name was not added to the waiting list for the course "<<courseCode<<"."; break; }
							else cout<<"|!| Error: invalid input. ";
						} 
					}
					else	cout<<"|!| Error: An strange error has occured.";
				}
				else
				{
					sis.registerCourse_Student(student, courseCode, sectionNumber);
					cout<<"The section "<<sectionNumber<<" of the course "<<courseCode<<" was added to your registered courses.";
				}
			}
		}


		else if (choice == 2)		// drop a course
		{
			if (student->registeredCoursesCount() <= 4) { cout<<"|!| Error: You cannot take less than 4 courses.\n\n"; continue; }
			cout<<">>> Please enter the code of the course you would like to drop: ";
			cin>>courseCode; cout<<endl;
			
			CourseOffering* course = student->findInResgisteredCourses(courseCode);
			if (!sis.getCourse(courseCode)) cout<<"|!| Error: The course whose code you entered could not be found.";
			else if (!course) cout<<"|!| Error: The course whose code you entered is not in your registered courses.";
			
			else if (sis.removeCourse_Student(student, courseCode, course))
				cout<<"The course "<<courseCode<<" was removed from your registered courses.";
		}
		
		
		else if (choice == 3)		// view offered courses
		{
			cout<<"Offered courses for your major:\n\n"<<"Course Code   |    Date/Time   |   Available slots  |  Section   |   Instructor Name\n";
			sis.print_CoursesOfferedByProgram(student->getProgram());
		}
		
		else if (choice == 4)	// View the information of a course
		{
			cout<<">>> Please enter the code of the course you would like to search for: ";
			cin.ignore(1); getline(cin, courseCode); cout<<endl;
			
			if (sis.getCourseGeneral(courseCode, courseGeneral))
			{
				cout<<"Course information:\n";
				courseGeneral->printInformation();
				cout<<"Course offered this semester: ";
				
				course = sis.getCourse(courseCode);
				if (!course) { cout<<"No\n\n"; continue; }				

				cout<<"Yes\n\n";
				course->printInformation(); course->printStudents();
				
				Link <CourseOffering>* courseLink = sis.getCourseLink(courseCode);
				if (!courseLink || !(courseLink->next)) { cout<<"\n\n"; continue; }
				
				while (courseLink->next->element.getSection() != "1") {	// print other sections
					cout<<endl;
					courseLink->next->element.printInformation(); course->printStudents();
					courseLink = courseLink->next;
					if (courseLink->next == NULL) break;
				}
			}
			else cout<<"|!| A course with the code you entered could not be found.";
		}
		
		
		else if (choice == 5) {		// view registered courses
			cout<<"Courses currently registered in:\n";
			student->printRegisteredCourses();
		}

		else if (choice == 6) {		// view current schedule
			cout<<"Current schedule:\n";
			student->printSchedule();
		}
		
		else if (choice == 7) {		// view previously taken courses
			cout<<"Previously taken courses:\n";
			student->printTakenCourses();
		}			
			
		else if (choice == 8)		// View remaining required courses to graduate
		{
			cout<<"Remaining required courses to graduate:\n\n"<<"Course Code   |      Title      |   Status   |   Year\n"; 
			sis.print_coursesToTakeLeft(student->getProgram(), student->getCoursesTaken());
		}
			
		else if (choice == 9) {		// View your information
			cout<<"Student information:\n\n";
			student->printInformation();
		}	
		
		else if (choice == 10) { cout<<endl<<endl; break; } 	// Log out
		
		else cout<<"|!| Error: Invalid input, please try again.\n";
		cout<<endl<<endl;
	}
}








void InstructorInformationSystem(Instructor* instructor, Registrar& sis)
{	
	string instructorID = instructor->getID(), studentID, courseCode, sectionNumber, studentName;
	CourseOffering* course; Student* student; int choice, choice2, grade; char option;
	while (true)
	{
		cout<<">>> Please enter what you would like to do:\n\t1 - View courses given currently\n\t2 - View information of a student\n\t3 - Add a grade to a student\n\t4 - View your information\n\t5 - Log out\n";
		cout<<">>> "; cin>>choice;
		if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');} // Prevents errors when the input is wrong
		
		
		if (choice == 1)			// View courses given currently
		{
			cout<<"Given courses:\n\n";
			if (instructor->getFirstGivenCourse(courseCode))
			{
				sectionNumber = instructor->getFirstGivenSection(); course = sis.getCourse(courseCode, sectionNumber);
				cout<<"Course Code: "<<courseCode<<"  Section "<<sectionNumber<<"\nDate/Time: "<<course->getTime()<<"\nNumber of Students: "<<course->getNbStudents()<<"\nStudents: ";
				course->printStudents();
			}
			
			if (instructor->getSecondGivenCourse(courseCode))
			{
				sectionNumber = instructor->getSecondGivenSection() ; course = sis.getCourse(courseCode, sectionNumber);
				cout<<"\n\nCourse Code: "<<courseCode<<"  Section "<<sectionNumber<<"\nDate/Time: "<<course->getTime()<<"\nNumber of Students: "<<course->getNbStudents()<<"\nStudents: ";
				course->printStudents();
			}
		}


		else if (choice == 2)		// View information of a student
		{ 		
			cout<<">>> Please enter the ID or the name of the student you would like to search for: ";
			cin.ignore(1); getline(cin, studentID); cout<<endl;
			
			if (sis.getStudent(studentID, student) || sis.getStudentByName(studentID, student)) 
			{			
				cout<<"- Student information:\n";
				student->printInformation();
				cout<<"\n- Courses currently registered in:";
				student->printRegisteredCourses();
				cout<<"\n- Previously taken courses: ";
				student->printTakenCourses();
			}
			
			else cout<<"|!| A student with the ID/Name you entered could not be found.";
		}
		
		
		
		else if (choice == 3)		// Add grade to a student
		{
			if (!instructor->getFirstGivenCourse(courseCode))
				{ cout<<"|!| Error: You are not giving a course.\n\n"; continue; }
			else sectionNumber =  instructor->getFirstGivenSection();
			
			if (instructor->getSecondGivenCourse(courseCode))	// if the instructor is giving two courses
			{
				instructor->getFirstGivenCourse(courseCode); course = sis.getCourse(courseCode, instructor->getFirstGivenSection());
				cout<<">>> Please select the course you would like to add a grade in (1 / 2):\n\t1 - "<<courseCode<<"  Section "<<course->getSection();
				
				instructor->getSecondGivenCourse(courseCode); course = sis.getCourse(courseCode, instructor->getSecondGivenSection());
				cout<<"\n\t2 - "<<courseCode<<"  Section "<<course->getSection()<<endl;
				
				cout<<">>> "; cin>>choice2;
				if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');}
				
				if (choice2 == 1)		{ instructor->getFirstGivenCourse(courseCode); sectionNumber = instructor->getFirstGivenSection(); }
				else if (choice2 == 2) 	{ instructor->getSecondGivenCourse(courseCode); sectionNumber = instructor->getSecondGivenSection(); }
				
				else { cout<<"|!| Error: Invalid input. Please enter a number between 1 and 2.\n\n"; continue; }
			}
			
			course = sis.getCourse(courseCode, sectionNumber);
			if (course->getNbStudents() <= 0)
				{ cout<<"|!| Error: No student is currently registered in the course "<<course->getCode()<<".\n\n"; continue; }
			
			while (true)
			{			
				cout<<"Students currently registered in the course "<<course->getCode()<<" section "<<course->getSection()<<": "; course->printStudents(); cout<<endl;	
				
				cout<<">>> Please enter the ID of the student you would like to add a grade to: ";
				cin>>studentID; cout<<endl;
				cout<<">>> Please enter the grade you would like to add: ";
				cin>>grade; if(cin.fail())	{cin.clear(); cin.ignore(100,'\n');} cout<<endl;
				
				if (grade < 1 || grade > 100)
					cout<<"|!| Error: Invalid input. Please enter a number between 1 and 100. ";
				else if (!sis.addGrade(courseCode, sectionNumber, studentID, grade))
					cout<<"|!| Error: The student with the ID you entered could not be found in the course.\n\n";
				else
					cout<<"\nThe grade "<<grade<<" was given to "<<studentID<<" for the course "<<courseCode<<" section "<<course->getSection()<<".\n\n";
				
				while (true)
				{
					cout<<"Would you like to add a grade to another student (Y/N) ? "; cin>>option;
					if (option == 'y' || option == 'Y' || option == 'n' || option == 'N') break;
					else cout<<"|!| Error: invalid input. ";
				}
				if (option == 'n' || option == 'N') break;
			}
		}

		else if (choice == 4)		// View your information
		{
			cout<<"Instructor information:\n\n";
			instructor->printInformation();
		}


		
		else if (choice == 5) { cout<<endl<<endl; break; } 	// Log out
		
		else cout<<"|!| Error: Invalid input, please try again.\n";
		cout<<endl<<endl;
	}
}