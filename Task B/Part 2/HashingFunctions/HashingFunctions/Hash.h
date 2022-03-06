#pragma once

class Hash {

public:
	virtual int GetAddress(int value) = 0;

	virtual void Insert(int value) { hashTable[GetAddress(value)]++; }

	int GetTableSize() { return tableSize; }

protected:
	int digits;
	int tableSize;
	int* hashTable;

	void SetupTableSize() {

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits);

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	void SetupTableSize(int _tableSize) {

		tableSize = _tableSize;

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}
};


class MidSquare : public Hash {


public:

	//defaults to 3 digits
	MidSquare()
	{
		//saves the number of digits to use
		digits = 3;

		SetupTableSize();

		//sets up the number required to resqure the value
		repeatSize = std::pow(10, digits - 1);
	}

	MidSquare(int _digits)
	{
		//saves the number of digits to use
		digits = _digits;

		SetupTableSize();

		//sets up the number required to resqure the value
		repeatSize = std::pow(10, digits - 1);
	}

	//takes a value and returns it's hashed key
	int GetAddress(int value) {

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
			valueSquared = GetAddress(valueSquared);

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

		SetupTableSize();
	}

	XOR(int _digits)
	{
		//saves the number of digits to use
		digits = _digits;

		SetupTableSize();
	}

	//takes a value and returns it's hashed key
	int GetAddress(int value) {

		//turns the value into a string (makes traversal easier)
		std::string valueString = std::to_string(value);

		std::vector<std::string> valueStringDivided;
		std::string buffer;

		//loops through all the value digits
		for (int i = 0; i < valueString.length(); i++) {

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
		return std::stoi(valueStringDivided[0]);
	}
};

class Division : public Hash {

public:
	//default table size is 1000
	Division()
	{

		SetupTableSize(1000);
		CalculateMValue();
	}

	Division(int tableSize)
	{

		SetupTableSize(tableSize);
		CalculateMValue();
	}

	int GetAddress(int value) {

		int remainder = value % M;

		return remainder;
	}

private:
	void CalculateMValue() {

		//loops backwards from the table size
		for (int i = GetTableSize() -1; i >= 0; i--) {

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
					return ;
				}
			}
		}

		//2 is a prime number that could be used if this wasn't available (we never check 2)
		M = 2;
	}

	int M;
};