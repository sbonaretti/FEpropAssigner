#include "CTImageReader.h"

#include <string>

namespace ctimage {

using namespace std;

QSharedPointer<CTImage> CTImageReader::getCtImage() {
	return ctImage;
}

void CTImageReader::setMetadata() {
	ctImage->size[0]
			= ctImage->itkImage->GetLargestPossibleRegion().GetSize()[0];
	ctImage->size[1]
			= ctImage->itkImage->GetLargestPossibleRegion().GetSize()[1];
	ctImage->size[2]
			= ctImage->itkImage->GetLargestPossibleRegion().GetSize()[2];

	ctImage->spacing[0] = ctImage->itkImage->GetSpacing()[0];
	ctImage->spacing[1] = ctImage->itkImage->GetSpacing()[1];
	ctImage->spacing[2] = ctImage->itkImage->GetSpacing()[2];

	ctImage->origin[0] = ctImage->itkImage->GetOrigin()[0];
	ctImage->origin[1] = ctImage->itkImage->GetOrigin()[1];
	ctImage->origin[2] = ctImage->itkImage->GetOrigin()[2];

	ctImage->pixelData = new CTImage::CTPixelType[ctImage->getNsum()];
	int i = 0;
	for (int z = 0; z < ctImage->getNz(); z++) {
		for (int x = 0; x < ctImage->getNx(); x++) {
			for (int y = 0; y < ctImage->getNy(); y++) {
				CTImage::ItkImageType::IndexType index;
				index[0] = x;
				index[1] = y;
				index[2] = z;

				ctImage->pixelData[i] = ctImage->itkImage->GetPixel(index);
				i++;
			}
		}
	}
}

CTImageReader::~CTImageReader() {

}

}
