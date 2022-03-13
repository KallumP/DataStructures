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

//tests values used in the report
void DemoPart1() {

	MidSquareTest();
	std::cout << std::endl << std::endl;

	XORTest();
	std::cout << std::endl << std::endl;

	DivisionTest();

	std::string input = "";
	std::cout << "Enter any value to continue" << std::endl;
	std::cin >> input;
	system("cls");
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

	srand(time(0));

	//puts random keys into the hash function
	for (int i = 0; i < keyCount; i++)
		addresses.push_back(hasher->GetAddress(rand() % (hasher->GetTableSize() * 10)));

	std::cout << "Random numbers used" << std::endl;
	OutputAvalancheScore(hasher, addresses);
}

//tests the avalanche property of the given hasher
void AvalanchePropertyTests(Hash* hasher) {

	for (int i = 1; i <= 5; i++)
		SequentialHashesAvalanche(hasher, 200, i);

	std::string input = "";
	do {

		RandomHashesAvalanche(hasher, 100000);

		//repeat
		std::cout << "Enter any value to repeat random keys or 'x' to stop" << std::endl;
		std::cin >> input;
		system("cls");
	} while (input != "x");
}

//demos part 2
void DemoPart2() {

	Hash* hasher;

	hasher = new MidSquare(3);
	std::cout << "Testing: midsquare avalanche" << std::endl << std::endl;
	AvalanchePropertyTests(hasher);
	system("cls");

	hasher = new XOR(3);
	std::cout << "Testing: xor avalanche" << std::endl << std::endl;
	AvalanchePropertyTests(hasher);
	system("cls");

	hasher = new Division(1000);
	std::cout << "Testing: division avalanche" << std::endl << std::endl;
	AvalanchePropertyTests(hasher);
	system("cls");

	delete hasher;
}




//tests the collisoin property of the given hash
void CollisisionTest(Hash* hasher, int keyCount) {

	//does 5 sequential inserts
	for (int i = 1; i <= 5; i++) {

		//resets the table for this set of keys
		hasher->ResetTable();

		//inserts a keyCount anount of values with a gap of i
		for (int j = 0; j < keyCount * i; j += i) {

			//std::cout << "Inserting key: " << j << " into hashtable" << std::endl;

			//insets this key
			hasher->Insert(j);
		}

		//outputs the number of collisions for this set
		int collisions = hasher->GetCollisions();
		std::cout << keyCount << " sequential keys inputted with a gap of " << i;
		std::cout << ". Number of collisions found: " << collisions;
		std::cout << ". Collision rate = " << 100 * ((double)collisions / keyCount) << "%" << std::endl;
	}



	//resets the table for this set of keys
	hasher->ResetTable();

	srand(time(0));

	//puts random keys into the hash function
	for (int i = 0; i < keyCount; i++)

		//inserts this random key
		hasher->Insert(rand() % (hasher->GetTableSize() * 10));

	//outputs the number of collisions for this set
	int collisions = hasher->GetCollisions();
	std::cout << keyCount << " random keys inputted";
	std::cout << ". Number of collisions found: " << collisions;
	std::cout << ". Collision rate = " << 100 * ((double)collisions / keyCount) << "%" << std::endl << std::endl;

}

//demos part 3 for a specific hash
void DemoPart3(Hash* hasher) {

	float tablePortion;

	tablePortion = 0.1;
	//tests inputting half the table size capacity of keys
	std::cout << "Testing: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
	CollisisionTest(hasher, hasher->GetTableSize() * tablePortion);


	tablePortion = 0.5;
	//tests inputting half the table size capacity of keys
	std::cout << "Testing: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
	CollisisionTest(hasher, hasher->GetTableSize() * tablePortion);

	tablePortion = 1.0;
	//tests inputting half the table size capacity of keys
	std::cout << "Testing: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
	CollisisionTest(hasher, hasher->GetTableSize() * tablePortion);
}

//Demos part 3
void DemoPart3() {

	Hash* hasher;
	std::string input = "";


	//loops through 4 table digit sizes to test
	for (int i = 3; i < 6; i++) {

		hasher = new MidSquare(i);

		DemoPart3(hasher);
	}


	//pause
	std::cout << "Enter any value to continue" << std::endl;
	std::cin >> input;
	system("cls");

	//loops through 4 digits of sizes to test
	for (int i = 3; i < 6; i++) {

		hasher = new XOR(i);

		DemoPart3(hasher);
	}

	//pause
	std::cout << "Enter any value to continue" << std::endl;
	std::cin >> input;
	system("cls");

	//loops through power of 10 sizes
	for (int i = 3; i < 6; i++) {

		hasher = new Division(std::pow(10, i));

		DemoPart3(hasher);
	}

	delete hasher;
}




int main()
{
	//DemoPart1();

	//DemoPart2();

	DemoPart3();
}