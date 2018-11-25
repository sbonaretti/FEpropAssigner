#include "ParameterParser.h"

#include <qstring.h>
#include <qregexp.h>

#include "NodePropertyAssigner.h"
#include "Interpolator3D.h"

namespace command {


ParameterParser::ParameterParser() {
	defaultLawParameterA = 6.85;
	defaultLawParameterB = 1.49;

	setDefaultValues();
}

string ParameterParser::parse(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		string result = parse(argv[i]);
		if (!result.empty())
			return result;
	}
	return "";
}

string ParameterParser::parse(char* arg) {
	string SUCCESS("");

	QRegExp rx;
	rx.setCaseSensitive(false);

	rx.setPattern("noGui");
	if (rx.search(arg) == 0)
		return SUCCESS;

	rx.setPattern("^Help$");
	if (rx.search(arg) == 0) {
		return getHelpString();
	}
	rx.setPattern("^Trilinear");
	if (rx.search(arg) == 0) {
		interpolationMethod = assignment::TRILINEAR_INTERPOLATION;
		return SUCCESS;
	}
	rx.setPattern("^Tricubic");
	if (rx.search(arg) == 0) {
		interpolationMethod = assignment::TRICUBIC_INTERPOLATION;
		return SUCCESS;
	}
	rx.setPattern("^Nodes$");
	if (rx.search(arg) == 0) {
		assignmentMethod = assignment::NODE_ASSIGNMENT;
		return SUCCESS;
	}
	rx.setPattern("^Elements$");
	if (rx.search(arg) == 0) {
		assignmentMethod = assignment::ELEMENT_ASSIGNMENT;
		return SUCCESS;
	}
	rx.setPattern("^Extrude=(\\d+)$");
	if (rx.search(arg) == 0) {
		QString count = rx.cap(1);
		if (count.toInt() < 0) {
			return "Extrusion count must be >= 0";
		}
		extrusionCount = count.toInt();
		return SUCCESS;
	}
	rx.setPattern("^Test$");
	if (rx.search(arg) == 0) {
		testRun = true;
		return SUCCESS;
	}
	rx.setPattern("^AbaqusInput=(.+)");
	if (rx.search(arg) == 0) {
		abaqusInputFile = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^AnsysInput=(.+)");
	if (rx.search(arg) == 0) {
		ansysInputFolder = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^DicomInput=(.+)");
	if (rx.search(arg) == 0) {
		dicomInputFolder = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^MetaimageInput=(.+)");
	if (rx.search(arg) == 0) {
		metaimageInputFile = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^AnsysOutput=(.+)");
	if (rx.search(arg) == 0) {
		ansysOutputFile = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^AbaqusOutput=(.+)");
	if (rx.search(arg) == 0) {
		abaqusOutputFile = string(rx.cap(1));
		return SUCCESS;
	}
	rx.setPattern("^a=(\\d+.?\\d?)");
	if (rx.search(arg) == 0) {
		lawParameterA = rx.cap(1).toDouble();
		return SUCCESS;
	}
	rx.setPattern("^b=(\\d+.?\\d?)");
	if (rx.search(arg) == 0) {
		lawParameterB = rx.cap(1).toDouble();
		return SUCCESS;
	}

	string notValid = string(arg).append(" is not a valid parameter");
	return notValid;
}

void ParameterParser::setDefaultValues() {
	assignmentMethod = defaultAssignmentMethod;
	interpolationMethod = defaultInterpolationMethod;
	testRun = false;
	extrusionCount = defaultExtrusionCount;
	lawParameterA = defaultLawParameterA;
	lawParameterB = defaultLawParameterB;
}

int ParameterParser::getInterpolationMethod() {
	return interpolationMethod;
}

int ParameterParser::getAssignmentMethod() {
	return assignmentMethod;
}

int ParameterParser::getExtrusionCount() {
	return extrusionCount;
}

bool ParameterParser::getTestRun() {
	return testRun;
}

string ParameterParser::getAbaqusInputFile() {
	return abaqusInputFile;
}

string ParameterParser::getAnsysInputFolder() {
	return ansysInputFolder;
}

string ParameterParser::getDicomInputFolder() {
	return dicomInputFolder;
}

string ParameterParser::getMetaimageInputFile() {
	return metaimageInputFile;
}

string ParameterParser::getAnsysOutputFile() {
	return ansysOutputFile;
}

string ParameterParser::getAbaqusOutputFile() {
	return abaqusOutputFile;
}

double ParameterParser::getLawParameterA() {
	return lawParameterA;
}

double ParameterParser::getLawParameterB() {
	return lawParameterB;
}

ParameterParser::~ParameterParser() {

}

string ParameterParser::getHelpString() {
	string help = "*** FEM Property Assigner 1.0 ***\n";
	help.append("Parameters: \n\n");
	help.append("  Help\t\t\t\t\t-\tshow this help\n");
	help.append("\n** Mesh input (required)**\n");
	help.append("  AnsysInput=\"<folder>\"\t\t\t-\tuse the Ansys files in the specified <folder> as mesh data\n");
	help.append("  AbaqusInput=\"<file>\"\t\t\t-\tuse the specified Abaqus <file> as mesh data\n");
	help.append("\n** CT image input (required)**\n");
	help.append("  DicomInput=\"<folder>\"\t\t\t-\tuse the Dicom files in the specified <folder> as CT image data\n");
	help.append("  MetaimageInput=\"<file>\"\t\t-\tuse the specified Metaimage <file> as CT image data\n");
	help.append("\n** CT image extrusion (optional, default=");
		stringstream sse;
		sse.clear();
		sse << defaultExtrusionCount << ")**\n";
		help.append(sse.str());
	help.append("  ExtrusionCount=\"<count>\"\t\t-\textrude the CT image <count> times\n");
	help.append("\n** Assignment method (optional, default: ");
		if (defaultAssignmentMethod == assignment::NODE_ASSIGNMENT)
			help.append("Nodes");
		else
			help.append("Elements");
		stringstream ssa;
		ssa << ", a=" << defaultLawParameterA << " b=" << defaultLawParameterB << ")**\n";
		help.append(ssa.str());
	help.append("  Nodes\t\t\t\t\t-\tassign to the nodes\n");
	help.append("  Elements\t\t\t\t-\tassign to the elements\n");
	help.append("  a=<number>\t\t\t\t-\tUse the specified <number> for the first parameter of the assignment law\n");
	help.append("  b=<number>\t\t\t\t-\tUse the specified <number> for the second parameter of the assignment law\n");
	help.append("\n** Interpolation method (optional, default=");
		if (defaultInterpolationMethod == assignment::TRILINEAR_INTERPOLATION)
			help.append("Trilinear");
		else
			help.append("Tricubic");
		help.append(", tricubic is not recommended)**\n");
	help.append("  Trilinear\t\t\t\t-\tuse trilinear interpolation\n");
	help.append("  Tricubic\t\t\t\t-\tuse tricubic interpolation\n");
	help.append("\n** Output file (required)**\n");
	help.append("  AnsysOutput=\"<file>\"\t\t\t-\texport the result to <file> in Ansys format\n");
	help.append("  AbaqusOutput=\"<file>\"\t\t\t-\texport the result to <file> in Abaqus format\n");
	help.append("\n** Test run (all other parameters are ignored)**\n");
	help.append("  Test\t\t\t\t\t-\trun the test routine\n");
	return help;
}

}
