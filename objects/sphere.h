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

#include <maya/MDagPath.h>

namespace pbrt {
	class Sphere : public Object {
		public:
			Sphere(const MDagPath &dagPath);
			~Sphere();
			
			void SetSphere(const MDagPath &dagPath);
			void Insert(std::ostream& fout) const;
		private:
			MStatus Sphere::TranslationMatrix(std::ostream& fout) const;
			MDagPath dagPath;
	};
	
}

#endif
