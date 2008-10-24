/*
 *  pointlight.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/9/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "pointlight.h"

#include <maya/MColor.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MGlobal.h>

namespace pbrt {
	
	void PointLight::Insert(std::ostream& fout) const {
		MStatus status;
			
		fout << "TransformBegin" << std::endl;
		
		if (TranslationMatrix(fout) != MStatus::kSuccess) { MGlobal::displayError("Error in outputting light translation"); return; }
		
		// get the color data
		MColor color = light.color();
		float intensity = light.intensity();
		
		// output the light source
		fout << "LightSource \"point\" ";
		fout << "\"color I\" [" << color.r*intensity << " " << color.g*intensity << " " << color.b*intensity << "]" << std::endl;
		
		fout << "TransformEnd" << std::endl;
		
	}
}
