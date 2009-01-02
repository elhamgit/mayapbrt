/*
 *  phongmat.cpp
 *  mayapbrt
 *
 *  Created by Martin Adam on 16/11/08.
 *  Czech Technical University
 *
 */

#include "phongmat.h"

#include <Maya/MPlug.h>
#include <Maya/MGlobal.h>
#include <Maya/MFnNumericData.h>

namespace pbrt {	
	void PhongMat::Insert(std::ostream& fout) const {
		
		MStatus status;
		MObject object;
		/*
		for(int j = 0; j < shaderNode.attributeCount(); j++)
		{			
			MPlug m = shaderNode.findPlug(shaderNode.attribute(j), false);
			std::cout << " " << shaderNode.attribute(j).apiTypeStr() << " " << " plug " << m.name() << std::endl;
		}
		std::cout << std::endl;*/
		//MPlug reflectivityPlug = MFnDependencyNode(shaderNode).findPlug("reflectivity");

		MPlug sigmaPlug = MFnDependencyNode(shaderNode).findPlug("cosinePower");		

		MPlug colorPlug = MFnDependencyNode(shaderNode).findPlug("color");
		status = colorPlug.getValue(object);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get color value out"); }

		MFnNumericData colorData(object, &status);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get color value out (2)");  }

		MPlug specularPlug = MFnDependencyNode(shaderNode).findPlug("specularColor");
		status = specularPlug.getValue(object);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get specular color value out"); }
		
		MFnNumericData specularData(object, &status);
		if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get specular color data"); }
				
		fout << "Material \"phongmat\" ";																		
		
		if (colorData.numericType() == MFnNumericData::k3Float) {
			float r,g,b;
			colorData.getData(r,g,b);						
			fout << "\"color rhod\" [" << r << " " << g << " " << b << "] ";
		} else if (colorData.numericType() == MFnNumericData::k3Double) {
			double r,g,b;
			colorData.getData(r,g,b);						
			fout << "\"color rhod\" [" << r << " " << g << " " << b << "] ";
		} else {
			MGlobal::displayWarning("Invalid data type");
		}		
		
		if (specularData.numericType() == MFnNumericData::k3Float) {
			float r,g,b;
			specularData.getData(r,g,b);				
			fout << "\"color rhos\" [" << r << " " << g << " " << b << "] ";
			
		} else if (specularData.numericType() == MFnNumericData::k3Double) {
			double r,g,b;
			specularData.getData(r,g,b);
			fout << "\"color rhos\" [" << r << " " << g << " " << b << "] ";
		} else {
			MGlobal::displayWarning("Invalid data type");
		}
			
		float sigma;
		status = sigmaPlug.getValue(sigma);
		if (status == MStatus::kSuccess) 
			fout << "\"float sigma\" [" << sigma << "] ";
		
		// sigma = cosine_power
		fout << std::endl;		
	}
}
