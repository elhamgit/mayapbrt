/*
 *  pbrtexport.h
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/5/04.
 *  Copyright 2004 University of Central Florida. All rights reserved.
 *
 */

#ifndef _PBRTEXPORTER_H
#define _PBRTEXPORTER_H

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MFnDagNode.h>

namespace pbrt {

	class Export: public MPxCommand
	{
	public:
		Export() {}
		virtual	~Export() {}
		static void* creator();
		virtual MStatus	doIt( const MArgList& );

	private:
		bool			isVisible(MFnDagNode & fnDag, MStatus *status = NULL);
	};

}

#endif

