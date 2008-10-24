/*
 *  material.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#include "material.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericData.h>

#include "shinymetal.h"
#include "plastic.h"
#include "uber.h"

using namespace std;

namespace pbrt {
	int Material::textureNum = 0;
	
	Material* Material::MaterialFactory(MObject &set) {
		MObject shaderNode = findShader(set);
		if (shaderNode == MObject::kNullObj) return NULL;
		
		MFnDependencyNode depNode(shaderNode);
		
		if (depNode.typeName() == "blinn") {
			return new Uber(depNode);
		} else if (depNode.typeName() == "lambert") {
			return new Plastic(depNode);			
		} else if (depNode.typeName() == "anisotropic") {
			// need to find a better material
		} else if (depNode.typeName() == "phong") {
			return new Uber(depNode);
		} else {
			return new Uber(depNode);
		}
		
		return NULL;
	}
	
	MObject Material::findShader(MObject &setNode) {
		MFnDependencyNode fnNode(setNode);
		MPlug shaderPlug = fnNode.findPlug("surfaceShader");
				
		if (!shaderPlug.isNull()) {			
			MPlugArray connectedPlugs;
			bool asSrc = false;
			bool asDst = true;
			shaderPlug.connectedTo( connectedPlugs, asDst, asSrc );

			if (connectedPlugs.length() != 1)
				cerr << "Error getting shader\n";
			else 
				return connectedPlugs[0].node();
		}			
		
		return MObject::kNullObj;
	}
	
	// finds the texture file or the color of the texture for a given surface
	// the following does not allow for multiple texture blending!!!!
	int Material::colorTexture(ostream &fout) const {
		MStatus status;
		int texcolor;
	
		MPlug colorPlug = MFnDependencyNode(shaderNode).findPlug("color");
		
		MItDependencyGraph dgIt(colorPlug, MFn::kFileTexture,
						   MItDependencyGraph::kUpstream, 
						   MItDependencyGraph::kBreadthFirst,
						   MItDependencyGraph::kNodeLevel, 
						   &status);
		
		dgIt.disablePruningOnFilter();
		
		if (!dgIt.isDone()) {
			// use a texture value
			MObject textureNode = dgIt.thisNode();
			MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
			MString textureName;
			filenamePlug.getValue(textureName);
			
			texcolor = ++textureNum;
			fout << "Texture \"" << texcolor << "\" \"color\" \"imagemap\" \"string filename\" [\"" << textureName.asChar() << "\"]" << endl;
		} else {
			// use a numerical value for the color
			MObject object;
			status = colorPlug.getValue(object);
			if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get color value out"); return 0; }
			
			MFnNumericData colorData(object, &status);
			if (status != MStatus::kSuccess) { MGlobal::displayWarning("Could not get color value out (2)"); return 0; }
			
			if (colorData.numericType() == MFnNumericData::k3Float) {
				float r,g,b;
				colorData.getData(r,g,b);
				
				texcolor = ++textureNum;
				fout << "Texture \"" << texcolor << "\" \"color\" \"constant\" \"color value\" [" << r << " " << g << " " << b << "]" << endl;
			} else if (colorData.numericType() == MFnNumericData::k3Double) {
				double r,g,b;
				colorData.getData(r,g,b);
				
				texcolor = ++textureNum;
				fout << "Texture \"" << texcolor << "\" \"color\" \"constant\" \"color value\" [" << r << " " << g << " " << b << "]" << endl;			
			} else {
				MGlobal::displayWarning("Invalid data tuple"); return 0;
			}
			
		}
		
		return texcolor;	
	}
	
	int Material::bumpTexture(ostream& fout) const {
		MStatus status;
		int texbump = 0;
	
		MPlug bumpPlug = MFnDependencyNode(shaderNode).findPlug("normalCamera");
		
		MItDependencyGraph dgIt(bumpPlug, MFn::kFileTexture,
						   MItDependencyGraph::kUpstream, 
						   MItDependencyGraph::kBreadthFirst,
						   MItDependencyGraph::kNodeLevel, 
						   &status);
		
		dgIt.disablePruningOnFilter();
		
		if (!dgIt.isDone()) {
			// use a texture value
			MObject textureNode = dgIt.thisNode();
			MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
			MString textureName;
			filenamePlug.getValue(textureName);
			
			texbump = ++textureNum;
			fout << "Texture \"" << texbump << "\" \"float\" \"imagemap\" \"string filename\" [\"" << textureName.asChar() << "\"]" << endl;
		}
		
		return texbump;
	}
	
	ostream& operator<< (ostream& fout, const Material& obj) {
		obj.Insert(fout);
		return fout;
	}

}
