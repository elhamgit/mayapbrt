/*
 *  PolySphere.h
 *  mayapbrt
 *
 *  Created by Martin Adam on 29/10/08.
 *  Czech Technical University
 *
 */

#ifndef __PBRTPOLYSPHERE_H
#define __PBRTPOLYSPHERE_H

#include "sphere.h"
#include <vector>
#include <ostream>

namespace pbrt {
	class PolySphere : public Sphere {
		public:
			PolySphere(const MFnDependencyNode &node);
			~PolySphere();
			
			virtual void SetSphere(const MFnDependencyNode &node);
			virtual void Insert(std::ostream& fout) const;
		private:
	};
}

#endif		// __PBRTPOLYSPHERE_H
