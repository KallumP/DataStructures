#include <iostream>
#include <vector>
#include <algorithm>



//std::vector<int> base;
int* base;
int comparisonCount;

//fills the array with a given amount of random numbers (sorted) between min and max
int FillListRandom(int amount, int gapMin, int gapMax) {

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

	return numberToAdd;
}

//fills the list with a given amount of sequential numbers
int FillList(int amount, int step) {

	base = new int[amount];
	for (int i = 0; i < amount; i++)
		base[i] = i * step;

	return amount * step;
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

//Returns the middle value if the key was found there, if not recur with the half it could be in. Returns -1 if not found
int BinarySearch(int key, int bottom, int top) {

	//finds the middle
	int middle = std::floor(bottom + (top - bottom) / 2);

	comparisonCount++;
	//if the key was found in the middle
	if (base[middle] == key)
		return middle;

	//if there was no more values left to check
	if (top == bottom)
		return -1;

	//recalls the search
	if (key < base[middle])
		BinarySearch(key, bottom, Constrain(middle - 1, bottom, middle));
	else if (key > base[middle])
		BinarySearch(key, Constrain(middle + 1, middle, top), top);
}

//finds the search space and then calls binary search on it
int ExponentialSearch(int key, int bottom, int top) {

	comparisonCount++;
	//check if key was present at the first address
	if (base[bottom] == key)
		return bottom;

	int i = bottom + 1;
	//keeps looping while there is still array space to check and the current index is smaller than the key
	while (i < top && base[i] <= key) {

		comparisonCount++;

		//doubles the seach space
		i = i * 2;
	}

	//calls binary search on the found search space
	return BinarySearch(key, i / 2, std::min(i, top));
}

//interplation and linear hybrid search
int InterpolationLinearSearch(int key, int bottom, int top) {

	//calls the first interplation search
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	int valueAtEstimate = base[estimatedPosition];


	//the returns the found value or if not calls the linear search on the rest of the array
	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return LinearSearch(key, bottom, Constrain(estimatedPosition - 1, bottom, top));
	else if (key > valueAtEstimate)
		return LinearSearch(key, Constrain(estimatedPosition + 1, bottom, top), top);
}


//interplation and exponential hybrid search
int InterpolationExponentialSearch(int key, int bottom, int top) {

	//calls the first interplation search
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	int valueAtEstimate = base[estimatedPosition];


	//the returns the found value or if not calls the exponential search on the rest of the array
	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return ExponentialSearch(key, bottom, Constrain(estimatedPosition - 1, bottom, top));
	else if (key > valueAtEstimate)
		return ExponentialSearch(key, Constrain(estimatedPosition + 1, bottom, top), top);
}


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


int main() {

	std::string input;

	do {

		//-------------------search prep

		int arrayCount = 10000000;
		int gapMin = 10;
		int gapMax = 0;

		std::cout << "Filling array with random numbers" << std::endl;

		//fills with random values with a given random gap
		int maxVal = FillListRandom(arrayCount, gapMin, gapMax);

		//fills with sequential data with a constant gap
		//int maxVal = FillList(arrayCount, 4);

		std::cout << "Array filled, generating random key to search for" << std::endl;


		//gets a random key (might not exist)
		//int key = rand() % maxVal;

		//gets a key that exists
		int key = base[rand() % arrayCount];

		std::cout << "Key generated " << std::endl;
		std::cout << std::endl << std::endl << std::endl;


		//-------------------searches
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



		//calls the exponential search
		comparisonCount = 0;
		std::cout << "vv Exponential search vv" << std::endl;
		returnedIndex = ExponentialSearch(key, 0, arrayCount - 1);
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

		std::cin >> input;
		system("cls");

		//repeats until x is pressed
	} while (input != "x");
}