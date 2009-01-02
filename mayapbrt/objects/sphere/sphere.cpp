/*
 *  sphere.cpp
 *  mayapbrt
 *
 *  Created by Martin Adam on 29/10/08.
 *  Czech Technical University
 *
 */

#include "sphere.h"

#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>


using namespace std;

namespace pbrt {

	Sphere::Sphere(const MFnDependencyNode &_node) 
	{}

	Sphere::~Sphere() 
	{}

	MStatus Sphere::TranslationMatrix(ostream& fout) const {
		MStatus status;		
		MObject	transformNode = dagPath.transform(&status);
		if (!status && status.statusCode () == MStatus::kInvalidParameter) 
			return MStatus::kFailure;
		
		MFnDagNode transform(transformNode, &status);
		if (!status) 
			return MStatus::kFailure;
		
		MTransformationMatrix matrix (transform.transformationMatrix());
		
		MVector translation = matrix.translation(MSpace::kWorld);
		fout << "Translate " <<  translation.x << " " << translation.y << " " << translation.z << endl;
		
		MEulerRotation rotation = matrix.eulerRotation().reorder(MEulerRotation::kXYZ);
		fout << "Rotation " << rotation.x << " 1 0 0" << endl;
		fout << "Rotation " << rotation.y << " 0 1 0" << endl;
		fout << "Rotation " << rotation.z << " 0 0 1" << endl;
		
		double scale[3];
		matrix.getScale(scale, MSpace::kWorld);
		fout << "Scale " << scale[0] << " " << scale[1] << " " << scale[2] << endl;

		return MStatus::kSuccess;
	}
}
