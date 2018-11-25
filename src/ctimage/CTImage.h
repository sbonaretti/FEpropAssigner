#ifndef CTIMAGE_H_
#define CTIMAGE_H_

#include "itkImage.h"

#include "CTImage.h"
#include <string>
#include <QSharedPointer>

/**
 * Contains classes for the processing of CT images
 */
namespace ctimage {

using namespace std;

class CTImage {
public:
	friend class DicomCTImageReader;
	friend class MetaimageCTImageReader;
	friend class CTImageReader;

	/**
	 * The type of a CT pixel value
	 */
	typedef signed short CTPixelType;
	/**
	 * The dimension of the CT image
	 */
	static const int Dimension = 3;
	/**
	 * The type of an itk::Image
	 */
	typedef itk::Image<CTPixelType, Dimension> ItkImageType;
protected:
	/** Pixel data array */
	CTPixelType* pixelData;
	/**
	 * The dimension in all (xyz) directions
	 */
	int size[Dimension];
	/**
	 * The spacing between two pixels in all directions
	 */
	double spacing[Dimension];
	/**
	 * The position of the first pixel of each direction
	 */
	double origin[Dimension];
	/**
	 * The itk::itkImage
	 */
	ItkImageType::Pointer itkImage;
public:
	CTImage();
	virtual ~CTImage();
	/**
	 * @return The CT image's pixels
	 */
	CTPixelType* getPixelData();
	/**
	 * @return Number of pixels in x direction
	 */
	int getNx();
	/**
	 * @return Number of pixels in y direction
	 */
	int getNy();
	/**
	 * @return Number of pixels in z direction
	 */
	int getNz();
	/**
	 * @return Number of pixels in total
	 */
	int getNsum();
	/**
	 * @return Spacing between pixels in x direction
	 */
	double getSx();
	/**
	 * @return Spacing between pixels in y direction
	 */
	double getSy();
	/**
	 * @return Spacing between pixels in z direction
	 */
	double getSz();
	/**
	 * @return The x-coordinate of the first pixel
	 */
	double getX0();
	/**
	 * @return The y-coordinate of the first pixel
	 */
	double getY0();
	/**
	 * @return The z-coordinate of the first pixel
	 */
	double getZ0();
	/**
	 * @return The itk::itkImage
	 */
	ItkImageType::Pointer getItkImage();
	/**
	 * Creates a full copy of the CT image
	 * @return Pointer to the duplicated ctimage::CTImage
	 */
	QSharedPointer<CTImage> createDeepCopy();
	/**
	 * Writes the image in DICOM format to the disk
	 * @param The destination folder
	 */
	void write(string);
};

}

#endif /* CTIMAGE_H_ */
