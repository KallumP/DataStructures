#include <iostream>
#include <vector>
#include <algorithm>



std::vector<int> base;

void FillListRandom(int amount) {

	for (int i = 0; i < amount; i++)
		base.push_back(rand() % 1000);
}

void FillList(int amount) {
	for (int i = 0; i < amount; i++)
		base.push_back(i);
}

void SortList() {

	std::sort(base.begin(), base.end());
}

void OutputList(std::vector<int> toOutput) {

	for (int i = 0; i < toOutput.size(); i++)
		std::cout << "Index: " << i << " Value: " << toOutput[i] << std::endl;
}

float InterpolateIndex(int key) {

	float k = key;
	float top = base.size() - 1;
	float bottom = 0;
	float VTop = base[top];
	float VBottom = base[bottom];

	 return std::floor((top - bottom) * ((k - VBottom) / (VTop - VBottom)) + bottom);
}

bool InterpolateSearch(int key, std::vector<int> V) {

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "Array to search: " << std::endl;
	OutputList(V);

	std::cout << std::endl;
	std::cout << "Searching for index of key: " << key << std::endl;

	int estimatedPosition = InterpolateIndex(key);
	int valueAtEstimate = V[estimatedPosition];
	std::cout << "Estimated position: " << estimatedPosition << std::endl;
	std::cout << "Value at estimated position " << V[estimatedPosition] << std::endl;

	return key == valueAtEstimate;
}

int main()
{

	FillListRandom(200);
	SortList();
	OutputList(base);
	std::cout << "^^^ Array filled out" << std::endl;


	InterpolateSearch(40, base);

}