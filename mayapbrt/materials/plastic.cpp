/*
 *  Plastic.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/16/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "plastic.h"

namespace pbrt {	
	void Plastic::Insert(std::ostream& fout) const {
		int texcolor=0, texbump=0;
		
		texcolor = colorTexture(fout);
		texbump = bumpTexture(fout);
		
		if (texcolor) {
			fout << "Material \"plastic\" \"texture Kd\" \"" << texcolor << "\"";
			if (texbump) fout << " \"texture bumpmap\" \"" << texbump << "\"";
			fout << std::endl;
		}
		
	}

}
