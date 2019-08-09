/*
 * SearchComp.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: alex
 */

#include <iostream>
#include <ctime>
#include <random>
using std::cout; using std::cin;

int myPow(int x, int p)
{
  if (p == 0) return 1;
  if (p == 1) return x;

  int tmp = myPow(x, p/2);
  if (p%2 == 0) return tmp * tmp;
  else return x * tmp * tmp;
}

// Sequential search
double seq_search(int target, int array[], int n) {
	clock_t start;
	start = clock();

	for (int i = 0; i < n; ++i) {
		if (array[i] == target) break;
	}

	// return i
	return (clock() - start)/ (double) CLOCKS_PER_SEC;
}

// Standard binary search - determine which half target is in, then repeat on smaller section
double bin_search(int target, int array[], int n) {
	clock_t start;
	start = clock();

	int right_bound = n;
	int left_bound = -1;
	int mid;

	// Divide subarray by 2 each check
	while (right_bound - left_bound > 1) {
		mid = (left_bound + right_bound)/2;
		if (array[mid] < target) {
			left_bound = mid;
		} else {
			right_bound = mid;
		}
	}

	// return mid
	return (clock() - start)/ (double) CLOCKS_PER_SEC;
}

// Quadratic binary search - determine which subarray of size sqrt(n) target is in, then determine which sub-subarray of size sqrt(sqrt(n)), etc...
double quad_bin_search(int target, int array[], int n) {
	clock_t start;
	start = clock();

	int right_bound = n;
	int left_bound = -1;

	// Get high and low keys, and check if we're done
	int high_key = array[right_bound-1];
	int low_key = array[left_bound + 1];
	if (high_key == low_key && array[0] == target) return (clock() - start)/ (double) CLOCKS_PER_SEC; // return high_key (or low_key)

	// Cast target to double so that we get floating point math before casting back to int
	int pos = (int) (high_key*((double)target - low_key)/(high_key-low_key));
	int interval = (int) sqrt(n);

	// Move to other side of target
	if (array[pos] < target) {
		while (array[pos] < target) {
			pos = pos + interval;
		}
	} else if (array[pos] > target) {
		while (array[pos] > target) {
			pos = pos - interval;
		}
	}

	// Recurse with interval of loop-th root of n, using subarray picked out above
	return (clock() - start)/ (double) CLOCKS_PER_SEC + quad_bin_search(target, &array[pos], interval);

}

// Driver to generate sorted arrays up to 10^MAX_SIZE elements, then compare the search methods for NUM_SEARCHES searches
int main() {

	const int MAX_SIZE = 6;
	const int NUM_SEARCHES = 100;
	int target;
	double total_seq_time, total_bin_time, total_quad_time;

	std::random_device rd;
	std::mt19937 eng(rd());

	// See when each starts to outperform others
	for (int i = 1; i <= MAX_SIZE; ++i) {

		int size = myPow(10,i);
		int array[size];

		std::uniform_int_distribution<> distr(0,size-1);

		// Init array
		for (int j = 0; j < size; ++j) {
			array[j] = j;
		}

		// Perform requested number of searches for random integer in range.
		total_seq_time = 0;
		total_bin_time = 0;
		total_quad_time = 0;
		for (int j = 0; j < NUM_SEARCHES; ++j) {
			target = distr(eng);
			total_seq_time += seq_search(target, array, size);
			total_bin_time += bin_search(target, array, size);
			total_quad_time += quad_bin_search(target, array, size);
		}

		// Print results for this array size
		cout << "Sequential search takes " << total_seq_time/1000. << " s for an input of size " << size << "\n";
		cout << "Binary search takes " << total_bin_time/1000. << " s for an input of size " << size << "\n";
		cout << "Quadratic binary search takes " << total_quad_time/1000. << " s for an input of size " << size << "\n";
		cout << "\n";

	};

}
