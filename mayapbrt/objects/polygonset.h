/*
 *  polygonset.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/7/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRTPOLYSET_H
#define _PBRTPOLYSET_H

#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>

#include "object.h"
#include "../materials/material.h"

namespace pbrt {
	class PolygonSet : public Object {
		public:
			PolygonSet(const MDagPath &_dagPath, MObject &_component, MObject &set) : dagPath(_dagPath), component(_component) {
				material = Material::MaterialFactory(set);
				
			}
			~PolygonSet() {
			}
			
			// instead of properly handling materials with 
			void Clear() { if (material) delete material; }
			void Insert(std::ostream& fout) const;			
		private:
			MDagPath dagPath;
			MObject component;
			Material *material;
			
			// Material data
	};

}

#endif

