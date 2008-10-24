/*
 *  camera.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#ifndef _PBRTCAMERA_H
#define _PBRTCAMERA_H

#include <iostream>
#include <maya/MDagPath.h>
#include <maya/MFnCamera.h>
#include <maya/MString.h>

#include "../exporterror.h"

namespace pbrt {
	
	class Camera {
		public:
			// default constructor which will get the active camera
			Camera(MString &_imageName, int width, int height, MDagPath &cameraPath) throw (ExportError);
			virtual ~Camera() {}
			
			// overload the insertion operator for easy output of the objects
			friend std::ostream& operator<<(std::ostream& fout, Camera &camera);
			
		protected:
			// these functions will do all the exporting from maya and will illustrate
			// how to access the appropriate data from the Camera object
			virtual void InsertOrtho(std::ostream& fout);
			virtual void InsertPerspective(std::ostream& fout);
			
			// maya camera object
			MFnCamera camera;
		
		private:
			// query's maya for the active camera on the display
			//void GetActiveCamera() throw (ExportError);
			
			// insert attributes common to all camera types
			void InsertCommon(std::ostream& fout);
			
			MString imageName;
			int width, height;
			
	};

}

#endif