/*
 *  sphere.h
 *  mayapbrt
 *
 *  Created by Martin Adam on 29/10/08.
 *  Czech Technical University
 *
 */

#ifndef __PBRTSPHERE_H
#define __PBRTSPHERE_H

#include "../object.h"
#include <vector>
#include <ostream>

#include <maya/MFnDependencyNode.h>
#include <maya/MDagPath.h>

namespace pbrt {
	class Sphere : public Object {
		public:
			Sphere(const MFnDependencyNode &node);
			~Sphere();
			
			virtual void SetSphere(const MFnDependencyNode &node) = 0;
			virtual void Insert(std::ostream& fout) const = 0;

		protected:
			MStatus Sphere::TranslationMatrix(std::ostream& fout) const;
			MDagPath dagPath;
			float radius;
	};
	
}

#endif
