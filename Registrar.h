#ifndef REGISTRAR_H
#define REGISTRAR_H
#include <iostream>
#include <fstream>
#include "CourseOffering.h"
#include "Student.h"
#include "Course.h"
#include "Instructor.h"
#include "LList.h"
#include "BST.h"
using namespace std;

class Registrar
{
	private:
		LList <Instructor> Instructors; LList <CourseOffering> offeredCourses;
		BST <string, Student, SIDComp, StudentComp> Students; BST <string, Course, CodeComp, CourseComp> Courses;
	
	public:
		Registrar();
		
		void buildTakenCourses(string filename);
		void buildStudentCurrentCourses(string filename);
		void buildStudentsCurrentCourseGrades(string filename);
		void AdmitStudent(Student* student)  { Students.insert(*student); }
		bool EndAddDropPeriod();
		
		void endOfSemesterProcedures() {
			Students.updateStatuses(); offeredCourses.emptyWaitingLists();
			offeredCourses.emptyStudentsAndInstructors(); Instructors.emptyCourseAssignments();
		}
		
		bool registerCourse_Student(Student* student, string courseCode, string sectionNb = "1");
		bool removeCourse_Student(Student* student, string courseCode, CourseOffering* course = NULL);
		bool addStudentToWaitingList(Student* student, string courseCode, string sectionNb);
		bool registerCourse_Instructor(string instructorID, string courseCode, string sectionNb = "1");
		bool addGrade(string courseCode, string sectionNb, string studentID, int courseGrade);
		int getNbOfSections(string courseCode);
		
		void print_coursesOffered() {offeredCourses.print(); }
		void print_Students() {Students.print(); }
		void print_courses() {Courses.print(); }
		void print_instructors() {Instructors.print(); }
		void print_coursesToTakeLeft(string program, stack<Course*>* CoursesTaken) { Courses.print_coursesToTakeLeft(program, CoursesTaken); }
		void print_CoursesOfferedByProgram(string program) { offeredCourses.print_CoursesByProgram(program); }
		void print_instructorsWhoCanGiveACourse() { Instructors.print_InstructorsWhoCanGiveACourse(); }
		void print_courseSectionsWithNoInstructor() { offeredCourses.print_CourseSectionsWithNoInstructor(); }
		
		bool getStudent(string studentID, Student*& student, string studentName = "") {
			return Students.find(studentID, student); // sets "student" to the pointer to the student whose ID is studentID
		}
		bool getStudentByName(string studentName, Student*& student) {
			return Students.find_ByName(studentName, student); // sets "student" to the pointer to the student whose name is studentName
		}
		bool getInstructor(string instructorID, Instructor*& instructor) {
			instructor = Instructors.searchInstructor(instructorID);
			return instructor;
		}
		bool getInstructorByName(string instructorName, Instructor*& instructor) {
			instructor = Instructors.searchInstructor_ByName(instructorName);
			return instructor;
		}
		
		bool getCourseGeneral(string courseCode, Course*& course) {
			return Courses.find(courseCode, course);
		}
		
		CourseOffering* getCourse(string courseCode, string sectionNb = "1") {
			return offeredCourses.searchCourse(courseCode, sectionNb);	// returns the pointer to the course whose code is courseCode
		}
		CourseOffering* getCourseByProgram(string courseCode, string program) {		// checks if courseCode corresponds to a course
			return offeredCourses.searchCourseByProgram(courseCode, program);		// that belongs to the program 'program'
		}
		Link <CourseOffering>* getCourseLink(string courseCode) {
			return offeredCourses.searchCourseLink(courseCode);
		}

};

Registrar::Registrar()
{
	string files[7] = {"instructors.txt", "coursesOffered.txt", "students.txt", "courses.txt", "coursesTaken.txt", "StudentsRequestsForRegistration.txt", "StudentsCourseGrades.txt"};
	for (int i = 0; i < 7; i++) {
		files[i] = "data/" + files[i];
		ifstream inFile; inFile.open(files[i]);
		if (!inFile) cout<<"File "<<files[i]<<" not found"<<endl;
	}
	
	Instructors.buildInstructors("data/instructors.txt"); offeredCourses.buildCourseOfferings("data/coursesOffered.txt");
	Students.buildStudents("data/students.txt"); Courses.buildCourses("data/courses.txt");
	Instructors.assignOfferedCourses(&offeredCourses); buildTakenCourses("data/coursesTaken.txt");
	buildStudentCurrentCourses("data/StudentsRequestsForRegistration.txt");
}



void Registrar::buildTakenCourses(string filename)
{ // for Courses taken in the past
	// finds the student and the course mentionned in every line, sets the grade to the course, and then pushes the course to the student's stack of taken courses
	ifstream inFile(filename);
	string line, elements[3];
	Student* student; Course* course;
	while(getline(inFile, line))
	{
		split(line, elements, 3);
		Students.find(elements[0], student); 
		Courses.find(elements[1], course);
		course->setStudentGrade(atoi(elements[2].c_str()));
		
		student->pushToTakenCourses(course);
	}
}

void Registrar::buildStudentCurrentCourses(string filename)
{ // for current CourseOferrings, end of the semester
	ifstream inFile(filename);
	string line, elements[3]; int grade, size = 0;
	CourseOffering* course; Student* student;
	
	while(getline(inFile, line))
	{
		split(line, elements, 2);
//		cout<<elements[1].length()<<endl;
//		course = offeredCourses.searchCourse(elements[1]);
		course = offeredCourses.searchCourse(elements[1].substr(0, elements[1].length() - 1));
		if (!course) { cout<<"|!| Error: could not find course "<<elements[1]<<endl; continue; }
		Students.find(elements[0], student);
		if (!student) { cout<<"|!| Error: could not find student "<<elements[0]<<endl; continue; }
		
		if (course->addStudent(elements[0]))
			student->addCourse(course);
	}
}


void Registrar::buildStudentsCurrentCourseGrades(string filename)
{ // for current CourseOferrings
	ifstream inFile(filename);
	string line, elements[3]; int grade;
	CourseOffering* course; Student* student;
	
	while(getline(inFile, line))
	{
		split(line, elements, 3);
		course = offeredCourses.searchCourse(elements[1]);
		grade = atoi(elements[2].c_str());
		
		addGrade(elements[1], "1", elements[0], grade);
	}
}

bool Registrar::EndAddDropPeriod()
{
	return true;	
}


bool Registrar::registerCourse_Student(Student* student, string courseCode, string sectionNb)
{
	CourseOffering*	course = offeredCourses.searchCourse(courseCode, sectionNb);

	if (course)
		return (course->getSlots() > 0) && course->addStudent(student->getSID()) && student->addCourse(course);
		
	return false;
}

bool Registrar::removeCourse_Student(Student* student, string courseCode, CourseOffering* course)
{
	if (course == NULL)
		CourseOffering* course = offeredCourses.searchCourse(courseCode);

	if (course && student->dropCourse(courseCode) && course->removeStudent(student->getSID()))
	{
		if (course->getWaitingListSize() > 0 && Students.find(course->getFirstWaitingListStudent(), student)) {  // add student from waiting list
			// student becomes the student to be taken off from the waiting list
			course->decrementWaitingList(); student->removeCourseFromWaitingList(courseCode);
			registerCourse_Student(student, courseCode, course->getSection());
		}
		return true;
	}
	return false;
}

bool Registrar::addStudentToWaitingList(Student* student, string courseCode, string sectionNb)
{
	CourseOffering* course = offeredCourses.searchCourse(courseCode, sectionNb);
	if (course) {
		course->addToWaitingList(student->getSID()); student->addCourseToWaitingList(courseCode);
		return true;
	}
	return false;
}


bool Registrar::registerCourse_Instructor(string instructorID, string courseCode, string sectionNb)
{	
	CourseOffering* course = offeredCourses.searchCourse(courseCode, sectionNb);
	Instructor* instructor = Instructors.searchInstructor(instructorID);	

	if (instructor && course)
		return instructor->RegisterCourse(courseCode, sectionNb) && course->assignInstructorIDName(instructorID, instructor->getName());
	// returns false if the course code is invalid, or if the instructor id is invalid, or if the instructor is already giving two courses
	return false;
}

bool Registrar::addGrade(string courseCode, string sectionNb, string studentID, int courseGrade)
{
	CourseOffering* course = offeredCourses.searchCourse(courseCode, sectionNb);
	Student* student;
	
	if (course && Students.find(studentID, student))
		return course->addGrade(studentID, courseGrade);
	
	return false;
}

int Registrar::getNbOfSections(string courseCode)
{
	int nbOfSections = 1;
	Link <CourseOffering>* courseLink = offeredCourses.searchCourseLink(courseCode);
	while (courseLink->next->element.getSection() != "1") {
		nbOfSections++; courseLink = courseLink->next;
	}
	return nbOfSections;
}

#endif