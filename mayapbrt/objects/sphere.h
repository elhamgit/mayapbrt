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

#include "object.h"
#include <vector>
#include <ostream>

#include <maya/MFnDependencyNode.h>

namespace pbrt {
	class Sphere : public Object {
		public:
			Sphere(const MFnDependencyNode &node);
			~Sphere();
			
			void SetSphere(const MFnDependencyNode &node);
			void Insert(std::ostream& fout) const;
		private:
			MStatus Sphere::TranslationMatrix(std::ostream& fout) const;
			MFnDependencyNode node;
			float radius;
	};
	
}

#endif
