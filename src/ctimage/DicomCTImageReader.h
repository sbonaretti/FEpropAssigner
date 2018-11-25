#ifndef DICOMCTIMAGEREADER_H_
#define DICOMCTIMAGEREADER_H_

#include <string>

#include "CTImageReader.h"

namespace ctimage {

/**
 * Reads a image stack in DICOM format
 */
class DicomCTImageReader: public CTImageReader {
public:
	/**
	 * Creates a new DICOM reader and reads the image
	 * @param The folder containing the DICOM stack
	 */
	DicomCTImageReader(string);
	virtual ~DicomCTImageReader();
};

}

#endif /* DICOMCTIMAGEREADER_H_ */
