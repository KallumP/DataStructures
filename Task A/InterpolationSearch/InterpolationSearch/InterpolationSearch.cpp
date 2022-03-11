#include <iostream>
#include <vector>
#include <algorithm>



//std::vector<int> base;
int* base;

//fills the array with a given amount of random numbers (sorted) between min and max
int FillListRandom(int amount, int gapMin, int gapMax) {

	base = new int[amount];
	
	int numberToAdd = 0;

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
void FillList(int amount, int step) {

	base = new int[amount];
	for (int i = 0; i < amount; i++)
		base[i] = i * step;
}

//outputs the full list
void OutputList(std::vector<int> toOutput) {

	for (int i = 0; i < toOutput.size(); i++)
		std::cout << "Index: " << i << " Value: " << toOutput[i] << std::endl;
}

//outputs a slice of the vector
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
int InterpolateSearch(int key, int comparisonCount, int bottom, int top) {

	//outputs the array
	OutputList(bottom, top);
	std::cout << "^^ Array to search ^^ " << std::endl;

	//outputs the array size
	std::cout << "Array size left to check: " <<  1 + top - bottom << std::endl;

	std::cout << "Start: " << bottom << ", End: " << top << std::endl;

	//outputs what key is being searched for
	std::cout << std::endl;
	std::cout << "Searching for index of key: " << key << std::endl;

	//checks if only one value is left to check
	if (bottom == top)

		//return if the last value was the right one
		if (base[bottom] == key)
			return bottom;
		else
			return -1;

	//gets the estimated position (using the interpolation algorithm)
	int estimatedPosition = InterpolateIndex(key, bottom, top);
	std::cout << "Estimated position: " << estimatedPosition << std::endl;

	//gets the value at the estimated position
	int valueAtEstimate = base[estimatedPosition];
	std::cout << "Value at estimated position " << base[estimatedPosition] << std::endl;

	//increases the number of comparisons done
	comparisonCount++;
	std::cout << "Comparisons done: " << comparisonCount << std::endl;


	std::cout << std::endl;
	std::cout << std::endl;


	if (valueAtEstimate == key)
		return estimatedPosition;
	else if (key < valueAtEstimate)
		return InterpolateSearch(key, comparisonCount, bottom, Constrain(estimatedPosition - 1, bottom, top));
	else if (key > valueAtEstimate)
		return InterpolateSearch(key, comparisonCount, Constrain(estimatedPosition + 1, bottom, top), top);
}




int main() {

	std::string input;

	do {

		int arrayCount = 10;
		int gapMin = 10;
		int gapMax= 0;

		int maxVal = FillListRandom(arrayCount, gapMin, gapMax);

		if (InterpolateSearch(rand() % maxVal, 0, 0, arrayCount - 1))
			std::cout << "Found" << std::endl;
		else
			std::cout << "Not found" << std::endl;

		std::cin >> input;
		system("cls");

	} while (input != "x");
}