#include "HorizontalVerticalStereoMatcher.h"

void CHorizontalVerticalStereoMatcher::LoadLineBundleFromFile(
	std::string left_line_bundle_file_name,
	std::string right_line_bundle_file_name
	)
{
	m_line_bundle[0]->read_from_file(left_line_bundle_file_name);
	m_line_bundle[1]->read_from_file(right_line_bundle_file_name);
}

//load unwrapped phase from file
void CHorizontalVerticalStereoMatcher::LoadUnwrappedPhaseFromFile(
	std::string left_horizontal_unwrapped_phase, 
	std::string left_vertical_unwrapped_phase,
	std::string right_horizontal_unwrapped_phase,
	std::string right_vertical_unwrapped_phase)
{
	{
		std::ifstream rf(left_horizontal_unwrapped_phase, std::ios::in | std::ios::binary);
		if (!rf) {
			std::cout << "Cannot open file!" << std::endl;
			return;
		}
		int width, height;
		rf.read((char*)&width, sizeof(int));
		rf.read((char*)&height, sizeof(int));
		rf.read((char*)m_pHorizontalUnwrappedPhase[0], sizeof(double) * width * height);
		rf.close();
		if (!rf.good()) {
			std::cout << "Error occurred at writing time!" << std::endl;
			return;
		}
		__visualize_normalize(m_pHorizontalUnwrappedPhase[0], 
			height, width, "Left Horizontal Unwrapped Phase");
	}

	{
		std::ifstream rf(left_vertical_unwrapped_phase, std::ios::in | std::ios::binary);
		if (!rf) {
			std::cout << "Cannot open file!" << std::endl;
			return;
		}
		int width, height;
		rf.read((char*)&width, sizeof(int));
		rf.read((char*)&height, sizeof(int));
		rf.read((char*)m_pVerticalUnwrappedPhase[0], sizeof(double) * width * height);
		rf.close();
		if (!rf.good()) {
			std::cout << "Error occurred at writing time!" << std::endl;
			return;
		}
		__visualize_normalize(m_pVerticalUnwrappedPhase[0], 
			height, width, "Left Vertical Unwrapped Phase");
	}

	{
		std::ifstream rf(right_horizontal_unwrapped_phase, std::ios::in | std::ios::binary);
		if (!rf) {
			std::cout << "Cannot open file!" << std::endl;
			return;
		}
		int width, height;
		rf.read((char*)&width, sizeof(int));
		rf.read((char*)&height, sizeof(int));
		rf.read((char*)m_pHorizontalUnwrappedPhase[1], sizeof(double) * width * height);
		rf.close();
		if (!rf.good()) {
			std::cout << "Error occurred at writing time!" << std::endl;
			return;
		}
		__visualize_normalize(m_pHorizontalUnwrappedPhase[1], 
			height, width, "Right Horizontal Unwrapped Phase");
	}

	{
		std::ifstream rf(right_vertical_unwrapped_phase, std::ios::in | std::ios::binary);
		if (!rf) {
			std::cout << "Cannot open file!" << std::endl;
			return;
		}
		int width, height;
		rf.read((char*)&width, sizeof(int));
		rf.read((char*)&height, sizeof(int));
		rf.read((char*)m_pVerticalUnwrappedPhase[1], sizeof(double) * width * height);
		rf.close();
		if (!rf.good()) {
			std::cout << "Error occurred at writing time!" << std::endl;
			return;
		}
		__visualize_normalize(m_pVerticalUnwrappedPhase[1], 
			height, width, "Right Vertical Unwrapped Phase");
	}
}


void CHorizontalVerticalStereoMatcher::_filter_noise(
	unsigned char ambient_threshold, 
	unsigned char  modulation_threshold)
{
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++) {

			int idx = i * m_width + j;
			unsigned char c = m_pAmbient[0][idx];
			m_pBuffer[0][idx * 3 + 0] = c;
			m_pBuffer[0][idx * 3 + 1] = c;
			m_pBuffer[0][idx * 3 + 2] = c;
			m_foreground[0][idx] = true;
			
			if (c < ambient_threshold) {
				m_pBuffer[0][idx * 3 + 0] = 255;
				m_pBuffer[0][idx * 3 + 1] = 0;
				m_pBuffer[0][idx * 3 + 2] = 0;
				m_foreground[0][idx] = false;
			}
			if (m_foreground[0][idx] && 
				m_pModulation[0][idx] < modulation_threshold) {
				m_pBuffer[0][idx * 3 + 0] = 0;
				m_pBuffer[0][idx * 3 + 1] = 255;
				m_pBuffer[0][idx * 3 + 2] = 0;
				m_foreground[0][idx] = false;
			}

		}

	__visualize(m_pBuffer[0], m_height, m_width, 3, "Left Foreground");

	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++) {

			int idx = i * m_width + j;
			unsigned char c = m_pAmbient[1][idx];
			m_pBuffer[1][idx * 3 + 0] = c;
			m_pBuffer[1][idx * 3 + 1] = c;
			m_pBuffer[1][idx * 3 + 2] = c;
			m_foreground[1][idx] = true;

			if (c < ambient_threshold) {
				m_pBuffer[1][idx * 3 + 0] = 255;
				m_pBuffer[1][idx * 3 + 1] = 0;
				m_pBuffer[1][idx * 3 + 2] = 0;
				m_foreground[1][idx] = false;
			}
			if (m_foreground[1][idx] &&
				m_pModulation[1][idx] < modulation_threshold) {
				m_pBuffer[1][idx * 3 + 0] = 0;
				m_pBuffer[1][idx * 3 + 1] = 255;
				m_pBuffer[1][idx * 3 + 2] = 0;
				m_foreground[1][idx] = false;
			}
		}

	__visualize(m_pBuffer[1], m_height, m_width, 3, "Right Foreground");

}

void CHorizontalVerticalStereoMatcher::LoadAmbientFromFile(std::string left_ambient, std::string right_ambient)
{
	//gray images for the left side
	cv::Mat  _image;
	_image = cv::imread(left_ambient.c_str(), cv::IMREAD_GRAYSCALE);
	cv::imshow(left_ambient.c_str(), _image);
	cv::waitKey(0);
	memcpy((void*)m_pAmbient[0], (void*)_image.data, sizeof(unsigned char) * m_width * m_height);
	{
		cv::Mat  _image;
		_image = cv::imread(right_ambient.c_str(), cv::IMREAD_GRAYSCALE);
		cv::imshow(right_ambient.c_str(), _image);
		cv::waitKey(0);
		memcpy((void*)m_pAmbient[1], (void*)_image.data, sizeof(unsigned char) * m_width * m_height);
	}
}

void CHorizontalVerticalStereoMatcher::LoadModulationFromFile(std::string left_modulation, std::string right_modulation)
{
	//gray images for the left side
	cv::Mat  _image;
	_image = cv::imread(left_modulation.c_str(), cv::IMREAD_GRAYSCALE);
	cv::imshow(left_modulation.c_str(), _image);
	cv::waitKey(0);
	memcpy((void*)m_pModulation[0], (void*)_image.data, sizeof(unsigned char) * m_width * m_height);
	{
		cv::Mat  _image;
		_image = cv::imread(right_modulation.c_str(), cv::IMREAD_GRAYSCALE);
		cv::imshow(right_modulation.c_str(), _image);
		cv::waitKey(0);
		memcpy((void*)m_pModulation[1], (void*)_image.data, sizeof(unsigned char) * m_width * m_height);
	}
}

CHorizontalVerticalStereoMatcher::CHorizontalVerticalStereoMatcher(int width, int height)
{
	m_width = width;
	m_height = height;
	m_points = new MeshLib::CPoint[width * height];
	if (m_points == NULL) {
		std::cerr << "Error in memory allocation" << std::endl;
		return;
	}

	for (int i = 0; i < 2; i++) {
		m_pHorizontalUnwrappedPhase[i] = new double[m_width * m_height];
		if (m_pHorizontalUnwrappedPhase[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
		}
		m_pVerticalUnwrappedPhase[i] = new double[m_width * m_height];
		if (m_pVerticalUnwrappedPhase[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
		}
	}

	for (int i = 0; i < 2; i++) {
		m_pAmbient[i] = new unsigned char[m_width * m_height];
		if (m_pAmbient[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
		}
	}
	
	for (int i = 0; i < 2; i++) {
		m_pModulation[i] = new unsigned char[m_width * m_height];
		if (m_pModulation[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
		}
	}

	for (int i = 0; i < 2; i++) {
		m_foreground[i] = new bool[m_width * m_height];
		if (m_foreground[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
		}
	}

	for (int i = 0; i < 2; i++) {
		m_line_bundle[i] = new CLineBundle(m_width, m_height);
		if (m_line_bundle[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
			return;
		}
	}

	//1140x912;
	m_projector_width = 1140;
	m_projector_height = 912;

	for (int i = 0; i < 2; i++) {
		m_projector_image[i] = new std::vector<int>[m_projector_width * m_projector_height];
		if (m_projector_image[i] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
			return;
		}
	}

	for (int k = 0; k < 2; k++) {
		m_pBuffer[k] = new unsigned char[m_width * m_height * 3];
		if (m_pBuffer[k] == NULL) {
			std::cerr << "Error in memory allocation" << std::endl;
			return;
		}
	}
}

CHorizontalVerticalStereoMatcher::~CHorizontalVerticalStereoMatcher() {
	
	delete []m_points;
	
	for (int i = 0; i < 2; i++) {
		delete[](m_pHorizontalUnwrappedPhase[i]);
		delete[](m_pVerticalUnwrappedPhase[i]);
	}
	for (int i = 0; i < 2; i++) {
		delete[](m_pAmbient[i]);
	}
	for (int i = 0; i < 2; i++) {
		delete[](m_pModulation[i]);
	}

	for (int i = 0; i < 2; i++) {
		delete[](m_foreground[i]);
	}
	for (int i = 0; i < 2; i++) {
		delete m_line_bundle[i];
	}
	
	for (int i = 0; i < 2; i++) {
		delete[]m_projector_image[i];
	}

	for (int k = 0; k < 2; k++) {
		delete[]m_pBuffer[k];
	}
}

void CHorizontalVerticalStereoMatcher::_unwrapped_phase_to_projector_coord(
	double phase_x, double phase_y, int & u, int & v)
{
	double x_lambda = (double)m_projector_width / 128.0;
	u = (int)(phase_x / (2 * PI) * x_lambda + 0.5);
	double y_lambda = (double)m_projector_height / 128.0;
	v = (int)(phase_y / (2 * PI) * y_lambda + 0.5);
}

//map from camera image to the projector image
void CHorizontalVerticalStereoMatcher::_map_camera_to_projector(int id) {
	int u, v;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {

			int idx = i * m_width + j;
			if (!m_foreground[id][idx]) continue;

			double phase_x = m_pHorizontalUnwrappedPhase[id][idx];
			double phase_y = m_pVerticalUnwrappedPhase[id][idx];
			_unwrapped_phase_to_projector_coord(phase_x, phase_y, u, v);
			m_projector_image[id][v * m_projector_width + u].push_back(idx);
		}
	}
}

/*
 *	match the pixel (left_i,left_j) on the left image to the pixel on the right image
 *  with minimal absolute phase distance 
 */
void CHorizontalVerticalStereoMatcher::_match_one_camera_pixel(int left_i,int left_j, int u, int v, 
	int disparity_threshold) {

	int neighbors[9][2] = { {-1,-1},{0,-1},{1,-1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1} };

	//the pixel index on the right image corresponding to (left_i,left_j)
	int    min_right_camera_idx = 0;
	//the minimal absolute phase difference
	double min_phase_error = 1e+20;

	double left_phase_x = m_pHorizontalUnwrappedPhase[0][left_i * m_width + left_j];
	double left_phase_y = m_pVerticalUnwrappedPhase[0][left_i * m_width + left_j];

	//insert your code here
	double maxPhase = abs(left_phase_x - m_pHorizontalUnwrappedPhase[1][0]) + abs(left_phase_y - m_pHorizontalUnwrappedPhase[1][1]);

	for (int i = 1; i < sizeof(m_pHorizontalUnwrappedPhase[1]); i++) {
		double currPhase = abs(left_phase_x - m_pHorizontalUnwrappedPhase[1][0]) + abs(left_phase_y - m_pHorizontalUnwrappedPhase[1][1]);
		if (currPhase > maxPhase) {
			maxPhase = currPhase;
			min_right_camera_idx = i;
		}
	}


	//min phase error 
	if (min_phase_error > disparity_threshold){
		std::cout << ".";
		int left_camera_idx = left_i * m_width + left_j;
		m_foreground[0][left_camera_idx] = false;
		m_pBuffer[0][left_camera_idx * 3 + 0] = 0;
		m_pBuffer[0][left_camera_idx * 3 + 1] = 255;
		m_pBuffer[0][left_camera_idx * 3 + 2] = 255;		
	}

	//compute the intesection point between two rays
	MeshLib::CLine & left_line  = m_line_bundle[0]->lines()[left_i * m_width + left_j];
	MeshLib::CLine & right_line = m_line_bundle[1]->lines()[min_right_camera_idx];

	CLineRegression LR;
	std::vector<MeshLib::CLine> ls;
	ls.push_back(left_line);
	ls.push_back(right_line);
	MeshLib::CPoint p = LR.intersect(ls);

	m_points[left_i * m_width + left_j] = p;
}

void CHorizontalVerticalStereoMatcher::_match_one_projector_pixel(
	int u, int v,
	int disparity_threshold)
{
	int idx = v * m_projector_width + u;
	if (m_projector_image[0][idx].empty()) return;

	for (auto left_idx : m_projector_image[0][idx]) {

		int i = left_idx / m_width;
		int j = left_idx % m_width;
		_match_one_camera_pixel(i, j, u, v, disparity_threshold);
	}
}


/*
 *	stereo-match two gray cameras
 */
void CHorizontalVerticalStereoMatcher::match(CStereoConfiguration& configure)
{
	//load ambient, modulation files
	LoadAmbientFromFile(
		configure.ambient_image[0],
		configure.ambient_image[1]);
	LoadModulationFromFile(
		configure.modulation_image[0],
		configure.modulation_image[1]);
	//load unwrapped phase files
	LoadUnwrappedPhaseFromFile(
		configure.horizontal_unwrapped_phase[0],
		configure.vertical_unwrapped_phase[0],
		configure.horizontal_unwrapped_phase[1],
		configure.vertical_unwrapped_phase[1]);
	//load camera calibration files
	LoadLineBundleFromFile(
		configure.line_bundle[0],
		configure.line_bundle[1]);
	//filter out low quality pixels
	_filter_noise(
		configure.ambient_threshold,
		configure.modulation_threshold);

	
	for (int k = 0; k < 2; k++) {
		_map_camera_to_projector(k);
	}
	//quad tree of the right image pixels to the projector image plane
	for (int v = 0; v < m_projector_height; v++) {
		for (int u = 0; u < m_projector_width; u++) {
			_match_one_projector_pixel(u, v, configure.disparity_threshold);
		}
	}
	__visualize(m_pBuffer[0], m_height, m_width, 3, "Mismatched Pixels");

}


void CHorizontalVerticalStereoMatcher::_points_to_obj(std::string file_name)
{
	std::ofstream fs;
	fs.open(file_name);
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++) {

			int idx = i * m_width + j;
			if (!m_foreground[0][idx]) continue;
			MeshLib::CPoint& pt = m_points[idx];
			fs << "v " << pt[0] << " " << pt[1] << " " << pt[2] << " ";
			unsigned char c = m_pAmbient[0][idx];
			int cr = c;
			fs << cr << " " << cr << " " << cr << std::endl;
		}
	fs.close();
}

