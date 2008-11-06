
#ifndef _LAUNCH_PBRT_H
#define _LAUNCH_PBRT_H

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MStatus.h>


class LaunchPBRT: public MPxCommand
{
public:
	LaunchPBRT() {}
	virtual	~LaunchPBRT() {}
	
	static void* creator();
	virtual MStatus	doIt( const MArgList& );
};

#endif
