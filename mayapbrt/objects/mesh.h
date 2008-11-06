/*
 *  mesh.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/7/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#ifndef __PBRTMESH_H
#define __PBRTMESH_H

#include "object.h"
#include "polygonset.h"
#include <vector>
#include <ostream>

#include <maya/MDagPath.h>

namespace pbrt {
	class Mesh : public Object {
		public:
			Mesh(const MDagPath &dagPath);
			~Mesh();
			
			void SetMesh(const MDagPath &dagPath);
			void Insert(std::ostream& fout) const;
		private:
			MStatus Mesh::TranslationMatrix(std::ostream& fout) const;
			std::vector<PolygonSet> polySets;
			MDagPath dagPath;
	};
	
}

#endif
