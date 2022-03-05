#include <iostream>

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>


class XORHash {


public:

	//defaults to 3 digits
	XORHash()
	{
		//saves the number of digits to use
		digits = 3;

		//sets up how big to make the hashmap
		tableSize = std::pow(10, digits) - 1;

		//instantiates up the hashmap
		hashTable = new int[tableSize];
	}

	XORHash(int _digits)
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

	void Insert(int value, int key) {

	}

	int digits;
	int tableSize;
	int repeatSize;
	int* hashTable;
};



//sets up the hash object to hash to 3 digits
XORHash a;

//a list of all the keys
std::vector<int> keys;



//test values used to test against my example values in the report
void TestExample() {
	int value;

	//tests values used in report example
	value = 234561;
	std::cout << "Value: " << value << " -> " << a.GetKey(value) << std::endl;

	value = 152132344;
	std::cout << "Value: " << value << " -> " << a.GetKey(value) << std::endl;

	value = 81223;
	std::cout << "Value: " << value << " -> " << a.GetKey(value) << std::endl;


	//tests values that are smaller than 3 digits
	for (int i = 0; i < 100; i++) {
		value = i;
		std::cout << "Value: " << value << " -> " << a.GetKey(value) << std::endl;
	}

}

int main()
{

	a = XORHash(3);


	bool test = true;
	if (test) {

		TestExample();
	}
	else {

		int valuesToInput = 200;
		keys.clear();

		//puts sequential values into the hash function
		for (int i = 0; i < valuesToInput; i++)
			keys.push_back(a.GetKey(i));

		//sorts all the returned keys
		std::sort(keys.begin(), keys.end());

		int totalDistance = 0;

		//loops through all the keys and sums all the distances between keys
		for (int i = 0; i < keys.size() - 2; i++)
			totalDistance += keys[i + 1] - keys[i];

		//gets the average distance (-1 because the gaps between the keys is one less than count of keys)
		float averageDistance = (float)totalDistance / keys.size() - 1;

		std::cout << "Total sequential values inputted: " << valuesToInput << std::endl;
		std::cout << "Average distance between all keys: " << averageDistance << std::endl;
		std::cout << "Best average distance is " << a.tableSize << " / " << valuesToInput << " = " << (float)a.tableSize / valuesToInput << std::endl;
		std::cout << std::endl << std::endl << std::endl;

		std::string input;
		do {
			int valuesToInput = 200;
			keys.clear();

			//puts random values into the hash function
			for (int i = 0; i < valuesToInput; i++)
				keys.push_back(a.GetKey(rand() % 1000));

			//sorts all the returned keys
			std::sort(keys.begin(), keys.end());

			int totalDistance = 0;

			//loops through all the keys and sums all the distances between keys
			for (int i = 0; i < keys.size() - 2; i++)
				totalDistance += keys[i + 1] - keys[i];

			//gets the average distance (-1 because the gaps between the keys is one less than count of keys)
			float averageDistance = (float)totalDistance / keys.size() - 1;

			std::cout << "Total values inputted: " << valuesToInput << std::endl;
			std::cout << "Average distance between all keys: " << averageDistance << std::endl;
			std::cout << "Best average distance is " << a.tableSize << " / " << valuesToInput << " = " << (float)a.tableSize / valuesToInput << std::endl;

			std::cin >> input;

		} while (input != "x");
	}
}