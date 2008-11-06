/*
 *  material.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#ifndef _PBRTMATERIAL_H
#define _PBRTMATERIAL_H

#include <iostream>
#include <maya/MFnSet.h>
#include <maya/MObject.h>

namespace pbrt {
	class Material {
		public:
			Material(MFnDependencyNode &_shaderNode) : shaderNode(_shaderNode) {}
			virtual ~Material() {}
			
			// create a material based off a given set
			static Material* MaterialFactory(MObject &set);			
			
			// overload the insertion operator for easy output of the objects
			friend std::ostream& operator<<(std::ostream& fout, const Material &object);			
		protected:
			static MObject findShader(MObject &setNode);
			
			int colorTexture(std::ostream& fout) const;
			int bumpTexture(std::ostream& fout) const;
			virtual void Insert(std::ostream& fout) const = 0;
			
			static int textureNum;
			MFnDependencyNode shaderNode;			

	};
}

#endif
