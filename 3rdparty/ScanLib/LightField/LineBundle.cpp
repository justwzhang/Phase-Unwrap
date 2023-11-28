#include "LineBundle.h"

void CLineBundle::read_from_file(std::string filename)
{
	std::ifstream rf(filename, std::ios::in | std::ios::binary);
	if (!rf) {
		std::cout << "Cannot open file!" << std::endl;
		return;
	}
	rf.read((char*)&m_width, sizeof(int));
	rf.read((char*)&m_height, sizeof(int));
	std::cout << "(Width, Height) (" << m_width << "," << m_height << ")" << std::endl;

	double buf[3];
	rf.read((char*)buf, sizeof(double) * 3);
	for (int i = 0; i < 3; i++) {
		m_optical_center[i]=buf[i];
	}
	std::cout << "Optical Center (" << m_optical_center[0] << "," << m_optical_center[1] <<"," << m_optical_center[2] << ")" << std::endl;

	double* line_ptr = new double[m_width * m_height * 6];
	rf.read((char*)line_ptr, sizeof(double) * 6 * m_width * m_height);
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++) {
			int idx = i * m_width + j;
			double* ptr = line_ptr + 6 * idx;

			MeshLib::CLine& L = m_lines[idx];
			L.base()[0] = ptr[0]; L.base()[1] = ptr[1]; L.base()[2] = ptr[2];
			L.direction()[0] = ptr[3]; L.direction()[1] = ptr[4]; L.direction()[2] = ptr[5];
		}
	delete[]line_ptr;
	
	rf.close();
	if (!rf.good()) {
		std::cout << "Error occurred at reading time!" << std::endl;
		return;
	}
}

