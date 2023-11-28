#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include "debugger.h"

static const int RecordBufferSize = 15;

//buffer to raw images
class CFrameBuffer
{
public:

	CFrameBuffer(int gray_rows = 1200, int gray_cols = 1600, int numImages = 42);
	~CFrameBuffer();

	/*
	 *	Load frame buffers form files
	 *
	 */
	void load_gray_calibration_fringe_images(const std::string filename);
	void load_gray_capture_fringe_images(const std::string  filename);
	/*
	*	Load from rgb image files
	*
	*/
	void load_rgb_calibration_fringe_images(const std::string filename);
	void load_rgb_capture_fringe_images(const std::string  filename);

public:
	/*
	 *	frame buffers
	 *
	 */
	//unsigned char* RecordBuffer[RecordBufferSize];
	unsigned char* m_pBuffer[42];
	int center_x() { return m_center_x; };
	int center_y() { return m_center_y; };

protected:
	/* gray camera image size */
	int  ROWS, COLS;
	int  m_center_x;
	int  m_center_y;
	int  m_numImages;
};
#endif

