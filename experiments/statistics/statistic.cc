#include "statistic.h"
#include <cmath>
//#include <float.h>
#include <stdio.h>
using namespace std;

namespace statistic {
	NQTable Statistic::normalQuantileTable;
	Statistic::Filler Statistic::filler;

	Statistic::Statistic(const char *id, bool accumulated) : keepSamples(!accumulated) {
		this->setName( id );
		this->min = DBL_MAX;
		this->max = -DBL_MAX;
		this->sum = 0.0;
		this->squareSum = 0.0;
		this->size = 0;
		this->confidenceLevel = 99.0;
		this->accuracy = 5.0f;
		this->sample = 0;
		if (!accumulated) {
			this->sample = new vector<double>;
			this->sample->reserve(150);
		}
	}

	Statistic::~Statistic() {
		if (this->sample != 0) {
			this->sample->clear();
			delete this->sample;
		}
	}

	const char *Statistic::getName(){
		return this->name.c_str();
	}

	void Statistic::setName(const char *id ){
		this->name = id;
	}
 
	/// Adds an sample to the sample Set
	void Statistic::addSample(double value) {
		if (this->sample != 0) {
			this->sample->push_back(value);
		}
		++this->size;

		if (this->min > value)
			this->min = value;

		if (this->max < value)
			this->max = value;

		this->sum += value;
		this->squareSum += (value * value);
	}

	/// Adds an sample to the sample Set
	void Statistic::addObservation(double value) {
		this->addSample(value);
	}

	/// Calculates the sample mean
	double Statistic::calculateMean() const {
		double mean = 0.0;

		if (this->size > 0)
			mean = this->sum / (double)this->size;

		return mean;
	}

	/// Calculates the sample standard deviation
	double Statistic::calculateStandardDeviation() const {
		double stdDev = 0.0;

		if (this->size > 0) {
			double mean = this->calculateMean();
			stdDev = mean * (this->size * mean - 2 * this->sum) + this->squareSum;
			if (stdDev < 0.0)
				stdDev = 0.0; // necess�io para evitar erros de arredondamento.
			else
				stdDev = sqrt(stdDev / (this->size));
		}

		return stdDev;
	}

	/// Returns the confidence interval for the sample mean based
	/// on the given confidence level
	double Statistic::calculateConfidenceInterval() const {
		double stdDev = this->calculateStandardDeviation();

		if (stdDev != 0.0) {
			double confidenceInterval;
			NQTable::const_iterator p1;
			double z;
			double alfa = 1.0 - (this->confidenceLevel / 100.0);
			double p = 1.0 - (alfa / 2.0);

			/// getting z value for p
			/// (if p is not in the table, the closest value is used)
			p1 = Statistic::normalQuantileTable.find(p);
			if (p1 == Statistic::normalQuantileTable.end()) {
				if (p < 0.5)
					p = 0.5;
				else if (p > 0.9999)
					p = 0.9999;
				else if (p < 0.9)
					p = ((double) floor(100.0 * p)) / 100.0;
				else if (p > 0.999)
					p = ((double) floor(10000.0 * p)) / 10000.0;
				else
					p = ((double) floor(1000.0 * p)) / 1000.0;
			}

 			/**
			 * I'm using find(p) instead of [p] because find(p) is const protected,
			 * while [p] isn't. This means that the value that I want is in
			 * find(p)->second, since find(p) will return a map iterator.
			*/
			z = Statistic::normalQuantileTable.find(p)->second;
			confidenceInterval = z * stdDev / sqrt((double) this->size);
			return confidenceInterval;
		}
		return 0.0;
	}

	/// Calculates the precision of the confidence interval, in %
	double Statistic::calculatePrecision() const {
		double precision = 0.0;
		double mean = this->calculateMean();
		double confidenceInterval = this->calculateConfidenceInterval();

		if (mean != 0.0)
			precision = 100.0 * confidenceInterval / mean;

		return precision;
	}

	/// Calculates the number of replications required to achieve the
	/// desired confidence level and accuracy
	int Statistic::calculateRequiredNumberOfReplications(double accuracy) {
		double stdDev = this->calculateStandardDeviation();
		if (stdDev != 0) {
			int rnReplications;
			NQTable::const_iterator p1;
			double z;
			double alfa = 1.0 - (this->confidenceLevel / 100.0);
			double p = 1.0 - (alfa / 2.0);
			double mean = this->calculateMean();

			/// getting z value for p
			/// (if p is not in the table, the closest value is used)
			p1 = Statistic::normalQuantileTable.find(p);
			if (p1 == Statistic::normalQuantileTable.end()) {
				if (p < 0.5)
					p = 0.5;
				else if (p > 0.9999)
					p = 0.9999;
				else if (p < 0.9)
					p = ((double) floor(100.0 * p)) / 100.0;
				else if (p > 0.999)
					p = ((double) floor(10000.0 * p)) / 10000.0;
				else
					p = ((double) floor(1000.0 * p)) / 1000.0;
			}

 			/**
			 * I'm using find(p) instead of [p] because find(p) is const protected,
			 * while [p] isn't. This means that the value that I want is in
			 * find(p)->second, since find(p) will return a map iterator.
			*/
			z = Statistic::normalQuantileTable.find(p)->second;
			rnReplications = (int) ceil(((100.0 * z * stdDev) / (accuracy * mean)) * ((100.0 * z * stdDev) / (accuracy * mean)));

			if (rnReplications < 0) {
				rnReplications = 0;
			}
			return rnReplications;
		}
		return 0;
	}

	/// Fills in the Normal Quantile Table with the quantiles of
	/// the Unit Normal Distribution
	void Statistic::fillNormalQuantileTable(void) {
		Statistic::normalQuantileTable.insert(NQPair(0.5,0.0));
		Statistic::normalQuantileTable.insert(NQPair(0.51,0.025));
		Statistic::normalQuantileTable.insert(NQPair(0.52,0.050));
		Statistic::normalQuantileTable.insert(NQPair(0.53,0.075));
		Statistic::normalQuantileTable.insert(NQPair(0.54,0.100));
		Statistic::normalQuantileTable.insert(NQPair(0.55,0.126));
		Statistic::normalQuantileTable.insert(NQPair(0.56,0.151));
		Statistic::normalQuantileTable.insert(NQPair(0.57,0.176));
		Statistic::normalQuantileTable.insert(NQPair(0.58,0.202));
		Statistic::normalQuantileTable.insert(NQPair(0.59,0.228));
		Statistic::normalQuantileTable.insert(NQPair(0.6,0.253));
		Statistic::normalQuantileTable.insert(NQPair(0.61,0.279));
		Statistic::normalQuantileTable.insert(NQPair(0.62,0.305));
		Statistic::normalQuantileTable.insert(NQPair(0.63,0.332));
		Statistic::normalQuantileTable.insert(NQPair(0.64,0.358));
		Statistic::normalQuantileTable.insert(NQPair(0.65,0.385));
		Statistic::normalQuantileTable.insert(NQPair(0.66,0.412));
		Statistic::normalQuantileTable.insert(NQPair(0.67,0.440));
		Statistic::normalQuantileTable.insert(NQPair(0.68,0.468));
		Statistic::normalQuantileTable.insert(NQPair(0.69,0.496));
		Statistic::normalQuantileTable.insert(NQPair(0.7,0.524));
		Statistic::normalQuantileTable.insert(NQPair(0.71,0.553));
		Statistic::normalQuantileTable.insert(NQPair(0.72,0.583));
		Statistic::normalQuantileTable.insert(NQPair(0.73,0.613));
		Statistic::normalQuantileTable.insert(NQPair(0.74,0.643));
		Statistic::normalQuantileTable.insert(NQPair(0.75,0.674));
		Statistic::normalQuantileTable.insert(NQPair(0.76,0.706));
		Statistic::normalQuantileTable.insert(NQPair(0.77,0.739));
		Statistic::normalQuantileTable.insert(NQPair(0.78,0.772));
		Statistic::normalQuantileTable.insert(NQPair(0.79,0.806));
		Statistic::normalQuantileTable.insert(NQPair(0.8,0.842));
		Statistic::normalQuantileTable.insert(NQPair(0.81,0.878));
		Statistic::normalQuantileTable.insert(NQPair(0.82,0.915));
		Statistic::normalQuantileTable.insert(NQPair(0.83,0.954));
		Statistic::normalQuantileTable.insert(NQPair(0.84,0.994));
		Statistic::normalQuantileTable.insert(NQPair(0.85,1.036));
		Statistic::normalQuantileTable.insert(NQPair(0.86,1.080));
		Statistic::normalQuantileTable.insert(NQPair(0.87,1.126));
		Statistic::normalQuantileTable.insert(NQPair(0.88,1.175));
		Statistic::normalQuantileTable.insert(NQPair(0.89,1.227));
		Statistic::normalQuantileTable.insert(NQPair(0.9,1.282));
		Statistic::normalQuantileTable.insert(NQPair(0.901,1.287));
		Statistic::normalQuantileTable.insert(NQPair(0.902,1.293));
		Statistic::normalQuantileTable.insert(NQPair(0.903,1.299));
		Statistic::normalQuantileTable.insert(NQPair(0.904,1.305));
		Statistic::normalQuantileTable.insert(NQPair(0.905,1.311));
		Statistic::normalQuantileTable.insert(NQPair(0.906,1.317));
		Statistic::normalQuantileTable.insert(NQPair(0.907,1.323));
		Statistic::normalQuantileTable.insert(NQPair(0.908,1.329));
		Statistic::normalQuantileTable.insert(NQPair(0.909,1.335));
		Statistic::normalQuantileTable.insert(NQPair(0.91,1.341));
		Statistic::normalQuantileTable.insert(NQPair(0.911,1.347));
		Statistic::normalQuantileTable.insert(NQPair(0.912,1.353));
		Statistic::normalQuantileTable.insert(NQPair(0.913,1.359));
		Statistic::normalQuantileTable.insert(NQPair(0.914,1.366));
		Statistic::normalQuantileTable.insert(NQPair(0.915,1.372));
		Statistic::normalQuantileTable.insert(NQPair(0.916,1.379));
		Statistic::normalQuantileTable.insert(NQPair(0.917,1.385));
		Statistic::normalQuantileTable.insert(NQPair(0.918,1.392));
		Statistic::normalQuantileTable.insert(NQPair(0.919,1.398));
		Statistic::normalQuantileTable.insert(NQPair(0.92,1.405));
		Statistic::normalQuantileTable.insert(NQPair(0.921,1.412));
		Statistic::normalQuantileTable.insert(NQPair(0.922,1.419));
		Statistic::normalQuantileTable.insert(NQPair(0.923,1.426));
		Statistic::normalQuantileTable.insert(NQPair(0.924,1.433));
		Statistic::normalQuantileTable.insert(NQPair(0.925,1.440));
		Statistic::normalQuantileTable.insert(NQPair(0.926,1.447));
		Statistic::normalQuantileTable.insert(NQPair(0.927,1.454));
		Statistic::normalQuantileTable.insert(NQPair(0.928,1.461));
		Statistic::normalQuantileTable.insert(NQPair(0.929,1.468));
		Statistic::normalQuantileTable.insert(NQPair(0.93,1.476));
		Statistic::normalQuantileTable.insert(NQPair(0.931,1.483));
		Statistic::normalQuantileTable.insert(NQPair(0.932,1.491));
		Statistic::normalQuantileTable.insert(NQPair(0.933,1.499));
		Statistic::normalQuantileTable.insert(NQPair(0.934,1.506));
		Statistic::normalQuantileTable.insert(NQPair(0.935,1.514));
		Statistic::normalQuantileTable.insert(NQPair(0.936,1.522));
		Statistic::normalQuantileTable.insert(NQPair(0.937,1.530));
		Statistic::normalQuantileTable.insert(NQPair(0.938,1.538));
		Statistic::normalQuantileTable.insert(NQPair(0.939,1.546));
		Statistic::normalQuantileTable.insert(NQPair(0.94,1.555));
		Statistic::normalQuantileTable.insert(NQPair(0.941,1.563));
		Statistic::normalQuantileTable.insert(NQPair(0.942,1.572));
		Statistic::normalQuantileTable.insert(NQPair(0.943,1.580));
		Statistic::normalQuantileTable.insert(NQPair(0.944,1.589));
		Statistic::normalQuantileTable.insert(NQPair(0.945,1.598));
		Statistic::normalQuantileTable.insert(NQPair(0.946,1.607));
		Statistic::normalQuantileTable.insert(NQPair(0.947,1.616));
		Statistic::normalQuantileTable.insert(NQPair(0.948,1.626));
		Statistic::normalQuantileTable.insert(NQPair(0.949,1.635));
		Statistic::normalQuantileTable.insert(NQPair(0.95,1.645));
		Statistic::normalQuantileTable.insert(NQPair(0.951,1.655));
		Statistic::normalQuantileTable.insert(NQPair(0.952,1.665));
		Statistic::normalQuantileTable.insert(NQPair(0.953,1.675));
		Statistic::normalQuantileTable.insert(NQPair(0.954,1.685));
		Statistic::normalQuantileTable.insert(NQPair(0.955,1.695));
		Statistic::normalQuantileTable.insert(NQPair(0.956,1.706));
		Statistic::normalQuantileTable.insert(NQPair(0.957,1.717));
		Statistic::normalQuantileTable.insert(NQPair(0.958,1.728));
		Statistic::normalQuantileTable.insert(NQPair(0.959,1.739));
		Statistic::normalQuantileTable.insert(NQPair(0.96,1.751));
		Statistic::normalQuantileTable.insert(NQPair(0.961,1.762));
		Statistic::normalQuantileTable.insert(NQPair(0.962,1.774));
		Statistic::normalQuantileTable.insert(NQPair(0.963,1.787));
		Statistic::normalQuantileTable.insert(NQPair(0.964,1.799));
		Statistic::normalQuantileTable.insert(NQPair(0.965,1.812));
		Statistic::normalQuantileTable.insert(NQPair(0.966,1.825));
		Statistic::normalQuantileTable.insert(NQPair(0.967,1.838));
		Statistic::normalQuantileTable.insert(NQPair(0.968,1.852));
		Statistic::normalQuantileTable.insert(NQPair(0.969,1.866));
		Statistic::normalQuantileTable.insert(NQPair(0.97,1.881));
		Statistic::normalQuantileTable.insert(NQPair(0.971,1.896));
		Statistic::normalQuantileTable.insert(NQPair(0.972,1.911));
		Statistic::normalQuantileTable.insert(NQPair(0.973,1.927));
		Statistic::normalQuantileTable.insert(NQPair(0.974,1.943));
		Statistic::normalQuantileTable.insert(NQPair(0.975,1.960));
		Statistic::normalQuantileTable.insert(NQPair(0.976,1.977));
		Statistic::normalQuantileTable.insert(NQPair(0.977,1.995));
		Statistic::normalQuantileTable.insert(NQPair(0.978,2.014));
		Statistic::normalQuantileTable.insert(NQPair(0.979,2.034));
		Statistic::normalQuantileTable.insert(NQPair(0.98,2.054));
		Statistic::normalQuantileTable.insert(NQPair(0.981,2.075));
		Statistic::normalQuantileTable.insert(NQPair(0.982,2.097));
		Statistic::normalQuantileTable.insert(NQPair(0.983,2.120));
		Statistic::normalQuantileTable.insert(NQPair(0.984,2.144));
		Statistic::normalQuantileTable.insert(NQPair(0.985,2.170));
		Statistic::normalQuantileTable.insert(NQPair(0.986,2.197));
		Statistic::normalQuantileTable.insert(NQPair(0.987,2.226));
		Statistic::normalQuantileTable.insert(NQPair(0.988,2.257));
		Statistic::normalQuantileTable.insert(NQPair(0.989,2.290));
		Statistic::normalQuantileTable.insert(NQPair(0.99,2.326));
		Statistic::normalQuantileTable.insert(NQPair(0.9901,2.330));
		Statistic::normalQuantileTable.insert(NQPair(0.9902,2.334));
		Statistic::normalQuantileTable.insert(NQPair(0.9903,2.338));
		Statistic::normalQuantileTable.insert(NQPair(0.9904,2.342));
		Statistic::normalQuantileTable.insert(NQPair(0.9905,2.346));
		Statistic::normalQuantileTable.insert(NQPair(0.9906,2.349));
		Statistic::normalQuantileTable.insert(NQPair(0.9907,2.353));
		Statistic::normalQuantileTable.insert(NQPair(0.9908,2.357));
		Statistic::normalQuantileTable.insert(NQPair(0.9909,2.362));
		Statistic::normalQuantileTable.insert(NQPair(0.991,2.366));
		Statistic::normalQuantileTable.insert(NQPair(0.9911,2.370));
		Statistic::normalQuantileTable.insert(NQPair(0.9912,2.374));
		Statistic::normalQuantileTable.insert(NQPair(0.9913,2.378));
		Statistic::normalQuantileTable.insert(NQPair(0.9914,2.382));
		Statistic::normalQuantileTable.insert(NQPair(0.9915,2.387));
		Statistic::normalQuantileTable.insert(NQPair(0.9916,2.391));
		Statistic::normalQuantileTable.insert(NQPair(0.9917,2.395));
		Statistic::normalQuantileTable.insert(NQPair(0.9918,2.400));
		Statistic::normalQuantileTable.insert(NQPair(0.9919,2.404));
		Statistic::normalQuantileTable.insert(NQPair(0.992,2.409));
		Statistic::normalQuantileTable.insert(NQPair(0.9921,2.414));
		Statistic::normalQuantileTable.insert(NQPair(0.9922,2.418));
		Statistic::normalQuantileTable.insert(NQPair(0.9923,2.423));
		Statistic::normalQuantileTable.insert(NQPair(0.9924,2.428));
		Statistic::normalQuantileTable.insert(NQPair(0.9925,2.432));
		Statistic::normalQuantileTable.insert(NQPair(0.9926,2.437));
		Statistic::normalQuantileTable.insert(NQPair(0.9927,2.442));
		Statistic::normalQuantileTable.insert(NQPair(0.9928,2.447));
		Statistic::normalQuantileTable.insert(NQPair(0.9929,2.452));
		Statistic::normalQuantileTable.insert(NQPair(0.993,2.457));
		Statistic::normalQuantileTable.insert(NQPair(0.9931,2.462));
		Statistic::normalQuantileTable.insert(NQPair(0.9932,2.468));
		Statistic::normalQuantileTable.insert(NQPair(0.9933,2.473));
		Statistic::normalQuantileTable.insert(NQPair(0.9934,2.478));
		Statistic::normalQuantileTable.insert(NQPair(0.9935,2.484));
		Statistic::normalQuantileTable.insert(NQPair(0.9936,2.489));
		Statistic::normalQuantileTable.insert(NQPair(0.9937,2.495));
		Statistic::normalQuantileTable.insert(NQPair(0.9938,2.501));
		Statistic::normalQuantileTable.insert(NQPair(0.9939,2.506));
		Statistic::normalQuantileTable.insert(NQPair(0.994,2.512));
		Statistic::normalQuantileTable.insert(NQPair(0.9941,2.518));
		Statistic::normalQuantileTable.insert(NQPair(0.9942,2.524));
		Statistic::normalQuantileTable.insert(NQPair(0.9943,2.530));
		Statistic::normalQuantileTable.insert(NQPair(0.9944,2.536));
		Statistic::normalQuantileTable.insert(NQPair(0.9945,2.543));
		Statistic::normalQuantileTable.insert(NQPair(0.9946,2.549));
		Statistic::normalQuantileTable.insert(NQPair(0.9947,2.556));
		Statistic::normalQuantileTable.insert(NQPair(0.9948,2.562));
		Statistic::normalQuantileTable.insert(NQPair(0.9949,2.569));
		Statistic::normalQuantileTable.insert(NQPair(0.995,2.576));
		Statistic::normalQuantileTable.insert(NQPair(0.9951,2.583));
		Statistic::normalQuantileTable.insert(NQPair(0.9952,2.590));
		Statistic::normalQuantileTable.insert(NQPair(0.9953,2.597));
		Statistic::normalQuantileTable.insert(NQPair(0.9954,2.605));
		Statistic::normalQuantileTable.insert(NQPair(0.9955,2.612));
		Statistic::normalQuantileTable.insert(NQPair(0.9956,2.620));
		Statistic::normalQuantileTable.insert(NQPair(0.9957,2.628));
		Statistic::normalQuantileTable.insert(NQPair(0.9958,2.636));
		Statistic::normalQuantileTable.insert(NQPair(0.9959,2.644));
		Statistic::normalQuantileTable.insert(NQPair(0.996,2.652));
		Statistic::normalQuantileTable.insert(NQPair(0.9961,2.661));
		Statistic::normalQuantileTable.insert(NQPair(0.9962,2.669));
		Statistic::normalQuantileTable.insert(NQPair(0.9963,2.678));
		Statistic::normalQuantileTable.insert(NQPair(0.9964,2.687));
		Statistic::normalQuantileTable.insert(NQPair(0.9965,2.697));
		Statistic::normalQuantileTable.insert(NQPair(0.9966,2.706));
		Statistic::normalQuantileTable.insert(NQPair(0.9967,2.716));
		Statistic::normalQuantileTable.insert(NQPair(0.9968,2.727));
		Statistic::normalQuantileTable.insert(NQPair(0.9969,2.737));
		Statistic::normalQuantileTable.insert(NQPair(0.997,2.748));
		Statistic::normalQuantileTable.insert(NQPair(0.9971,2.759));
		Statistic::normalQuantileTable.insert(NQPair(0.9972,2.770));
		Statistic::normalQuantileTable.insert(NQPair(0.9973,2.782));
		Statistic::normalQuantileTable.insert(NQPair(0.9974,2.794));
		Statistic::normalQuantileTable.insert(NQPair(0.9975,2.807));
		Statistic::normalQuantileTable.insert(NQPair(0.9976,2.820));
		Statistic::normalQuantileTable.insert(NQPair(0.9977,2.834));
		Statistic::normalQuantileTable.insert(NQPair(0.9978,2.848));
		Statistic::normalQuantileTable.insert(NQPair(0.9979,2.863));
		Statistic::normalQuantileTable.insert(NQPair(0.998,2.878));
		Statistic::normalQuantileTable.insert(NQPair(0.9981,2.894));
		Statistic::normalQuantileTable.insert(NQPair(0.9982,2.911));
		Statistic::normalQuantileTable.insert(NQPair(0.9983,2.929));
		Statistic::normalQuantileTable.insert(NQPair(0.9984,2.948));
		Statistic::normalQuantileTable.insert(NQPair(0.9985,2.968));
		Statistic::normalQuantileTable.insert(NQPair(0.9986,2.989));
		Statistic::normalQuantileTable.insert(NQPair(0.9987,3.011));
		Statistic::normalQuantileTable.insert(NQPair(0.9988,3.036));
		Statistic::normalQuantileTable.insert(NQPair(0.9989,3.062));
		Statistic::normalQuantileTable.insert(NQPair(0.999,3.090));
		Statistic::normalQuantileTable.insert(NQPair(0.9991,3.121));
		Statistic::normalQuantileTable.insert(NQPair(0.9992,3.156));
		Statistic::normalQuantileTable.insert(NQPair(0.9993,3.195));
		Statistic::normalQuantileTable.insert(NQPair(0.9994,3.239));
		Statistic::normalQuantileTable.insert(NQPair(0.9995,3.291));
		Statistic::normalQuantileTable.insert(NQPair(0.9996,3.353));
		Statistic::normalQuantileTable.insert(NQPair(0.9997,3.432));
		Statistic::normalQuantileTable.insert(NQPair(0.9998,3.540));
		Statistic::normalQuantileTable.insert(NQPair(0.9999,3.719));
	}

	void Statistic::resetSample() {
		if (this->sample != 0)
			this->sample->clear();
		this->min = DBL_MAX;
		this->max = -DBL_MAX;
		this->sum = 0.0;
		this->squareSum = 0.0;
		this->size = 0;
	}

	void Statistic::print( FILE *out ){
		fprintf( out, "<<< Statistics for \"%s\" >>>\n", this->getName() );
		fprintf( out, "Sample size..............................: %d\n", this->getSampleSize() );
		fprintf( out, "Minimum value............................: %f\n", this->getSampleMin() );
		fprintf( out, "Maximum value............................: %f\n", this->getSampleMax() );
		fprintf( out, "Sample Mean..............................: %f\n", this->getSampleMean() );
		fprintf( out, "Standard deviation.......................: %f\n", this->getSampleStandardDeviation() );
		fprintf( out, "Confidence Level.........................: %f\n", this->getConfidenceLevel() );
		fprintf( out, "Confidence Interval (CI).................: %f\n", this->getConfidenceInterval() );
		fprintf( out, "Target Precision for CI (error)..........: %4.2f%%\n", this->getAccuracy() );
		fprintf( out, "Obtained Precision for CI................: %4.2f%%\n", this->getPrecision() );
		fprintf( out, "Required Sample size for target precision: %d\n", this->getRequiredNumberOfReplications() );
	}
	
	///------------------------------------------------------------------------
	void Statistic::printSamples( FILE *out ){
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ) {
			fprintf( out, "%4d. %8.3f\n", i, this->getSample(i) );
		}
	}

	///------------------------------------------------------------------------
	FairnessIndex::FairnessIndex() : Statistic( "Fairness Indexes", false) 
	{
	}

	///------------------------------------------------------------------------
	/// Gini Fairness Index
	///------------------------------------------------------------------------
	double FairnessIndex::gini() const {
		if(this->getSampleSize() == 0 ) {
			return 0;
		}

		double sum_of_differences = 0;
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ) {
			for( unsigned int j = 0; j < this->getSampleSize(); ++j ) {
				double diff = this->getSample(i) - this->getSample(j);
				/// sum the absolute (positive) value of the difference
				sum_of_differences += ABS(diff);
			}
		}
		double divisor = 2 * this->getSampleSize() * this->getSampleSize() * this->mean();
		double gini = (divisor != 0 ? sum_of_differences / divisor : 0 );

		return gini;
	}

	///------------------------------------------------------------------------
	/// Jain Fairness Index
	///------------------------------------------------------------------------
	double FairnessIndex::jain() const {
		if(this->getSampleSize() == 0 ) {
			return 0;
		}
		
		double jain = this->sum * this->sum / (this->getSampleSize() * this->squareSum);
		return jain;
	}	

	///------------------------------------------------------------------------
	/// MinMax Fairness Index
	///------------------------------------------------------------------------
	double FairnessIndex::minmax() const {
		if(this->getSampleSize() == 0 || this->getMax() == 0 ) {
			return 0;
		}

		double minmax = this->getMin() / this->getMax();
		return minmax;
	}	

	///------------------------------------------------------------------------
	/// Dianati Fairness Index
	///------------------------------------------------------------------------
	double FairnessIndex::dianati() const {
		//TODO: Get alfa as parameter
		double alfa = 0.2;

		//TODO: Fix min requirements: according to QoS
		double *Xmin = new double[this->getSampleSize()];
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ){
			Xmin[i] = 0;
		}
			
		//fprintf( stderr, "Start calculations...\n" );
		//Start calculations

		double *Smin = new double[this->getSampleSize()];
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ){
			Smin[i] = Xmin[i] / this->sum;
		}
		
		double *S = new double[this->getSampleSize()];
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ){
			S[i] = 1/this->getSampleSize() + alfa*( (this->getSample(i) / this->sum) - 1/this->getSampleSize());
		}

		//fprintf( stderr, "Utility function...\n" );
		// ----------------- Utility function
		double *Us = new double[this->getSampleSize()];
		double *U1 = new double[this->getSampleSize()];
		for( unsigned int i = 0; i < this->getSampleSize(); ++i ) {
			if( (this->getSample(i) / this->sum) < Smin[i] ) { //U1
				if( S[i] < Smin[i] ) {
					Us[i] = 0;
				} else {
					Us[i] = sqrt(Smin[i]);
				}
				if( 1 < Smin[i] ) {
					U1[i] = 0;
				} else {
					U1[i] = sqrt(Smin[i]);
				}
			} else { //U2
				if( S[i] <= Smin[i] ) {
					Us[i] = 0;
				} else if( S[i] >= (this->getSample(i) / this->sum) ) {
					Us[i] = sqrt((this->getSample(i) / this->sum) - Smin[i]);
				} else {
					Us[i] = sqrt(S[i] - Smin[i]);
				}
				if( 1 <= Smin[i] ) {
					U1[i] = 0;
				} else if( 1 >= (this->getSample(i) / this->sum) ) {
					U1[i] = sqrt((this->getSample(i) / this->sum) - Smin[i]);
				} else {
					U1[i] = sqrt(1 - Smin[i]);
				}
			}
		}
		
		double SumUs = 0;
		double SumU1 = 0;
		for( unsigned int i = 0; i < this->getSampleSize(); ++i) {
			SumUs+= Us[i];
			SumU1+= U1[i];
		}
		double dianati = SumUs/SumU1;
		dianati *= dianati;
		
		delete Xmin;
		delete Smin;
		delete S;
		delete Us;
		delete U1;
		
		return dianati;
	}

	///------------------------------------------------------------------------
	void FairnessIndex::reset(){
		this->resetSample();
	}

};
