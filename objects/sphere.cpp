/*
 *  sphere.cpp
 *  mayapbrt
 *
 *  Created by Martin Adam on 29/10/08.
 *  Czech Technical University
 *
 */

#include "sphere.h"

#include <maya/MObjectArray.h>
#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

using namespace std;

namespace pbrt {

	Sphere::Sphere(const MDagPath &_dagPath) : dagPath(_dagPath) {
		SetSphere(_dagPath);
	}

	Sphere::~Sphere() {	
	}
	
	// grab the current mesh and setup the polygon sets
	void Sphere::SetSphere(const MDagPath &dPath) {
		
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
