#ifndef LList_H
#define LList_H
#include <fstream>
#include <iostream>
#include <string>
#include "CourseOffering.h"
#include "Instructor.h"
using namespace std;

template <class Elem> struct Link 
{
  		Elem element; Link *next;
		Link(const Elem& elemval, Link* nextval = NULL)		{ element = elemval;  next = nextval; }
		Link(Link* nextval = NULL)	{ next = nextval; }
};


template <class Elem> class LList
{
	private:
		Link<Elem> *head, *tail, *fence;
		int leftcnt, rightcnt, size, maxSize;
		
		void init() {
			fence = tail = head = new Link<Elem>;
			leftcnt = rightcnt = size = 0;
		}
		void removeall() {
			while (head != NULL)
				{ fence = head; head = head->next; delete fence; }
		}
	
	public:
		LList(int maxsize = 1000)	{ init(); maxSize = maxsize;}
		~LList() { removeall(); }
		void clear() { removeall(); init(); }
	
		void setStart() 
		{
			fence = head; rightcnt += leftcnt;
			leftcnt = 0; 
		}
		void setEnd()
		{
			fence = tail; leftcnt += rightcnt; rightcnt = 0; 
		}
		void next() 
		{
			if (fence != tail) 		 // Don't move fence if right empty
				{ fence = fence->next; rightcnt--; leftcnt++; }
		}
		void prev() 
		{
			Link<Elem>* temp = head;
			if (fence == head) return; // No prev Elem
			while (temp->next!=fence)
				temp=temp->next;
			fence = temp;
			leftcnt--; rightcnt++;
		}
		
		int leftLength() const  { return leftcnt; }
		int rightLength() const { return rightcnt; }

		bool getValue(Elem& it) const 
		{
			if(rightLength() == 0) return false;
			it = fence->next->element;
			return true; 
		}
		
		bool insert(const Elem& item) 
		{
			if (size < maxSize)
			{
				fence->next = new Link<Elem>(item, fence->next);  
				if (tail == fence) tail = fence->next;
				rightcnt++; size++; return true;
			}
			else return false;
		}

		bool append(const Elem& item) 
		{
			if (size < maxSize) {
				tail = tail->next = new Link<Elem>(item, NULL);
				rightcnt++; size++; return true;
			}
			else return false;
		}
		
		bool remove(Elem& it) 
		{
			if (fence->next == NULL) return false; 
			it = fence->next->element; // Remember value
		 	
			Link<Elem>* ltemp = fence->next;
			fence->next = ltemp->next; // Remove
				
			if (tail == ltemp) // Reset tail
				tail = fence;
			
			delete ltemp; rightcnt--; size--; return true;
		}
		
		void print()
		{
			if (head == NULL) return;
			for (Link<Elem>* it = head->next; it != NULL; it = it->next) {
				it->element.print(); cout<<endl;}
		}
		
		
		
		
		
		
		
		
		Link<Elem>* searchCourseLink(string code)
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getCode() == code)
					return it;
			return NULL;
		}
	
		Elem* searchCourse(string courseCode, string sectionNb = "1")
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getCode() == courseCode && it->element.getSection() == sectionNb)
					return &(it->element);
			return NULL;
		}
		
		Elem* searchCourseByProgram(string courseCode, string program)
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getCode() == courseCode && it->element.getCode().substr(0, 2) == program)
					return &(it->element);
			return NULL;
		}
		
		Elem* searchInstructor(string instructorID)
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getID() == instructorID)
					return &(it->element);
			return NULL;
		}
		
		Elem* searchInstructor_ByName(string instructorName)
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getName() == instructorName)
					return &(it->element);
			return NULL;
		}
		
		
		void print_CoursesByProgram(string program)
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getCode().substr(0, 2) == program)
					{ it->element.print(); cout<<endl; }
		}
		
		void print_InstructorsWhoCanGiveACourse()
		{
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				if (it->element.getNbCoursesGivingNow() < 2)
					{ it->element.print(); cout<<endl; }
		}
		
		void print_CourseSectionsWithNoInstructor()
		{
			if (head == NULL) return;
			for (Link<Elem>* it = head->next; it != NULL; it = it->next)
				if (it->element.getInstructorID() == "")
					{ it->element.print(); cout<<endl; }
		}
		
	
		void setCourseDate(string code, string newDate)	
			{ (this->searchCourse(code))->setDate(newDate); }
		

		void split(string str, string items[], int item_total, int item_nb = 0, int pos = 0)
		{	// splits "item_total" number of elements in items[] separated by ',' inside str
			// Note: This function can sometimes not take the last character of a text
			//       To fix this, just add a space to the end of the text
			if (item_nb == item_total)	return;
			int initial_pos = pos;

			for (pos = pos; pos < str.length(); pos++)
				if (pos + 1 == str.length() || str[pos] == ',')
				{
					items[item_nb] = str.substr(initial_pos, pos - initial_pos);
					return split(str, items, item_total, item_nb + 1, pos + 1);
				}
		}


		void buildCourseOfferings(string filename)
		{
			this->clear(); ifstream in(filename);
			string line = "", attr[3], prevCourseCode = "";
			int courseSection = 1;

			while (getline(in, line))
			{
				split(line, attr, 3);
//				cout<<attr[0]<<"  "<<attr[1]<<"  "<<attr[2]<<endl;
				if (attr[0] == prevCourseCode) courseSection++;
				else { courseSection = 1; prevCourseCode = attr[0]; }
				this->append(CourseOffering(attr[0], attr[1], attr[2], to_string(courseSection)));
			}
		}
		
		void buildInstructors(string filename)
		{
			this->clear(); ifstream in(filename);
			string line = "", attr[7];

			while (getline(in, line))
			{
				split(line, attr, 7);
				this->append(Instructor(attr[0], attr[1], attr[2], attr[3], attr[4], attr[5], attr[6]));
				attr[6] = "";
			}
		}
		
		
		
		void assignOfferedCourses(LList<CourseOffering>* offeredCourses)
		{	// For Instructors
			string courseCode; CourseOffering* course;
			for (Link<Elem>* it = head->next; it != NULL; it = it->next)
			{
				if (it->element.getFirstGivenCourse(courseCode)) {
					course = offeredCourses->assignInstructor(courseCode, it->element.getID(), it->element.getName());
					if (course) it->element.setFirstGivenSection(course->getSection());
				}
				if (it->element.getSecondGivenCourse(courseCode)) {
					course = offeredCourses->assignInstructor(courseCode, it->element.getID(), it->element.getName());
					if (course)  it->element.setSecondGivenSection(course->getSection());
				}
			}
		}
		
		CourseOffering* assignInstructor(string courseCode, string instructorID, string instructorName)
		{
//			if (courseCode.length() != 5) cout<<"oy!"<<endl;
			Link <CourseOffering>* courseLink = searchCourseLink(courseCode);
			if (courseLink == NULL) { 
//				cout<<"- "<<courseCode<<" "<<courseCode.length()<<" not found"<<endl;
				return NULL; }
			
			if (courseLink->element.getInstructorID() == "")
				courseLink->element.assignInstructorIDName(instructorID, instructorName);

			else 
				while (courseLink->next && courseLink->next->element.getSection() != "1")	// search for another section
				{
					courseLink = courseLink->next;
					if (courseLink->element.getInstructorID() == "")
						{ courseLink->element.assignInstructorIDName(instructorID, instructorName); break; }
				}
			return &(courseLink->element);
		}
		
		
		void emptyCourseAssignments()
		{	// For Instructors
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				it->element.emptyCourseAssignments();
		}
		
		void emptyStudentsAndInstructors()
		{	// For offeredCourses
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				it->element.emptyStudentsAndInstructors();
		}
		
		void emptyWaitingLists() {
			for (Link<Elem>* it = head; it != NULL; it = it->next)
				it->element.emptyWaitingList();
		}
};


#endif