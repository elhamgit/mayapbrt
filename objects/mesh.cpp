/*
 *  mesh.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/7/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "mesh.h"

#include <maya/MObjectArray.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

using namespace std;

namespace pbrt {

	Mesh::Mesh(const MDagPath &_dagPath) : dagPath(_dagPath) {
		SetMesh(_dagPath);
	}

	Mesh::~Mesh() {
		for (vector<PolygonSet>::iterator iter = polySets.begin(); iter != polySets.end(); iter++)
			iter->Clear();
		polySets.clear();
	}
	
	// grab the current mesh and setup the polygon sets
	void Mesh::SetMesh(const MDagPath &dPath) {
		MObjectArray fPolygonSets;
		MObjectArray fPolygonComponents;
		MDagPath dagPath(dPath);
		polySets.clear();
		
		MFnMesh fMesh = MFnMesh(dagPath);
		
		//Have to make the path include the shape below it so that
		//we can determine if the underlying shape node is instanced.
		//By default, dag paths only include transform nodes.
		dagPath.extendToShape();

		//If the shape is instanced then we need to determine which instance this path refers to.
		int instanceNum = 0;
		if (dagPath.isInstanced())
			instanceNum = dagPath.instanceNumber();

		//Get the connected sets and members - these will be used to determine texturing of different faces
		if (!fMesh.getConnectedSetsAndMembers(instanceNum, fPolygonSets, fPolygonComponents, true)) {
			MGlobal::displayError("MFnMesh::getConnectedSetsAndMembers"); 
			return;
		}
		
		unsigned int setCount = fPolygonSets.length();
		if (setCount > 1)
			setCount--;
		
		for (unsigned int i=0; i < setCount; i++)
			polySets.push_back(PolygonSet(dagPath, fPolygonComponents[i], fPolygonSets[i]));
		
	}
	
	// insert the maya data into a pbrt input file
	void Mesh::Insert(std::ostream& fout) const {
		// go through all the different polygon sets and output the data
		for (unsigned int i=0; i < polySets.size(); i++) {
			fout << polySets[i] << endl;
		}
	}
	
	MStatus Mesh::TranslationMatrix(ostream& fout) const {
		MStatus status;
	
		MObject	transformNode = dagPath.transform(&status);
		if (!status && status.statusCode () == MStatus::kInvalidParameter) return MStatus::kFailure;
		
		MFnDagNode transform(transformNode, &status);
		if (!status) return MStatus::kFailure;
		
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
