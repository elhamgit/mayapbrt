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
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

using namespace std;

namespace pbrt {

	Sphere::Sphere(const MFnDependencyNode &_node) : node(_node) {
		SetSphere(_node);
	}

	Sphere::~Sphere() {	
	}
	
	// grab the current mesh and setup the polygon sets
	void Sphere::SetSphere(const MFnDependencyNode &node) {		
		MPlug plug(node.object(), node.attribute("radius"));
		radius = plug.asFloat();
	}
	
	// insert the maya data into a pbrt input file
	void Sphere::Insert(std::ostream& fout) const {
			fout << "sphere" << endl;
	}

	MStatus Sphere::TranslationMatrix(ostream& fout) const {
		MStatus status;		
		
		return MStatus::kSuccess;
	}


}
