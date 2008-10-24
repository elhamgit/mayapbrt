
#ifndef _PBRT_VIEW_H
#define _PBRT_VIEW_H

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MStatus.h>

class PBRTView: public MPxCommand
{
public:
	PBRTView() {}
	virtual	~PBRTView() {}
	
	static void* creator();
	virtual MStatus	doIt( const MArgList& );
};

#endif
