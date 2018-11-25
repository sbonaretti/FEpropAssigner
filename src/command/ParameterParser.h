#ifndef PARAMETERPARSER_H_
#define PARAMETERPARSER_H_

#include <string>
#include "Interpolator3D.h"
#include "NodePropertyAssigner.h"

namespace command {

using namespace std;

/**
 * Parses user input parameters
 */
class ParameterParser {
private:
	/**
	 * Set the values to a default state
	 */
	void setDefaultValues();
	/**
	 * Gets the help output to be displayed the user
	 * @return The help output
	 */
	string getHelpString();
	/**
	 * The method for the interpolation
	 * Either assignment::TRILINEAR_INTERPOLATION or assignment::TRICUBIC_INTERPOLATION
	 */
	int interpolationMethod;
	/**
	 * The number of iterations for the CT image extrusion
	 */
	int extrusionCount;
	/**
	 * The method for the assignment
	 * Either assignment::NODE_ASSIGNMENT or assignment::ELEMENT_ASSIGNMENT
	 */
	int assignmentMethod;
	/**
	 * True if the tests will to be executed
	 */
	bool testRun;
	/**
	 * The Abaqus input file
	 */
	string abaqusInputFile;
	/**
	 * The Ansys input folder
	 */
	string ansysInputFolder;
	/**
	 * The Dicom input folder
	 */
	string dicomInputFolder;
	/**
	 * The Metaimage input file
	 */
	string metaimageInputFile;
	/**
	 * The Abaqus file for the export
	 */
	string abaqusOutputFile;
	/**
	 * The Ansys file for the export
	 */
	string ansysOutputFile;
	/**
	 * The first parameter for the assignment law
	 */
	double lawParameterA;
	/**
	 * The second parameter for the assignment law
	 */
	double lawParameterB;

	/**
	 * Default value for lawParameterA
	 */
	double defaultLawParameterA;
	/**
	 * Default value for lawParameterB
	 */
	double defaultLawParameterB;
	/**
	 * Default value for extrusionCount
	 */
	static const int defaultExtrusionCount = 3;
	/**
	 * Default value for interpolationMethod
	 */
	static const int defaultInterpolationMethod = assignment::TRILINEAR_INTERPOLATION;
	/**
	 * Default value for assignmentMethod
	 */
	static const int defaultAssignmentMethod = assignment::NODE_ASSIGNMENT;
public:
	ParameterParser();
	/**
	 * Parses an array of program parameters
	 * @warning The first value in the char array is ignored as it usually contains the program's name
	 * @param argc The parameter count
	 * @param argv The parameter array
	 * @return The error message. If the return string is empty the parsing was successful
	 */
	string parse(int argc, char** argv);
	/**
	 * Parses a single parameter
	 * @param arg A parameter
	 * @return The error message. If the return string is empty the parsing was successful
	 */
	string parse(char* arg);
	/**
	 * Returns true if the tests will to be executed
	 * @return Whether the tests will be executed
	 */
	bool getTestRun();
	/**
	 * Returns the number of extrusion iterations in the CT image extrusion
	 * @return The number of extrusion iterations
	 */
	int getExtrusionCount();
	/**
	 * Returns the interpolation method
	 * @return assignment::TRILINEAR_INTERPOLATION or assignment::TRICUBIC_INTERPOLATION
	 */
	int getInterpolationMethod();
	/**
	 * Returns the assignment method
	 * @return assignment::NODE_ASSIGNMENT or assignment::ELEMENT_ASSIGNMENT
	 */
	int getAssignmentMethod();
	/**
	 * Returns the Abaqus input file
	 * @return The file location
	 */
	string getAbaqusInputFile();
	/**
	 * Returns the Ansys input folder
	 * @return The folder location
	 */
	string getAnsysInputFolder();
	/**
	 * Returns the Dicom input folder
	 * @return The folder location
	 */
	string getDicomInputFolder();
	/**
	 * Returns the Abaqus input file
	 * @return The file location
	 */
	string getMetaimageInputFile();
	/**
	 * Returns the Abaqus file destination
	 * @return The file destination
	 */
	string getAbaqusOutputFile();
	/**
	 * Returns the Ansys file destination
	 * @return The file destination
	 */
	string getAnsysOutputFile();
	/**
	 * Gets the first parameter for the assignment law
	 * @return The first law assignment parameter
	 */
	double getLawParameterA();
	/**
	 * Gets the second parameter for the assignment law
	 * @return The second law assignment parameter
	 */
	double getLawParameterB();
	virtual ~ParameterParser();
};

}

#endif /* PARAMETERPARSER_H_ */
