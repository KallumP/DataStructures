#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "Hash.h"

//Outputs to the console the key for a given value using a given hasher
void OutputAddress(Hash* hasher, int value) {
	std::cout << "Value: " << value << " -> " << hasher->GetAddress(value) << std::endl;
}


void MidSquareTest(Hash* hasher) {

	std::cout << "Test values used in report for the midsquare hash" << std::endl;
	//example values from the report
	OutputAddress(hasher, 23);
	OutputAddress(hasher, 154);
	OutputAddress(hasher, 8);
	OutputAddress(hasher, 326);
	OutputAddress(hasher, 3);

	std::cout << std::endl << "Test special values for the midsquare hash" << std::endl;
	//special cases
	OutputAddress(hasher, 0);
	OutputAddress(hasher, 0);

	//values that need multiple squares
	for (int i = 0; i < 10; i++)
		OutputAddress(hasher, i);
}
void XORTest(Hash* hasher) {

	std::cout << "Test values used in report for the XOR hash" << std::endl;

	//example values from the report
	OutputAddress(hasher, 234561);
	OutputAddress(hasher, 152132344);
	OutputAddress(hasher, 81223);

	std::cout << std::endl << "Test special values for the XOR hash" << std::endl;
	//tests values that are smaller than 3 digits
	for (int i = 0; i < 100; i++)
		OutputAddress(hasher, i);
}

//Outputs the avalanche score using the list of given keys (average distance between all keys compared to the best possible distances)
void OutputAvalanchScore(Hash* hasher, std::vector<int> keys) {

	//sorts all the returned keys
	std::sort(keys.begin(), keys.end());

	int totalDistance = 0;

	//loops through all the keys and sums all the distances between keys
	for (int i = 0; i < keys.size() - 2; i++)
		totalDistance += keys[i + 1] - keys[i];

	//gets the average distance (-1 because the gaps between the keys is one less than count of keys)
	float averageDistance = (float)totalDistance / keys.size() - 1;
	float bestDistance = (float)hasher->GetTableSize() / keys.size();
	float avalanchePercent = 100 * (averageDistance / bestDistance);

	std::cout << "Total values inputted: " << keys.size() << std::endl;
	std::cout << "Average distance between all keys: " << averageDistance << std::endl;
	std::cout << "Best average distance is " << hasher->GetTableSize() << " / " << keys.size() << ": " << bestDistance << std::endl;
	std::cout << "Avalanche score: " << avalanchePercent << "%" << std::endl;
	std::cout << std::endl;
}

//Hashes a specified number of sequential values (with a specified gap) and outputs the avalanche property using the given hash type
void SequentialHashesAvalanche(Hash* hasher, int valueCount, int valueGap) {

	std::vector<int> keys;

	//puts sequential values into the hash function
	for (int i = 0; i < valueCount * valueGap; i += valueGap)
		keys.push_back(hasher->GetAddress(i));

	std::cout << "Sequential value gap of " << valueGap << " used." << std::endl;
	OutputAvalanchScore(hasher, keys);
}

//Hashes a specified number of random values and outputs the avalanche property using the given hash type
void RandomHashesAvalanche(Hash* hasher, int valueCount) {

	std::vector<int> keys;

	//puts random values into the hash function
	for (int i = 0; i < valueCount; i++)
		keys.push_back(hasher->GetAddress(rand() % 1000));

	std::cout << "Random numbers used" << std::endl;
	OutputAvalanchScore(hasher, keys);
}

//Demos part 1 and 2
void Demo(Hash* hasher) {

	for (int i = 1; i <= 5; i++)
		SequentialHashesAvalanche(hasher, 200, i);

	std::string input = "";
	do {

		RandomHashesAvalanche(hasher, 200);
		std::cout << "Press enter to repeat random values or 'x' to stop";
		std::cin >> input;

	} while (input != "x");

	delete hasher;
}

int main()
{

	std::cout << "Testing: midsquare" << std::endl;
	Demo(new MidSquare(3));
	system("cls");

	std::cout << "Testing: xor" << std::endl;
	Demo(new XOR(3));
	system("cls");


	//tests the values used in the report
	MidSquareTest(new MidSquare(3));

	std::cout << std::endl << std::endl;

	XORTest(new XOR(3));
}