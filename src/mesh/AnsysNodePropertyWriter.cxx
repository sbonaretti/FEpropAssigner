#include "AnsysNodePropertyWriter.h"

#include <iostream>
#include <fstream>
#include <exception>
#include "Mesh.h"

namespace mesh {

AnsysNodePropertyWriter::AnsysNodePropertyWriter(string file,
		NodePropertyAssigner* assigner) :
	NodePropertyWriter(file, assigner) {
}

void AnsysNodePropertyWriter::write() {
	ofstream file(this->file.c_str(), ios::out);
	if (!file.good()) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "The Ansys output file could not be opened for writing";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}
	file << "/prep7\n";
	file << "allsel,all\n";
	file << "bfdele,all,all\n";

	for (int i = 0; i < assigner->getMesh()->getNodeIds()->size(); i++) {
		int nodeNr = assigner->getMesh()->getNodeIds()->at(i);
		double value = assigner->getNodeValues()->at(i);
		file << "bf," << nodeNr << ",temp," << value << endl;
	}

	file.close();
}

}
