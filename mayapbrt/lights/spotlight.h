/*
 *  spotlight.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/9/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRTSPOTLIGHT_H
#define _PBRTSPOTLIGHT_H

#include <maya/MFnSpotLight.h>

#include "light.h"

namespace pbrt {
	
	class SpotLight : public Light {
		public:
			SpotLight(const MDagPath &dagPath) : Light(dagPath), light(dagPath) {}
			~SpotLight() {}
						
		protected:
			void Insert(std::ostream& fout) const;
			MFnSpotLight light;
	};

}

#endif
