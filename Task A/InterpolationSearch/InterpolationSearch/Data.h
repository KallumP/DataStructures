#pragma once
#include <vector>;
#include "pbPlots.hpp"
#include "supportLib.hpp"


class Data {

public:



	Data() {


		bool success;
		StringReference* errorMessage = new StringReference();
		RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

		x = { -2, -1, 0, 1, 2 };
		y = { 2, -1, -2, -1, 2 };

		success = DrawScatterPlot(imageReference, 600, 400, &x, &y, errorMessage);

		if (success) {
			std::vector<double>* pngdata = ConvertToPNG(imageReference->image);
			WriteToFile(pngdata, "example1.png");
			DeleteImage(imageReference->image);
		} else {
			std::cerr << "Error: ";
			for (wchar_t c : *errorMessage->string) {
				std::wcerr << c;
			}
			std::cerr << std::endl;
		}

		//return success ? 0 : 1;
	}


private:
	std::vector<double> x;
	std::vector<double> y;

};