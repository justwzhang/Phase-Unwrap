#include "HorizontalVerticalMultipleWaveLengthPhaseUnwrapper.h"
#include "debugger.h"

void CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper::_phase_unwrap(
	std::vector<double*> wrapped_phases,
	std::vector<double*> unwrapped_phases)
{
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int idx = i * m_width + j;
			unwrapped_phases[0][idx] = wrapped_phases[0][idx];
			unwrapped_phases[7][idx] = wrapped_phases[7][idx];
		}

	__visualize_normalize_and_save(unwrapped_phases[0], m_height, m_width, "unwrapped_phase_0.png");
	__visualize_normalize_and_save(wrapped_phases[1], m_height, m_width, "wrapped_phase_1.png");

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{ 
			int idx = i * m_width + j;
			int m2 = __double_wavelength_phase_unwrap(
				unwrapped_phases[0][idx],
				wrapped_phases[1][idx]);
			unwrapped_phases[1][idx] = 2 * PI * m2 + wrapped_phases[1][idx];
		}

	__visualize_normalize_and_save(unwrapped_phases[1], m_height, m_width, "unwrapped_phase_1.png");

 	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			int idx = i * m_width + j;
			int m2 = __double_wavelength_phase_unwrap(
				unwrapped_phases[7][idx],
				wrapped_phases[8][idx]);
			unwrapped_phases[8][idx] = 2 * PI * m2 + wrapped_phases[8][idx];
		}

	__visualize_normalize_and_save(unwrapped_phases[8], m_height, m_width, "unwrapped_phase_8.png");


	for (size_t k = 0; k < 6; k++) {

		double* phi_1 = wrapped_phases[k];
		double* Phi_1 = unwrapped_phases[k];

		double* phi_2 = wrapped_phases[k + 1];
		double* Phi_2 = unwrapped_phases[k + 1];

		for (int i = 0; i < m_height; i++)
			for (int j = 0; j < m_width; j++)
			{
				int idx = i * m_width + j;
				double a = Phi_1[idx];
				double b = phi_2[idx];
				int m2 = __double_wavelength_phase_unwrap(
					Phi_1[idx], phi_2[idx]);
				Phi_2[idx] = 2 * PI * m2 + phi_2[idx];
			}

		__visualize_normalize_and_save(Phi_2, m_height, m_width, "_horizontal_unwrapped_phase.png");
	}

	for (size_t k = 7; k < 13; k++) {

		double* phi_1 = wrapped_phases[k];
		double* Phi_1 = unwrapped_phases[k];

		double* phi_2 = wrapped_phases[k + 1];
		double* Phi_2 = unwrapped_phases[k + 1];

		for (int i = 0; i < m_height; i++)
			for (int j = 0; j < m_width; j++)
			{
				int idx = i * m_width + j;
				double a = Phi_1[idx];
				double b = phi_2[idx];
				int m2 = __double_wavelength_phase_unwrap(
					Phi_1[idx],
					phi_2[idx]);
				Phi_2[idx] = 2 * PI * m2 + phi_2[idx];
			}

		__visualize_normalize_and_save(Phi_2, m_height, m_width, "_vertical_unwrapped_phase.png");
	}

}


void CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper::phase_unwrap(char* fringe_image_files, char * label)
{
	m_frame_buffer.load_gray_capture_fringe_images(std::string(fringe_image_files));

	std::vector<double*> wrapped_phases;
	std::vector<double*> unwrapped_phases;

	for (int k = 0; k < m_nPeriods; k++) {

		CCapturePhaseUnwrapper* pUnwrapper = new CCapturePhaseUnwrapper(m_width, m_height,3);
		if (pUnwrapper == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
			return;
		}
		m_phase_unwrappers.push_back(pUnwrapper);
		pUnwrapper->LoadImgFromMem(m_frame_buffer.m_pBuffer + 3 * k);
		pUnwrapper->label() = std::string(label);
		pUnwrapper->compute_modulation();
		pUnwrapper->compute_ambient();
		pUnwrapper->compute_phase();

		wrapped_phases.push_back(pUnwrapper->wrapped_phase());
		unwrapped_phases.push_back(pUnwrapper->unwrapped_phase());
	}

	_phase_unwrap(wrapped_phases, unwrapped_phases);

	m_phase_unwrappers[6]->write_unwrapped_phase_to_file(std::string(label) + "_horizontal_unwrapped_phase.bin");

	__visualize_normalize_and_save(
		m_phase_unwrappers[6]->unwrapped_phase(),
		m_height,
		m_width,
		std::string(label) + "_horizontal_unwrapped_phase.png"
	);

	m_phase_unwrappers[13]->write_unwrapped_phase_to_file(std::string(label) + "_vertical_unwrapped_phase.bin");

	__visualize_normalize_and_save(
		m_phase_unwrappers[13]->unwrapped_phase(),
		m_height,
		m_width,
		std::string(label) + "_vertical_unwrapped_phase.png"
	);

	for (auto pu : m_phase_unwrappers) {
		delete pu;
	};

}


int CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper::__double_wavelength_phase_unwrap(
	double unwrapped_phase,
	double wrapped_phase)
{
	// unwrapped_phase = L1
	// wrapped_phase = L2 = L1 / 2
	//insert your code here
	return round(unwrapped_phase/PI - wrapped_phase/(2*PI));
}

