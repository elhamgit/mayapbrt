// Main Maya-PBRT Loader
// Mark Colbert

#include "pbrtexport.h"
#include "launchpbrt.h"
#include "pbrtview.h"

#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>


MStatus initializePlugin( MObject obj )
{ 
	MStatus status;

	// load the plugins
	MFnPlugin plugin ( obj, "Mark Colbert", "7.0", "Any" );
	status = plugin.registerCommand( "pbrtexport", pbrt::Export::creator );
	if ( !status )
		status.perror("registerCommand");

	status = plugin.registerCommand( "pbrtview", PBRTView::creator );
	if ( !status )
		status.perror("registerCommand");

	status = plugin.registerCommand( "launchpbrt", LaunchPBRT::creator );
	if ( !status )
		status.perror("registerCommand");

	// create the renderer
	MGlobal::executeCommand("global proc pbrtRender(int $width, int $height, string $doShadows, string $arg4, string $cameraname, string $layerargs ) { "
							"	string $location = `workspace -q -rd`;						"
							"	string $output = $location + \"/temp.pbrt\";				"
							"	string $tga = $location + \"/pbrt.tga\";					"
							"	pbrtexport $output $tga $width $height $cameraname;			"
							"	launchpbrt ($output);										"
							"	pbrtview $tga;												"
							"}																"
							"renderer -rendererUIName \"PBRT Renderer\"						"
							"		  -renderProcedure \"pbrtRender\"						"
							"		   pbrt;												");


	MGlobal::executeCommand("renderer -edit -addGlobalsTab \"Common\" \"createMayaSoftwareCommonGlobalsTab\" \"updateMayaSoftwareCommonGlobalsTab\" pbrt;");

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin plugin( obj );
	status = plugin.deregisterCommand( "pbrtexport" );
	if ( ! status )
		status.perror("deregisterCommand");

	status = plugin.deregisterCommand( "pbrtview" );
	if ( ! status )
		status.perror("deregisterCommand");

	status = plugin.deregisterCommand( "launchpbrt" );
	if ( ! status )
		status.perror("deregisterCommand");

	MGlobal::executeCommand("renderer -unregisterRenderer pbrt;");

	return status;
}