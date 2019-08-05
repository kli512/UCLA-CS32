#include "Set.h"
#include <iostream>

using namespace std;

Set::Set() {
	// initializes an empty set
	n_items = 0;
	head = nullptr;
	tail = nullptr;
}

Set::~Set() {
	// if set is empty, no dynamic memory to clear
	if (head == nullptr)
		return;

	// clears dynamically allocated memory
	Node* cNode = head;
	Node* next;

	while (cNode != nullptr) {
		next = cNode->nextNode;
		delete cNode;
		cNode = next;
	}
}

void Set::dump() const {
	// dumps information about set for debugging

	// dumping information on the set
	cerr << "--------------------" << endl;
	cerr << "Set at: " << this << endl;
	if (head == nullptr) {
		cerr << "Head at: nullptr" << endl;
	}
	else {
		cerr << "Head at: " << head << endl;
	}

	if (tail == nullptr) {
		cerr << "Tail at: nullptr" << endl;
	}
	else {
		cerr << "Tail at: " << tail << endl;
	}
	cerr << "n_items: " << n_items << endl << endl;

	// dumping information of the internally linked list
	for (Node* cNode = head; cNode != nullptr; cNode = cNode->nextNode) {
		cerr << "Address: " << cNode;
		cerr << "\nm_item: " << cNode->m_item;
		if (cNode->prevNode == nullptr) {
			cerr << "\nprevNode: nullptr";
		}
		else {
			cerr << "\nprevNode: " << cNode->prevNode;
		}

		if (cNode->nextNode == nullptr) {
			cerr << "\nnextNode: nullptr";
		}
		else {
			cerr << "\nnextNode: " << cNode->nextNode;
		}
		cerr << endl << endl;
	}
	cerr << "--------------------" << endl;

}

Set::Set(const Set& other) {
	// sets fields based on other
	n_items = other.n_items;

	if (other.n_items == 0) { // if other is empty, set this to empty and return
		head = nullptr;
		tail = nullptr;
		return;
	}

	// creating new linked list
	head = new Node;
	head->prevNode = nullptr;

	Node* m_node = head;

	// copies all but the last node, dynamically allocating new nodes along the way
	for (Node* cNode = other.head; cNode->nextNode != nullptr; cNode = cNode->nextNode) {
		Node* newNode = new Node;

		m_node->m_item = cNode->m_item;
		m_node->nextNode = newNode;
		newNode->prevNode = m_node;

		m_node = newNode;
	}

	//copies data to last node and sets next to nullptr
	m_node->m_item = other.tail->m_item;
	m_node->nextNode = nullptr;

	//sets tail pointer
	tail = m_node;
}

Set& Set::operator=(const Set& rhs) {
	// checks if lhs and rhs are the same list
	if (this != &rhs) {
		Set temp(rhs); // creates temporary copy of rhs
		swap(temp); // switches contents of lhs and temp
	} // temp will be destroyed at the end of this loop and the dynamically allocated memory will be cleared if ~Set() is done correctly

	return *this;
}

bool Set::empty() const {
	return n_items == 0; // returns whether or not the size is 0
}

int Set::size() const {
	return n_items; // returns the size
}

bool Set::insert(const ItemType& value) {
	if (contains(value)) // checks if value already exists in the set
		return false;

	if (n_items == 0) { // since there is no dummy node, case for first node being inserted
		// creates new node and sets its fields
		Node* newNode = new Node;
		newNode->m_item = value;
		newNode->prevNode = nullptr;
		newNode->nextNode = nullptr;

		// sets the set's fields
		head = newNode;
		tail = newNode;

		//increments number of items
		n_items++;
		return true;
	}

	// creates new node and sets its fields
	Node* newNode = new Node;
	newNode->m_item = value;
	newNode->prevNode = tail;
	newNode->nextNode = nullptr;

	// sets the set's fields
	tail->nextNode = newNode;
	tail = newNode;

	//increments number of items
	n_items++;
	return true;
}

bool Set::erase(const ItemType& value) {
	if(!contains(value)) // checks if value exists within the set
		return false;

	Node* cNode = head;
	for (; cNode != nullptr; cNode = cNode->nextNode) { // iterates through the list to find the node corresponding to value
		if (cNode->m_item == value)
			break;
	}

	if (cNode->prevNode != nullptr) {
		cNode->prevNode->nextNode = cNode->nextNode; // links the node above value to the one below
	}
	else {
		head = cNode->nextNode; // if removing head node, point head to new first node
	}

	if (cNode->nextNode != nullptr) {
		cNode->nextNode->prevNode = cNode->prevNode; // links the node below value to the one above
	}
	else {
		tail = cNode->prevNode; // if removing tail node, point tail to new tail node
	}

	n_items--; // decrements number of items
	delete cNode; // deletes the removed node
	return true;
}

bool Set::contains(const ItemType& value) const {
	for (Node* cNode = head; cNode != nullptr; cNode = cNode->nextNode) { // iterates through linked list
		if (cNode->m_item == value)
			return true; // returns true if value is found
	}

	return false;
}

bool Set::get(int pos, ItemType& value) const {
	if (pos >= n_items || pos < 0) // checks if given index is valid
		return false;

	Set t_set(*this); // creates a temporary copy of the set to erase values from
	
	for (int i = 0; i < pos; i++) { // removes the minimum value from t_set i times
		ItemType min = t_set.head->m_item;
		for (Node* cNode = t_set.head; cNode != nullptr; cNode = cNode->nextNode) { // finds the min value in t_set
			min = (cNode->m_item < min) ? cNode->m_item : min;
		}
		t_set.erase(min);
	}

	ItemType min = t_set.head->m_item;
	for (Node* cNode = t_set.head; cNode != nullptr; cNode = cNode->nextNode) { // finds minimum value left in t_set and sets min to it
		min = (cNode->m_item < min) ? cNode->m_item : min;
	}

	value = min; // sets value to the desired data

	return true;
}

void Set::swap(Set& other)
{
	// swaps the number of items in each set
	int t_items = n_items;
	n_items = other.n_items;
	other.n_items = t_items;

	// swaps the heads of the sets
	Node* t_head = head;
	head = other.head;
	other.head = t_head;

	//swaps the tails of the sets
	Node* t_tail = tail;
	tail = other.tail;
	other.tail = t_tail;
}

void unite(const Set& s1, const Set& s2, Set& result) {
	result = s1; // copies the contents of s1 in result

	for (int i = 0; i < s2.size(); i++) { // inserts elements of s2 into s1 by element
		ItemType to_insert; // temp variable to store what to insert
		s2.get(i, to_insert);
		result.insert(to_insert); // implementation of insert means no repeats
	}
}

void subtract(const Set& s1, const Set& s2, Set& result) {

	if (&s1 == &s2) { // if subtracting a set from itself, set result to an empty set
		Set empty;
		result = empty;
		return;
	}
	
	result = s1;

	for (int i = 0; i < s2.size(); i++) { // removes elemnts of s2 from s1
		ItemType to_erase; // temp variable to store what to remove
		s2.get(i, to_erase);
		 
		result.erase(to_erase); // implementation of erase means to_erase only removed it if exists
	}
}