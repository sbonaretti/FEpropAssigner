#include "AbaqusNodePropertyWriter.h"

#include "NodePropertyWriter.h"

#include "ElementPropertyWriter.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <vector>
#include "Mesh.h"

namespace mesh {

AbaqusNodePropertyWriter::AbaqusNodePropertyWriter(string file,
		NodePropertyAssigner* assigner) :
		NodePropertyWriter(file, assigner) {
}

void AbaqusNodePropertyWriter::write() {
	ofstream abaqusFile(this->file.c_str(), ios::out);
	if (!abaqusFile.good()) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "The Abaqus output file could not be opened for writing";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	vector<Mesh::Point>* nodes = assigner->getMesh()->getPointVector();
	map<int, Mesh::Element>* elements = assigner->getMesh()->getElements();

	// heading
	abaqusFile << "*Heading"  << endl;
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
	for (map<int, Mesh::Element>::iterator it = elements->begin(); it
				!= elements->end(); ++it) {
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
	abaqusFile << "*Elset, elset=_PickedSet4, internal" << endl;
	for (int i=0; i<elements->size(); i++){
		if (i == elements->size()-1)
			abaqusFile << elements->size() << endl;
		else
			abaqusFile << i+1 << ", ";
		if (i%10 == 0)
			abaqusFile << endl;

	}
	abaqusFile << "** Section: Section-1" << endl;
	abaqusFile << "*Solid Section, elset=_PickedSet4, material=Material-1" << endl;
	abaqusFile << "1.," << endl;
	abaqusFile << "*End Part" << endl;

	// assembly
	abaqusFile << "**" << endl;
	abaqusFile << "** ASSEMBLY" << endl;
	abaqusFile << "*Assembly, name=Assembly" << endl;
	abaqusFile << "*Instance, name=PART-1-1, part=PART-1" << endl;
	abaqusFile << "*End Instance" << endl;
	abaqusFile << "**" << endl;

	abaqusFile << "*End Assembly" << endl;

	// materials
	abaqusFile << "**" << endl;
	abaqusFile << "** MATERIALS" << endl;
	abaqusFile << "*Material, name=Material-1" << endl;
	abaqusFile << "*Elastic" << endl;
	abaqusFile << assigner->getMinValue() << ", " << 0.3 << ", " <<  assigner->getMinValue() << endl;
	abaqusFile << assigner->getMinValue() << ", " << 0.3 << ", " <<  assigner->getMaxValue() << endl;
	abaqusFile << "** ----------------------------------------------------------------" << endl;
	abaqusFile << "*INITIAL CONDITION, TYPE=TEMPERATURE" << endl;

	for (int i = 0; i < assigner->getMesh()->getNodeIds()->size(); i++) {
		int nodeNr = assigner->getMesh()->getNodeIds()->at(i);
		double value = assigner->getNodeValues()->at(i);

		abaqusFile << "PART-1-1." << nodeNr << ", " << value << endl;
	}
	abaqusFile << "**" << endl;
	abaqusFile << "** STEP: Step-1" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Step, name=Step-1" << endl;
	abaqusFile << "*Static" << endl;
	abaqusFile << "1., 1., 1e-05, 1." << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "** FIELD OUTPUT: F-Output-1" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Output, field" << endl;
	abaqusFile << "*Node Output" << endl;
	abaqusFile << "CF, NT, RF, U" << endl;
	abaqusFile << "*Element Output, directions=YES" << endl;
	abaqusFile << "LE, PE, PEEQ, PEMAG, S" << endl;
	abaqusFile << "*Contact Output" << endl;
	abaqusFile << "CDISP, CSTRESS" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "** HISTORY OUTPUT: H-Output-1" << endl;
	abaqusFile << "**" << endl;
	abaqusFile << "*Output, history, variable=PRESELECT" << endl;
	abaqusFile << "*End Step" << endl;

	// close file
	abaqusFile.close();
}

}

