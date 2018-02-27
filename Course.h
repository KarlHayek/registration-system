#ifndef COURSE_H
#define COURSE_H
#include <string>
#include <iomanip>
using namespace std;

class Course
{
	private:
		string title, code, year;
		char status;	// order: title, code, status, year
		int studentGrade;
	public:
		Course(string title = "", string code = "", char status = '\0', string year = "");
		string getCode() {return code;}
		string getTitle()	{return title;}
		void printInformation();
		void print() { cout<<"\n "<< setfill(' ') << left << code << "\t\t  " << setw(18) << title << "   "<< status << "\t  " << year;}
		
		void setStudentGrade(int grade) { studentGrade = grade;}
		int getStudentGrade() { return studentGrade;}
};

Course::Course(string t, string c, char s, string y)
{
	title = t; code = c; year = y; status = s;
}

void Course::printInformation()
{
	cout << "Title:\t\t" << title << endl << "Code:\t\t" << code << endl << "Status:\t\t" << status << endl << "Year:\t\t" << year << endl;
}

struct CodeComp
{		static bool eq(string i, Course s)	{return (i == s.getCode()); }
		static bool lt(string i, Course s)	{return (i < s.getCode()); }	
		static bool gt(string i, Course s)	{return (i > s.getCode()); }
};

struct CourseComp
{		static bool eq(Course s1, Course s2)	{return (s1.getCode() == s2.getCode()); }
		static bool lt(Course s1, Course s2)	{return (s1.getCode() < s2.getCode()); }
		static bool gt(Course s1, Course s2)	{return (s1.getCode() > s2.getCode()); }
};

#endif