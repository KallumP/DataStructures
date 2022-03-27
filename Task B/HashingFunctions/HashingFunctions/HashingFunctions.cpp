#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "Hash.h"
#include "Grapher.h"



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
	//tests some keys that are smaller than 3 digits
	for (int i = 0; i < 20; i++)
		OutputAddress(hasher, i);
	
	for (int i = 80; i < 100; i++)
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


	std::cout << "Part 1 done" << std::endl;
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
		std::cout << "Enter any value to repeat random keys or 'x' to continue" << std::endl;
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

	std::cout << "Part 2 done" << std::endl;
	std::string input = "";
	std::cout << "Enter any value to continue" << std::endl;
	std::cin >> input;
	system("cls");
	delete hasher;
}




//tests the collision property of the given hash
void CollisionTest(Hash* hasher, Grapher* g, int keyCount, int digits) {

	hasher->ResetTable();


	//inserts a keyCount anount of values with a gap of 1
	for (int j = 0; j < keyCount * 1; j += 1)
		hasher->Insert(j);
	g->TakeSizeValues(digits, 100 * ((double)hasher->GetCollisions() / keyCount), -1, -1, -1, -1);
	hasher->ResetTable();


	//inserts a keyCount anount of values with a gap of 2
	for (int j = 0; j < keyCount * 2; j += 2)
		hasher->Insert(j);
	g->TakeSizeValues(digits, -1, 100 * ((double)hasher->GetCollisions() / keyCount), -1, -1, -1);
	hasher->ResetTable();


	//inserts a keyCount anount of values with a gap of 3
	for (int j = 0; j < keyCount * 3; j += 3)
		hasher->Insert(j);
	g->TakeSizeValues(digits, -1, -1, 100 * ((double)hasher->GetCollisions() / keyCount), -1, -1);
	hasher->ResetTable();


	//inserts a keyCount anount of values with a gap of 4
	for (int j = 0; j < keyCount * 4; j += 4)
		hasher->Insert(j);
	g->TakeSizeValues(digits, -1, -1, -1, 100 * ((double)hasher->GetCollisions() / keyCount), -1);
	hasher->ResetTable();


	//inserts a keyCount anount of values with a gap of 5
	for (int j = 0; j < keyCount * 5; j += 5)
		hasher->Insert(j);
	g->TakeSizeValues(digits, -1, -1, -1, -1, 100 * ((double)hasher->GetCollisions() / keyCount));
	hasher->ResetTable();

}


//graphs the collision properties of the midsquare hash
void GraphMidsquare(Grapher* g, int minDigits, int maxDigits) {

	Hash* hasher;
	float tablePortion;

	g = new Grapher();
	tablePortion = 0.1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new MidSquare(i);
		std::cout << "Graphing midsquare: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Midsquare collision rates (inserting tenth table)", L"Table sizes (digits)", "_midsquare collisions (tenth).png");
	g = new Grapher();

	tablePortion = 0.5;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new MidSquare(i);
		std::cout << "Graphing midsquare: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Midsquare collision rates (inserting half table)", L"Table sizes (digits)", "_midsquare collisions (half).png");
	g = new Grapher();

	tablePortion = 1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new MidSquare(i);
		std::cout << "Graphing midsquare: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Midsquare collision rates (inseting full table)", L"Table sizes (digits)", "_midsquare collisions (full).png");
	g = new Grapher();
}

//graphs the collision properties of the XOR hash
void GraphXOR(Grapher* g, int minDigits, int maxDigits) {

	Hash* hasher;
	float tablePortion;

	g = new Grapher();
	tablePortion = 0.1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new XOR(i);
		std::cout << "Graphing xor: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"XOR collision rates (inserting tenth table)", L"Table sizes (digits)", "_xor collisions (tenth).png");
	g = new Grapher();

	tablePortion = 0.5;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new XOR(i);
		std::cout << "Graphing xor: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"XOR collision rates (inserting half table)", L"Table sizes (digits)", "_xor collisions (half).png");
	g = new Grapher();

	tablePortion = 1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new XOR(i);
		std::cout << "Graphing xor: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"XOR collision rates (inseting full table)", L"Table sizes (digits)", "_xor collisions (full).png");
	g = new Grapher();
}

//graphs the collision properties of the division hash
void GraphDivision(Grapher* g, int minDigits, int maxDigits) {

	Hash* hasher;
	float tablePortion;

	g = new Grapher();
	tablePortion = 0.1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new Division(std::pow(10, i));
		std::cout << "Graphing division: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Division collision rates (inserting tenth table)", L"Table sizes (digits)", "_division collisions (tenth).png");
	g = new Grapher();

	tablePortion = 0.5;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new Division(std::pow(10, i));
		std::cout << "Graphing division: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Division collision rates (inserting half table)", L"Table sizes (digits)", "_division collisions (half).png");
	g = new Grapher();

	tablePortion = 1;
	//loops through 4 table digit sizes to test
	for (int i = minDigits; i < maxDigits; i++) {

		hasher = new Division(std::pow(10, i));
		std::cout << "Graphing division: " << hasher->GetHashType() << " collisions on table size : " << hasher->GetTableSize() << " and " << tablePortion << " table size key count" << std::endl;
		CollisionTest(hasher, g, hasher->GetTableSize() * tablePortion, i);
	}
	g->DrawGraphSequentials(L"Division collision rates (inseting full table)", L"Table sizes (digits)", "_division collisions (full).png");

	g = new Grapher();
}


void GraphRandoms(Grapher* g) {

	std::cout << "Graphing collisions" << std::endl;

	Hash* hasher;
	srand(time(0));

	int digits = 3;
	int fractionOfTable = 10;

	g = new Grapher();
	hasher = new MidSquare(digits);
	int valueToPut = 0;

	//loops for half the number of spaces in the hashtable
	for (int i = 0; i < hasher->GetTableSize() / fractionOfTable; i++)
		hasher->Insert(valueToPut += rand() % hasher->GetTableSize());

	int* hashTable = hasher->GetTable();
	for (int i = 0; i < hasher->GetTableSize(); i++)
		g->TakeRandom(i, hashTable[i], -1, -1);

	g->DrawGraphRandom(L"Hashtable collision status for midsquare (3 digits, tenth input size)", L"Hashtable index", "_Collision count midsquare.png");
	hasher->ResetTable();



	g = new Grapher();
	hasher = new XOR(digits);
	valueToPut = 0;

	//loops for half the number of spaces in the hashtable
	for (int i = 0; i < hasher->GetTableSize() / fractionOfTable; i++)
		hasher->Insert(valueToPut += rand() % hasher->GetTableSize());

	hashTable = hasher->GetTable();
	for (int i = 0; i < hasher->GetTableSize(); i++)
		g->TakeRandom(i, hashTable[i], -1, -1);

	g->DrawGraphRandom(L"Hashtable collision status for XOR (3 digits, tenth input size)", L"Hashtable index", "_Collision count xor.png");
	hasher->ResetTable();



	g = new Grapher();
	hasher = new Division(digits);
	valueToPut = 0;

	//loops for half the number of spaces in the hashtable
	for (int i = 0; i < hasher->GetTableSize() / fractionOfTable; i++)
		hasher->Insert(valueToPut += rand() % hasher->GetTableSize());

	hashTable = hasher->GetTable();
	for (int i = 0; i < hasher->GetTableSize(); i++)
		g->TakeRandom(i, hashTable[i], -1, -1);

	g->DrawGraphRandom(L"Hashtable collision status for division (3 digits, tenth input size)", L"Hashtable index", "_Collision count division.png");
	hasher->ResetTable();
}


//Demos part 3
void DemoPart3(int minDigits, int maxDigits) {

	Grapher* g = new Grapher();

	//GraphMidsquare( g, minDigits, maxDigits);

	//GraphXOR( g, minDigits, maxDigits);

	//GraphDivision( g, minDigits, maxDigits);

	GraphRandoms(g);

	delete g;

	std::cout << "Part 3 done" << std::endl;
	std::string input = "";
	std::cout << "Enter any value to continue" << std::endl;
	std::cin >> input;
	system("cls");
}




int main()
{
	//DemoPart1();

	//DemoPart2();

	DemoPart3(3, 6);
}