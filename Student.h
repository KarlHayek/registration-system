#ifndef Student_h
#define Student_h
#include <iostream>
#include <iomanip>
#include <stack>
#include "Course.h"
#include "CourseOffering.h"
using namespace std;

class Student
{
	private:
		string SID, SName, Program, Status;
		CourseOffering* CoursesRegistered[5];
		int nbRegisteredCourses, nbCoursesOnWaitingList, nbCurrentFailedCourses, nbPastFailedCourses;
		stack<Course*> CoursesTaken;
		string CoursesOnWaitingList[5], FailedCourses[10];
		
	public:
		Student(string sid = "", string name = "", string program = "", string status = "Clear");
				
		bool addCourse(CourseOffering* course);
		bool dropCourse(string courseCode);
		bool updateStatus();		 // end of semester
		bool manipCurrentCourses(); // end of semester
		Course* convToCourse(CourseOffering* course);
	 
		string getSID() { return SID; }
		string getSName() { return SName; }
		string getProgram() { return Program; }
		double getCurrentGpa();
		double getGpa();
		string getStatus() {return Status; }
		void setStatus(string status) { Status = status; }
		stack<Course*>* getCoursesTaken() { return &CoursesTaken; }
				
		int registeredCoursesCount() { return nbRegisteredCourses;}
		void pushToTakenCourses(Course* course) { CoursesTaken.push(course); }
		
		bool addCourseToWaitingList(string courseCode);
		bool removeCourseFromWaitingList(string courseCode);
		
		CourseOffering* findInResgisteredCourses(string courseCode) {
			for (int i = 0; i < nbRegisteredCourses; i++)
				if (CoursesRegistered[i]->getCode() == courseCode) return CoursesRegistered[i];
			return NULL;
		}
		
		//	Register for up to 15 credit hours per semester, and no less than 12 credit hours, unless he or she is in the last semester, or cannot find courses to register for
		void printInformation(){
			cout<<"Name:\t\t"<<SName<<endl;
			cout<<"Student ID:\t"<<SID<<endl;
			cout<<"Program:\t"<<Program<<endl;
			cout<<"Status:\t\t"<<Status<<endl;
		}
		void printRegisteredCourses();		
		void printTakenCourses();
		void printSchedule();
		void print() {
			cout<<"   "<<SID<<"\t       "<<SName<<"\t\t";
			if (SName.length() < 17) cout<<"\t";
//			else cout<<"\t";
			cout<<Program<<"\t      "<<Status<<endl;
		}
};

struct SIDComp
{		static bool eq(string i, Student s)	{return (i == s.getSID()); }
		static bool lt(string i, Student s)	{return (i < s.getSID()); }	
		static bool gt(string i, Student s)	{return (i > s.getSID()); }
};
struct StudentComp
{		static bool eq(Student s1, Student s2)	{return (s1.getSID() == s2.getSID()); }
		static bool lt(Student s1, Student s2)	{return (s1.getSID() < s2.getSID()); }
		static bool gt(Student s1, Student s2)	{return (s1.getSID() > s2.getSID()); }
};


Student::Student(string sid, string name, string program, string status)
{
	SID = sid; SName = name; Program = program;
//	cout<<"Status b: "<<status<<"; "<<endl;
	if (status[0] == 'C') Status = "Clear";
	else if (status[0] == 'P') Status = "Probation";
	nbRegisteredCourses = 0; nbCoursesOnWaitingList = 0; nbCurrentFailedCourses = 0; nbPastFailedCourses = 0;
//	cout<<"Status a: "<<Status<<endl;
}

bool Student::addCourse(CourseOffering* course)
{
	if (findInResgisteredCourses(course->getCode())) return false;	// the course is already registered
	
	if (nbRegisteredCourses < 5){ // up to 5 courses // each course is 3 credits
		CoursesRegistered[nbRegisteredCourses++] = course;
		return true;
	}
	else return false;
}

bool Student::dropCourse(string courseCode)
{	
	for (int i = 0; i < nbRegisteredCourses; i++)
		if (CoursesRegistered[i]->getCode() == courseCode && nbRegisteredCourses > 4) // no less than 4 courses
		{
			for (int j = i; j < nbRegisteredCourses-1; j++)
				CoursesRegistered[j] = CoursesRegistered[j+1]; // shift down
			nbRegisteredCourses--;
			return true;
		}
	return false;
}


void Student::printRegisteredCourses()
{
	if (nbRegisteredCourses == 0) {cout<<"There are no currently registered courses.\n"; }
	else
	{
		cout<<"\nCourse Code  | Section |   Grade"<<endl;
		for (int i = 0; i < nbRegisteredCourses; i++) 
			cout<<"   "<<CoursesRegistered[i]->getCode()<<"\t  "<<CoursesRegistered[i]->getSection()<<"\t    "<<CoursesRegistered[i]->getStudentGrade(SID)<<endl;
		
		double gpa = getCurrentGpa();
		if (gpa != -1) cout<<"Current semeseter GPA: "<<setprecision(4)<<gpa + 0.0005<<endl;
	}
	
	if (nbCoursesOnWaitingList > 0) {
		cout<<"\nCourse waiting lists currently on: ";
		for (int i = 0; i < nbCoursesOnWaitingList; i++)  cout<<CoursesOnWaitingList[i]<<"  ";
		cout<<endl; }
}

void Student::printTakenCourses()
{
	stack <Course*> TempCoursesTaken = CoursesTaken;
	if (TempCoursesTaken.empty()) {cout<<"There are no previously taken courses.\n"; return;}
	
	cout<<"\nCourse Code    |    Grade"<<endl;
	while (!TempCoursesTaken.empty()){
		cout<<"   "<<TempCoursesTaken.top()->getCode()<<"\t      "<<TempCoursesTaken.top()->getStudentGrade()<<endl;
		TempCoursesTaken.pop();
	}
	cout<<"Total GPA: "<<setprecision(4)<<getGpa() + 0.0005<<endl;
	
	if (nbPastFailedCourses > 0) {
		cout<<"\nFailed courses: ";
		for (int i = 0; i < nbPastFailedCourses; i++)
			cout<<FailedCourses[i]<<"  ";
		cout<<endl;
	}
}

void Student::printSchedule()
{
	cout<<"\nCourse Code    |    Date/Time    |    Instructor"<<endl;
	for (int i = 0; i < nbRegisteredCourses; i++) 
		cout<<"   "<<CoursesRegistered[i]->getCode()<<"\t      "<<CoursesRegistered[i]->getTime()<<"\t      "<<CoursesRegistered[i]->getInstructorName()<<endl;
}

double Student::getCurrentGpa()
{
	double gpa = 0; int counter = 0;
	for (int i = 0; i < nbRegisteredCourses; i++)
		if (CoursesRegistered[i]->getStudentGrade(SID) > 0) {	// the grade of a course is -1 by default
			gpa += CoursesRegistered[i]->getStudentGrade(SID);
			counter++;
		}
	gpa = gpa / counter;
	if (gpa > 0)	return gpa;
	else return -1;
}

double Student::getGpa()
{
	stack <Course*> TempCoursesTaken = CoursesTaken; double gpa = 0; int nbOfCourses = 0;
	while (!TempCoursesTaken.empty()){
		gpa += TempCoursesTaken.top()->getStudentGrade();
		TempCoursesTaken.pop(); nbOfCourses++;
	}
	return gpa / nbOfCourses;
}


bool Student::addCourseToWaitingList(string courseCode)
{
	if (nbCoursesOnWaitingList >= 5) return false;
	CoursesOnWaitingList[nbCoursesOnWaitingList++] = courseCode;
	return true;
}

bool Student::removeCourseFromWaitingList(string courseCode)
{ 
	if (nbCoursesOnWaitingList <= 0) return false;
	for (int i = 0 ; i < nbCoursesOnWaitingList; i++)
		if (courseCode == CoursesOnWaitingList[i])
		{
			for (int j = i; j < nbCoursesOnWaitingList-1; j++)
				CoursesOnWaitingList[j] = CoursesOnWaitingList[j + 1];		 // shift down
			
			nbCoursesOnWaitingList--; return true;
		}
	return false;
}



Course* Student::convToCourse(CourseOffering* course)
{//	Course(string title = "", string code = "", char status = '\0', string year = "");
	Course* course2 = new Course("", course->getCode());
	course2->setStudentGrade(course->getStudentGrade(SID));
	return course2;
}


bool Student::updateStatus()
{
	if (!manipCurrentCourses())
		{ Status = "Dropped"; return false; }

	if (Status[0] == 'C' && nbCurrentFailedCourses > 0)
		Status = "Probation";
		
	else if (Status[0] == 'P' && nbCurrentFailedCourses == 0  && nbPastFailedCourses == 0)
		Status = "Clear";
		
	if (CoursesTaken.size() == 16)
		Status = "Graduated";

	return true;
}

bool Student::manipCurrentCourses()
{
	nbCurrentFailedCourses = 0;
	for (int i = 0; i < nbRegisteredCourses; i++)
	{
		if (CoursesRegistered[i]->getStudentGrade(SID) >= 60)			  // if the student passed the course
		{
			for (int j = 0; j < nbPastFailedCourses; j++)
				if (CoursesRegistered[i]->getCode() == FailedCourses[j])	// and if the student has failed the course before
				{
					for (int k = j; k < nbPastFailedCourses-1; k++)
						FailedCourses[k] = FailedCourses[k + 1];			// remove the course from the array of failed courses
					nbPastFailedCourses--;
				}
						
			CoursesTaken.push(convToCourse(CoursesRegistered[i]));
		}
							
		else if (CoursesRegistered[i]->getStudentGrade(SID) > -1)		// if the student did not pass the course
		{												  				  
			for (int j = 0; j < nbPastFailedCourses; j++)
				if (CoursesRegistered[i]->getCode() == FailedCourses[j])	// and if the student has failed the course before
					return false; 													 // drop the student
			FailedCourses[nbPastFailedCourses++] = CoursesRegistered[i]->getCode(); // else add the course to the array of failed courses
			nbCurrentFailedCourses++;			
//			cout<<SName<<" failed "<<FailedCourses[nbPastFailedCourses-1]<<". Total: "<<nbPastFailedCourses<<endl;
		}
 	}
	nbRegisteredCourses = nbCoursesOnWaitingList = 0;
	return true;
}

#endif