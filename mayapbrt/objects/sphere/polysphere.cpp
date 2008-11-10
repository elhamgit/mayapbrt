/*
 *  polysphere.cpp
 *  mayapbrt
 *
 *  Created by Martin Adam on 29/10/08.
 *  Czech Technical University
 *
 */

#include "polysphere.h"

#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDagNode.h>


using namespace std;

namespace pbrt {

	PolySphere::PolySphere(const MFnDependencyNode &_node) : Sphere(_node) 
	{
		SetSphere(_node);
	}

	PolySphere::~PolySphere() 
	{}
	
	// grab the current mesh and setup the polygon sets
	void PolySphere::SetSphere(const MFnDependencyNode &node) 
	{		
		MPlug plug(node.object(), node.attribute("radius"));
		radius = plug.asFloat();

		MPlugArray myPlugs, fathersPlugs;
		node.getConnections(myPlugs);
		for(int i = 0; i < myPlugs.length(); i++)
		{				
			if(myPlugs[i].isNetworked())
			{
				myPlugs[i].connectedTo(fathersPlugs, true, true);
				for(int j = 0; j < fathersPlugs.length(); j++)
				{
					MFnDagNode fatherMesh(fathersPlugs[j].node());
					fatherMesh.getPath(dagPath);
				}
			}				
		}
	}
	
	// insert the maya data into a pbrt input file
	void PolySphere::Insert(std::ostream& fout) const 
	{
		fout << "AttributeBegin" << endl;

		TranslationMatrix(fout);
		fout << "Shape \"sphere\" \"float radius\" ["<< radius << "]" << endl;

		fout << "AttributeEnd" << endl;
	}

}


/* ---------- POKUSY ----------------

MPlugArray mpArray;
			dNode.getConnections(mpArray);
			for(int i = 0; i < mpArray.length(); i++)
			{
				cout << mpArray[i].name() << " ";
				if(mpArray[i].isCompound())
					cout << "Compound" << endl;
				if(mpArray[i].isNetworked())
				{
					cout << "Networked " << mpArray[i].numConnectedChildren() << " parent: " << mpArray[i].parent().name();
					cout << " | potomek ";
					MPlugArray mpa;
					mpArray[i].connectedTo(mpa, true, true);
					for(int j = 0; j < mpa.length(); j++)
					{
						cout << mpa[j].name() << "  " << mpa[j].info() << " ";
						MFnDagNode mfndn(mpa[j].node());
						cout << "path: " << mfndn.fullPathName();
					}
				}				
			}
			cout << endl;
			MObject pobj;
			mpArray[0].getValue(pobj);
			MFnDependencyNode dNod(pobj);
			MPlug plug(sphereItDag.thisNode(), dNode.attribute("radius"));
			cout << dNode.name() << " , " << dNode.typeName() << ";  " << plug.asDouble() << dNod.name() << " " << endl;
			//MFnSphereData test(sphereItDag.item(), &status);
			//cout << " " << status.errorString() << " type " << test.type() << "  " << test.radius() << " " << endl;	
*/