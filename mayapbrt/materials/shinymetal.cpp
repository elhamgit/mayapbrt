/*
 *  shinymetal.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/11/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "shinymetal.h"

namespace pbrt {	
	void ShinyMetal::Insert(std::ostream& fout) const {
		int texcolor=0, texbump=0;
		
		texcolor = colorTexture(fout);
		texbump = bumpTexture(fout);
		
		if (texcolor) {
			fout << "Material \"shinymetal\" \"texture Kd\" \"" << texcolor << "\"";
			if (texbump)
				fout << " \"texture bumpmap\" \"" << texbump << "\"";
			fout << std::endl;
		}
		
	}

}
