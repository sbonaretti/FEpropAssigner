/**
 * @mainpage FEM Property Assigner
 * @version 1.0
 * @author Andreas Siegrist
 * @date 2010
 *
 * FEM Property Assigner is a program that assigns tissue properties of CT images to volumetric meshes
 */

#include <QApplication>
#include <iostream>
#include <string>

#include <Logger.h>
#include <MainWindow.h>
#include "Runner.h"

#include <QFile>
#include <QString>
#include <QTextStream>

using namespace std;

int main(int argc, char** argv) {
	bool guiMode = true;
	for (int i=0; i<argc; i++) {
		QString s(argv[i]);
		QString a("noGui");
		if (s.contains(a, Qt::CaseInsensitive)) {
			guiMode = false;
			break;
		}
	}
	if (guiMode) {
		QString jo("/test.txt");
		QFile file(jo);
		file.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream abaqusFile(&file);

		abaqusFile << argv[0] << endl;
		abaqusFile << argv[1] << endl;
		file.close();


		// GUI mode
		QApplication app(argc, argv);

		gui::MainWindow w;
		w.show();

		app.exec();
	} else {
		// Command line mode
		command::Runner runner(argc, argv);
		runner.run();
	}

	return EXIT_SUCCESS;
}

