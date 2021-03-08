#ifndef INTERVAL_LIST_H
#define INTERVAL_LIST_H

#include <vector>
#include <cmath>

class interval_list {
	private:
		std::vector<interval>* intervals;

	public:
	/* Constructor */
	interval_list() { intervals = new vector<vec3>(); }

	/* Adds an interval to the list
	*	@in: interval to add
	*/
	void add(interval in) {
		intervals->push_back(in);
	}

	/* Gets the interval at index i
	*	@index: The index of the element
	*	returns the element at i
	*/
	interval at(int index) {
		return (*intervals)[index];
	}

	/* Gets the size of the intervals
	*	returns the size of the intervals
	*/
	int size() {
		return intervals->size();
	}

	/* Copy Constructor */
	interval_list(const interval_list& in) {
		interval_list im = in;
		for (int i = 0; i < im->size(); i++) {
			intervals.push_back(im.at(i));
		}
	}

	/* Removes the ith element from list
	*	@index: the ith index of element
	*	returns the interval that was removed
	*/
	interval remove(int index) {
		interval in = *(intervals.begin() + index);
		intervals.erase(intervals.begin() + index);
		return in;
	}
};

#endif
