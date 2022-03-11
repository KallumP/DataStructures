#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "Hash.h"

//Outputs to the console the key for a given key using a given hasher
void OutputAddress(Hash* hasher, int key) {
	std::cout << "Key: " << key << " -> " << hasher->GetAddress(key) << std::endl;
}


void MidSquareTest() {

	Hash* hasher = new MidSquare(3);

	std::cout << "Test keys used in report for the midsquare hash" << std::endl;
	//example keys from the report
	OutputAddress(hasher, 23);
	OutputAddress(hasher, 154);
	OutputAddress(hasher, 8);
	OutputAddress(hasher, 326);
	OutputAddress(hasher, 3);

	std::cout << std::endl << "Test special keys for the midsquare hash" << std::endl;
	//keys that need multiple squares
	for (int i = 0; i < 10; i++)
		OutputAddress(hasher, i);
}

void XORTest() {

	Hash* hasher = new XOR(3);

	std::cout << "Test keys used in report for the XOR hash" << std::endl;
	//example keys from the report
	OutputAddress(hasher, 234561);
	OutputAddress(hasher, 152132344);
	OutputAddress(hasher, 81223);

	std::cout << std::endl << "Test special keys for the XOR hash" << std::endl;
	//tests keys that are smaller than 3 digits
	for (int i = 0; i < 100; i++)
		OutputAddress(hasher, i);
}

void DivisionTest() {

	Hash* hasher = new Division(1000);

	std::cout << "Test keys used in report for the division hash" << std::endl;
	//example keys from the report
	OutputAddress(hasher, 43);
	OutputAddress(hasher, 897);
	OutputAddress(hasher, 27363);
	OutputAddress(hasher, 7894320);

}

//sums the binary value (all 1s) of a given decimal value
int SumBinary(int value) {

	int sum = 0;

	//loops enough for a 32 bit input
	for (int i = 31; i >= 0; i--) {

		//bit shifts the value to the current bit
		int k = value >> i;

		//sums if it was 1
		if (k & 1)
			sum++;
	}

	return sum;
}

void OutputAvalancheScore(Hash* hasher, std::vector<int> addresses) {

	double sumScore = 0;
	int scoreCount = 0;

	//loops through all the addresses
	for (int i = 0; i < addresses.size() - 1; i++) {

		//gets the two values to compare
		int addressA = addresses[i];
		int addressB = addresses[i + 1];

		//XORs the two values (to see what bits changed)
		int XORed = addressA ^ addressB;

		//sums the binary digits from the XOR result
		int XORResultSummed = SumBinary(XORed);

		//gets the number of bits in the hash ouput
		double bitsNoRound = std::log2(hasher->GetTableSize());
		int outputBits = std::ceil(bitsNoRound);

		//gets the avalanche score for these two values
		double score = (double)XORResultSummed / outputBits;

		//sums the score
		sumScore += score;
		scoreCount++;
	}

	//gets the average score for all the values
	double averageScore = (double)sumScore / scoreCount;

	//output
	std::cout << "Total keys inputted: " << addresses.size() << std::endl;
	std::cout << "Average avalanche score between all keys: " << averageScore << std::endl << std::endl;
}

//Outputs the avalanche score using the list of given addresses (average distance between all keys compared to the best possible distances)
void OutputSpreadScore(Hash* hasher, std::vector<int> addresses) {

	//sorts all the returned keys
	std::sort(addresses.begin(), addresses.end());

	int totalDistance = 0;

	//loops through all the keys and sums all the distances between keys
	for (int i = 0; i < addresses.size() - 2; i++)
		totalDistance += addresses[i + 1] - addresses[i];

	//gets the average distance (-1 because the gaps between the keys is one less than count of keys)
	float averageDistance = (float)totalDistance / addresses.size() - 1;
	float bestDistance = (float)hasher->GetTableSize() / addresses.size();
	float avalanchePercent = 100 * (averageDistance / bestDistance);

	std::cout << "Total keys inputted: " << addresses.size() << std::endl;
	std::cout << "Average distance between all keys: " << averageDistance << std::endl;
	std::cout << "Best average distance is " << hasher->GetTableSize() << " / " << addresses.size() << ": " << bestDistance << std::endl;
	std::cout << "Spread score: " << avalanchePercent << "%" << std::endl;
	std::cout << std::endl;
}

//Hashes a specified number of sequential keys (with a specified gap) and outputs the avalanche property using the given hash type
void SequentialHashesAvalanche(Hash* hasher, int keyCount, int keyGap) {

	std::vector<int> addresses;

	//puts sequential keys into the hash function
	for (int i = 0; i < keyCount * keyGap; i += keyGap)
		addresses.push_back(hasher->GetAddress(i));

	std::cout << "Sequential key gap of " << keyGap << " used." << std::endl;
	OutputAvalancheScore(hasher, addresses);
}

//Hashes a specified number of random keys and outputs the avalanche property using the given hash type
void RandomHashesAvalanche(Hash* hasher, int keyCount) {

	std::vector<int> addresses;

	//puts random keys into the hash function
	for (int i = 0; i < keyCount; i++)
		addresses.push_back(hasher->GetAddress(rand() % 1000));

	std::cout << "Random numbers used" << std::endl;
	OutputAvalancheScore(hasher, addresses);
}

//Demos part 1 and 2
void Demo(Hash* hasher) {

	for (int i = 1; i <= 5; i++)
		SequentialHashesAvalanche(hasher, 200, i);

	std::string input = "";
	do {

		RandomHashesAvalanche(hasher, 10000);
		std::cout << "Press enter to repeat random keys or 'x' to stop";
		std::cin >> input;

	} while (input != "x");

	delete hasher;
}

int main()
{

	std::cout << "Testing: midsquare" << std::endl << std::endl;
	Demo(new MidSquare(3));
	system("cls");

	std::cout << "Testing: xor" << std::endl << std::endl;
	Demo(new XOR(3));
	system("cls");

	std::cout << "Testing: division" << std::endl << std::endl;
	Demo(new Division(1000));
	system("cls");

	MidSquareTest();
	std::cout << std::endl << std::endl;

	XORTest();
	std::cout << std::endl << std::endl;

	DivisionTest();
}