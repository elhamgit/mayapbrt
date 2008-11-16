/*
 *  phongmat.h
 *  mayapbrt
 *
 *  Created by Martin Adam on 16/11/08.
 *  Czech Technical University
 *
 */

#ifndef _PBRT_PHONGMAT_MATERIAL_H
#define _PBRT_PHONGMAT_MATERIAL_H

#include "material.h"

namespace pbrt {
	class PhongMat: public Material {
		public:
			PhongMat(MFnDependencyNode &_shaderNode) : Material(_shaderNode) {}
			~PhongMat() {}

		protected:
			void Insert(std::ostream& fout) const;

	};
}

#endif
