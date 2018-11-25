#ifndef METAFILECTIMAGEREADER_H_
#define METAFILECTIMAGEREADER_H_

#include <string>

#include "CTImageReader.h"

namespace ctimage {

/**
 * Reads a CT image in Metaimage fomat from disk
 */
class MetaimageCTImageReader: public CTImageReader {
public:
	/**
	 * Creates a Metaimage reader and reads the specified file
	 * @param The Metaimage file location
	 */
	MetaimageCTImageReader(string);
	virtual ~MetaimageCTImageReader();
};

}

#endif /* METAFILECTIMAGEREADER_H_ */
