#include "AnsysElementPropertyWriter.h"

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <vector>
#include "Mesh.h"

namespace mesh {

AnsysElementPropertyWriter::AnsysElementPropertyWriter(string file,
		assignment::ElementPropertyAssigner* assigner) :
	ElementPropertyWriter(file, assigner) {

}

void AnsysElementPropertyWriter::write() {
	ofstream file(this->file.c_str(), ios::out);
	if (!file.good()) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "The Ansys output file could not be opened for writing";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	map<int, Mesh::Element>* elements = assigner->getMesh()->getElements();

	file << "/prep7\n";
	file << "allsel,all\n";
	file << "bfdele,all,all\n";

	int i=0;
	for (map<int, Mesh::Element>::iterator it = assigner->getMesh()->getElements()->begin(); it
			!= assigner->getMesh()->getElements()->end(); ++it) {
		int elementId = (*it).first;
		double value = assigner->getElementValues()->at(i);
		file << "bfe,el_" << elementId << ",temp,1," << value << endl;
		i++;
	}

	file.close();
}

}
