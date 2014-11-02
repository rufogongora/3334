#include <iostream>
#include <cstdlib>

/*
	NAME : RODOLFO GONGORA
	SID  : 20214481
	CLASS: CSCI 3334
	TERM : FALL 2014
	***PHASE 1***
*/



//All fancy data structures for optimization and stuff

using namespace std;

class optQ
{
private:
	class node
	{
	public:
		//put what you need in here
		string data;
		
		node * next;

		node (string x)
		{
			data = x;
			next = NULL;			
		}
	};

	node * first, * last;

public:
	
	optQ()
	{
		first = NULL;
		last = NULL;
	}

	~optQ()
	{}

	//return true if empty, false if not
	bool empty()
	{
		if(first == NULL)
			return true;
		else
			return false;
	}

	//add item to top of stack
	void push(string x)
	{
		node* tmp = new node(x);
		tmp->next = NULL;

		if ( first == NULL )
		{
	
			first = last = tmp;
		}
	       	else
		{
			last->next = tmp;
			last = tmp;
	       }
	}

	//remove and return top item from stack
	string pop()
	{
		if (first != NULL)
		{
			string x = first->data;
			node * tmp = first;
			first = first->next;
			delete tmp;
			return x;
		}
		return "";
	}

	void print()
	{
		node * cntr = first;
		while (cntr != NULL)
		{
			cout<<cntr->data<<endl;
			cntr = cntr->next;
		}

	}

};

class tokenList
{
	class node
	{
		public:
		string data;
		node * next;
		node(string x)
		{
			data = x;
			next = NULL;
		}
	};
	node * first;
	int count;

public:
	tokenList()
	{
		count = 0;		
		first = NULL;
	}
	void addToken(string word)
	{
		if (count == 0)
		{
			node * adder = new node(word);
			first = adder;
		}
		else
		{	
			node * newNode = new node(word);
			node * counter = first;
			while(counter->next != NULL)
			{
				counter = counter->next;
			}
			counter->next = newNode;
			
		}
		count++;
	}
	void print()
	{
		print(first);
	}
	void print(node * aNode)
	{
		if (aNode != NULL)
		{
			cout<<aNode->data<<endl;
			print(aNode->next);
		}
	}
	int size()
	{
		return count;
	}
	bool isEmpty()
	{
		return count == 0;
	}
	optQ * createQueue()
	{
		node * currNode = first;
		optQ * aQueue = new optQ();
		for (int i = 0; i < size(); i++)
		{
			if (currNode != NULL)
			{
				aQueue->push(currNode->data);
				currNode = currNode->next;
			}
		}
		return aQueue;
	}
};
