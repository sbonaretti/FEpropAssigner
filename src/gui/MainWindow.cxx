#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <sstream>
#include <exception>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageViewer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkContourFilter.h>
#include <vtkProperty.h>
#include <vtkGlyph3D.h>
#include <vtkPointData.h>
#include <vtkCubeSource.h>
#include <vtkMaskPoints.h>
#include <vtkLookupTable.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkLegendScaleActor.h>
#include <vtkAxisActor2D.h>

#include <qslider.h>
#include <qobject.h>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QDoubleValidator>

#include "MeshLoader.h"
#include "CTImage.h"
#include "AnsysMeshLoader.h"
#include "AbaqusMeshLoader.h"
#include "DicomCTImageReader.h"
#include "MetaimageCTImageReader.h"
#include "NodePropertyAssigner.h"
#include "VtkPolyDataCreator.h"
#include "itkImageToVTKImageFilter.h"
#include "AssignmentActor.h"
#include "Interpolator3D.h"
#include "NodePropertyAssigner.h"
#include "AbaqusNodePropertyWriter.h"
#include "AbaqusElementPropertyWriter.h"
#include "AnsysNodePropertyWriter.h"
#include "AnsysElementPropertyWriter.h"
#include "PropertyWriter.h"
#include "NodePropertyWriter.h"

#include "VolumeCorrector.h"

namespace gui {

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent), ui(new Ui::MainWindow) {

	ui->setupUi(this); // load the ui file
	QErrorMessage::qtHandler(); // default handler for debug, warning and error messages

	connect(ui->extrusionSlider, SIGNAL(valueChanged(int)), this, SLOT(
			extrusionSliderChanged(int)));

	connect(ui->loadAnsysButton, SIGNAL(clicked()), this, SLOT(loadAnsys()));
	connect(ui->loadAbaqusButton, SIGNAL(clicked()), this, SLOT(loadAbaqus()));
	connect(ui->loadDicomButton, SIGNAL(clicked()), this, SLOT(loadDicom()));
	connect(ui->loadMetafileButton, SIGNAL(clicked()), this, SLOT(
			loadMetafile()));

	connect(ui->showFEMModel, SIGNAL(stateChanged(int)), this, SLOT(
			toggleFEMModel(int)));
	connect(ui->showCTImage, SIGNAL(stateChanged(int)), this, SLOT(
			toggleCTImage(int)));
	connect(ui->showExtrudedImage, SIGNAL(stateChanged(int)), this, SLOT(
			toggleExtrudedCTImage(int)));
	connect(ui->showAssignment, SIGNAL(stateChanged(int)), this, SLOT(
			toggleAssignment(int)));
	connect(ui->showScale, SIGNAL(stateChanged(int)), this, SLOT(toggleScale(
			int)));

	connect(ui->cubicInterpolation, SIGNAL(toggled(bool)), this, SLOT(toggleInterpolationMethod(bool)));
	connect(ui->nodeAssign, SIGNAL(toggled(bool)), this, SLOT(toggleAssignmentMethod(bool)));

	connect(ui->run, SIGNAL(clicked()), this, SLOT(run()));

	connect(ui->exportExtrusionButton, SIGNAL(clicked()), this, SLOT(
			exportExtrudedImage()));
	connect(ui->exportAnsysButton, SIGNAL(clicked()), this, SLOT(exportAnsys()));
	connect(ui->exportAbaqusButton, SIGNAL(clicked()), this, SLOT(
			exportAbaqus()));

	//setup window
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<
			vtkRenderWindow>::New();

	//setup renderer
	renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->SetBackground(1,1,1);
	renderWindow->AddRenderer(renderer);

	renderer->ResetCamera();

	this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

	this->scaleActor = vtkSmartPointer<vtkLegendScaleActor>::New();
	this->scaleActor->RightAxisVisibilityOff();
	this->scaleActor->TopAxisVisibilityOff();
	this->scaleActor->GetLeftAxis()->SetFontFactor(1.1);
	this->scaleActor->GetBottomAxis()->SetFontFactor(1.1);

	this->setProperty("windowTitle", "FEM Property Assigner 1.0");
	this->toggleInterpolationMethod(false);
	this->toggleAssignmentMethod(false);

	QDoubleValidator* doubleValidator = new QDoubleValidator(0, 10000.0, 8,
			this);
	this->ui->lawAField->setValidator(doubleValidator);
	this->ui->lawBField->setValidator(doubleValidator);

	clear();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::showActor(vtkSmartPointer<vtkProp> actor, QCheckBox* checkBox) {
	actor->VisibilityOn();
	checkBox->setChecked(true);
	this->ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::hideActor(vtkSmartPointer<vtkProp> actor, QCheckBox* checkBox) {
	actor->VisibilityOff();
	checkBox->setChecked(false);
	this->ui->qvtkWidget->GetRenderWindow()->Render();
}

void MainWindow::changeEvent(QEvent* e) {
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::extrusionSliderChanged(int value) {
	stringstream ss;
	ss << "Extrude " << value << " times";
	ui->extrusionLabel->setText(ss.str().c_str());
}

void MainWindow::loadAnsys() {
	QString folder = QFileDialog::getExistingDirectory(this, tr(
			"Open Directory with Ansys files"));
	if (!folder.isNull()) {
		try {
			AnsysMeshLoader meshLoader(folder.toStdString());
			this->mesh = meshLoader.getMesh();
			loadMesh();
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

void MainWindow::loadAbaqus() {
	QString file = QFileDialog::getOpenFileName(this, tr("Open Abaqus file"),
			"", "*.inp");
	if (!file.isNull()) {
		try {
			AbaqusMeshLoader meshLoader(file.toStdString());
			this->mesh = meshLoader.getMesh();
			loadMesh();
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

void MainWindow::loadMesh() {
	// is this the first loaded mesh/actor ?
	bool firstActor = false;
	if (renderer->HasViewProp(femModelActor)) {
		clear();
		firstActor = true;
	}

	vtkSmartPointer<vtkPolyData> polyData = VtkPolyDataCreator::getVtkPolyData(
			mesh);

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<
			vtkPolyDataMapper>::New();
	mapper->SetInput(polyData);
	mapper->SetScalarVisibility(true);

	renderer->RemoveActor(femModelActor);

	femModelActor = vtkSmartPointer<vtkActor>::New();
	femModelActor->SetMapper(mapper);

	femModelActor->GetProperty()->SetEdgeVisibility(true);
	femModelActor->GetProperty()->SetEdgeColor(0.0, 0.2, 0.0);
	//femModelActor->GetProperty()->SetColor(0.2, 1.0, 0.2);
	femModelActor->GetProperty()->SetColor(0.0, 0.8, 0.81);

	renderer->AddActor(femModelActor);

	this->ui->showFEMModel->setEnabled(true);
	this->ui->showScale->setEnabled(true);
	renderer->Clear();

	if (firstActor || !checkLoadComplete())
		renderer->ResetCamera();

	showActor(femModelActor, this->ui->showFEMModel);
}

void MainWindow::loadDicom() {
	QString folder = QFileDialog::getExistingDirectory(this, tr(
			"Open directory with Dicom files"));
	if (!folder.isNull()) {
		try {
			DicomCTImageReader dicomCTImageReader(folder.toStdString());
			this->ctImage = dicomCTImageReader.getCtImage();
			loadCtImage();
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

void MainWindow::loadMetafile() {
	QString file = QFileDialog::getOpenFileName(this, tr("Open Metaimage"), "",
			"Images (*.mhd)");
	if (!file.isNull()) {
		try {
			MetaimageCTImageReader metafileCTImageReader(file.toStdString());
			this->ctImage = metafileCTImageReader.getCtImage();
			loadCtImage();
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

void MainWindow::loadCtImage() {
	if (renderer->HasViewProp(ctActor))
		clear();

	typedef itk::ImageToVTKImageFilter<CTImage::ItkImageType> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New(); // itk to vtk connector
	connector->SetInput(ctImage->getItkImage());

	vtkSmartPointer<vtkContourFilter> skinExtractor = vtkSmartPointer<
			vtkContourFilter>::New();
	skinExtractor->SetInput(connector->GetOutput());
	skinExtractor->SetValue(0, 200);

	vtkSmartPointer<vtkPolyDataMapper> skinMapper = vtkSmartPointer<
			vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinExtractor->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	renderer->RemoveActor(ctActor);

	ctActor = vtkSmartPointer<vtkActor>::New();
	ctActor->SetMapper(skinMapper);
	ctActor->GetProperty()->SetColor(1.0, 0.8, 0.2);

	renderer->AddActor(ctActor);

	this->ui->showCTImage->setEnabled(true);
	this->ui->showScale->setEnabled(true);
	renderer->Clear();

	if (!checkLoadComplete())
		renderer->ResetCamera();

	showActor(ctActor, this->ui->showCTImage);
}

void MainWindow::showMessage(string msg) {
	QMessageBox box;
	box.setText(msg.c_str());
	box.exec();
}

void MainWindow::toggleFEMModel(int state) {
	if (state == Qt::Checked) {
		showActor(femModelActor, this->ui->showFEMModel);
	} else {
		hideActor(femModelActor, this->ui->showFEMModel);
	}
}

void MainWindow::toggleCTImage(int state) {
	if (state == Qt::Checked) {
		showActor(ctActor, this->ui->showCTImage);
	} else {
		hideActor(ctActor, this->ui->showCTImage);
	}
}

void MainWindow::toggleExtrudedCTImage(int state) {
	if (state == Qt::Checked) {
		showActor(extrudedCtActor, this->ui->showExtrudedImage);
	} else {
		hideActor(extrudedCtActor, this->ui->showExtrudedImage);
	}
}

void MainWindow::toggleAssignment(int state) {
	if (state == Qt::Checked) {
		showActor(assignmentActor, this->ui->showAssignment);
	} else {
		hideActor(assignmentActor, this->ui->showAssignment);
	}
}

void MainWindow::toggleAssignmentMethod(bool t) {
	if (ui->nodeAssign->isChecked()) {
		this->assignmentMethod = assignment::NODE_ASSIGNMENT;
	} else {
		this->assignmentMethod = assignment::ELEMENT_ASSIGNMENT;
	}
}

void MainWindow::toggleInterpolationMethod(bool t) {
	if (ui->cubicInterpolation->isChecked()) {
		this->interpolationMethod = assignment::TRICUBIC_INTERPOLATION;
	} else {
		this->interpolationMethod = assignment::TRILINEAR_INTERPOLATION;
	}
}

void MainWindow::toggleScale(int state) {
	if (state == Qt::Checked) {
		this->renderer->AddActor(scaleActor);
	} else {
		this->renderer->RemoveActor(scaleActor);
	}
	this->renderer->GetRenderWindow()->Render();
}

bool MainWindow::checkLoadComplete() {
	if (mesh != NULL && ctImage != NULL) {
		this->ui->run->setEnabled(true);
		return true;
	}
	return false;
}

void MainWindow::run() {
	QSharedPointer<CTImage> image;
	if (ui->extrusionSlider->value() > 0) {
		VolumeCorrector volumeCorrector(ctImage, mesh,
				ui->extrusionSlider->value());

		this->extrudedCtImage = volumeCorrector.getExtrudedCTImage();
		image = extrudedCtImage;
	} else {
		image = ctImage;
	}

	typedef itk::ImageToVTKImageFilter<CTImage::ItkImageType> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New(); // itk to vtk connector
	connector->SetInput(image->getItkImage());

	vtkSmartPointer<vtkContourFilter> skinExtractor = vtkSmartPointer<
			vtkContourFilter>::New();
	skinExtractor->SetInput(connector->GetOutput());
	skinExtractor->SetValue(0, 200);

	vtkSmartPointer<vtkPolyDataMapper> skinMapper = vtkSmartPointer<
			vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinExtractor->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	renderer->RemoveActor(extrudedCtActor);
	if (ui->extrusionSlider->value() > 0) {
		extrudedCtActor = vtkSmartPointer<vtkActor>::New();
		extrudedCtActor->SetMapper(skinMapper);
		extrudedCtActor->GetProperty()->SetColor(0.0, 0.1, 1.0);

		renderer->AddActor(extrudedCtActor);

		this->ui->showExtrudedImage->setEnabled(true);
		this->ui->exportExtrusionButton->setEnabled(true);
	} else {
		this->ui->showExtrudedImage->setEnabled(false);
		this->ui->exportExtrusionButton->setEnabled(false);
	}

	try {
		double a = this->ui->lawAField->text().toDouble();
		double b = this->ui->lawBField->text().toDouble();

		if (this->assignmentMethod == assignment::NODE_ASSIGNMENT) {
			assignment::NodePropertyAssigner* assigner =
					new assignment::NodePropertyAssigner(image, mesh,
							this->interpolationMethod, a, b);
			QSharedPointer<assignment::NodePropertyAssigner> qA(assigner);
			this->nodeAssigner = qA;
		} else {
			assignment::ElementPropertyAssigner* assigner =
					new assignment::ElementPropertyAssigner(image, mesh,
							this->interpolationMethod, a, b);
			QSharedPointer<assignment::ElementPropertyAssigner> qA(assigner);
			this->elementAssigner = qA;
			this->nodeAssigner = qA;
		}

		renderer->RemoveActor(assignmentActor);
		assignmentActor = AssignmentActor::New(this->nodeAssigner.data());
		renderer->AddActor(assignmentActor);
		this->ui->showAssignment->setEnabled(true);

		hideActor(ctActor, this->ui->showCTImage);
		hideActor(femModelActor, this->ui->showFEMModel);
		if (extrudedCtActor != NULL)
			hideActor(extrudedCtActor, this->ui->showExtrudedImage);
		showActor(assignmentActor, this->ui->showAssignment);

		this->ui->exportAnsysButton->setEnabled(true);
		this->ui->exportAbaqusButton->setEnabled(true);
	} catch (exception& e) {
		showMessage(e.what());
	}
}

void MainWindow::exportExtrudedImage() {
	QString folder = QFileDialog::getExistingDirectory(this,
			"Select directory for extruded image");
	if (!folder.isNull()) {
		extrudedCtImage->write(folder.toStdString());
	}
}

void MainWindow::clear() {
	renderer->RemoveAllViewProps();

	ui->showFEMModel->setEnabled(false);
	ui->showFEMModel->setChecked(false);
	ui->showScale->setEnabled(false);
	ui->showScale->setChecked(false);
	ui->showCTImage->setEnabled(false);
	ui->showCTImage->setChecked(false);
	ui->showAssignment->setEnabled(false);
	ui->showAssignment->setChecked(false);
	ui->exportAnsysButton->setEnabled(false);
	ui->exportAbaqusButton->setEnabled(false);

	ui->extrusionSlider->setValue(3);
	ui->run->setEnabled(false);

}

void MainWindow::exportAnsys() {
	QString file = QFileDialog::getSaveFileName(this);
	if (!file.isNull()) {

		try {
			if (this->assignmentMethod == assignment::NODE_ASSIGNMENT) {
				AnsysNodePropertyWriter writer(file.toStdString(),
						this->nodeAssigner.data());
				writer.write();
			} else {
				AnsysElementPropertyWriter writer(file.toStdString(),
						this->elementAssigner.data());
				writer.write();
			}
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

void MainWindow::exportAbaqus() {
	QString file = QFileDialog::getSaveFileName(this);
	if (!file.isNull()) {
		try {
			if (this->assignmentMethod == assignment::NODE_ASSIGNMENT) {
				AbaqusNodePropertyWriter writer(file.toStdString(),
						this->nodeAssigner.data());
				writer.write();
			} else {
				AbaqusElementPropertyWriter writer(file.toStdString(),
						this->elementAssigner.data());
				writer.write();
			}
		} catch (exception& e) {
			showMessage(e.what());
		}
	}
}

}
