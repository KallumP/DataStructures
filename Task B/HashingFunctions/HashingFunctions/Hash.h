#pragma once

class Hash {

public:
	~Hash() {
		delete hashTable;
	}

	virtual int GetAddress(int key) = 0;

	//increments the value in the array where the key would be inserted
	void Insert(int key) {
		hashTable[GetAddress(key)]++;
	}

	//returns how many collisions were found in the array
	int GetCollisions() {

		int collisions = 0;

		//loops through all addresses in the array
		for (int i = 0; i < GetTableSize(); i++)

			//if there was more than one insertion in this address
			if (hashTable[i] > 1)

				//adds on the extra collisions
				collisions += hashTable[i] - 1;

		return collisions;
	}

	//resets the array with 0s
	void ResetTable() {

		delete hashTable;
		hashTable = new int[tableSize];

		std::fill(hashTable, hashTable + tableSize, 0);
	}

	int GetTableSize() {
		return tableSize;
	}

	std::string GetHashType() {
		return hashType;
	}




protected:
	int digits;
	int tableSize;
	int* hashTable;
	std::string hashType;

	void SetupTable() {

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits);

		ResetTable();
	}

	void SetupTable(int _tableSize) {

		tableSize = _tableSize;

		ResetTable();
	}
};


class MidSquare : public Hash {

public:

	//defaults to 3 digits
	MidSquare() {

		hashType = "midsquare";

		//saves the number of digits to use
		digits = 3;

		SetupTable();

		//sets up the number required to resqure the key
		repeatSize = std::pow(10, digits - 1);
	}

	MidSquare(int _digits) {

		hashType = "midsquare";

		//saves the number of digits to use
		digits = _digits;

		SetupTable();

		//sets up the number required to resqure the key
		repeatSize = std::pow(10, digits - 1);
	}

	//takes a key and returns it's hashed address
	int GetAddress(int key) {

		//keeps halving the key if the square cannot be stored in an int
		while (key >= 46340)
			key /= 2;

		//0 and 1 don't work for this
		if (key == 0 || key == 1)
			return key;

		int address;
		int keySquared;
		std::string keySquaredString;

		//squares the input key
		keySquared = key * key;

		//checks if the result had enough digits
		while (keySquared < repeatSize)

			//resquares the key
			keySquared = keySquared * keySquared;

		//turns the result into a string (for easy traversal)
		keySquaredString = std::to_string(keySquared);

		//find the middle position
		int middle = std::ceil(keySquaredString.length() / 2.0) - 1;

		//calculates how many digits to back from the middle
		int halfDigits = std::floor(digits / 2);

		if (digits % 2 == 0)
			halfDigits--;

		//gets the middle digits
		std::string keyString = keySquaredString.substr(middle - halfDigits, digits);

		//turns the middle digits string into an integer
		address = std::stoi(keyString);

		return address;
	}

private:
	int repeatSize;

};

class XOR : public Hash {

public:

	//defaults to 3 digits
	XOR()
	{
		hashType = "XOR";

		//saves the number of digits to use
		digits = 3;


		SetupTable();
	}

	XOR(int _digits)
	{
		hashType = "XOR";

		//saves the number of digits to use
		digits = _digits;


		SetupTable();
	}

	//takes a key and returns it's hashed key
	int GetAddress(int key) {

		//turns the key into a string (makes traversal easier)
		std::string keyString = std::to_string(key);

		std::vector<std::string> keyStringDivided;
		std::string buffer;

		//loops through all the key digits
		for (int i = 0; i < keyString.length(); i++) {

			//puts this digit into the buffer string
			buffer += keyString[i];

			//checks if this digit is the last required one for this division 
			if (i % digits == digits - 1) {

				//adds the buffer into the list of divded strings
				keyStringDivided.push_back(buffer);

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
			keyStringDivided.push_back(buffer);

			//resets the buffer
			buffer = "";
		}

		std::string foldBuffer = "";

		//while there are still two divisions left
		while (keyStringDivided.size() > 1) {

			//loop through all numbers in the division
			for (int i = 0; i < digits; i++) {

				//XORs the current digits
				int a = keyStringDivided[0][i];
				int b = keyStringDivided[1][i];
				int c = a ^ b;

				//checks if the result was two digits
				if (c > 9)
					c -= 10;

				//adds the result to the key string
				foldBuffer += std::to_string(c);
			}

			//removes the first key
			keyStringDivided.erase(keyStringDivided.begin());

			//assigns the new first key to the result (ready to be folded onto the next division
			keyStringDivided[0] = foldBuffer;

			foldBuffer = "";
		}

		//gets the last key left in the list of divisions
		return std::stoi(keyStringDivided[0]);
	}
};

class Division : public Hash {

public:
	//default table size is 1000
	Division()
	{
		hashType = "division";

		SetupTable(1000);
		CalculateMValue();
	}

	Division(int tableSize)
	{
		hashType = "division";


		SetupTable(tableSize);
		CalculateMValue();
	}

	int GetAddress(int key) {

		int remainder = key % M;

		return remainder;
	}

private:
	void CalculateMValue() {

		//loops backwards from the table size
		for (int i = GetTableSize() - 1; i >= 0; i--) {

			if (i % 2 != 0) {

				bool prime = true;

				//loops through 0 to half of the number being checked
				for (int j = 2; j < std::ceil(i / 2.0); j++) {

					int remainder = i % j;
					//if the number is not divisible
					if (remainder == 0) {
						prime = false;
						break;
					}
				}

				if (prime) {

					M = i;
					return;
				}
			}
		}

		//2 is a prime number that could be used if this wasn't available (we never check 2)
		M = 2;
	}

	int M;
};