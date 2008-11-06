/*
 *  exporterror.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EXPORTERROR_H
#define _EXPORTERROR_H

#include <string>
#include <iostream>

namespace pbrt {

	// generic export error class to be thrown around during errors during export
	// instead of numerous MStatus values being passed to and fro
	class ExportError {
		public:
			ExportError() {}
			ExportError(std::string errorDesc) : error(errorDesc) {}
			~ExportError() {}
		
			friend std::ostream& operator<< (std::ostream& fout, ExportError& errorObj) {
				fout << errorObj.error;
				return fout;
			}
			
		private:
			std::string error;
	};

}

#endif

