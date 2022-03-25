#pragma once
#include <vector>;
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>


#include "pbPlots.hpp"
#include "supportLib.hpp"

//each data object is for one table size and holds the Xs for each search
//on getter for a search it returns the average of all those Xs
//the grapher will draw each x for each table size 

class Data {

public:
	Data() {

	}


	std::vector<std::vector<double>> GetGap1() {
		return gap1Data;
	}
	std::vector<std::vector<double>> GetGap2() {
		return gap2Data;
	}
	std::vector<std::vector<double>> GetGap3() {
		return gap3Data;
	}
	std::vector<std::vector<double>> GetGap4() {
		return gap4Data;
	}
	std::vector<std::vector<double>> GetGap5() {
		return gap5Data;
	}
	std::vector<std::vector<double>> GetRandom1() {
		return random1Data;
	}

	void UpdateGap1(double x, int y) {
		UpdateAverage(&gap1Data, x, y);
	}
	void UpdateGap2(double x, int y) {
		UpdateAverage(&gap2Data, x, y);
	}
	void UpdateGap3(double x, int y) {
		UpdateAverage(&gap3Data, x, y);
	}
	void UpdateGap4(double x, int y) {
		UpdateAverage(&gap4Data, x, y);
	}
	void UpdateGap5(double x, int y) {
		UpdateAverage(&gap5Data, x, y);
	}
	void UpdateRandom1(double x, int y) {
		UpdateAverage(&random1Data, x, y);
	}

	//returns the average of the 
	void UpdateAverage(std::vector<std::vector<double>>* data, double newX, double newY) {

		bool found = false;

		//loops through all table size data
		for (int i = 0; i < data->size(); i++) {

			//if the current data was for the right table size
			if ((*data)[i][0] == newX) {

				//timeses the average Y with the count of Y
				double totalY = (*data)[i][1] * (*data)[i][2];

				//adds the new x onto the total x
				totalY += newY;

				//increments the number of values used for the average
				(*data)[i][2]++;

				//divides the total by the number of values
				double average = totalY / (*data)[i][2];

				(*data)[i][1] = average;

				found = true;
			}
		}

		//if there wasn't an entry for this table size
		if (!found) {

			std::vector<double> toAdd = { newX, newY, 1 };
			(*data).push_back(toAdd);
		}
	}

private:

	//outer holds data for each different table size

	//inner vector = average comparisons, table size (unique), value count
	std::vector<std::vector<double>>  gap1Data;
	std::vector<std::vector<double>>  gap2Data;
	std::vector<std::vector<double>>  gap3Data;
	std::vector<std::vector<double>>  gap4Data;
	std::vector<std::vector<double>>  gap5Data;
	std::vector<std::vector<double>>  random1Data;

};

struct Points {
	std::vector<double> toDrawX;
	std::vector<double> toDrawY;
};

class Grapher {

public:

	Grapher() {

		graphWidth = 1500;
		graphHeight = 1000;

		gap1 = CreateRGBColor(1, 0, 0);
		gap2 = CreateRGBColor(0, 1, 0);
		gap3 = CreateRGBColor(0, 0, 1);
		gap4 = CreateRGBColor(0, 1, 1);
		gap5 = CreateRGBColor(1, 0, 1);


		rand1 = CreateRGBColor(1, 0, 0);
		rand2 = CreateRGBColor(0, 1, 0);
		rand3 = CreateRGBColor(0, 0, 1);

		legendX = 250;
		legendY = -10;
	}

	void DrawGraphSequentials(const wchar_t* graphTitle, const wchar_t* xAxisTitle, std::string imageName) {

		bool success;
		StringReference* errorMessage = new StringReference();
		RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

		//sets up the graph
		ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();
		settings->width = graphWidth;
		settings->height = graphHeight;
		settings->autoBoundaries = true;
		settings->autoPadding = true;
		settings->title = toVector(graphTitle);;
		settings->xLabel = toVector(xAxisTitle);
		settings->yLabel = toVector(L"Collision rate (%)");

		//sets up the series for gap 1
		ScatterPlotSeries* gap1Series = GetDefaultScatterPlotSeriesSettings();
		Points pGap1 = PrepareData(data.GetGap1());
		gap1Series->xs = &pGap1.toDrawX;
		gap1Series->ys = &pGap1.toDrawY;
		gap1Series->linearInterpolation = true;
		gap1Series->lineType = toVector(L"solid");
		gap1Series->color = gap1;
		settings->scatterPlotSeries->push_back(gap1Series);

		//sets up the series for gap 2
		ScatterPlotSeries* gap2Series = GetDefaultScatterPlotSeriesSettings();
		Points pGap2 = PrepareData(data.GetGap2());
		gap2Series->xs = &pGap2.toDrawX;
		gap2Series->ys = &pGap2.toDrawY;
		gap2Series->linearInterpolation = true;
		gap2Series->lineType = toVector(L"solid");
		gap2Series->color = gap2;
		settings->scatterPlotSeries->push_back(gap2Series);

		//sets up the data for gap 3
		ScatterPlotSeries* gap3Series = GetDefaultScatterPlotSeriesSettings();
		Points pGap3 = PrepareData(data.GetGap3());
		gap3Series->xs = &pGap3.toDrawX;
		gap3Series->ys = &pGap3.toDrawY;
		gap3Series->linearInterpolation = true;
		gap3Series->lineType = toVector(L"solid");
		gap3Series->color = gap3;
		settings->scatterPlotSeries->push_back(gap3Series);

		//sets up the series for gap 4
		ScatterPlotSeries* gap4Series = GetDefaultScatterPlotSeriesSettings();
		Points pGap4 = PrepareData(data.GetGap4());
		gap4Series->xs = &pGap4.toDrawX;
		gap4Series->ys = &pGap4.toDrawY;
		gap4Series->linearInterpolation = true;
		gap4Series->lineType = toVector(L"solid");
		gap4Series->color = gap4;
		settings->scatterPlotSeries->push_back(gap4Series);

		//sets up the series for gap 5
		ScatterPlotSeries* gap5Series = GetDefaultScatterPlotSeriesSettings();
		Points pGap5 = PrepareData(data.GetGap5());
		gap5Series->xs = &pGap5.toDrawX;
		gap5Series->ys = &pGap5.toDrawY;
		gap5Series->linearInterpolation = true;
		gap5Series->lineType = toVector(L"solid");
		gap5Series->color = gap5;
		settings->scatterPlotSeries->push_back(gap5Series);


		//draws all the graph series
		success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

		legendY = -10;

		//draws the legend
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Sequential w/ gap of 1"), gap1);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Sequential w/ gap of 2"), gap2);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Sequential w/ gap of 3"), gap3);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Sequential w/ gap of 4"), gap4);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Sequential w/ gap of 5"), gap5);

		//saves the graph to an image
		if (success) {
			std::vector<double>* pngdata = ConvertToPNG(imageReference->image);
			WriteToFile(pngdata, imageName);
			DeleteImage(imageReference->image);
		} else {
			std::cerr << "Error: ";
			for (wchar_t c : *errorMessage->string) {
				std::wcerr << c;
			}
			std::cerr << std::endl;
		}
	}


	void DrawGraphRandom(const wchar_t* graphTitle, const wchar_t* xAxisTitle, std::string imageName) {

		bool success;
		StringReference* errorMessage = new StringReference();
		RGBABitmapImageReference* imageReference = CreateRGBABitmapImageReference();

		//sets up the graph
		ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();
		settings->width = graphWidth;
		settings->height = graphHeight;
		settings->autoBoundaries = true;
		settings->autoPadding = true;
		settings->title = toVector(graphTitle);;
		settings->xLabel = toVector(xAxisTitle);
		settings->yLabel = toVector(L"Collisions");

		//sets up the series for random
		ScatterPlotSeries* randomSeries = GetDefaultScatterPlotSeriesSettings();
		Points pRand = PrepareData(data.GetRandom1());
		randomSeries->xs = &pRand.toDrawX;
		randomSeries->ys = &pRand.toDrawY;
		randomSeries->linearInterpolation = false;
		randomSeries->pointType = toVector(L"dots");
		randomSeries->color = rand1;
		settings->scatterPlotSeries->push_back(randomSeries);


		//draws all the graph series
		success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

		legendY = -10;

		//draws the legend
		//DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Random table size 999"), rand1);
		//DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Random table size 9999"), rand2);
		//DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Random table size 99999"), rand3);



		//saves the graph to an image
		if (success) {
			std::vector<double>* pngdata = ConvertToPNG(imageReference->image);
			WriteToFile(pngdata, imageName);
			DeleteImage(imageReference->image);
		} else {
			std::cerr << "Error: ";
			for (wchar_t c : *errorMessage->string) {
				std::wcerr << c;
			}
			std::cerr << std::endl;

		}
	}

	void TakeSizeValues(double xAxis, int g1, int g2, int g3, int g4, int g5) {

		if (g1 != -1)
			data.UpdateGap1(xAxis, g1);

		if (g2 != -1)
			data.UpdateGap2(xAxis, g2);

		if (g3 != -1)
			data.UpdateGap3(xAxis, g3);

		if (g4 != -1)
			data.UpdateGap4(xAxis, g4);

		if (g5 != -1)
			data.UpdateGap5(xAxis, g5);

	}

	void TakeRandom(double xAxis, int r1, int r2, int r3) {

		if (r1 != -1)

			data.UpdateRandom1(xAxis, r1);
	}

	Points PrepareData(std::vector<std::vector<double>> dataToProcess) {

		Points p;

		//loops through all the data points
		for (int i = 0; i < dataToProcess.size(); i++) {

			//adds this value to be drawn
			p.toDrawX.push_back(dataToProcess[i][0]);
			p.toDrawY.push_back(dataToProcess[i][1]);
		}

		return p;
	}


private:

	Data data;

	int graphWidth;
	int graphHeight;

	int legendX;
	int legendY;


	RGBA* gap1;
	RGBA* gap2;
	RGBA* gap3;
	RGBA* gap4;
	RGBA* gap5;

	RGBA* rand1;
	RGBA* rand2;
	RGBA* rand3;

};

