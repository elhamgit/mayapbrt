/*
 *  uber.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 12/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "uber.h"

#include <Maya/MPlug.h>
#include <Maya/MGlobal.h>
#include <Maya/MFnNumericData.h>

namespace pbrt {	
	void Uber::Insert(std::ostream& fout) const {
		int texcolor=0, texbump=0;
		
		texcolor = colorTexture(fout);
		texbump = bumpTexture(fout);
		
		MStatus status;
		MObject object;
		MPlug reflectivityPlug = MFnDependencyNode(shaderNode).findPlug("reflectivity");
		MPlug translucencePlug = MFnDependencyNode(shaderNode).findPlug("translucence");

		MPlug specularPlug = MFnDependencyNode(shaderNode).findPlug("specularColor");
		status = specularPlug.getValue(object);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get specular color value out"); }
		
		MFnNumericData specularData(object, &status);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get specular color data"); }
		
		
		
		if (texcolor) {
			fout << "Material \"uber\" \"texture Kd\" \"" << texcolor << "\" ";
			if (texbump) fout << "\"texture bumpmap\" \"" << texbump << "\" ";
			
			float r;
			status = reflectivityPlug.getValue(r);
			if (status == MStatus::kSuccess) fout << "\"color Kr\" [" << r << " " << r << " " << r << "] ";
			
			if (specularData.numericType() == MFnNumericData::k3Float) {
				float r,g,b;
				specularData.getData(r,g,b);				
				fout << "\"color Ks\" [" << r << " " << g << " " << b << "] ";
				
			} else if (specularData.numericType() == MFnNumericData::k3Double) {
				double r,g,b;
				specularData.getData(r,g,b);
				fout << "\"color Ks\" [" << r << " " << g << " " << b << "] ";
			}			
			
			float opacity;
			status = translucencePlug.getValue(opacity);
			opacity = 1.f - opacity;
			if (status == MStatus::kSuccess) fout << "\"color opacity\" [" << opacity << " " << opacity << " " << opacity << "] ";
			
			fout << std::endl;
		}
		
	}

}
