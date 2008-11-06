/*
 *  light.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _PBRTLIGHT_H
#define _PBRTLIGHT_H

#include <iostream>
#include <maya/MDagPath.h>
#include <maya/MStatus.h>

namespace pbrt {
	
	class Light {
		public:
			Light(const MDagPath &_dagPath) :  dagPath(_dagPath) { }
			virtual ~Light() {}
			
			// overload the insertion operator for easy output of the objects
			friend std::ostream& operator<<(std::ostream& fout, Light &Light);
			
			static Light* LightFactory(const MDagPath &dagPath);
			
		protected:
			MDagPath dagPath;
			
			virtual void Insert(std::ostream& fout) const = 0;
			MStatus TranslationMatrix(std::ostream& fout) const;
			
	};

}

#endif
