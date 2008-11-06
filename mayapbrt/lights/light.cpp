/*
 *  light.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

#include "light.h"
#include "pointlight.h"
#include "spotlight.h"
#include <iostream>

using namespace std;

namespace pbrt {

	// factory which determines all available light types and will return the 
	// appropriate light to use in exporting
	Light* Light::LightFactory(const MDagPath &dagPath) {
		MFnLight light(dagPath);
		MStatus status;
		
		MFnSpotLight spotTest(dagPath, &status); cout << (status==MStatus::kSuccess) << endl;		
		if (status==MStatus::kSuccess) {
			return new SpotLight(dagPath);
		}
		
		MFnPointLight pointTest(dagPath, &status); cout << (status==MStatus::kSuccess) << endl;		
		if (status==MStatus::kSuccess) {
			return new PointLight(dagPath);
		}

		MGlobal::displayWarning("Light type not supported");
		
		return NULL;
	}

	std::ostream& operator<< (std::ostream& fout, Light& light) {
		light.Insert(fout);
		return fout;
	}
	
	MStatus Light::TranslationMatrix(std::ostream& fout) const {
		MStatus status;
	
		MObject	transformNode = dagPath.transform(&status);
		if (!status && status.statusCode () == MStatus::kInvalidParameter) return MStatus::kFailure;
		
		MFnDagNode transform(transformNode, &status);
		if (!status) return MStatus::kFailure;
		
		MTransformationMatrix transMatrix (transform.transformationMatrix());
		MMatrix matrix = transMatrix.asMatrix();
		
		fout << "ConcatTransform [" << matrix.matrix[0][0] << " " << matrix.matrix[0][1] << " " << matrix.matrix[0][2] << " " << matrix.matrix[0][3] << " " <<
									  matrix.matrix[1][0] << " " << matrix.matrix[1][1] << " " << matrix.matrix[1][2] << " " << matrix.matrix[1][3] << " " << 
									  matrix.matrix[2][0] << " " << matrix.matrix[2][1] << " " << matrix.matrix[2][2] << " " << matrix.matrix[2][3] << " " <<
									  matrix.matrix[3][0] << " " << matrix.matrix[3][1] << " " << matrix.matrix[3][2] << " " << matrix.matrix[3][3] << "]" << endl;
		
		
		return MStatus::kSuccess;
	}
	
}
