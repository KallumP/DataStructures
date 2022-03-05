#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

class Hash {

public:
	virtual int GetKey(int value) = 0;

	virtual void InsertValue(int value) { hashTable[GetKey(value)]++; }

	int GetTableSize() { return tableSize; }

protected:
	int digits;
	int tableSize;
	int* hashTable;
};


class MidSquare : public Hash {


public:

	//defaults to 3 digits
	MidSquare()
	{
		//saves the number of digits to use
		digits = 3;

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits) - 1;

		//sets up the number required to resqure the value
		repeatSize = std::pow(10, digits - 1);

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	MidSquare(int _digits)
	{
		//saves the number of digits to use
		digits = _digits;

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits);

		//sets up the number required to resqure the value
		repeatSize = std::pow(10, digits - 1);

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	//takes a value and returns it's hashed key
	int GetKey(int value) {

		//0 and 1 don't work for this
		if (value == 0 || value == 1)
			return value;

		int key;
		int valueSquared;
		std::string valueSquaredString;

		//squares the input value
		valueSquared = value * value;

		//checks if the result had enough digits
		if (valueSquared < repeatSize)

			//repeats the algorithm recursively
			valueSquared = GetKey(valueSquared);

		//turns the result into a string (for easy traversal)
		valueSquaredString = std::to_string(valueSquared);

		//find the middle position
		int middle = std::ceil(valueSquaredString.length() / 2.0) - 1;

		//calculates how many digits to back from the middle
		int halfDigits = std::floor(digits / 2);

		//gets the middle values
		std::string keyString = valueSquaredString.substr(middle - halfDigits, digits);

		//turns the middle value string into an integer
		key = std::stoi(keyString);

		return key;
	}

private:
	int repeatSize;

};

class XOR : public Hash {


public:

	//defaults to 3 digits
	XOR()
	{
		//saves the number of digits to use
		digits = 3;

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits) - 1;

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	XOR(int _digits)
	{
		//saves the number of digits to use
		digits = _digits;

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits);

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	//takes a value and returns it's hashed key
	int GetKey(int value) {

		int key;

		//turns the value into a string (makes traversal easier)
		std::string valueString = std::to_string(value);

		std::vector<std::string> valueStringDivided;
		std::string buffer;

		//loops through all the value digits
		for (int i = 0; i < valueString.length(); i++)
		{

			//puts this digit into the buffer string
			buffer += valueString[i];

			//checks if this digit is the last required one for this division 
			if (i % digits == digits - 1) {

				//adds the buffer into the list of divded strings
				valueStringDivided.push_back(buffer);

				//resets the buffer
				buffer = "";
			}
		}

		//checks if the buffer was not empty (partial division)
		if (buffer != "") {

			//pads out the last division with 0s
			for (int i = buffer.length(); i < digits; i++)
				buffer += "0";

			//adds the buffer into the list of divded strings
			valueStringDivided.push_back(buffer);

			//resets the buffer
			buffer = "";
		}

		std::string foldBuffer = "";

		//while there are still two divisions left
		while (valueStringDivided.size() > 1) {

			//loop through all numbers in the division
			for (int i = 0; i < digits; i++) {

				//XORs the current digits
				int a = valueStringDivided[0][i];
				int b = valueStringDivided[1][i];
				int c = a ^ b;

				//checks if the result was two digits
				if (c > 9)
					c -= 10;

				//adds the result to the key string
				foldBuffer += std::to_string(c);
			}

			//removes the first value
			valueStringDivided.erase(valueStringDivided.begin());

			//assigns the new first value to the result (ready to be folded onto the next division
			valueStringDivided[0] = foldBuffer;

			foldBuffer = "";
		}

		//gets the last value left in the list of divisions
		key = std::stoi(valueStringDivided[0]);
		return key;
	}
};

//Outputs to the console the key for a given value using a given hasher
void OutputKey(Hash* hasher, int value) {
	std::cout << "Value: " << value << " -> " << hasher->GetKey(value) << std::endl;
}


void MidSquareTest(Hash* hasher) {

	std::cout << "Test values used in report for the midsquare hash" << std::endl;
	//example values from the report
	OutputKey(hasher, 23);
	OutputKey(hasher, 154);
	OutputKey(hasher, 8);
	OutputKey(hasher, 326);
	OutputKey(hasher, 3);

	std::cout << std::endl << "Test special values for the midsquare hash" << std::endl;
	//special cases
	OutputKey(hasher, 0);
	OutputKey(hasher, 0);

	//values that need multiple squares
	for (int i = 0; i < 10; i++)
		OutputKey(hasher, i);
}
void XORTest(Hash* hasher) {

	std::cout << "Test values used in report for the XOR hash" << std::endl;

	//example values from the report
	OutputKey(hasher, 234561);
	OutputKey(hasher, 152132344);
	OutputKey(hasher, 81223);

	std::cout << std::endl << "Test special values for the XOR hash" << std::endl;
	//tests values that are smaller than 3 digits
	for (int i = 0; i < 100; i++)
		OutputKey(hasher, i);
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
	for (int i = 0; i < valueCount; i += valueGap)
		keys.push_back(hasher->GetKey(i));

	OutputAvalanchScore(hasher, keys);
}

//Hashes a specified number of random values and outputs the avalanche property using the given hash type
void RandomHashesAvalanche(Hash* hasher, int valueCount) {

	std::vector<int> keys;

	//puts random values into the hash function
	for (int i = 0; i < valueCount; i++)
		keys.push_back(hasher->GetKey(rand() % 1000));

	OutputAvalanchScore(hasher, keys);
}

//Demos part 1 and 2
void Demo(Hash* hasher) {

	for (int i = 1; i <= 5; i++)
		SequentialHashesAvalanche(hasher, 200, i);

	std::string input = "";
	do {

		RandomHashesAvalanche(hasher, 200);
		std::cout << "Press enter to repeat or x to stop";
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
	
	std::cout << std::endl<< std::endl;

	XORTest(new XOR(3));
}