#ifndef MYSTACK_H_
#define MYSTACK_H_

class MyStack{
private:
	int capacity;
	int* items;
	int topIndex = -1;

public:
	MyStack();
	MyStack(int n);
	~MyStack();

	bool isEmpty();
	void push(const int& v);
	int top();
	int pop();
};

#endif /* MYSTACK_H_ */
