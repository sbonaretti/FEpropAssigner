#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkLegendScaleActor.h>
#include <vtkPropAssembly.h>
#include <string>

#include <QMainWindow>
#include <QCheckBox>

#include "Mesh.h"
#include "CTImage.h"

namespace assignment {
	class NodePropertyAssigner;
	class ElementPropertyAssigner;
}

namespace Ui {
class MainWindow;
}

/**
 * Contains classes for the graphical user interface mode of the application
 */
namespace gui {

using namespace std;
using namespace ctimage;
using namespace mesh;

/**
 * This class provides the main application window
 */
class MainWindow: public QMainWindow {
Q_OBJECT
public:
	/**
	 * Creates a new GUI window
	 * @note The form is loaded from the QT form file mainwindow.ui
	 */
	MainWindow(QWidget* parent = 0);
	/**
	 * Displays a specified actor and enables/checks it's corresponding checkbox
	 */
	void showActor(vtkSmartPointer<vtkProp> , QCheckBox*);
	/**
	 * Hides a specified actor and disables/unchecks it's corresponding checkbox
	 */
	void hideActor(vtkSmartPointer<vtkProp> , QCheckBox*);
	/**
	 * Sets the window in its initial state
	 */
	void clear();
	~MainWindow();
protected:
	void changeEvent(QEvent *e);
private:
	/** The renderer */
	vtkSmartPointer<vtkRenderer> renderer;
	/** The FEM model (mesh) actor */
	vtkSmartPointer<vtkActor> femModelActor;
	/** The CT image model / isosurface  actor*/
	vtkSmartPointer<vtkActor> ctActor;
	/** The extruded CT image model / isosurface actor */
	vtkSmartPointer<vtkActor> extrudedCtActor;
	/** The assignment result actor */
	vtkSmartPointer<vtkPropAssembly> assignmentActor;
	/** The scale actor */
	vtkSmartPointer<vtkLegendScaleActor> scaleActor;
	/** The QT form interface */
	Ui::MainWindow *ui;
	/** The loaded mesh::Mesh */
	QSharedPointer<Mesh> mesh;
	/** The loaded ctimage::CTImage */
	QSharedPointer<CTImage> ctImage;
	/** The extruded ctimage::CTImage */
	QSharedPointer<CTImage> extrudedCtImage;
	/**
	 * The interpolation method
	 */
	int interpolationMethod;
	/**
	 * The assignment method
	 */
	int assignmentMethod;
	/** The node assigner */
	QSharedPointer<assignment::NodePropertyAssigner> nodeAssigner;
	/** The element assigner */
	QSharedPointer<assignment::ElementPropertyAssigner> elementAssigner;

	/** Loads the mesh into the GUI */
	void loadMesh();
	/** Loads the CT image into the GUI */
	void loadCtImage();
	/**
	 * Displays a message to the user
	 * @param The message to be displayed
	 */
	void showMessage(std::string msg);
	/**
	 * Checks if both the mesh::Mesh and ctimage::CTImage are loaded
	 * @return True if both the mesh::Mesh and ctimage::CTImage are loaded
	 */
	bool checkLoadComplete();

private slots:
	/**
	 * FEM model checkbox slot
	 * @param state Qt::Checked if the checkbox is in checked state
	 */
	void toggleFEMModel(int state);
	/**
	 * CT image checkbox slot
	 * @param state Qt::Checked if the checkbox is in checked state
	 */
	void toggleCTImage(int state);
	/**
	 * Extruded CT image checkbox slot
	 * @param state Qt::Checked if the checkbox is in checked state
	 */
	void toggleExtrudedCTImage(int state);
	/**
	 * Assignment result checkbox slot
	 * @param state Qt::Checked if the checkbox is in checked state
	 */
	void toggleAssignment(int state);
	/**
	 * Scale checkbox slot
	 * @param state Qt::Checked if the checkbox is in checked state
	 */
	void toggleScale(int state);
	/**
	 * Interpolation method switch slot
	 * @param t
	 */
	void toggleInterpolationMethod(bool t);
	/**
	 * Assignment method switch slot
	 * @param t
	 */
	void toggleAssignmentMethod(bool t);
	/**
	 * Ansys load button slot
	 */
	void loadAnsys();
	/**
	 * Abaqus load button slot
	 */
	void loadAbaqus();
	/**
	 * Dicom load button slot
	 */
	void loadDicom();
	/**
	 * Metaimage load button slot
	 */
	void loadMetafile();
	/**
	 * Extrusion slider slot
	 * @param The slider's value
	 */
	void extrusionSliderChanged(int value);
	/**
	 * Dicom export slot
	 */
	void exportExtrudedImage();
	/**
	 * Ansys export slot
	 */
	void exportAnsys();
	/**
	 * Abaqus export slot
	 */
	void exportAbaqus();
	/**
	 * Run button slot
	 */
	void run();
};

}

#endif /* MAINWINDOW_H_ */

