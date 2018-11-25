#include "CTImage.h"

#include <string>

#include <QSharedPointer>

#include "itkImageDuplicator.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"

namespace ctimage {

using namespace std;

CTImage::CTImage() {

}

CTImage::~CTImage() {
	delete[] pixelData;
}

CTImage::CTPixelType* CTImage::getPixelData() {
	return pixelData;
}

CTImage::ItkImageType::Pointer CTImage::getItkImage() {
	return itkImage;
}

int CTImage::getNx() {
	return size[0];
}

int CTImage::getNy() {
	return size[1];
}

int CTImage::getNz() {
	return size[2];
}

int CTImage::getNsum() {
	return getNx() * getNy() * getNz();
}

double CTImage::getSx() {
	return spacing[0];
}

double CTImage::getSy() {
	return spacing[1];
}

double CTImage::getSz() {
	return spacing[2];
}

double CTImage::getX0() {
	return origin[0];
}

double CTImage::getY0() {
	return origin[1];
}

double CTImage::getZ0() {
	return origin[2];
}

QSharedPointer<CTImage> CTImage::createDeepCopy() {
	QSharedPointer<CTImage> imageCopy(new CTImage());

	typedef itk::ImageDuplicator<ItkImageType> DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();

	duplicator->SetInputImage(getItkImage());
	duplicator->Update();

	imageCopy->itkImage = duplicator->GetOutput();
	imageCopy->pixelData = new CTPixelType[getNsum()];

	for (int i = 0; i < getNsum(); i++) {
		imageCopy->pixelData[i] = pixelData[i];
	}

	for (int i = 0; i < Dimension; i++) {
		imageCopy->size[i] = size[i];
		imageCopy->spacing[i] = spacing[i];
		imageCopy->origin[i] = origin[i];
	}

	return imageCopy;
}

void CTImage::write(string directory) {
	typedef itk::GDCMImageIO ImageIOType;
	typedef itk::GDCMSeriesFileNames NamesGeneratorType;

	typedef unsigned short OutputPixelType;
	const unsigned OutputDimension = 2;
	typedef itk::Image<OutputPixelType, OutputDimension> Image2DType;
	typedef itk::ImageSeriesWriter<ItkImageType, Image2DType> SeriesWriterType;

	ImageIOType::Pointer gdcmIO = ImageIOType::New();

	SeriesWriterType::Pointer writer = SeriesWriterType::New();
	writer->SetInput(itkImage);
	writer->SetImageIO(gdcmIO);

	//NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
	//namesGenerator->SetUseSeriesDetails(true);
	//namesGenerator->SetOutputDirectory(directory + );

	typedef itk::NumericSeriesFileNames NameGeneratorType;
	NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
	nameGenerator->SetSeriesFormat(directory + "/extrusion%d.dcm");
	nameGenerator->SetStartIndex(1);
	nameGenerator->SetEndIndex(getNz());
	nameGenerator->SetIncrementIndex(1);

	const SeriesWriterType::FileNamesContainer & filenames =
			nameGenerator->GetFileNames();

	writer->SetFileNames(filenames);
	writer->SetMetaDataDictionary(itkImage->GetMetaDataDictionary());

	writer->Update();
}

}
