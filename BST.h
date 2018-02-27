#ifndef BST_h
#define BST_h
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;					

template <class Elem> class BinNode
{
	private:
		Elem it;         // The node's value
		BinNode* lc;  // Pointer to left child
		BinNode* rc;  // Pointer to right child
			
	public:
		BinNode() { lc = rc = NULL; }
		BinNode(Elem e, BinNode* l = NULL, BinNode* r = NULL)
			{ it = e; lc = l; rc = r; }
		
		Elem& val() { return it; }
		inline BinNode<Elem>* left() const	 { return lc; }
		inline BinNode<Elem>* right() const	 { return rc; }
		
		void setVal(const Elem& e) 			 { it = e; }
		void setLeft(BinNode<Elem>* b)		 { lc = b; }
		void setRight(BinNode<Elem>* b)		 { rc = b; }
		bool isLeaf() { return (lc == NULL) && (rc == NULL); }
};


// BST implementation for the Dictionary ADT
template <class Key, class Elem, class KEComp, class EEComp>
class BST
{// private data and helper functions
	private:
		BinNode<Elem>* root;   // Root of the BST
		int nodecount;         // Number of nodes 
		
		void clearhelp(BinNode<Elem>*);
		BinNode<Elem>* inserthelp(BinNode<Elem>*, const Elem&);
		BinNode<Elem>* deletemin(BinNode<Elem>*, BinNode<Elem>*&);
		BinNode<Elem>* removehelp(BinNode<Elem>*, const Key&, BinNode<Elem>*&);
		bool findhelp(BinNode<Elem>*, const Key&, Elem*&) const; 				// changed the third parameter ('Elem&' to 'Elem*&')
		void printhelp(BinNode<Elem>*, int level) const;

	public:
		BST() { root = NULL; nodecount = 0; }
		~BST() { clearhelp(root); } 
		
		void clear() { clearhelp(root); root = NULL; nodecount = 0; }
		
		bool insert(const Elem& e) {
			root = inserthelp(root, e);
			nodecount++; return true; }
		
		bool remove(const Key& K)
		{					
			BinNode<Elem>* t = NULL; //we check for null below
			root = removehelp(root, K, t);//root may change
			if (t == NULL) return false;
//			e = t->val(); // t points to a floating node now, get the
			nodecount--;  // value that it points to before deleting it
			delete t; //Now, we can get rid of it
			return true; 
		}
		
		bool removeMin(Elem& e) 
		{ // Delete node with record having minimum key value
			if (root == NULL) return false; // Empty
			BinNode<Elem>* t;
			 // return pointer to node with minimum value
			root = deletemin(root, t);
			e = t->val(); // Get the value of t before deleting it
			delete t; nodecount--; return true;
		}

		bool find(const Key& K, Elem*& e) const
			{ return findhelp(root, K, e); }
		 
		int size() { return nodecount; }
		 
		void print() const 
		{
			if (root == NULL)
				cout << "The BST is empty.\n";
			else printhelp(root, 0);
		}
		
		
		
		
		void buildStudents(string filename);
		void buildCourses(string filename);
		
		void printByProgram(string program)
			{ printByProgram_help(program, root); }
		
		void printByProgram_help(string program, BinNode<Elem>* subroot)
		{
			if (subroot == NULL) return;
		 
			else if (program < subroot->val().getCode().substr(0, 2))
				return printByProgram_help(program, subroot->left());

			else if (program > subroot->val().getCode().substr(0, 2))
				return printByProgram_help(program, subroot->right());

			else  {subroot->val().print(); printByProgram_help(program, subroot->right()); }
		}
		
		
		void print_coursesToTakeLeft(string program, stack<Course*>* CoursesTaken)
			{ print_coursesToTakeLeft_help(program, CoursesTaken, root); }

		void print_coursesToTakeLeft_help(string program, stack<Course*>* CoursesTaken, BinNode<Elem>* subroot)
		{
			if (subroot == NULL) return;
			
			if (program < subroot->val().getCode().substr(0, 2))
				return print_coursesToTakeLeft_help(program, CoursesTaken, subroot->left());

			else if (program > subroot->val().getCode().substr(0, 2))
				return print_coursesToTakeLeft_help(program, CoursesTaken, subroot->right());

			else {						// found the course
				stack<Course*> CoursesTakenTemp = *CoursesTaken;
				while (!CoursesTakenTemp.empty()) {									// if the course found is in the stack of taken courses
					if (subroot->val().getCode() == CoursesTakenTemp.top()->getCode()) //  of the student, don't print it 
						return print_coursesToTakeLeft_help(program, CoursesTaken, subroot->right());
					CoursesTakenTemp.pop();
				}
				
				subroot->val().print();
				print_coursesToTakeLeft_help(program, CoursesTaken, subroot->right());
			}
		}
		

		bool find_ByName(const string &studentName, Elem*& e) const
			{ return find_ByName_help(studentName, e, root); }
			
		bool find_ByName_help(const string& studentName, Elem*& e, BinNode<Elem>* subroot) const 
		{	// preorder traversal, because we are searching the BST with a secondary key
			if (subroot == NULL) return false;

			if (studentName == subroot->val().getSName())
				{ e = &(subroot->val());  return true; }
				
			return (find_ByName_help(studentName, e, subroot->left()) || find_ByName_help(studentName, e, subroot->right()));
		}
		
		void updateStatuses()
		{
			stack<string> studentsToRemove;
			updateStatuses_help(root, &studentsToRemove);
			
			while (!studentsToRemove.empty()) {
				remove(studentsToRemove.top());
				studentsToRemove.pop();
			}
		}
		
		void updateStatuses_help(BinNode<Elem>* subroot, stack<string>* studentsToRemove)
		{
			if (subroot == NULL) return;
			
			if (!subroot->val().updateStatus())						// if the student has failed a course twice
				studentsToRemove->push(subroot->val().getSID());		// drop the student
				
			updateStatuses_help(subroot->left(), studentsToRemove);
			updateStatuses_help(subroot->right(), studentsToRemove);
		}
};



template <class Key, class Elem, class KEComp, class EEComp>
bool BST<Key, Elem, KEComp, EEComp>:: findhelp(BinNode<Elem>* subroot, const Key& K, Elem*& e) const 
{
	if (subroot == NULL) return false;
 
	else if (KEComp::lt(K, subroot->val())) 
		return findhelp(subroot->left(), K, e);

	else if (KEComp::gt(K, subroot->val()))   
		return findhelp(subroot->right(), K, e);

	//found it. Get its value e which will traverse through the
	// chain of recursive calls all the way to the calling function
	else { e = &(subroot->val());  return true; }
}


template <class Key, class Elem, class KEComp, class EEComp>
BinNode<Elem>* BST<Key,Elem,KEComp,EEComp>:: inserthelp(BinNode<Elem>* subroot, const Elem& val) 
{
	if (subroot == NULL) // Empty: create node
	// the returned node becomes an argument to setLeft or setRight
		return new BinNode<Elem>(val, NULL, NULL);

	 // application of EEComp: comparing node values (records)
	if (EEComp::lt(val, subroot->val()))
		subroot->setLeft(inserthelp(subroot->left(), val));
	else
		subroot->setRight(inserthelp(subroot->right(), val));
	 // Return subtree with node inserted
	return subroot;
}

template <class Key, class Elem, class KEComp, class EEComp>
BinNode<Elem>* BST<Key, Elem, KEComp, EEComp>:: deletemin(BinNode<Elem>* subroot, BinNode<Elem>*& min) 
{
	if (subroot->left() == NULL) 
	{
		min = subroot;
		return subroot->right();
	}
	else 
	{ // Minimum value is always found as the left-most node
		subroot->setLeft(
			deletemin(subroot->left(), min));
		return subroot; // in the last step into the recursion, the original subroot will be returned
	}
}

template <class Key, class Elem, class KEComp, class EEComp>
BinNode<Elem>* BST<Key,Elem,KEComp,EEComp>:: removehelp(BinNode<Elem>* subroot, const Key& K, BinNode<Elem>*& t) 
{ //t is the node to remove (if we find it using K)
	if (subroot == NULL) return NULL;

	else if (KEComp::lt(K, subroot->val()))
		subroot->setLeft(removehelp(subroot->left(), K, t));

	else if (KEComp::gt(K, subroot->val()))
		subroot->setRight(removehelp(subroot->right(), K, t));

	else 
	{ // Found it: remove it
		BinNode<Elem>* temp;
		t = subroot;
		if (subroot->left() == NULL) //replace it with
			subroot = subroot->right();//its right child
		else if (subroot->right() == NULL)//replace it
			subroot = subroot->left(); //with its left child
		else 
		{ // Both children are non-empty; look for min in right subtree
		 deletemin(subroot->right(), temp);
			Elem te = subroot->val();
			subroot->setVal(temp->val());
			temp->setVal(te);
			t = temp;
		} 
	}
		return subroot;
}


template <class Key, class Elem, class KEComp, class EEComp>
void BST<Key,Elem,KEComp,EEComp>:: clearhelp(BinNode<Elem>* root)
{
	if (root == NULL) return;
	clearhelp(root->left());
	clearhelp(root->right());
	delete root;
}

template <class Key, class Elem, class KEComp, class EEComp>
void BST<Key,Elem,KEComp,EEComp>:: printhelp(BinNode<Elem>* root, int level) const
{	// inorder traversal, prints in increasing order
	if (root == NULL) return;
	printhelp(root->left(), level+1);
//	cout<<root->val()<<endl;	// normal printing
	root->val().print();		// Student/Course print function call
	printhelp(root->right(), level+1);
}










void split(string str, string items[], int item_total, int item_nb = 0, int pos = 0)
{	// splits "item_total" number of elements in items[] separated by ',' inside str
	if (item_nb == item_total)	return;
	int initial_pos = pos;

	for (pos = pos; pos <= str.length(); pos++)
		if (pos == str.length() || str[pos] == ',')
		{
			items[item_nb] = str.substr(initial_pos, pos - initial_pos);
			return split(str, items, item_total, item_nb + 1, pos + 1);
		}
}


template <class Key, class Elem, class KEComp, class EEComp>
void BST<Key,Elem,KEComp,EEComp>:: buildStudents(string filename)
{
//	this->clear(); ifstream inFile(filename);
//	string line, elements[4];
//	while(getline(inFile, line))
//	{
//		split(line, elements, 4);
////		for (int i = 0; i < 4; i++)
////			cout<<elements[i]<<" ";
////		cout<<endl;
//		this->insert(Elem(elements[0], elements[1], elements[2], elements[3]));
//	}
	
	this->clear(); ifstream inFile(filename);
	string ID, name, program, status;
	
	while (getline(inFile, ID, ','))
	{
		getline(inFile, name, ',');
		getline(inFile, program, ',');
		getline(inFile, status, '\n');
//		cout<<ID<<" "<<name<<" "<<program<<" "<<status<<endl;
		this->insert(Elem(ID, name, program, status));
	}
}

template <class Key, class Elem, class KEComp, class EEComp>
void BST<Key,Elem,KEComp,EEComp>:: buildCourses(string filename)
{
	this->clear(); ifstream inFile(filename);
	string line, elements[4];
	while(getline(inFile, line))
	{
		split(line, elements, 4);
		this->insert(Elem(elements[0], elements[1], elements[2].at(0), elements[3]));
	}
}



#endif