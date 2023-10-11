#include "HashTableQuad.h"
#include <ctime>
#include <iostream>
using namespace std;

// time: O(n), space: O(1)
HashTableQuad::HashTableQuad(int maxNum, double load)
{
	size = ceil(maxNum/load);
	keys = 0;
	maxload = load;

	size = nextPrime(size);

	for(int i = 0; i < size; i++) {
		table.push_back(0);
	}
}

// time: O(n), space: O(1)
int HashTableQuad::nextPrime(int n)
{
	int prime = n;
	bool isPrime = (prime == 1 || prime == 2) ? true : false;

	while(!isPrime) {
		for(int j = 2; j < prime; j++) {
			if(prime%j == 0) {
				isPrime = false;
				break;
			}
			isPrime = true;
		}
		if(!isPrime) {
			prime++;
		}
	}

	return prime;
}

// time: O(1), space: O(1)
void HashTableQuad::insert(int n)
{
	if(isIn(n)) {
		return;
	}

	int i = 0;

	while(table[(n+(i*i))%size] != 0) {
		i++;
		if(i > size) {
			return;
		}
	}

	table[(n+(i*i))%size] = n;
	keys++;

	while(double(keys)/size > maxload) {
		rehash();
	}
}

// time: O(n), space: O(1)
void HashTableQuad::rehash()
{
	HashTableQuad newTable = HashTableQuad(2*size*maxload, maxload);

	if(newTable.size < 2*size) {
		int temp = newTable.size;
		newTable.size = nextPrime(2*size);
		for(int i = temp; i < newTable.size; i++) {
			newTable.table.push_back(0);
		}
	}

	for(int i = 0; i < size; i++) {
		if(table[i] != 0) {
			newTable.insert(table[i]);
		}
	}

	table = newTable.table;
	size = newTable.size;
	keys = newTable.keys;
}

// time: O(1), space: O(1)
bool HashTableQuad::isIn(int n)
{
	int i = 0;

	while((n+(i*i))%size < size) {
		if(table[(n+(i*i))%size] == n) {
			return true;
		} else if(table[(n+(i*i))%size] == 0) {
			break;
		}
		i++;
	}

	return false;
}

// time: O(n), space: O(1)
void HashTableQuad::printKeys()
{
	for(int i = 0; i < size; i++) {
		if(table[i] != 0) {
			cout << table[i] << " ";
		}
	}
	cout << endl;
}

// time: O(n), space: O(1)
void HashTableQuad::printKeysAndIndexes()
{
	for(int i = 0; i < size; i++) {
		if(table[i] != 0) {
			cout << i << ":" << table[i] << " ";
		}
	}
	cout << endl;
}

int HashTableQuad::getNumKeys() {
    return keys;
}

int HashTableQuad::getTableSize() {
    return size;
}

double HashTableQuad::getMaxLoadFactor() {
    return maxload;
}

int HashTableQuad::isInCount(int n)
{
	int i = 0;

	while((n+(i*i))%size < size) {
		if(table[(n+(i*i))%size] == n) {
			return true;
		} else if(table[(n+(i*i))%size] == 0) {
			break;
		}
		i++;
	}

	return i+1;
}

int HashTableQuad::insertCount(int n)
{
	if(isIn(n)) {
		return isInCount(n);
	}

	while((keys+1.0)/size > maxload) {
		rehash();
	}

	int i = 0;

	while(table[(n+(i*i))%size] != 0) {
		i++;
		if(i > size) {
			return 0;
		}
	}

	table[(n+(i*i))%size] = n;
	keys++;

	return i+1;
}

std::vector<double> HashTableQuad::simProbeSuccess()
{
	vector<double> result(9);
	double load;
	double avg = 0;
	double finalAvg = 0;

	for(int i = 1; i < 10; i++) {
		load = i/10.0;

		for(int k = 0; k < 100; k++) {
			HashTableQuad testHash = HashTableQuad(100000, load);
			avg = 0;
			for(int j = 0; j < 100000; j++) {
				avg += testHash.insertCount((rand()+1)*(rand()+1));
			}
			avg /= 100000;
			finalAvg += avg;
		}
		finalAvg /= 100;
		result[i-1] = finalAvg;
	}

	return result;
}
