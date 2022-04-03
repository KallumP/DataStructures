#include <iostream>
#include <vector>
#include <algorithm>

#include "Grapher.h"


//std::vector<int> base;
int* base;
int comparisonCount;

//fills the array with a given amount of random numbers (sorted) between min and max
void FillListRandom(int amount, int gapMin, int gapMax) {

	base = new int[amount];
	int numberToAdd = 0;

	srand(time(0));

	//adds in the wanted amount of values
	for (int i = 0; i < amount; i++) {

		//gets the next random number
		numberToAdd += rand() % (gapMax - gapMin) + gapMin;

		//adds the random number
		base[i] = numberToAdd;
	}
}

//fills the list with a given amount of sequential numbers
void FillListSequential(int amount, int step) {

	base = new int[amount];
	for (int i = 0; i < amount; i++)
		base[i] = i * step;
}

//fills a list with a geometric progression
void FillListGeometric(int amount, double ratio) {

	base = new int[amount];
	for (int i = 0; i < amount; i++)
		base[i] = std::pow(ratio, i - 1);
}

//fills a list a perfectly fitting geometric progression
float GetBestRatio(int amount) {

	int max = 2147483647;
	double ratio = std::pow(max, 1 / (double)(amount - 1));

	return ratio;
}

//outputs a slice of the array
void OutputList(int bottom, int top) {

	for (int i = bottom; i <= top; i++)
		std::cout << "Index: " << i << " Value: " << base[i] << std::endl;
}

//constrains a value between two bounds
int Constrain(int value, int lowerBound, int upperBound) {
	if (value < lowerBound)
		return lowerBound;
	else if (value > upperBound)
		return upperBound;
	return value;
}

//interpolation index search using passed start and end indexes
int InterpolateIndex(int key, int bottom, int top) {

	float k = key;
	float VTop = base[top];
	float VBottom = base[bottom];

	return Constrain(std::floor((top - bottom) * ((k - VBottom) / (VTop - VBottom)) + bottom), bottom, top);
}

//interpolation search. Return the found index, or -1 if not found
int InterpolateSearch(int key, int bottom, int top, bool showArray, bool showAnyCLI) {

	if (showArray) {
		//outputs the array section being worked with
		OutputList(bottom, top);
		std::cout << "^^ Array to search ^^ " << std::endl;
	}

	//outputs the array size
	if (showAnyCLI) {

		std::cout << "Array size left to check: " << 1 + top - bottom << std::endl;
		std::cout << "Start: " << bottom << ", End: " << top << std::endl;

		//outputs what key is being searched for
		std::cout << std::endl;
		std::cout << "Searching for index of key: " << key << std::endl;
	}

	//checks if only one value is left to check
	if (bottom == top)

		//return if the last value was the right one
		if (base[bottom] == key)
			return bottom;
		else
			return -1;

	//gets the estimated position (using the interpolation algorithm)
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	if (showAnyCLI) std::cout << "Estimated position: " << estimatedPosition << std::endl;

	//gets the value at the estimated position
	int valueAtEstimate = base[estimatedPosition];
	if (showAnyCLI) std::cout << "Value at estimated position " << base[estimatedPosition] << std::endl;

	//increases the number of comparisons done
	comparisonCount++;

	if (showAnyCLI) {
		std::cout << "Comparisons done: " << comparisonCount << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}

	//the recursive calls have constrains to stop the ends from overlapping
	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return InterpolateSearch(key, bottom, Constrain(estimatedPosition - 1, bottom, top), showArray, showAnyCLI);
	else if (key > valueAtEstimate)
		return InterpolateSearch(key, Constrain(estimatedPosition + 1, bottom, top), top, showArray, showAnyCLI);
}

//returns the found index by looping sequentially, or returns -1 for not found
int LinearSearch(int key, int bottom, int top) {

	//loops through each value
	for (int i = bottom; i <= top; i++) {

		comparisonCount++;

		//checks if this was the value
		if (base[i] == key)
			return  i;
	}
	return -1;
}

//same as linear search but starting from the top of the array
int LinearSearchBackward(int key, int bottom, int top) {

	//loops backwards through each value
	for (int i = top; i >= bottom; i--) {

		comparisonCount++;

		//checks if this was the value
		if (base[i] == key)

			return  i;
	}

	return -1;
}

//recursivley splits the array in half until one value is left. Return the position if the key was found there or -1 if not
int BinarySearch(int key, int bottom, int top) {

	//if there is only one more value left to check
	if (top == bottom) {

		comparisonCount++;
		//if the key was found in the last position
		if (base[bottom] == key)
			return bottom;
		else
			return -1;
	}

	//finds the middle
	int middle = std::floor(bottom + (top - bottom) / 2);

	comparisonCount++;
	//recalls the search
	if (key <= base[middle])
		BinarySearch(key, bottom, std::max(middle, bottom));
	else if (key > base[middle])
		BinarySearch(key, std::min(middle + 1, top), top);
}

//finds the search space and then calls binary search on it
int ExponentialSearch(int key, int bottom, int top) {

	comparisonCount++;
	//check if key was present at the first address
	if (base[bottom] == key)
		return bottom;

	int i = 1;
	comparisonCount++;
	//keeps looping while there is still array space to check and the current index is smaller than the key
	while (bottom + i < top && base[bottom + i] <= key) {

		comparisonCount++;

		//doubles the seach space
		i = i * 2;
	}

	//calls binary search on the found search space
	return BinarySearch(key, std::max(bottom + 1, bottom + i / 2), std::min(bottom + i, top));
}

//exponential search starting from top to bottom
int ExponentialSearchBackward(int key, int bottom, int top) {

	comparisonCount++;
	//check if key was present at the first address
	if (base[top] == key)
		return top;

	int i = 1;
	comparisonCount++;
	//keeps looping while there is still array space to check and the current index is bigger than the key
	while (top - i > bottom && base[top - i] >= key) {

		comparisonCount++;

		//doubles the seach space
		i = i * 2;
	}

	//calls binary search on the found search space
	return BinarySearch(key, std::max(top - i, bottom), std::min(top - i / 2, top - 1));
}

//interplation and linear hybrid search
int InterpolationLinearSearch(int key, int bottom, int top) {

	//calls the first interplation search
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	int valueAtEstimate = base[estimatedPosition];

	comparisonCount++;
	//the returns the found value or if not calls the linear search on the rest of the array
	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return LinearSearchBackward(key, bottom, Constrain(estimatedPosition - 1, bottom, top));
	else if (key > valueAtEstimate)
		return LinearSearch(key, Constrain(estimatedPosition + 1, bottom, top), top);
}

//interplation and exponential hybrid search
int InterpolationExponentialSearch(int key, int bottom, int top) {

	//calls the first interplation search
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	int valueAtEstimate = base[estimatedPosition];

	comparisonCount++;
	//the returns the found value or if not calls the exponential search on the rest of the array
	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return ExponentialSearchBackward(key, bottom, Constrain(estimatedPosition - 1, bottom, top));
	else if (key > valueAtEstimate)
		return ExponentialSearch(key, Constrain(estimatedPosition + 1, bottom, top), top);
}

//outputs the result
void OutputFinalResult(int key, int returnedIndex) {

	//outputs the result
	if (returnedIndex != -1) {

		std::cout << "Found at: " << returnedIndex << std::endl;
		std::cout << "Key: " << key << std::endl;
		std::cout << "Value at " << returnedIndex << ": " << base[returnedIndex] << std::endl;
		std::cout << "Comparisons done: " << comparisonCount << std::endl;

	} else
		std::cout << "Not found" << std::endl;

}



//tests the base search methods (interpolation, linear, exponential)
void BaseSearchTest() {

	std::cout << "Base search test" << std::endl;

	int arrayCount = 100000;
	int gapMin = 10;
	int gapMax = 0;

	std::cout << "Filling array with random numbers" << std::endl;
	FillListRandom(arrayCount, gapMin, gapMax);
	std::cout << "Array filled, generating random key to search for" << std::endl;

	//gets a key that exists
	srand(time(0));
	int key = base[rand() % arrayCount];
	std::cout << "Key generated";
	std::cout << std::endl << std::endl << std::endl;

	int returnedIndex;

	//calls the interpolation search
	comparisonCount = 0;
	std::cout << "vv Interpolation search vv" << std::endl;
	returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the linear search
	comparisonCount = 0;
	std::cout << "vv Linear search vv" << std::endl;
	returnedIndex = LinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the linear backward search
	comparisonCount = 0;
	std::cout << "vv Linear backward search vv" << std::endl;
	returnedIndex = LinearSearchBackward(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the exponential search
	comparisonCount = 0;
	std::cout << "vv Exponential search vv" << std::endl;
	returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the exponential backward search
	comparisonCount = 0;
	std::cout << "vv Exponential backward search vv" << std::endl;
	returnedIndex = ExponentialSearchBackward(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;
}

//tests the hybrid search methods
void HybridSearchTest() {

	std::cout << "Hybrid search test" << std::endl;

	int arrayCount = 100000;
	int gapMin = 10;
	int gapMax = 0;

	std::cout << "Filling array with random numbers" << std::endl;
	FillListRandom(arrayCount, gapMin, gapMax);
	std::cout << "Array filled, generating random key to search for" << std::endl;

	//gets a key that exists
	srand(time(0));
	int key = base[rand() % arrayCount];
	std::cout << "Key generated";
	std::cout << std::endl << std::endl << std::endl;

	int returnedIndex;

	//calls the interpolation search
	comparisonCount = 0;
	std::cout << "vv Interpolation search vv" << std::endl;
	returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation linear search
	comparisonCount = 0;
	std::cout << "vv Interplation linear search vv" << std::endl;
	returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation exponential search
	comparisonCount = 0;
	std::cout << "vv Interplation exponential search vv" << std::endl;
	returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;
}

//tests searching every value in the array
void ExtensiveTest() {

	std::cout << "Extensive test" << std::endl;

	int arrayCount = 100000;
	int gapMin = 10;
	int gapMax = 0;

	FillListRandom(arrayCount, gapMin, gapMax);
	int key;
	int returnedIndex;

	std::cout << "Starting searching all " << arrayCount << " values" << std::endl;
	for (int i = 0; i < arrayCount; i++) {

		key = base[i];

		//calls the linear search
		returnedIndex = LinearSearch(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Linear search wrong at: " << i << std::endl;

		//calls the linear backward search
		returnedIndex = LinearSearchBackward(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Linear backward search wrong at: " << i << std::endl;

		//calls the exponential search
		returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Exponential search wrong at: " << i << std::endl;

		//exponential backwards
		returnedIndex = ExponentialSearchBackward(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Exponential backward search wrong at: " << i << std::endl;

		//calls the interpolation search
		returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
		if (returnedIndex != i)
			std::cout << "Interpolation search wrong at: " << i << std::endl;

		//calls the interpolation linear search
		returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Interpolation linear search wrong at: " << i << std::endl;

		//calls the interpolation exponential search
		returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
		if (returnedIndex != i)
			std::cout << "Interpolation exponential search wrong at: " << i << std::endl;
	}
	std::cout << "Extensive test done" << std::endl << std::endl;;
}

//tests perfect arithmetic progression values (should always predict with the first interpolation)
void PerfectAPTest(int arrayCount, int gap) {

	std::cout << "Perfect arithmetic progressoin test" << std::endl;


	std::cout << "Filling with sequential numbers" << std::endl;
	FillListSequential(arrayCount, gap);
	std::cout << "Array filled with: " << arrayCount << " gap: " << gap << ", generating key to search for" << std::endl;

	//gets a key that exists
	srand(time(0));
	int key = base[rand() % arrayCount];
	std::cout << "Key generated " << std::endl;
	std::cout << std::endl << std::endl << std::endl;

	int returnedIndex;

	//calls the interpolation search
	comparisonCount = 0;
	std::cout << "vv Interpolation search vv" << std::endl;
	returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation linear search
	comparisonCount = 0;
	std::cout << "vv Interplation linear search vv" << std::endl;
	returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation exponential search
	comparisonCount = 0;
	std::cout << "vv Interplation exponential search vv" << std::endl;
	returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;
}

//tests random numbers that aproximate arithmetic progression
void RandomAPTest(int arrayCount, int gapMin, int gapMax) {

	std::cout << "Aproximating arithmetic progression test" << std::endl;

	std::cout << "Filling with random numbers" << std::endl;
	FillListRandom(arrayCount, gapMin, gapMax);
	std::cout << "Array filled with: " << arrayCount << ", generating random key to search for" << std::endl;

	//gets a key that exists
	srand(time(0));
	int key = base[rand() % arrayCount];
	std::cout << "Key generated " << std::endl;
	std::cout << std::endl << std::endl << std::endl;

	int returnedIndex;

	//calls the interpolation search
	comparisonCount = 0;
	std::cout << "vv Interpolation search vv" << std::endl;
	returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation linear search
	comparisonCount = 0;
	std::cout << "vv Interplation linear search vv" << std::endl;
	returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation exponential search
	comparisonCount = 0;
	std::cout << "vv Interplation exponential search vv" << std::endl;
	returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;
}

//tests numbers that fit a geometric progression
void GeometricTest(int arrayCount, float ratio) {

	std::cout << "Geometric series test" << std::endl;

	std::cout << "Filling with a geometric progression" << std::endl;
	FillListGeometric(arrayCount, ratio);
	std::cout << "Array filled with: " << arrayCount << ", ratio: " << ratio << ", generating random key to search for" << std::endl;
	//OutputList(0, arrayCount - 1);

	//gets a key that exists
	srand(time(0));
	int key = base[rand() % arrayCount];
	std::cout << "Key generated " << std::endl;
	std::cout << std::endl << std::endl << std::endl;

	int returnedIndex;

	//calls the interpolation search
	comparisonCount = 0;
	std::cout << "vv Interpolation search vv" << std::endl;
	returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl << std::endl;

	//calls the linear search
	comparisonCount = 0;
	std::cout << "vv Linear search vv" << std::endl;
	returnedIndex = LinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the interpolation linear search
	comparisonCount = 0;
	std::cout << "vv Interplation linear search vv" << std::endl;
	returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl << std::endl;



	//calls the interpolation exponential search
	comparisonCount = 0;
	std::cout << "vv Interplation exponential search vv" << std::endl;
	returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

	//calls the exponential search
	comparisonCount = 0;
	std::cout << "vv Exponential search vv" << std::endl;
	returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
	OutputFinalResult(key, returnedIndex);
	std::cout << std::endl;

}

//graphs doing a set of arithmetic series approximation arrays with random keys
void GraphArithmeticRandomKeys(int minArray, int maxArray, int arrayStep, int searchCount, bool debug) {

	std::cout << "Graphing aproximating arithmetic progression random keys" << std::endl;

	Grapher* g = new Grapher();

	//loops through the different array sizes (powers of ten)
	for (int i = minArray; i <= maxArray; i += arrayStep) {

		int arrayCount = i;
		//std::cout << "Progress: " << i << "/" << maxArray << std::endl;

		if (debug)
			std::cout << "Filling with a arithmetic progression" << std::endl;
		FillListRandom(arrayCount, 0, 100);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", generating random key to search for" << std::endl;

		srand(time(0));

		//does the search on a set amount of random keys
		for (int i = 0; i < searchCount; i++) {

			//gets a key that exists
			int key = base[rand() % arrayCount];
			if (debug)
				std::cout << "Key generated: " << key << std::endl;

			int returnedIndex;

			//calls the interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
			g->TakeSizeValues(arrayCount, comparisonCount, -1, -1, -1, -1);

			//calls the linear search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = LinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, comparisonCount, -1, -1, -1);

			//calls the exponential search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, comparisonCount, -1, -1);

			//calls the linear interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, comparisonCount, -1);

			//calls the exponential interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, -1, comparisonCount);
		}
	}

	g->DrawGraph(L"Arithmetic data random keys", L"Table size", "_arithmetic, random.png", true, true, true, true, true);
	g->DrawGraph(L"Arithmetic data random keys (no linear)", L"Table size", "_arithmetic, random (no linear).png", false, true, true, true, true);

	delete g;

	std::cout << "Arithmetic random done" << std::endl << std::endl;
}

//graphs doing a set of perfect arithmetic series arrays with random keys
void GraphArithmeticBestCase(int minArray, int maxArray, int arrayStep, int searchCount, bool debug) {

	std::cout << "Graphing perfect arithmetic progression random keys" << std::endl;

	Grapher* g = new Grapher();

	//loops through the different array sizes (powers of ten)
	for (int i = minArray; i <= maxArray; i += arrayStep) {

		int arrayCount = i;
		//std::cout << "Progress: " << i << "/" << maxArray << std::endl;

		if (debug)
			std::cout << "Filling with a arithmetic progression" << std::endl;
		FillListSequential(arrayCount, 1);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", generating random key to search for" << std::endl;

		srand(time(0));

		//does the search on a set amount of random keys
		for (int i = 0; i < searchCount; i++) {

			//gets a key that exists
			int key = base[rand() % arrayCount];
			if (debug)
				std::cout << "Key generated: " << key << std::endl;

			int returnedIndex;

			//calls the interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
			g->TakeSizeValues(arrayCount, comparisonCount, -1, -1, -1, -1);

			//calls the linear search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = LinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, comparisonCount, -1, -1, -1);

			//calls the exponential search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, comparisonCount, -1, -1);

			//calls the linear interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, comparisonCount, -1);

			//calls the exponential interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, -1, comparisonCount);
		}
	}

	g->DrawGraph(L"Perfect arithmetic series", L"Table size", "_perfect arithmetic.png", true, true, true, true, true);
	g->DrawGraph(L"Perfect arithmetic series (no linear)", L"Table size", "_perfect arithmetic (no linear).png", false, true, true, true, true);

	delete g;

	std::cout << "Arithmetic perfect done" << std::endl << std::endl;
}

//does set of geometric progression for the given array sizes and graphs the avarage comparisons for each search type
void GraphGeometricRandomKeys(int minArray, int maxArray, int arrayStep, int searchCount, bool debug) {

	std::cout << "Graphing geometric series random keys" << std::endl;

	//gets the best ratio for the largest array size
	double ratio = GetBestRatio(maxArray);

	Grapher* g = new Grapher();

	//loops through the different array sizes (powers of ten)
	for (int i = minArray; i <= maxArray; i += arrayStep) {

		int arrayCount = i;

		if (debug)
			std::cout << "Filling with a geometric progression" << std::endl;
		FillListGeometric(arrayCount, ratio);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", ratio: " << ratio << ", generating random key to search for" << std::endl;

		srand(time(0));

		//does the search on a set amount of random keys
		for (int i = 0; i < searchCount; i++) {

			//gets a key that exists
			int key = base[rand() % arrayCount];
			if (debug)
				std::cout << "Key generated: " << key << std::endl;

			int returnedIndex;

			//calls the interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
			g->TakeSizeValues(arrayCount, comparisonCount, -1, -1, -1, -1);

			//calls the linear search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = LinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, comparisonCount, -1, -1, -1);

			//calls the exponential search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, comparisonCount, -1, -1);

			//calls the linear interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, comparisonCount, -1);

			//calls the exponential interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, -1, comparisonCount);
		}
	}

	g->DrawGraph(L"Geometric data random keys", L"Table size", "_geometric, random.png", true, true, true, true, true);
	g->DrawGraph(L"Geometric data random keys (Only exponentials)", L"Table size", "_geometric, random (only exponentials).png", false, true, false, false, true);

	delete g;

	std::cout << "Geometric random done" << std::endl << std::endl;
}

//does set of geometric progression for the given array sizes and graphs the avarage comparisons for each search type
void GraphGeometricRandomKeysExponentialsOnly(int minArray, int maxArray, int arrayStep, int searchCount, bool debug) {

	std::cout << "Graphing geometric series random keys only exponential searches" << std::endl;

	//gets the best ratio for the largest array size
	double ratio = GetBestRatio(maxArray);

	Grapher* g = new Grapher();

	//loops through the different array sizes (powers of ten)
	for (int i = minArray; i <= maxArray; i += arrayStep) {

		int arrayCount = i;

		if (debug)
			std::cout << "Filling with a geometric progression" << std::endl;
		FillListGeometric(arrayCount, ratio);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", ratio: " << ratio << ", generating random key to search for" << std::endl;

		srand(time(0));

		//does the search on a set amount of random keys
		for (int i = 0; i < searchCount; i++) {

			//gets a key that exists
			int key = base[rand() % arrayCount];
			if (debug)
				std::cout << "Key generated: " << key << std::endl;
			int returnedIndex;


			//calls the exponential search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, comparisonCount, -1, -1);

			//calls the exponential interpolation search and saves its data to the grapher
			comparisonCount = 0;
			returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
			g->TakeSizeValues(arrayCount, -1, -1, -1, -1, comparisonCount);
		}
	}

	g->DrawGraph(L"Geometric data random keys (Only exponentials)", L"Table size", "_geometric, random (only exponentials) large array size.png", false, true, false, false, true);

	delete g;

	std::cout << "Geometric random done" << std::endl << std::endl;
}

//does set of geometric progression for the given array sizes, searches for the first value and graphs the comparisons for each search type
void GeometricBestCase(int minArray, int maxArray, int arrayStep, bool debug) {

	std::cout << "Graphing geometric series best case" << std::endl;

	//gets the best ratio for the largest array size
	double ratio = GetBestRatio(maxArray);

	Grapher* g = new Grapher();

	//loops through the different array sizes (powers of ten)
	for (int i = minArray; i <= maxArray; i += arrayStep) {

		int arrayCount = i;

		if (debug)
			std::cout << "Filling with a geometric progression" << std::endl;
		FillListGeometric(arrayCount, ratio);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", ratio: " << ratio << ", generating random key to search for" << std::endl;

		//gets a key that exists
		int key = base[0];
		if (debug)
			std::cout << "Key generated: " << key << std::endl;

		int returnedIndex;

		//calls the interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
		g->TakeSizeValues(arrayCount, comparisonCount, -1, -1, -1, -1);

		//calls the linear search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = LinearSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(arrayCount, -1, comparisonCount, -1, -1, -1);

		//calls the exponential search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(arrayCount, -1, -1, comparisonCount, -1, -1);

		//calls the linear interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(arrayCount, -1, -1, -1, comparisonCount, -1);

		//calls the exponential interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(arrayCount, -1, -1, -1, -1, comparisonCount);

	}

	g->DrawGraph(L"Geometric data first value key", L"Table size", "_geometric, first value.png", true, true, true, true, true);

	delete g;

	std::cout << "Geometric best case done" << std::endl << std::endl;
}

//graphs how the searches change over different ratios
void GeometricRatios(int arrayCount, int ratioCount, bool debug) {

	std::cout << "Graphing different geometric series middle key" << std::endl;

	//gets the best ratio for the largest array size
	double maxRatio = GetBestRatio(arrayCount);

	double minRatio = 1.0;

	double difference = maxRatio - minRatio;

	double ratioStep = difference / ratioCount;

	Grapher* g = new Grapher();


	for (double i = minRatio; i < maxRatio; i += ratioStep) {

		double ratio = i;

		if (debug)
			std::cout << "Filling with a geometric progression" << std::endl;
		FillListGeometric(arrayCount, ratio);
		if (debug)
			std::cout << "Array filled with: " << arrayCount << ", ratio: " << ratio << ", generating random key to search for" << std::endl;

		//gets a key that exists
		int key = base[arrayCount /2];
		if (debug)
			std::cout << "Key generated: " << key << std::endl;

		int returnedIndex;

		//calls the interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolateSearch(key, 0, arrayCount - 1, false, false);
		g->TakeSizeValues(ratio, comparisonCount, -1, -1, -1, -1);

		//calls the linear search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = LinearSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(ratio, -1, comparisonCount, -1, -1, -1);

		//calls the exponential search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(ratio, -1, -1, comparisonCount, -1, -1);

		//calls the linear interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolationLinearSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(ratio, -1, -1, -1, comparisonCount, -1);

		//calls the exponential interpolation search and saves its data to the grapher
		comparisonCount = 0;
		returnedIndex = InterpolationExponentialSearch(key, 0, arrayCount - 1);
		g->TakeSizeValues(ratio, -1, -1, -1, -1, comparisonCount);

	}

	g->DrawGraph(L"Geometric data different ratios (table size 5000)", L"Ratio", "_geometric, different ratios.png", true, true, true, true, true);
	g->DrawGraph(L"Geometric data different ratios (table size 5000) (Only exponentials)", L"Ratio", "_geometric, different ratios (only exponentials).png", false, true, false, false, true);

	delete g;

	std::cout << "Geometric different ratios done" << std::endl << std::endl;
}


int main() {

	std::string input;
	do {

		//BaseSearchTest();

		//HybridSearchTest();

		//ExtensiveTest();

		//PerfectAPTest(1000, 3);

		//RandomAPTest(100000000, 0, 10);

		//GeometricTest(10000, GetBestRatio(10000));

		GraphArithmeticRandomKeys(100, 30000, 100, 100, false);

		GraphArithmeticBestCase(100, 10000, 100, 10, false);

		GraphGeometricRandomKeys(100, 5000, 10, 1000, false);

		GraphGeometricRandomKeysExponentialsOnly(100, 30000, 50, 500, false);

		GeometricBestCase(100, 5000, 100, false);

		GeometricRatios(5000, 500, false);

		std::cout << "Task A done" << std::endl;
		std::cin >> input;
		system("cls");
		//repeats until x is pressed
	} while (input != "x");
}