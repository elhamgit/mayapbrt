/*
 *  object.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PBRTOBJECT_H
#define _PBRTOBJECT_H

#include <iostream>

namespace pbrt {
	
	class Object {
		public:
			Object() {}
			virtual ~Object() {}
			
			// overload the insertion operator for easy output of the objects
			friend std::ostream& operator<<(std::ostream& fout, const Object &object);
			
		protected:
			virtual void Insert(std::ostream& fout) const = 0;
			
	};

}

#endif
