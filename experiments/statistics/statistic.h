#ifndef _STATISTIC_H_
#define _STATISTIC_H_

#include <vector>
#include <map>
#include <string>
#include <float.h>

#define ABS(x)	((x)>0?(x):-(x))

using namespace std;

namespace statistic {
	typedef map <double,double> NQTable;
	typedef pair <double,double> NQPair;

	/** This class defines methods to provide statistics about samples
	*/
	class Statistic {
		private:
			string name;
			bool keepSamples;
			double min;
			double max;
			unsigned size;
			double confidenceLevel;
			double accuracy;
			vector<double> *sample;
			static NQTable normalQuantileTable;
			class Filler {
				public:
					Filler() {
						Statistic::fillNormalQuantileTable();
					}
			};
			static Filler filler;

			double calculateMean() const ;
			double calculateStandardDeviation() const;
			double calculateConfidenceInterval() const;
			double calculatePrecision() const;
			int calculateRequiredNumberOfReplications(double accuracy);
			static void fillNormalQuantileTable();
		protected:
			double sum;
			double squareSum;

		public:
			Statistic(const char *id = "(noId)", bool accumulated = true);
			~Statistic();

			const char *getName();
			void setName(const char *id );
			void addSample(double value);
			void addObservation(double value);

			unsigned getSize() const {
				return this->size;
			}

			const vector<double> &getSamples() const {
				return *this->sample;
			}

			// This method returns the sample size
			// (i.e., the number of observations within the sample)
			unsigned getSampleSize() const {
				return this->size;
			}

			// Returns the minimal observation of the sample
			double getSampleMin() const {
				return this->size > 0 ? this->min : 0.0;
			}

			// Returns the minimal observation of the sample
			double getMin() const {
				return this->getSampleMin();
			}

			// Returns the maximal observation of the sample
			double getSampleMax() const {
				return this->size > 0 ? this->max : 0.0;
			}

			// Returns the maximal observation of the sample
			double getMax() const {
				return this->getSampleMax();
			}

			// Returns the sample mean
			double getSampleMean() const {
				return this->calculateMean();
			}

			// Returns the sample mean
			double mean() const {
				return this->getSampleMean();
			}

			// Returns the sample standard deviation
			double getSampleStandardDeviation() const {
				return this->calculateStandardDeviation();
			}

			// Returns the sample standard deviation
			double sd() const {
				return this->getSampleStandardDeviation();
			}

			// Returns the confidence interval for the sample mean based
			// on the default confidence level
			double getConfidenceInterval() const {
				return this->calculateConfidenceInterval();
			}

			// Returns the precision of the default confidence interval
			double getPrecision() const {
				return this->calculatePrecision();
			}

			// Returns the number of replications required to achieve the
			// desired confidence level and accuracy
			int getRequiredNumberOfReplications(double accuracy) {
				return this->calculateRequiredNumberOfReplications(accuracy);
			}

			int getRequiredNumberOfReplications() {
				return this->calculateRequiredNumberOfReplications(this->accuracy);
			}

			//Returns the confidence level
			double getConfidenceLevel() const {
				return this->confidenceLevel;
			}

			//Returns a sample by his index
//			double getSampleByIndex(int index) const {
			double getSample(unsigned int index) const {
				if( this->sample ) {
					return (*this->sample)[index];
				} else {
					return DBL_MAX;
				}
			}

			// Sets the confidence level value
			void setConfidenceLevel(double confidenceLevel) {
				this->confidenceLevel = confidenceLevel;
			}

			void setAccuracy(double accuracy) {
				this->accuracy = accuracy;
			}

			double getAccuracy() const {
				return this->accuracy;
			}

			// Resets the sample (i.e., deletes all the observations of the sample)
			void resetSample();
			
			// Resets the sample (same as resetSample())
			void reset(){
				this->resetSample();
			}

			void print( FILE *out );
			void printSamples( FILE *out );
		};
	
	/** This class defines methods to provide statistics about samples
	*/
	class FairnessIndex : public Statistic {
		public:
			FairnessIndex();
			double jain() const;
			double gini() const;
			double dianati() const;
			double minmax() const;
			void reset();
	};
};

#endif
