/*
 *  polygonset.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/7/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#include "polygonset.h"

#include <vector>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

using namespace std;

namespace pbrt {

	// ********************************************************************
	// MItMeshPolygon::getTriangle() returns object-relative vertex
	// indices; BUT MItMeshPolygon::normalIndex() and ::getNormal() need
	// face-relative vertex indices! This converts vertex indices from
	// object-relative to face-relative.
	//
	// param  getVertices: Array of object-relative vertex indices for
	//                     entire face.
	// param  getTriangle: Array of object-relative vertex indices for
	//                     local triangle in face.
	//
	// return Array of face-relative indicies for the specified vertices.
	//        Number of elements in returned array == number in getTriangle
	//        (should be 3).
	//
	// note   If getTriangle array does not include a corresponding vertex
	//        in getVertices array then a value of (-1) will be inserted
	//        in that position within the returned array.
	// ********************************************************************
	MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle ) {
		MIntArray   localIndex;
		unsigned    gv, gt;

		for ( gt = 0; gt < getTriangle.length(); gt++ ) {
			for ( gv = 0; gv < getVertices.length(); gv++ ) {
				if ( getTriangle[gt] == getVertices[gv] ) {
					localIndex.append( gv );
					break;
				}
			}

			// if nothing was added, add default "no match"
			if ( localIndex.length() == gt )
			  localIndex.append( -1 );
		}

		return localIndex;
	}


	void PolygonSet::Insert(ostream& fout) const {
		MStatus status;
		vector<float> points, normals;
		vector<int> indices;
		vector<float> uvs;
		MFnMesh mesh(dagPath);
		MItMeshPolygon itMeshPolygon(dagPath, const_cast<MObject&>(component));
		
		// cache the points
		MPointArray meshPoints;
		mesh.getPoints( meshPoints, MSpace::kWorld );

		//  Cache normals for each vertex
		MFloatVectorArray  meshNormals;
		// Normals are per-vertex per-face..
		// use MItMeshPolygon::normalIndex() for index
		mesh.getNormals( meshNormals );		

		// cache the UVs
		// Get UVSets for this mesh
		MStringArray  UVSets;
		status = mesh.getUVSetNames( UVSets );
		
		// Get all UVs for the first UV set.
		MFloatArray   u, v;
		mesh.getUVs( u, v, &UVSets[0] );
		
		//int offset =0;
		int count = 0;
		
		fout << "AttributeBegin" << endl;
		if (material) fout << *material << endl;
		
		fout << "Shape \"trianglemesh\" ";
		
		
		// get the indices for the triangles in the mesh
		for (; !itMeshPolygon.isDone(); itMeshPolygon.next()) {
			if (!itMeshPolygon.hasValidTriangulation()) {
				MGlobal::displayError("MItMeshPolygon::hasValidTriangulation");
				continue;
			}
			
			int numTriangles;
			itMeshPolygon.numTriangles(numTriangles);
			while ( numTriangles-- ) {
			
				// Get object-relative indices for the vertices in this face.
				MIntArray polygonVertices;
				itMeshPolygon.getVertices( polygonVertices );			
			
				MPointArray nonTweaked;
				// object-relative vertex indices for each triangle
				MIntArray triangleVertices;
				// face-relative vertex indices for each triangle
				MIntArray localIndex;

				status = itMeshPolygon.getTriangle( numTriangles,
												nonTweaked,
												triangleVertices,
												MSpace::kWorld );
				
				if (status = MStatus::kSuccess) {
					// push back the points
					points.push_back(meshPoints[triangleVertices[0]].x);
					points.push_back(meshPoints[triangleVertices[0]].y);
					points.push_back(meshPoints[triangleVertices[0]].z);
					
					points.push_back(meshPoints[triangleVertices[1]].x);
					points.push_back(meshPoints[triangleVertices[1]].y);
					points.push_back(meshPoints[triangleVertices[1]].z);
					
					points.push_back(meshPoints[triangleVertices[2]].x);
					points.push_back(meshPoints[triangleVertices[2]].y);
					points.push_back(meshPoints[triangleVertices[2]].z);					
					
					localIndex = GetLocalIndex( polygonVertices, triangleVertices);
										
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[0] ) ].x);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[0] ) ].y);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[0] ) ].z);
					
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[1] ) ].x);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[1] ) ].y);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[1] ) ].z);
					
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[2] ) ].x);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[2] ) ].y);
					normals.push_back(meshNormals[ itMeshPolygon.normalIndex( localIndex[2] ) ].z);
					
					// note: this only looks at the UVs for the default map set
					
					int uvID[3];
					MStatus status;
					// Get UV values for each vertex within this polygon
					for ( int vtxInPolygon = 0; vtxInPolygon < 3; vtxInPolygon++ ) {
						if((status = itMeshPolygon.getUVIndex( localIndex[vtxInPolygon],
											  uvID[vtxInPolygon],
											  &UVSets[0] )) == MStatus::kFailure)
											  break;
					}
					if(status == MStatus::kSuccess)
					{
						uvs.push_back(u[uvID[0]]); uvs.push_back(v[uvID[0]]);
						uvs.push_back(u[uvID[1]]); uvs.push_back(v[uvID[1]]);
						uvs.push_back(u[uvID[2]]); uvs.push_back(v[uvID[2]]);
					}
					
					count += 3;
				}
								
			}
			
			/*			
			MFloatArray uArray, vArray;
			MPointArray facePoints;
			MVectorArray faceNormals;
			int numverts = itMeshPolygon.polygonVertexCount();
			
			itMeshPolygon.getPoints(facePoints, MSpace::kWorld);
			itMeshPolygon.getNormals(faceNormals, MSpace::kWorld);
			itMeshPolygon.getUVs(uArray, vArray);
						
			for (int i=0; i < numverts; i++) {
				points.push_back(facePoints[i].x);
				points.push_back(facePoints[i].y);
				points.push_back(facePoints[i].z);
				
				normals.push_back(faceNormals[i].x);
				normals.push_back(faceNormals[i].y);
				normals.push_back(faceNormals[i].z);			
				
				uvs.push_back(uArray[i]);
				uvs.push_back(vArray[i]);
			}
			
			int numtri;
			itMeshPolygon.numTriangles(numtri);
			int v0 = 0;
			int v1 = 1;
			int v2 = 2;
			for (int k=0; k<numtri; k++){
				indices.push_back(v0+offset);
				indices.push_back(v1+offset);
				indices.push_back(v2+offset);								

				v1 = v2;
				v2 ++;
				if (v2 >= numverts) {
					v2 = 0;
				}
			}
			offset += numverts;	
			*/
		}
		
		
		fout << "\"point P\" [";
		for (vector<float>::const_iterator itP = points.begin(); itP != points.end(); itP++)
			fout << *itP << " ";
		fout << "]" << endl;
		
		fout << "\"normal N\" [";
		for (vector<float>::const_iterator itP = normals.begin(); itP != normals.end(); itP++)
			fout << *itP << " ";		
		fout << "]" << endl;
		
		fout << "\"float uv\" [";
		for (vector<float>::const_iterator itP = uvs.begin(); itP != uvs.end(); itP++)
			fout << *itP << " ";		
		fout << "]" << endl;
		
		fout << "\"integer indices\" [";
		/*
		for (vector<int>::const_iterator itP = indices.begin(); itP != indices.end(); itP++)
			fout << *itP << " ";		
		*/
		for (int i=0; i < count; i++)
			fout << i << " ";
		fout << "]" << endl;
		
		
		fout << "AttributeEnd" << endl;
	}

}
