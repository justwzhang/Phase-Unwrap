#ifndef _CAPTURE_PHASE_UNWRAPPER_H_
#define _CAPTURE_PHASE_UNWRAPPER_H_

#include <vector>
#include <string>
#include <fstream>

#include "BasePhaseUnwrapper.h"


const int capture_phase_image_num = 3;

#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "Geometry\Point.h"


/*
*	Difference Phase Unwrapper
*/
class CCapturePhaseUnwrapper : public CBasePhaseUnwrapper
{

public:
	CCapturePhaseUnwrapper(int width = 1600, int height = 1200, int nRaws = 3):
	CBasePhaseUnwrapper(width,height,nRaws){};

	~CCapturePhaseUnwrapper() {};
	void LoadImgFromMem(unsigned char* arr[]);

	std::string& label() { return m_label; };

public:

	void compute_modulation();
	void compute_ambient();
	void compute_phase();
	
protected:
	std::string m_label;

};

#endif
