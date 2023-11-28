#ifndef _LINE_BUNDLE_H_
#define _LINE_BUNDLE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "Geometry/Point.h"
#include "Geometry/Line.h"
#include "LineRegression.h"

/********************************************************************************
*	Line bundle - Light Field Camera model
*
*	In the image plane, each pixel is associated with a ray in the physical world
*
*	David Gu
*	
*	08/20/2021
* 
**********************************************************************************/

class CLineBundle
{
public:
	/*---------------------------------------------------------------------
	*	
	* Constructor of the line bundle
	* 
	*----------------------------------------------------------------------*/
	CLineBundle(int width, int height) : m_width(width), m_height(height) {

		m_lines = new MeshLib::CLine[m_width * m_height];
		if (m_lines == NULL) {
			std::cerr << "Memory Allocation Error in CLineBundle" << std::endl;
			return;
		}
	}

	/*-----------------------------------------------------------------------
	* 
	*  Destructor of the line bundle
	*
	------------------------------------------------------------------------*/
	~CLineBundle() {
		delete[]m_lines;
	}

	/*-----------------------------------------------------------------------
	*
	*	File I/O
	* 
	*-----------------------------------------------------------------------*/
	void read_from_file(std::string filename);

public:
	/*-----------------------------------------------------------------------------
	*
	*	Access the data members
	*-----------------------------------------------------------------------------*/
	int& width()  { return m_width;  };
	int& height() { return m_height; };
	MeshLib::CLine* lines() { return m_lines; };
	MeshLib::CPoint& optical_center() { return m_optical_center; };

protected:
	//the dimension of the light field camera
	int m_width, m_height;
	//two D array of rays, each ray is associated with a pixel
	MeshLib::CLine* m_lines;
	//the virtual optical center of the rays
	MeshLib::CPoint m_optical_center;
	
};
#endif
