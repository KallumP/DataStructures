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


	std::vector<std::vector<double>> GetInterpolationData() {
		return interpolationData;
	}
	std::vector<std::vector<double>> GetLinearData() {
		return linearData;
	}
	std::vector<std::vector<double>> GetExponentialData() {
		return expoData;
	}
	std::vector<std::vector<double>> GetInterpolationLinearData() {
		return interLinearData;
	}
	std::vector<std::vector<double>> GetInterpolationExponentialData() {
		return interExpoData;
	}

	void UpdateInterpolation(int x, int y) {
		UpdateAverage(&interpolationData, x, y);
	}
	void UpdateLinear(int x, int y) {
		UpdateAverage(&linearData, x, y);
	}
	void UpdateExponential(int x, int y) {
		UpdateAverage(&expoData, x, y);
	}
	void UpdateInterpolationLinear(int x, int y) {
		UpdateAverage(&interLinearData, x, y);
	}
	void UpdateInterpolationExponential(int x, int y) {
		UpdateAverage(&interExpoData, x, y);
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
	std::vector<std::vector<double>>  interpolationData;
	std::vector<std::vector<double>>  linearData;
	std::vector<std::vector<double>>  expoData;
	std::vector<std::vector<double>>  interLinearData;
	std::vector<std::vector<double>>  interExpoData;

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

		interpolationColor = CreateRGBColor(1, 0, 0);
		interpolationLinearColor = CreateRGBColor(0, 1, 0);
		interpolationExponentialColor = CreateRGBColor(0, 0, 1);
		linearColor = CreateRGBColor(0, 1, 1);
		exponentialColor = CreateRGBColor(1, 0, 1);

		legendX = 250;
		legendY = -10;
	}

	void DrawInterpolationGraph(const wchar_t* graphTitle, std::string imageName) {

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
		settings->xLabel = toVector(L"Table size");
		settings->yLabel = toVector(L"Comparisons");

		//sets up the series for interpolation
		ScatterPlotSeries* InterpolationSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInter = PrepareData(data.GetInterpolationData());
		InterpolationSeries->xs = &pInter.toDrawX;
		InterpolationSeries->ys = &pInter.toDrawY;
		InterpolationSeries->linearInterpolation = true;
		InterpolationSeries->lineType = toVector(L"solid");
		InterpolationSeries->color = interpolationColor;
		settings->scatterPlotSeries->push_back(InterpolationSeries);

		//sets up the series for inter linear
		ScatterPlotSeries* InterLinearSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInterLin = PrepareData(data.GetInterpolationLinearData());
		InterLinearSeries->xs = &pInterLin.toDrawX;
		InterLinearSeries->ys = &pInterLin.toDrawY;
		InterLinearSeries->linearInterpolation = true;
		InterLinearSeries->lineType = toVector(L"solid");
		InterLinearSeries->color = interpolationLinearColor;
		settings->scatterPlotSeries->push_back(InterLinearSeries);

		//sets up the data for inter expo
		ScatterPlotSeries* InterExpoSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInterExpo = PrepareData(data.GetInterpolationExponentialData());
		InterExpoSeries->xs = &pInterExpo.toDrawX;
		InterExpoSeries->ys = &pInterExpo.toDrawY;
		InterExpoSeries->linearInterpolation = true;
		InterExpoSeries->lineType = toVector(L"solid");
		InterExpoSeries->color = interpolationExponentialColor;
		settings->scatterPlotSeries->push_back(InterExpoSeries);

		//draws all the graph series
		success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

		//draws the legend
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation"), interpolationColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation Linear"), interpolationLinearColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation Exponential"), interpolationExponentialColor);


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

	void DrawAllSearchGraphs(const wchar_t* graphTitle, std::string imageName) {

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
		settings->xLabel = toVector(L"Table size");
		settings->yLabel = toVector(L"Comparisons");

		//sets up the series for interpolation
		ScatterPlotSeries* InterpolationSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInter = PrepareData(data.GetInterpolationData());
		InterpolationSeries->xs = &pInter.toDrawX;
		InterpolationSeries->ys = &pInter.toDrawY;
		InterpolationSeries->linearInterpolation = true;
		InterpolationSeries->lineType = toVector(L"solid");
		InterpolationSeries->color = interpolationColor;
		settings->scatterPlotSeries->push_back(InterpolationSeries);

		//sets up the series for inter linear
		ScatterPlotSeries* InterLinearSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInterLin = PrepareData(data.GetInterpolationLinearData());
		InterLinearSeries->xs = &pInterLin.toDrawX;
		InterLinearSeries->ys = &pInterLin.toDrawY;
		InterLinearSeries->linearInterpolation = true;
		InterLinearSeries->lineType = toVector(L"solid");
		InterLinearSeries->color = interpolationLinearColor;
		settings->scatterPlotSeries->push_back(InterLinearSeries);

		//sets up the data for inter expo
		ScatterPlotSeries* InterExpoSeries = GetDefaultScatterPlotSeriesSettings();
		Points pInterExpo = PrepareData(data.GetInterpolationExponentialData());
		InterExpoSeries->xs = &pInterExpo.toDrawX;
		InterExpoSeries->ys = &pInterExpo.toDrawY;
		InterExpoSeries->linearInterpolation = true;
		InterExpoSeries->lineType = toVector(L"solid");
		InterExpoSeries->color = interpolationExponentialColor;
		settings->scatterPlotSeries->push_back(InterExpoSeries);

		//sets up the series for linear
		ScatterPlotSeries* LinearSeries = GetDefaultScatterPlotSeriesSettings();
		Points pLinear = PrepareData(data.GetLinearData());
		LinearSeries->xs = &pLinear.toDrawX;
		LinearSeries->ys = &pLinear.toDrawY;
		LinearSeries->linearInterpolation = true;
		LinearSeries->lineType = toVector(L"solid");
		LinearSeries->color = linearColor;
		settings->scatterPlotSeries->push_back(LinearSeries);

		//sets up the series for exponential
		ScatterPlotSeries* ExpoSeries = GetDefaultScatterPlotSeriesSettings();
		Points pExpo = PrepareData(data.GetExponentialData());
		ExpoSeries->xs = &pExpo.toDrawX;
		ExpoSeries->ys = &pExpo.toDrawY;
		ExpoSeries->linearInterpolation = true;
		ExpoSeries->lineType = toVector(L"solid");
		ExpoSeries->color = exponentialColor;
		settings->scatterPlotSeries->push_back(ExpoSeries);

		//draws all the graph series
		success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

		//draws the legend
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation"), interpolationColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation Linear"), interpolationLinearColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Interpolation Exponential"), interpolationExponentialColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Linear"), linearColor);
		DrawText(imageReference->image, legendX, legendY += 20, toVector(L"Exponential"), exponentialColor);

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

	void TakeSizeValues(int tableSize, int interpolation, int linear, int expo, int interLinear, int interExpo) {

		if (interpolation != -1)
			data.UpdateInterpolation(tableSize, interpolation);

		if (linear != -1)
			data.UpdateLinear(tableSize, linear);

		if (expo != -1)
			data.UpdateExponential(tableSize, expo);

		if (interLinear != -1)
			data.UpdateInterpolationLinear(tableSize, interLinear);

		if (interExpo != -1)
			data.UpdateInterpolationExponential(tableSize, interExpo);
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


	RGBA* interpolationColor;
	RGBA* interpolationLinearColor;
	RGBA* interpolationExponentialColor;
	RGBA* linearColor;
	RGBA* exponentialColor;
};

