#ifndef _HORIZONTAL_VERTICAL_MULTIPLE_WAVELENGTH_PHASE_UNWRAPPER_H_
#define _HORIZONTAL_VERTICAL_MULTIPLE_WAVELENGTH_PHASE_UNWRAPPER_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <complex>

#include "FrameBuffer.h"
#include "CapturePhaseUnwrapper.h"

const int multi_wave_length_phase_num = 14;
/*
*	Horizontal Vertical Wave Length Phase Unwrapper
*/
class CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper
{
public:
	CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper(int width = 1600, int height = 1200, int nPeriods = 14)
		:m_frame_buffer( width, height, nPeriods * 3),
		m_width(width),
		m_height(height),
		m_nPeriods( nPeriods )
	{};

	~CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper() {};

	//given list of image files and the label, compute the unwrapped phase
	void phase_unwrap(char* file_name, char * label);
	

protected:
	void _phase_unwrap(std::vector<double*> wrapped_phases, std::vector<double*> unwrapped_phases);

	double* m_pWrappedPhase[multi_wave_length_phase_num];
	double* m_pUnwrappedPhase[multi_wave_length_phase_num];

	long m_width;
	long m_height;
	int  m_nPeriods;

protected:

	//double wavelength phase unwrap
	//return wrap count
	int __double_wavelength_phase_unwrap(
		double unwrapped_phase,  //unwrapped phase with wavelength 2lambda
		double wrapped_phase	 //wrapped phase with wavelength lambda
	);

protected:

	CFrameBuffer m_frame_buffer;
	std::vector<CCapturePhaseUnwrapper*> m_phase_unwrappers;
};
#endif


