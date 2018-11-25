#ifndef CTIMAGEREADER_H_
#define CTIMAGEREADER_H_

#include <string>
#include <memory>

#include <qsharedpointer.h>

#include "CTImage.h"

#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkImageData.h"

namespace ctimage {

using namespace std;

/**
 * Reads a CT Image from disk
 */
class CTImageReader {
protected:
	/**
	 * The image which was read
	 */
	QSharedPointer<CTImage> ctImage;
	/**
	 * Sets up ctImage's members holding the morphologic information of the image
	 */
	void setMetadata();
public:
	/**
	 * @return The image which was read
	 */
	QSharedPointer<CTImage> getCtImage();
	virtual ~CTImageReader();
};

}

#endif /* CTIMAGEREADER_H_ */
