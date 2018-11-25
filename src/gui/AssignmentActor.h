#ifndef ASSIGNMENTACTOR_H_
#define ASSIGNMENTACTOR_H_

#include <vtkActor.h>
#include <vtkPropAssembly.h>
#include "NodePropertyAssigner.h"

namespace gui {

/**
 * An vtkPropAssembly containing an actor with a color map visualisation of a assignment::NodePropertyAssigner result and a vtkScalarBarActor with a corresponding color scale
 */
class AssignmentActor: public vtkPropAssembly {
public:
	/**
	 * Returns a new instance
	 */
	static AssignmentActor* New(assignment::NodePropertyAssigner*);
	virtual ~AssignmentActor();
private:
	AssignmentActor();
};

}

#endif /* ASSIGNMENTACTOR_H_ */
