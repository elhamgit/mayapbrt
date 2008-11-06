/*
 *  launchpbrt.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/17/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include <windows.h>
#include <maya/MStatus.h>
#include <maya/MGlobal.h>
#include <maya/MProgressWindow.h>
#include <string>

using namespace std;
#include <stdlib.h>

#include "launchpbrt.h"

static bool rendering;
DWORD WINAPI render(void *arg);
DWORD WINAPI progress(void *arg);

void* LaunchPBRT::creator() {
	return new LaunchPBRT;
}

MStatus LaunchPBRT::doIt( const MArgList &args ) {
	MString arg;
	MStatus status;
	
	if (args.length() < 1) {
		MGlobal::displayError("LaunchPBRT must have an input file to launch");
		return MStatus::kFailure;
	}
	
	arg = args.asString(0, &status);
	if (!status) {
		MGlobal::displayError("Error parsing argument");
		return MStatus::kFailure;
	}
	
	// get the pbrt search path from the environment
	string searchpath = getenv("PBRT_SEARCHPATH");
	if (searchpath == "PBRT_SEARCHPATH=") { 
		MGlobal::displayError("PBRT_SEARCH path not set");
		return MStatus::kFailure;
	}
	
	// set executable path
	string executable = searchpath + "\\pbrt \"";
	executable += arg.asChar() + string("\"");	
	
	rendering = true;
	
	HANDLE renderThread, progressThread;
	DWORD renderThreadId, progressThreadId;
	
	char *exec = const_cast<char *> (executable.c_str());

	render(exec);

	// TODO: Rebuilt multi-threaded progress bar
	// If you are interested, more likely than not you should set an idle call back function
	// to read the piped data, this would allow users to actually still have control over maya
	// during rendering

		//if ((renderThread = CreateThread(NULL, 0, render, exec, 0, &renderThreadId)) == 0) {
		//	MGlobal::displayError("Unable to create render thread");
		//	return MStatus::kFailure;
		//}
		//if ((progressThread = CreateThread(NULL, 0, progress, NULL, 0, &progressThreadId)) == 0) {
		//	MGlobal::displayError("Unable to create progress thread");
		//	TerminateThread(renderThread, 0);
		//	return MStatus::kFailure;
		//}
		
		//int returnVal[1];
		//pthread_join(renderThread, (void**) &returnVal);

		//WaitForSingleObject(renderThread, INFINITE);
		//WaitForSingleObject(progress, INFINITE);

	
	return MStatus::kSuccess;
	
}

DWORD WINAPI render(void *arg) {
	cout << "Rendering Thread Output" << endl;
	cout << (const char*) arg << endl; 
	system((const char*) arg);
	rendering = false;
	return NULL;
}



// TODO: Rebuild progress bar thread
/*
DWORD WINAPI progress(void *arg) {
	MStatus status;
	
	if (!MProgressWindow::reserve())
	{
		MGlobal::displayError("Progress window already in use.");
		status = MS::kFailure;
		return NULL;
	}
	
	MProgressWindow::setProgressRange(0, 100);
	MProgressWindow::setTitle("PBRT Rendering");
	MProgressWindow::setInterruptable(false);
	MProgressWindow::setProgress(0);
	
	MProgressWindow::startProgress();
	
	int i=0;
	while (rendering) {
		i = (i+1)%100;
		MProgressWindow::setProgress(i);		
		Sleep(2000);
	}
	
	MProgressWindow::endProgress();
	
	return NULL;
}
*/
