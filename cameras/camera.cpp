/*
 *  camera.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "camera.h"

// maya includes
#include <maya/M3dView.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MRenderView.h>
#include <iomanip>

using namespace std;

namespace pbrt {
	
	Camera::Camera(MString &_imageName, int _width, int _height, MDagPath &cameraPath) throw (ExportError) : camera(cameraPath) {
		imageName = _imageName;
		width = _width;
		height = _height;
				

//		GetActiveCamera();
	}
	
	// Camera::GetActiveCamera()
	// Query's maya's windowing system for the current camera being selected
	// and used by the user
	//void Camera::GetActiveCamera() throw (ExportError) {
	//	MStatus status;

	//	// Get the current camera
	//	M3dView curView = M3dView::active3dView();
	//	MDagPath camDagPath;
	//	curView.getCamera( camDagPath );
	//	
	//	// construct the camera data for pbrt
	//	camera.setObject(camDagPath);	if (status != MStatus::kSuccess) throw ExportError("Could not get active camera from maya");
	//}
	
	void Camera::InsertCommon(std::ostream& fout) {
		MStatus status;
		
		// utilizes the lookat points so we don't have to worry about handedness (too much)
		MPoint eye = camera.eyePoint(MSpace::kWorld,&status);				if (status != MStatus::kSuccess) throw ExportError("Could not get eye point");
		MVector up = camera.upDirection(MSpace::kWorld,&status);			if (status != MStatus::kSuccess) throw ExportError("Could not get up vector");	
		MPoint at = camera.centerOfInterestPoint(MSpace::kWorld,&status);	if (status != MStatus::kSuccess) throw ExportError("Could not get at point");	
		fout << "LookAt " << eye.x << " " << eye.y << " " << eye.z << " "
						  << at.x << " " << at.y << " " << at.z << " "
						  << up.x << " " << up.y << " " << up.z << endl;
	
		
	}
	
	// Camera::InsertOrtho
	// creates pbrt commands for inputting orthographic camera
	void Camera::InsertOrtho(std::ostream& fout) {
		// TODO: Support orthographic cameras
		InsertCommon(fout);
	}
	
	// Camera::InsertPerspective
	// creates pbrt commands for inputting perspective camera
	void Camera::InsertPerspective(std::ostream& fout) {
		InsertCommon(fout);
		fout << "Camera \"perspective\" ";
		//fout << "\"float focaldistance\" [" << camera.focalLength() << "] ";
		fout << "\"float fov\" [" << camera.horizontalFieldOfView()*180.0/3.14159265359 << "] ";
		fout << "\"float frameaspectratio\" [" << camera.aspectRatio() << "] ";
		fout << "\"float hither\" [" << camera.nearClippingPlane() << "]" << endl;
		
		fout << "Film \"tga\" \"integer xresolution\" [" << width << "] \"integer yresolution\" [" << height << "] ";
		fout << "\"string filename\" [\"" << imageName.asChar() << "\"]" << endl;
	}
	
	// overloaded output operator
	ostream& operator<< (ostream& fout, Camera& camera) {
		if (camera.camera.isOrtho())
			camera.InsertOrtho(fout);		
		else
			camera.InsertPerspective(fout);
			
		return fout;
	}

}	