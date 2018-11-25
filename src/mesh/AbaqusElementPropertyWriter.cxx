#include "AbaqusElementPropertyWriter.h"

#include "ElementPropertyWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <vector>
#include "Mesh.h"

namespace mesh {

AbaqusElementPropertyWriter::AbaqusElementPropertyWriter(string file,
		assignment::ElementPropertyAssigner* assigner) :
	ElementPropertyWriter(file, assigner) {

}

void AbaqusElementPropertyWriter::write() {
	ofstream abaqusFile(this->file.c_str(), ios::out);
	if (!abaqusFile.good()) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "The Abaqus output file could not be opened for writing";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	map<int, Mesh::Element>* elements = assigner->getMesh()->getElements();
	vector<Mesh::Point>* nodes = assigner->getMesh()->getPointVector();

	// heading
	abaqusFile << "*Heading" << endl;
	abaqusFile << "**Abaqus input file" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "**PARTS" << endl;
	abaqusFile << "*Part, name=PART-1" << endl;

	// nodes
	abaqusFile << "**" << endl;
	abaqusFile << "*Node" << endl;
	for (int i = 0; i < nodes->size(); i++) {
		abaqusFile << i + 1 << ", " << nodes->at(i).x << ", " << nodes->at(i).y << ", "
				<< nodes->at(i).z << endl;
	}

	// elements
	int i=0;
	for (map<int, Mesh::Element>::iterator it = assigner->getMesh()->getElements()->begin(); it
				!= assigner->getMesh()->getElements()->end(); ++it) {
		abaqusFile << "*Element, type=" << "C3D10"
				<< ", elset=P_SET_" << i + 1 << endl;
		abaqusFile << (*it).first;
		Mesh::Element element = (*it).second;
		for (int a=0; a<element.triangleNodes.size(); a++) {
			abaqusFile << ", " << element.triangleNodes[a];
		}
		for (int a=0; a<element.intermediateNodes.size(); a++) {
			abaqusFile << ", " << element.intermediateNodes[a];
		}
		abaqusFile << endl;
		i++;
	}

	// part
	abaqusFile << "**" << endl;
	for (int i = 0; i < elements->size(); i++) {
		abaqusFile << "*Solid Section, elset=P_SET_" << i + 1
				<< ", material=MATRL_" << i + 1 << endl;
		abaqusFile << "1.," << endl;
	}
	abaqusFile << "*End Part" << endl;

	// assembly
	abaqusFile << "**" << endl;
	abaqusFile << "** ASSEMBLY" << endl;
	abaqusFile << "*Assembly, name=Assembly" << endl;
	abaqusFile << "*Instance, name=PART-1-1, part=PART-1" << endl;
	abaqusFile << "*End Instance" << endl;

	// materials
	abaqusFile << "**" << endl;
	abaqusFile << "** MATERIALS" << endl;
	for (int i = 0; i < elements->size(); i++) {
		abaqusFile << "**" << endl;
		abaqusFile << "*Material, name=MATRL_" << i + 1 << endl;
		abaqusFile << "*Elastic, Type=ISO" << endl;
		double value = assigner->getElementValues()->at(i);
		abaqusFile << value << ", " << 0.3 << ", " << value << endl;
	}

	// step
	abaqusFile
			<< "** ----------------------------------------------------------------"
			<< endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*INITIAL CONDITIONS, TYPE=temperature" << endl;
	abaqusFile << "** STEP" << endl;
	abaqusFile << "*Step, name=Step-1" << endl;
	abaqusFile << "*Static" << endl;
	abaqusFile << "1., 1., 1e-05, 1." << endl;

	// file end
	abaqusFile << "**" << endl;
	abaqusFile << "** OUTPUT REQUESTS" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Restart, write, frequency=0" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "** FIELD OUTPUT: F-Output-1" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Output, field, variable=PRESELECT" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "** HISTORY OUTPUT: H-Output-1" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Output, history, variable=PRESELECT" << endl;
	abaqusFile << "*End Step" << endl;

	abaqusFile.close();
}

}
