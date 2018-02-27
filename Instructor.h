#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H
#include <iostream>
#include <iomanip>
using namespace std;

class Instructor
{
	private:
		string ID, Name, Title, Specialty, dateHired;
	 	string CoursesGiven[2], Sections[2];
		int nbCoursesGivingNow;
	public:
		Instructor(string id = "", string name = "", string specialty = "", string title = "", string datehired = "", string course1 = "", string course2 = "");
		bool RegisterCourse(string courseCode, string sectionNb = "1");
		string getID () { return ID; }
		string getName() { return Name; }
		int getNbCoursesGivingNow() {return nbCoursesGivingNow;}
		
		bool getFirstGivenCourse(string& courseCode) {
//			cout<<nbCoursesGivingNow<<" value is: "<<(nbCoursesGivingNow >= 1)<<endl;
			if (nbCoursesGivingNow >= 1) courseCode = CoursesGiven[0];
//			cout<<"course being passed is: "<<courseCode<<endl;
			return (nbCoursesGivingNow >= 1);
			}
		bool getSecondGivenCourse(string& courseCode) {
			if (nbCoursesGivingNow >= 2) courseCode = CoursesGiven[1];
			return (nbCoursesGivingNow >= 2);
			}
		
		void setFirstGivenSection(string sectionNb) { Sections[0] = sectionNb; }
		void setSecondGivenSection(string sectionNb) { Sections[1] = sectionNb; }
		string getFirstGivenSection() { return Sections[0]; }
		string getSecondGivenSection() { return Sections[1];}
			
		void print() {
			cout<<"    "<<ID<<"\t      "<<Name<<"   \t    "<<Title<<"\t\t  "<<Specialty<<"\t\t";
			if (nbCoursesGivingNow >= 1) cout<<CoursesGiven[0];
			if (nbCoursesGivingNow >= 2) cout<<", "<<CoursesGiven[1];
		}
		
		void printInformation()
		{
			cout<<"Name:\t\t"<<Name<<endl;
			cout<<"Title:\t\t"<<Title<<endl;
			cout<<"Instructor ID:\t"<<ID<<endl;
			cout<<"Specialty:\t"<<Specialty<<endl;
			cout<<"Date Hired:\t"<<dateHired<<endl;
			cout<<"Currently given courses: "<<CoursesGiven[0];//<<" "<<CoursesGiven[0].length();
			if (nbCoursesGivingNow == 2) cout<<", "<<CoursesGiven[1];//<<" "<<CoursesGiven[1].length()<<endl;
		}
		
		
		void emptyCourseAssignments()
		{
			nbCoursesGivingNow = 0;
			CoursesGiven[0] = CoursesGiven[1] = Sections[0] = Sections[1] = "1";
		}
};

Instructor::Instructor(string id, string name, string specialty, string title, string datehired, string course1, string course2)
{
	ID = id; Name = name; Specialty = specialty; Title = title; dateHired = datehired; nbCoursesGivingNow = 0;
	CoursesGiven[0] = course1; CoursesGiven[1] = course2;
	if (course1 != "") nbCoursesGivingNow++; if (course2 != "") nbCoursesGivingNow++;
//	cout<<ID<<"\t"<<CoursesGiven[0]<<"\t"<<CoursesGiven[1]<<endl;		// populating test
}

bool Instructor::RegisterCourse(string courseCode, string sectionNb)
{	
	if (nbCoursesGivingNow >= 2) return false;
	CoursesGiven[nbCoursesGivingNow] = courseCode;
	Sections[nbCoursesGivingNow++] = sectionNb;
	return true;
}

#endif