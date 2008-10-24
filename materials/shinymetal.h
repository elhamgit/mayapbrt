/*
 *  shinymetal.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/11/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRT_SHINY_METAL_MATERIAL_H
#define _PBRT_SHINY_METAL_MATERIAL_H

#include "material.h"

namespace pbrt {
	class ShinyMetal: public Material {
		public:
			ShinyMetal(MFnDependencyNode &_shaderNode) : Material(_shaderNode) {}
			~ShinyMetal() {}

		protected:
			void Insert(std::ostream& fout) const;

	};
}

#endif

