#include "VolumeCorrector.h"

#include <vector>
#include <cfloat>

#include "Logger.h"
#include "CTImage.h"
#include "Mesh.h"

#include "vtkCell.h"

namespace ctimage {

int compare(const void *a, const void *b) {
	return *((double *) a) > *((double *) b) ? 1 : *((double *) a)
			< *((double *) b) ? -1 : 0;
}

VolumeCorrector::VolumeCorrector(QSharedPointer<CTImage> ctImage,
		QSharedPointer<mesh::Mesh> mesh, int extrusionCount) {
	this->mesh = mesh;
	this->extrudedCTImage = ctImage->createDeepCopy();
	this->extrusionCount = extrusionCount;

	extrude();
}

void VolumeCorrector::extrude() {
	QSharedPointer<CTImage> extrudedCTImage(this->extrudedCTImage);
	bool* ctBool = new bool[extrudedCTImage->getNsum()];
	bool* ctSlim = new bool[extrudedCTImage->getNsum()];
	bool* ctPeeled = new bool[extrudedCTImage->getNsum()];

	VolumeCorrector::inTetrahedron(ctBool);

	VolumeCorrector::chopSurface(ctBool, ctSlim, ctPeeled);

	CTImage::CTPixelType* ctExtended =
			new CTImage::CTPixelType[extrudedCTImage->getNsum()];

	VolumeCorrector::extendSurface(extrudedCTImage->getPixelData(), ctSlim,
			ctExtended, ctBool);
	int sum = 0;
	for (int i = 0; i < extrudedCTImage->getNsum(); i++) {
		if (ctPeeled[i] != 0) {
			if (extrudedCTImage->getPixelData()[i] > ctExtended[i]) {
				ctSlim[i] = 1;
				sum++;
			}
		}
	}

	VolumeCorrector::extendSurface(extrudedCTImage->getPixelData(), ctSlim,
			ctExtended, ctBool);

	bool* newCtBool = new bool[extrudedCTImage->getNsum()];
	CTImage::CTPixelType* newCtExtended =
			new CTImage::CTPixelType[extrudedCTImage->getNsum()];

	for (int i = 1; i < extrusionCount; i++) {
		VolumeCorrector::extendSurface(ctExtended, ctBool, newCtExtended,
				newCtBool);

		bool* tempCtBool = ctBool;
		CTImage::CTPixelType* tempCtExtended = ctExtended;

		ctExtended = newCtExtended;
		ctBool = newCtBool;

		newCtBool = tempCtBool;
		newCtExtended = tempCtExtended;
	}
	int i = 0;
	for (int z = 0; z < extrudedCTImage->getNz(); z++) {
		for (int x = 0; x < extrudedCTImage->getNx(); x++) {
			for (int y = 0; y < extrudedCTImage->getNy(); y++) {
				CTImage::ItkImageType::IndexType index;
				index[0] = x;
				index[1] = y;
				index[2] = z;
				extrudedCTImage->getPixelData()[i] = ctExtended[i];
				extrudedCTImage->getItkImage()->SetPixel(index, ctExtended[i]); // visualize extended CT

				i++;
			}
		}
	}

	delete[] ctBool;
	delete[] ctSlim;
	delete[] ctPeeled;
	delete[] newCtBool;
	delete[] newCtExtended;
}

QSharedPointer<CTImage> VolumeCorrector::getExtrudedCTImage() {
	return extrudedCTImage;
}

void VolumeCorrector::inTetrahedron(bool* ctBool) {
	for (int i = 0; i < extrudedCTImage->getNsum(); i++) {
		ctBool[i] = 0;
	}
	int i, j, k, npg, ncr, ypos, a, b, c, v[3];
	double s[3];
	int nt = mesh->getTriangleVector()->size();
	int nx = extrudedCTImage->getNx();
	int ny = extrudedCTImage->getNy();
	int nz = extrudedCTImage->getNz();

	vector<mesh::Mesh::Triangle>* triangleVector = mesh->getTriangleVector();
	vector<mesh::Mesh::Point>* pointVector = mesh->getPointVector();

	double* cr = new double[nt];
	Line* pg = new Line[nt];

	// Loop through planes
	for (i = 0; i < nz; i++) {
		npg = 0;
		for (j = 0; j < nt; j++) {
			a = (*triangleVector)[j].a - 1;
			b = (*triangleVector)[j].b - 1;
			c = (*triangleVector)[j].c - 1;

			// Check if triangle is in plane.
			if ((*pointVector)[a].z <= (*pointVector)[b].z
					&& (*pointVector)[a].z <= (*pointVector)[c].z) {
				v[0] = a;
				if ((*pointVector)[b].z <= (*pointVector)[c].z) {
					v[1] = b;
					v[2] = c;
				} else {
					v[1] = c;
					v[2] = b;
				}
			} else if ((*pointVector)[b].z <= (*pointVector)[a].z
					&& (*pointVector)[b].z <= (*pointVector)[c].z) {
				v[0] = b;
				if ((*pointVector)[a].z <= (*pointVector)[c].z) {
					v[1] = a;
					v[2] = c;
				} else {
					v[1] = c;
					v[2] = a;
				}
			} else {
				v[0] = c;
				if ((*pointVector)[a].z <= (*pointVector)[b].z) {
					v[1] = a;
					v[2] = b;
				} else {
					v[1] = b;
					v[2] = a;
				}
			}

			double zCoord = extrudedCTImage->getZ0() + i
					* extrudedCTImage->getSz();
			if ((*pointVector)[v[0]].z < zCoord - DBL_EPSILON
					&& (*pointVector)[v[2]].z >= zCoord) {
				s[0] = (zCoord - (*pointVector)[b].z) / ((*pointVector)[c].z
						- (*pointVector)[b].z);
				s[1] = (zCoord - (*pointVector)[a].z) / ((*pointVector)[c].z
						- (*pointVector)[a].z);
				s[2] = (zCoord - (*pointVector)[a].z) / ((*pointVector)[b].z
						- (*pointVector)[a].z);

				if (s[0] >= 0.0 && s[0] <= 1.0)
					if (s[1] >= 0.0 && s[1] <= 1.0) {
						pg[npg].a.x = (1 - s[0]) * (*pointVector)[b].x + s[0]
								* (*pointVector)[c].x;
						pg[npg].a.y = (1 - s[0]) * (*pointVector)[b].y + s[0]
								* (*pointVector)[c].y;
						pg[npg].b.x = (1 - s[1]) * (*pointVector)[a].x + s[1]
								* (*pointVector)[c].x;
						pg[npg].b.y = (1 - s[1]) * (*pointVector)[a].y + s[1]
								* (*pointVector)[c].y;
						npg++;
					} else {
						pg[npg].a.x = (1 - s[0]) * (*pointVector)[b].x + s[0]
								* (*pointVector)[c].x;
						pg[npg].a.y = (1 - s[0]) * (*pointVector)[b].y + s[0]
								* (*pointVector)[c].y;
						pg[npg].b.x = (1 - s[2]) * (*pointVector)[a].x + s[2]
								* (*pointVector)[b].x;
						pg[npg].b.y = (1 - s[2]) * (*pointVector)[a].y + s[2]
								* (*pointVector)[b].y;
						npg++;
					}
				else if (s[1] >= 0.0 && s[1] <= 1.0) {
					pg[npg].a.x = (1 - s[1]) * (*pointVector)[a].x + s[1]
							* (*pointVector)[c].x;
					pg[npg].a.y = (1 - s[1]) * (*pointVector)[a].y + s[1]
							* (*pointVector)[c].y;
					pg[npg].b.x = (1 - s[2]) * (*pointVector)[a].x + s[2]
							* (*pointVector)[b].x;
					pg[npg].b.y = (1 - s[2]) * (*pointVector)[a].y + s[2]
							* (*pointVector)[b].y;
					npg++;
				}
			}
		}

		for (j = 0; j < nx; j++) {
			ncr = 0;
			for (k = 0; k < npg; k++) {
				double xCoord = extrudedCTImage->getX0() + j
						* extrudedCTImage->getSx();
				s[0] = (xCoord - pg[k].a.x) / (pg[k].b.x - pg[k].a.x);
				if (s[0] >= -DBL_EPSILON && s[0] <= 1.0 + DBL_EPSILON) {

					// Check thoroughly if segment is counted.
					if (pg[k].a.x > pg[k].b.x) {
						if (pg[k].b.x < xCoord - 2.0 * DBL_EPSILON) {
							cr[ncr] = (1.0 - s[0]) * pg[k].a.y + s[0]
									* pg[k].b.y;
							ncr++;
						}
					} else if (pg[k].a.x < pg[k].b.x) {
						if (pg[k].a.x < xCoord - 2.0 * DBL_EPSILON) {
							cr[ncr] = (1.0 - s[0]) * pg[k].a.y + s[0]
									* pg[k].b.y;
							ncr++;
						}
					}
				}
			}
			qsort(cr, ncr, sizeof(double), compare);

			if (ncr % 2) {
				free(pg);
				free(cr);
			} else {
				for (k = 0; k < (ncr / 2); k++) {
					ypos = 0;

					while (cr[2 * k] > (extrudedCTImage->getY0() + ypos
							* extrudedCTImage->getSy())) {
						ypos++;
					}
					while (cr[2 * k + 1] > (extrudedCTImage->getY0() + ypos
							* extrudedCTImage->getSy())) {
						ctBool[ypos + j * ny + i * ny * nx] = true;
						ypos++;
					}
				}
			}
		}
	}

	delete cr;
	delete pg;
}

void VolumeCorrector::chopSurface(bool *ctBool, bool *ctSlim, bool *ctPeeled) {
	int nx = extrudedCTImage->getNx();
	int ny = extrudedCTImage->getNy();
	int nz = extrudedCTImage->getNz();

	for (int i = 0; i < nx * ny * nz; i++) {
		ctPeeled[i] = 0;
		ctSlim[i] = 0;
	}
	int ix, iy, iz, i, j, sum;

	for (iz = 0; iz < nz; iz++)
		for (ix = 1; ix < nx - 1; ix++)
			for (iy = 1; iy < ny - 1; iy++) {
				sum = 0;
				for (i = -1; i < 2; i++)
					for (j = -1; j < 2; j++)
						sum += ctBool[iy + j + ny * (ix + i + iz * nx)];
				if (sum == 9)
					ctSlim[iy + ny * (ix + iz * nx)] = true;
				else {
					ctSlim[iy + ny * (ix + iz * nx)] = false;
					if (ctBool[iy + ny * (ix + iz * nx)])
						ctPeeled[iy + ny * (ix + iz * nx)] = true;
					else
						ctPeeled[iy + ny * (ix + iz * nx)] = false;
				}
			}
}

void VolumeCorrector::extendSurface(CTImage::CTPixelType* E, bool* c,
		CTImage::CTPixelType* Ex, bool* cx) {
	int nx = extrudedCTImage->getNx();
	int ny = extrudedCTImage->getNy();
	int nz = extrudedCTImage->getNz();
	int ix, iy, iz, i, j;
	float R[9], s, t;

	for (i = 0; i < nx * ny * nz; i++) {
		Ex[i] = 0;
		cx[i] = 0;
	}

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			R[i + j * 3] = (i != 1 ? (j != 1 ? 1.0 / sqrt(2.0) : 1)
					: (j != 1 ? 1 : 0));
		}

	for (iz = 0; iz < nz; iz++)
		for (ix = 0; ix < nx; ix++)
			for (iy = 0; iy < ny; iy++)
				if (!c[iy + ny * (ix + iz * nx)])
					Ex[iy + ny * (ix + iz * nx)] = 0;
				else {
					cx[iy + ny * (ix + iz * nx)] = 1;
					Ex[iy + ny * (ix + iz * nx)] = E[iy + ny * (ix + iz * nx)];
				}

	for (iz = 0; iz < nz; iz++)
		for (ix = 1; ix < nx - 1; ix++)
			for (iy = 1; iy < ny - 1; iy++)
				if (!c[iy + ny * (ix + iz * nx)]) {
					s = 0;
					for (i = -1; i < 2; i++)
						for (j = -1; j < 2; j++)
							s += R[i + 1 + (j + 1) * 3] * c[iy + j + ny * (ix
									+ i + iz * nx)];
					if (s) {
						t = 0;
						for (i = -1; i <= 1; i++)
							for (j = -1; j <= 1; j++)
								t += R[i + 1 + (j + 1) * 3] * E[iy + j + ny
										* (ix + i + iz * nx)] * c[iy + j + ny
										* (ix + i + iz * nx)];
						Ex[iy + ny * (ix + iz * nx)] = t / s;
						cx[iy + ny * (ix + iz * nx)] = 1;
					}
				}
}

VolumeCorrector::~VolumeCorrector() {
	// TODO Auto-generated destructor stub
}

}
