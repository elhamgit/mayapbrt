/*
 *  pointlight.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/9/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRTPOINTLIGHT_H
#define _PBRTPOINTLIGHT_H

#include <maya/MFnPointLight.h>

#include "light.h"

namespace pbrt {
	
	class PointLight : public Light {
		public:
			PointLight(const MDagPath &dagPath) : Light(dagPath), light(dagPath) {}
			~PointLight() {}
						
		protected:
			void Insert(std::ostream& fout) const;
			MFnPointLight light;
	};

}

#endif
