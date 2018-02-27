#ifndef CourseOffering_h
#define CourseOffering_h
#include <fstream>
#include <string>
#include <queue>
using namespace std;

class CourseOffering
{
	private:
		string ccode, dateTime, InstructorID, InstructorName, sectionNb, SID[20];
		int slots, nbRegisteredStudents, capacity, waitinglistcount, SGrade[20];
		
		queue<string> studentWaitingList;
	
	public:
		CourseOffering(string code = "", string datetime = "", string slotsRemaining = "", string section = "1");
	
		bool addStudentWithGrade(string studentID, int grade);
		bool addStudent(string studentID);
		bool removeStudent(string studentID);
		bool addGrade(string studentID, int grade);

		void setTime(string d)	{dateTime = d; }
		string getTime() { return dateTime; }
		string getCode() { return ccode;}
		int getSlots() { return slots; }
		int getNbStudents() { return capacity - slots; }
		string getSection() { return sectionNb; }
		string getInstructorID() { return InstructorID; }
		string getInstructorName() { return InstructorName; }
		int getStudentGrade(string studentID);

		void print(){
			cout<<"   "<<ccode<<"\t     "<<dateTime<<"\t         "<<slots<<"\t\t  "<<sectionNb<<"\t\t"<<InstructorName;
		}
		void printInformation(){
			cout<<"Section Number: "<<sectionNb<<"\nInstructor:\t"<<InstructorName<<"\nDate/Time:\t"<<dateTime<<"\nSlots left:\t"<<slots<<endl;
		}
		void printStudents();
		bool assignInstructorIDName (string instructorID, string instructorName);
		
		void addToWaitingList(string studentID) { studentWaitingList.push(studentID); waitinglistcount++;	}
		string getFirstWaitingListStudent() {return studentWaitingList.front();}
		int getWaitingListSize() { return waitinglistcount;}
		void decrementWaitingList() { studentWaitingList.pop(); waitinglistcount--;}
		
		void emptyWaitingList() { 
			while (!studentWaitingList.empty())
				studentWaitingList.pop();
			waitinglistcount = 0;		
		}
		
		void emptyStudentsAndInstructors()
			{ slots = capacity; InstructorID = InstructorName = ""; }
};

CourseOffering::CourseOffering(string code, string datetime, string maxcapacity, string section)
{
		ccode = code; dateTime = datetime; sectionNb = section;
		for (int i = 0; i < 20; i++) {
			SID[i] = '\0'; SGrade[i] = -1;
		}
		waitinglistcount = 0; InstructorName = InstructorID = ""; //slots = 20; 
		slots = atoi(maxcapacity.c_str()); capacity = atoi(maxcapacity.c_str());
//		cout<<ccode<<"\t"<<dateTime<<"\t"<<sectionNb<<endl; // population test
	}
	
	
int CourseOffering::getStudentGrade(string studentID)
{
	for (int i = 0; i < getNbStudents(); i++)
		if (SID[i] == studentID)
			return SGrade[i];
	return -1;
}

void CourseOffering::printStudents() {
	cout<<"\n    StudentID    |    Grade"<<endl;
	for (int i = 0; i < getNbStudents(); i++)
		cout<<"    "<<SID[i]<<"\t\t"<<SGrade[i]<<endl;
	
	if (waitinglistcount > 0)
	{
		cout<<"Students currently on waiting list: ";
		queue<string> studentWaitingListTemp = studentWaitingList;
		
		while (!studentWaitingListTemp.empty()) {
			cout<<studentWaitingListTemp.front()<<", ";
			studentWaitingListTemp.pop();
		}
	}
}

bool CourseOffering::assignInstructorIDName (string instructorID, string instructorName) {
	if (InstructorID == ""){
		InstructorID = instructorID; InstructorName = instructorName;
		return true;}
	return false;
}
	

bool CourseOffering::addStudentWithGrade(string studentID, int grade)
{
	if (getNbStudents() < 20 && slots > 0) {
		SID[getNbStudents()] = studentID;
		SGrade[getNbStudents()] = grade;
		slots--;
		return true;
	}
	return false;
}


bool CourseOffering::addStudent(string studentID)
{
	if (getNbStudents() < 20 && slots > 0) {
		SID[getNbStudents()] = studentID;
		SGrade[getNbStudents()] = -1;
		slots--;
		return true;
	}
	return false;
}

bool CourseOffering::removeStudent(string studentID)
{	
	for (int i = 0; i < getNbStudents(); i++)
		if (SID[i] == studentID)
		{
			for (int j = i; j < getNbStudents()-1; j++) {
				SID[j] = SID[j+1];		 // shift down
				SGrade[j] = SGrade[j+1];
			}
			slots++;
			return true;
		}
	return false;
}


bool CourseOffering::addGrade(string studentID, int grade)
{
	for (int i = 0; i < getNbStudents(); i++)
		if (SID[i] == studentID)
		{
			SGrade[i] = grade;
			return true;
		}
	return false;
}


#endif