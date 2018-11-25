#include "MetaimageCTImageReader.h"

#include "CTImage.h"
#include <itkImageFileReader.h>
#include <itkExceptionObject.h>

#include <QSharedPointer>

#include <exception>

namespace ctimage {

MetaimageCTImageReader::MetaimageCTImageReader(string filename) {
	typedef itk::ImageFileReader<CTImage::ItkImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();

	reader->SetFileName(filename);

	try {
		reader->Update();
	} catch (itk::ExceptionObject &excp) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "The specified Metaimage file is invalid";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	//auto_ptr<CTImage> i (new CTImage());
	QSharedPointer<CTImage> qpointer(new CTImage());
	this->ctImage = qpointer;
	ctImage->itkImage = reader->GetOutput();

	setMetadata();
}

MetaimageCTImageReader::~MetaimageCTImageReader() {
}

}
