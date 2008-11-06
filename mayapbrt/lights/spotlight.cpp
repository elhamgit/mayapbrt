/*
 *  spotlight.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/9/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */


#include "spotlight.h"

#include <maya/MColor.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MGlobal.h>

namespace pbrt {
	void SpotLight::Insert(std::ostream& fout) const {
		MStatus status;
			
		fout << "TransformBegin" << std::endl;
		
		if (TranslationMatrix(fout) != MStatus::kSuccess) { MGlobal::displayError("Error in outputting light translation"); return; }
		
		// get the color data
		MColor color = light.color();
		float intensity = light.intensity();
		
		// output the light source
		fout << "LightSource \"spot\" ";
		fout << "\"color I\" [" << color.r*intensity << " " << color.g*intensity << " " << color.b*intensity << "]" << " ";
		fout << "\"point from\" [0 0 0] \"point to\" [0 0 -1]" << std::endl;
		fout << "\"float coneangle\" [" << light.coneAngle()*180.f/M_PI << "]" << " ";
		fout << "\"float conedeltaangle\" [" << light.dropOff()*180.f/M_PI << "]" << std::endl;
		fout << "TransformEnd" << std::endl;
		
	}
	
}
