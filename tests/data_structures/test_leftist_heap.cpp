#include <cassert>

#include "../../include/leftist_heap.hpp"

int main() {
	LeftistHeap<int> left;
	left.push(4);
	left.push(1);
	left.push(3);
	assert(left.top() == 1 && left.size() == 3);
	left.pop();
	assert(left.top() == 3);

	LeftistHeap<int> right;
	right.push(2);
	right.push(0);
	left.merge(right);
	assert(right.empty() && left.size() == 4);
	assert(left.top() == 0);
	left.pop();
	assert(left.top() == 2);
}
