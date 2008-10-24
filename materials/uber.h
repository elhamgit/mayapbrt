/*
 *  uber.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 12/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRT_UBER_MATERIAL_H
#define _PBRT_UBER_MATERIAL_H

#include "material.h"

namespace pbrt {
	class Uber: public Material {
		public:
			Uber(MFnDependencyNode &_shaderNode) : Material(_shaderNode) {}
			~Uber() {}

		protected:
			void Insert(std::ostream& fout) const;

	};
}

#endif
