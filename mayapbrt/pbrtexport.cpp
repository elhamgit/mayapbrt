/*
 *  pbrtexport.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *
 */

#include "pbrtexport.h"
#include "cameras/camera.h"
#include "lights/light.h"
#include "objects/mesh.h"
#include "objects/sphere/polysphere.h"

#include <fstream>
#include <iomanip>
#include <maya/MPlug.h>
#include <maya/MItDag.h>
#include <maya/MGlobal.h>
#include <maya/MFnCamera.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MDataHandle.h>
#include <maya/MDataBlock.h>
#include <maya/MPlugArray.h>
#include <maya/MIteratorType.h>
#include <maya/MIntArray.h>  
 	 	
using namespace std;

namespace pbrt {
	
	void* Export::creator() {
		return new Export;
	}
	
	// main loop of the plugin
	// this will open an output file and go  through all the visible meshes and
	// lights and put them in a pbrt format
	MStatus	Export::doIt( const MArgList& args) {
		MStatus status;
		MDagPath tempDagPath;
		
		if (args.length() < 5) {
			MGlobal::displayError("At least 5 arguments are required for PBRTExport");
			return MStatus::kFailure;
		}
		
		MString outputfile = args.asString(0, &status);
		if (!status) {
			MGlobal::displayError("Error parsing output file argument");
			return MStatus::kFailure;
		}

		MString imagefile = args.asString(1, &status);
		if (!status) {
			MGlobal::displayError("Error parsing image file argument");
			return MStatus::kFailure;
		}

		int width = args.asInt(2, &status);
		if (!status) {
			MGlobal::displayError("Error parsing width");
			return MStatus::kFailure;
		}

		int height = args.asInt(3, &status);
		if (!status) {
			MGlobal::displayError("Error parsing height");
			return MStatus::kFailure;
		}
		
		MString cameraName;
		cameraName = args.asString(4, &status);
		if (!status) {
			MGlobal::displayError("Error parsing camera name");
			return MStatus::kFailure;
		}

		// initialize the output
		ofstream fout;
		fout.open(outputfile.asChar(), ios::out);
		fout << setprecision(9) << setiosflags(ios_base::fixed);	// make sure we keep the output readable to pbrt
				
		// create default camera and output it
		// find the appopriate camera
		cout << cameraName.asChar() << " " << width << " " << height << endl;
		MItDag cameraItDag(MItDag::kDepthFirst, MFn::kCamera, &status);
		
		bool found = false;
		MDagPath cameraPath;

		while (!cameraItDag.isDone() && !found) {
			cameraItDag.getPath(tempDagPath);
			
			MString currCamName = MFnDagNode(tempDagPath.transform()).partialPathName();
			if ((currCamName == cameraName) || (cameraName == tempDagPath.partialPathName())) {
				found = true;
				cout << "Found Camera " << tempDagPath.partialPathName().asChar() << endl;
				cameraPath = tempDagPath;
			}
			cameraItDag.next();
		}

		if (!found) {
			MGlobal::displayError("Could not find the camera");
			return MStatus::kFailure;
		}

		//Obtain Camera Settings and write them to file
		try{
			Camera cam(imagefile, width, height, cameraPath);
			fout << cam;
		}catch(ExportError e){
			fout << e << endl;
		}

		fout << "WorldBegin" << endl;
		
		// loop through all the lights
		MItDag lightItDag(MItDag::kDepthFirst, MFn::kLight, &status);
		for (; !lightItDag.isDone(); lightItDag.next()) {
			lightItDag.getPath(tempDagPath);
			
			// visibility test
			MFnDagNode visTest(tempDagPath);
			if (isVisible(visTest)) {
				Light* light = Light::LightFactory(tempDagPath);
				if (light) { 
					fout << *light << endl;
					delete light;
				}
			}
		}

///******************************************************************************************************
///******SPHERE***************SPHERE************SPHERE***************SPHERE**********SPHERE**************
///******************************************************************************************************

		MIntArray itTypeArray(3);
		itTypeArray.set(MFn::kPolySphere, 0);
		itTypeArray.set(MFn::kSphere, 1);
		itTypeArray.set(MFn::kRenderSphere, 2);
		MIteratorType itType;
		itType.setFilterList(itTypeArray);
		// loop through all the spheres
		MItDependencyNodes sphereItDag(itType);
		//MItDag sphereItDag;
		for (; !sphereItDag.isDone(); sphereItDag.next()) 
		{			
			MFnDependencyNode dNode(sphereItDag.thisNode());
			switch(sphereItDag.thisNode().apiType())
			{
				case MFn::kPolySphere:
				case MFn::kSphere:
					{
						PolySphere sphere(dNode);
						fout << sphere << endl;
					}
			};
		}

///******************************************************************************************************
///******************************************************************************************************

		// loop through all the meshes
		MItDag meshItDag(MItDag::kDepthFirst, MFn::kMesh, &status);
		for (; !meshItDag.isDone(); meshItDag.next()) {
			meshItDag.getPath(tempDagPath);
			
			// visiblity test
			MFnDagNode visTest(tempDagPath);
			if (isVisible(visTest)) {
				Mesh mesh(tempDagPath);
				fout << mesh << endl;
			}
		}
		
		// finish off the output file
		fout << "WorldEnd" << endl;
		fout.close();
		
		return MStatus::kSuccess;
	}

	bool Export::isVisible(MFnDagNode & fnDag, MStatus *status) 
	//Summary:	determines if the given DAG node is currently visible
	//Args   :	fnDag - the DAG node to check
	//Returns:	true if the node is visible;		
	//			false otherwise
	{
		bool madeStatus = false;
		if (status == NULL) { status = new MStatus; madeStatus = true; }
		
		if(fnDag.isIntermediateObject())
			return false;

		MPlug visPlug = fnDag.findPlug("visibility", status);
		if (MStatus::kFailure == *status) {
			MGlobal::displayError("MPlug::findPlug");
			if (madeStatus) delete status;
			return false;
		} else {
			bool visible;
			*status = visPlug.getValue(visible);
			if (MStatus::kFailure == *status) {
				MGlobal::displayError("MPlug::getValue");
			}
			if (madeStatus) delete status;
			return visible;
		}
	}
}