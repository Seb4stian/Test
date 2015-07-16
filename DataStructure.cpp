#include <iostream>
#include <stdio.h>
#include <unordered_map>

using namespace std;

struct Node {
	int value;
	void * datos;
	Node * next;
	Node * prev;
};

class Lista {

protected:
	Node * head;
	Node * tail;
public:
	Lista() {
		head = tail = NULL;
	}
	virtual void addElement( int v ) {
		Node * tem = new Node();
		tem->value = v;
		tem->prev = tem->next = NULL;

		if (tail == NULL) {
			head = tail = tem;
		}
		else {
			tail->next = tem;
			tail = tem;
		}
	}
	virtual void print() {
		
		Node * tem = head;
		while (tem != NULL) {
			cout <<  tem->value << ", ";
			tem = tem->next;
		}
	}

};

class DoubleLista : public Lista {

public:
	virtual void addElement(int v) {
		Node * tem = new Node();
		tem->value = v;
		tem->prev = tem->next = NULL;
		if (tail == NULL) {
			head = tail = tem;
		}
		else {
			tail->next = tem;
			tem->prev = tail;
			tail = tem;
		}
	}
	virtual void addElement(Node * tem) {
	
		if (tem == NULL)
			return;

		tem->prev = tem->next = NULL;
		if (tail == NULL) {
			head = tail = tem;
		}
		else {
			tail->next = tem;
			tem->prev = tail;
			tail = tem;
		}
	}
	virtual void removeLast() {
		if (tail != NULL) {
			Node * tem = tail;
			tail = tail->prev;
			if (tail != NULL) {
				tail->next = NULL;
			}
			else {
				head = NULL;
			}
			delete tem;
		}
	}
	void printReversa() {
		Node * tem = tail;
		while (tem != NULL) {
			cout << tem->value << ", ";
			tem = tem->prev;
		}
	}
};

class Pila : public DoubleLista {

public:
	void push(int v) {
		DoubleLista::addElement(v);
	}
	void push(Node * v) {
		DoubleLista::addElement(v);
	}
	int pop() {

		int v = 0;
		v = seek();
		removeLast();
		return v;

	}
	void removeFirst() {

		if (head == NULL)
			return;

		Node * tem = head;
		head = head->next;
		delete tem;

	}
	int seek() {

		int v = 0;
		if (tail != NULL) {
			v = tail->value;
		}
		return v;

	}
	void print() {
		DoubleLista::printReversa();
	}
};

class CacheList : public Pila {

public:
	virtual void addElement(int v, void * data) {
		
		Node * tem = new Node();
		tem->value = v;
		tem->datos = data;
		tem->prev = tem->next = NULL;
		if (tail == NULL) {
			head = tail = tem;
		}
		else {
			tail->next = tem;
			tem->prev = tail;
			tail = tem;
		}

	}
	virtual void print() {
		
		Node * tem = head;
		while (tem != NULL) {
			cout << tem->value <<  ": " << (char *)(tem->datos) << endl;
			tem = tem->next;
		}

	}
	virtual void * getData(int key) {

		Node * tem = head;
		while (tem != NULL) {
			if (tem->value == key)
				return tem->datos;

			tem = tem->next;
		}
		return NULL;

	}
	void moveToBegining(Node * node) {

		if (node == NULL || node == tail )
			return;

		if (node == head)
			head = head->next;

		if (node->prev != NULL)
			node->prev->next = node->next;

		if (node->next != NULL)
			node->next->prev = node->prev;

		push(node);
	}
};

class Datos {

protected:
	CacheList * datos;

public:
	Datos() {
		datos = new CacheList();
		datos->addElement(1, const_cast<char*>("uno") );
		datos->addElement(2, const_cast<char*>("dos") );
		datos->addElement(3, const_cast<char*>("tres") );
		datos->addElement(4, const_cast<char*>("cuatro") );
		datos->addElement(5, const_cast<char*>("cinco") );
		datos->addElement(6, const_cast<char*>("seix"));
		datos->addElement(7, const_cast<char*>("siete"));
		datos->addElement(8, const_cast<char*>("ocho"));
		datos->addElement(9, const_cast<char*>("nueve"));
		datos->addElement(10, const_cast<char*>("dies"));
	}

	virtual void * getData(int key) {

		return datos->getData(key);

	}
};

class DatosLRUCache : public Datos {

private: 
	unordered_map<int, Node *> mymap;
	CacheList lista;
	int size = 0;
	int maxSize = 5;
	void addToCache(Node * node) {

		lista.push(node);
		mymap[node->value] = node;
		if (size < maxSize)
			size++;
		else {
			lista.removeFirst();
			mymap.erase(node->value);
		}

	}
	void moveToBegining( Node * node ) {

		lista.moveToBegining(node);

	}
	void print() {
		cout << endl;
		lista.print();
		cout << endl;
	}

public:

	virtual void * getData(int key) {
	
		Node * node = mymap[key];
		if (node != NULL) {

			moveToBegining(node);
			print();
			return node->datos;

		}
		node = new Node();
		node->datos = datos->getData(key);
		node->value = key;
		node->next = node->prev = NULL;
		addToCache(node);
		print();
		return node->datos;

	}

};

int main()
{

	DatosLRUCache * datos = new DatosLRUCache();
	cout << (char *)datos->getData(1) << endl;
	cout << (char *)datos->getData(2) << endl;
	cout << (char *)datos->getData(4) << endl;
	cout << (char *)datos->getData(4) << endl;
	cout << (char *)datos->getData(2) << endl;
	cout << (char *)datos->getData(3) << endl;
	cout << (char *)datos->getData(1) << endl;
	cout << (char *)datos->getData(3) << endl;
	cout << (char *)datos->getData(6) << endl;
	cout << (char *)datos->getData(7) << endl;
	cout << (char *)datos->getData(1) << endl;
	cout << (char *)datos->getData(4) << endl;
	cout << "hola mundo" << endl;
    return 0;
}

