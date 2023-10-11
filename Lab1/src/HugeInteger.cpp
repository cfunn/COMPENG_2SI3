#include "HugeInteger.h"
using namespace std;

HugeInteger::HugeInteger(const std::string& val) {
	int valSize = val.size();

	if(valSize == 0) {
		throw("String cannot be empty.");
	}

	pos = val[0] == '-' ? false : true;
	size = pos ? valSize : valSize - 1;

	for(int i = pos ? 0 : 1; i < valSize; i++) {
		if(!isdigit(val[i])) {
			throw("String must contain digits only.");
		}
		hugeInt.push_back(val[i] - '0');
	}
}

HugeInteger::HugeInteger(int n) {
	if(n <= 0) {
		throw("Must be a positive integer.");
	}

	pos = rand() % 2;
	size = n;

	hugeInt.push_back((rand() % 9) + 1);

	for(int i = 1; i < size; i++) {
		hugeInt.push_back(rand() % 10);
	}
}

HugeInteger HugeInteger::add(const HugeInteger& h) {
	int add, carry = 0;
	string sum = "";
	auto i1 = h.hugeInt.end();
	auto i2 = hugeInt.end();

	if((pos && !h.pos) || (!pos && h.pos)) { // subtract instead if one is negative
		HugeInteger new_h = h;
		new_h.pos = !new_h.pos;
		return subtract(new_h);
	}

	while(i1 != h.hugeInt.begin() && i2 != hugeInt.begin()) { // iterate until one of the vectors reaches its beginning
		i1--;
		i2--;
		add = *i1 + *i2 + carry;
		sum.insert(0, 1, char(add%10 + '0'));
		carry = add/10;
	}

	while(i1 != h.hugeInt.begin()) { // if the first one is longer
		i1--;
		add = *i1 + carry;
		sum.insert(0, 1, char(add%10 + '0'));
		carry = add/10;
	}

	while(i2 != hugeInt.begin()) { // if the second one is longer
		i2--;
		add = *i2 + carry;
		sum.insert(0, 1, char(add%10 + '0'));
		carry = add/10;
	}

	if(carry > 0) {
		sum.insert(0, 1, char(carry + '0'));
	}

	if (!pos && !h.pos) {
		sum.insert(0, 1, '-');
	}

	return HugeInteger(sum);
}

HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	HugeInteger new_h = h;

	while(new_h.size < size) {
		new_h.hugeInt.emplace(new_h.hugeInt.begin(), 0);
		new_h.size++;
	}

	if((!h.pos && pos) || (!pos && h.pos)) {
		new_h.pos = !new_h.pos;
		return add(new_h).removeLeadingZeros();
	}
	else if(!pos && !h.pos) {
		new_h.pos = !new_h.pos;
		pos = !pos;
		new_h = subtract(new_h);
		new_h.pos = !new_h.pos;
		return new_h.removeLeadingZeros();
	}

	const HugeInteger comp = complement(new_h); // 10s complement of h
	HugeInteger diff = add(comp);

	if(compareTo(h) != -1) {
		diff = HugeInteger(diff.toString().erase(0, 1));
	} else {
		diff = complement(diff);
		diff.pos = false;
	}

	return diff.removeLeadingZeros();
}

HugeInteger HugeInteger::complement(const HugeInteger& h) {
	string comp_str = "";

	for (int i = 0; i < h.size; i++) {
		comp_str += char((9 - h.hugeInt.at(i)) + '0');
	}

	HugeInteger comp = HugeInteger(comp_str).add(HugeInteger("1"));

	return comp;
}

HugeInteger HugeInteger::removeLeadingZeros() {
	string str = toString();

	if(!pos) { // take away negative sign
		str.erase(0, 1);
	}

	while(str.size() > 1 && str[0] == '0') {
		str.erase(0, 1);
		size--;
	}

	if(!pos && str != "0") { // add back negative sign if needed
		str.insert(0, 1, '-');
	}

	return HugeInteger(str);
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	string product = "";
	HugeInteger sum = HugeInteger("0");
	int multiply = 0;
	int carry = 0;

	for(int i = h.size-1; i >= 0; i--) {
		for(int n = 0; n < h.size-1-i; n++) { // adding zeros to end if needed
			product += "0";
		}
		for(int j = size-1; j >= 0; j--) {
			multiply = (hugeInt.at(j) * h.hugeInt.at(i)) + carry;
			carry = multiply / 10;
			product.insert(0, 1, char(multiply%10 + '0'));
		}
		if(carry != 0) {
			product.insert(0, 1, char(carry + '0'));
			carry = 0;
		}
		sum = sum.add(HugeInteger(product));
		product = "";
	}

	if((pos && !h.pos) || (!pos && h.pos)) {
		sum.pos = !sum.pos;
	}

	return sum.removeLeadingZeros();
}

int HugeInteger::compareTo(const HugeInteger& h) {
	if((pos && !h.pos) || (size > h.size && pos && h.pos) || (size < h.size && !pos && !h.pos)) {
		return 1;
	} else if((!pos && h.pos) || (size < h.size && pos && h.pos) || (size > h.size && !pos && !h.pos)) {
		return -1;
	}

	for(int i = 0; i < size; i++) {
		if((hugeInt.at(i) > h.hugeInt.at(i) && pos && h.pos) || (hugeInt.at(i) < h.hugeInt.at(i) && !pos && !h.pos)) {
			return 1;
		} else if((hugeInt.at(i) < h.hugeInt.at(i) && pos && h.pos) || (hugeInt.at(i) > h.hugeInt.at(i) && !pos &&!h.pos)) {
			return -1;
		}
	}

	return 0;
}

std::string HugeInteger::toString() {
	string str = pos ? "" : "-";

	for(int i = 0; i < size; i++) {
		str += char(hugeInt.at(i) + '0');
	}

	return str;
}
