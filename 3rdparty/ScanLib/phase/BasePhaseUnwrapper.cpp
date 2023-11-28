#include "BasePhaseUnwrapper.h"

void CBasePhaseUnwrapper::write_unwrapped_phase_to_file(std::string filename)
{
	std::ofstream wf(filename, std::ios::out | std::ios::binary);
	if (!wf) {
		std::cout << "Cannot open file!" << std::endl;
		return;
	}
	wf.write((char*)&m_width, sizeof(int));
	wf.write((char*)&m_height, sizeof(int));
	wf.write((char*)m_pUnwrappedPhase, sizeof(double) * m_width * m_height);
	wf.close();
	if (!wf.good()) {
		std::cout << "Error occurred at writing time!" << std::endl;
		return;
	}
}
