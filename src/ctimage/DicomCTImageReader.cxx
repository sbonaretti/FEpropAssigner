#include "DicomCTImageReader.h"

#include "CTImage.h"
#include "Logger.h"

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace ctimage {

DicomCTImageReader::DicomCTImageReader(string directory) {
	typedef itk::ImageSeriesReader<CTImage::ItkImageType> ReaderType;

	typedef itk::GDCMImageIO ImageIOType;
	typedef itk::GDCMSeriesFileNames NamesGeneratorType;

	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

	namesGenerator->SetInputDirectory(directory);

	const ReaderType::FileNamesContainer & filenames =
			namesGenerator->GetInputFileNames();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetImageIO(gdcmIO);
	reader->SetFileNames(filenames);

	try {
		reader->Update();
	} catch (itk::ExceptionObject &excp) {
		class FolderNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "Could not find valid dicom files in the specified folder";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	//auto_ptr<CTImage> i( new CTImage());
	QSharedPointer<CTImage> qpointer(new CTImage());
	this->ctImage = qpointer;

	ctImage->itkImage = reader->GetOutput();

	setMetadata();
}

DicomCTImageReader::~DicomCTImageReader() {
}

}
