#include "MyStack.h"
#include <iostream>

MyStack::MyStack(): MyStack(100) {}

MyStack::MyStack(int n) {
	items = new int[n];
	capacity = n;
}

MyStack::~MyStack() {
	delete[] items;
}

bool MyStack::isEmpty() {
	return (topIndex < 0);
}

void MyStack::push(const int& e) {
	if(topIndex == capacity - 1) {
		int* newArray = new int[2 * capacity];
		for(int i = 0; i < capacity; i++) {
			newArray[i] = items[i];
		}
		delete[] items;
		items = newArray;
		capacity *= 2;
	}
	items[++topIndex] = e;
}

int MyStack::top() {
	if(isEmpty()) {
		throw std::underflow_error("Stack underflow");
	} else {
		return (items[topIndex]);
	}
}

int MyStack::pop() {
	if(isEmpty()) {
		throw std::underflow_error("Stack underflow");
	} else {
		int tempE = items[topIndex];
		topIndex--;
		return (tempE);
	}
}
