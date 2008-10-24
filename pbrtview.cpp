/*
 *  pbrtview.cpp
 *  mayapbrt
 *
 *  Created by Mark Colbert on 10/18/04.
 *  Copyright 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "pbrtview.h"

#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <stdio.h>

typedef struct {
	// data for the decoder
	unsigned char idlength;
	unsigned char datatypecode;
	unsigned char colormaptype;
	unsigned char colormapdepth;
	short int colormaplength;
	
	// data for texture
	short int width;
	short int height;
	unsigned char bitdepth;	
	RV_PIXEL *data;
} Tga;

void* PBRTView::creator() {
	return new PBRTView;
}

MStatus PBRTView::doIt(const MArgList &args) {
	Tga tgaFile;
	MStatus status;
	
	if (args.length() < 1) {
		MGlobal::displayError("PBRTView must have a valid targa file to launch");
		return MStatus::kFailure;
	}

	if (!MRenderView::doesRenderEditorExist())
	{
		setResult( "Cannot PBRTView in batch render mode. "
				   "Please run in interactive mode, "
				   "so that the render editor exists." );
		return MS::kFailure;
	}
		
	MString file = args.asString(0, &status);
	if (!status) {
		MGlobal::displayError("Error parsing argument");
		return MStatus::kFailure;
	}

	// TGA Parser
	// the following will parse a targa of type 2 or 10 (uncompressed or RLE)

	FILE* fptr = fopen(file.asChar(), "rb");
	if (fptr == NULL) {
		MGlobal::displayError("Could not open file");
		return MStatus::kFailure;
	}

	char headerbytes[18];
	for (int j=0; j < 18; j++) 
		headerbytes[j] = fgetc(fptr);

	tgaFile.idlength = headerbytes[0];
	tgaFile.colormaptype = headerbytes[1];
	tgaFile.datatypecode = headerbytes[2];
	tgaFile.colormaplength = ((((short) headerbytes[6]) << 8) | 0x00FF) & (((short) headerbytes[5]) | 0xFF00);
	tgaFile.width = ((((short) headerbytes[13]) << 8) | 0x00FF) & (((short) headerbytes[12]) | 0xFF00);
	tgaFile.height = ((((short) headerbytes[15]) << 8) | 0x00FF) & (((short) headerbytes[14]) | 0xFF00);
	tgaFile.bitdepth = headerbytes[16];

	tgaFile.data = (RV_PIXEL*) malloc(sizeof(RV_PIXEL)*tgaFile.width*tgaFile.height);

	// if anything is not up to par, just throw an exception from the parser
	if (tgaFile.data == NULL) {
		fclose(fptr);
		MGlobal::displayError("Unable to malloc memory");
		return MStatus::kFailure;
	}
	if (tgaFile.datatypecode != 2 && tgaFile.datatypecode != 10) {
		fclose(fptr);
		MGlobal::displayError("Can only handle image type 2 and 10");
		return MStatus::kFailure;
	}
	if (tgaFile.bitdepth != 24 && tgaFile.bitdepth != 32) {
		fclose(fptr);
		MGlobal::displayError("Can only handle pixel depths of 24 and 32");
		return MStatus::kFailure;
	}
	if (tgaFile.colormaptype != 0 && tgaFile.colormaptype != 1) {
		fclose(fptr);
		MGlobal::displayError("Can only handle colour map types of 0 and 1");
		return MStatus::kFailure;
	}


	unsigned int skipover = tgaFile.idlength;
	skipover += tgaFile.colormapdepth*tgaFile.colormaplength;
	fseek(fptr, skipover, SEEK_CUR);

	unsigned int byteCount = tgaFile.bitdepth / 8;
	unsigned char p[5];

	int n =0;
	while (n < tgaFile.width*tgaFile.height) {
		if (tgaFile.datatypecode == 2) {                     /* Uncompressed */
			if (fread(p,1,byteCount,fptr) != byteCount) {
				fclose(fptr);
				free(tgaFile.data);
				MGlobal::displayError("Unexpected end of file");
				return MStatus::kFailure;
			}
			
			tgaFile.data[n].r = p[2];
			tgaFile.data[n].g = p[1];
			tgaFile.data[n].b = p[0];
			tgaFile.data[n].a = p[3];
			
			if (byteCount == 3) tgaFile.data[n].a = 255;
			n++;
		} else if (tgaFile.datatypecode == 10) {             /* Compressed */
			if (fread(p,1,byteCount+1,fptr) != byteCount+1) {
				fclose(fptr);
				MGlobal::displayError("Unexpected end of file");
				return MStatus::kFailure;
			}
			int j = p[0] & 0x7f;
			
			tgaFile.data[n].r = p[3];
			tgaFile.data[n].g = p[2];
			tgaFile.data[n].b = p[1];
			tgaFile.data[n].a = (byteCount == 4)?p[4]:255;
			n++;
			
			if (p[0] & 0x80) {         /* RLE chunk */
				for (int i=0;i<j;i++) {
					tgaFile.data[n].r = p[3];
					tgaFile.data[n].g = p[2];
					tgaFile.data[n].b = p[1];
					tgaFile.data[n].a = (byteCount == 4)?p[4]:255;
					n++;
				}
			} else {                   /* Normal chunk */
				for (int i=0;i<j;i++) {
					if (fread(p,1,byteCount,fptr) != byteCount) {
						fclose(fptr);				   
						MGlobal::displayError("Unexpected end of file");
						return MStatus::kFailure;
					}
					tgaFile.data[n].r = p[3];
					tgaFile.data[n].g = p[2];
					tgaFile.data[n].b = p[1];
					tgaFile.data[n].a = (byteCount == 4)?p[4]:255;				   					
					n++;
				}
			}
		}
	}
	
	// now that we have the data put it on the screen
	if (MRenderView::startRender( tgaFile.width, tgaFile.height, false) != MS::kSuccess)
	{
		MGlobal::displayError( "PBRTView: error occured in startRender" );
		free(tgaFile.data);
		return MS::kFailure;
	}
	
	if (MRenderView::updatePixels(0, tgaFile.width-1, 0, tgaFile.height-1, tgaFile.data) != MS::kSuccess)
	{
		MGlobal::displayError( "PBRTView: error occured in updatePixels." );
		free(tgaFile.data);
		return MS::kFailure;
	}

	// release the data
	free(tgaFile.data);

	if (MRenderView::refresh(0, tgaFile.width-1, 0, tgaFile.height-1) != MS::kSuccess)
	{
		MGlobal::displayError( "PBRTView: error occured in refresh." );
		return MS::kFailure;
	}
	
	if (MRenderView::endRender() != MS::kSuccess)
	{
		MGlobal::displayError( "PBRTView: error occured in endRender." );
		return MS::kFailure;
	}
	
	return MStatus::kSuccess;
	
	
}
