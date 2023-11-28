#ifndef _HORIZONTAL_VERTICAL_STEREO_MATCH_H_
#define _HORIZONTAL_VERTICAL_STEREO_MATCH_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Geometry/line.h"
#include "LineBundle.h"
#include "StereoConfiguration.h"
#include "debugger.h"

#include <Eigen/Core> 
#include <Eigen/Dense>

#ifndef PI
#define PI 3.1415926535897938979323846
#endif

#ifndef TWOPI
#define TWOPI PI*2.0
#endif

/*
*	StereoMatcher
*/
class CHorizontalVerticalStereoMatcher
{

public:
	CHorizontalVerticalStereoMatcher(int width = 1600, int height = 1200);
	~CHorizontalVerticalStereoMatcher();
	/*
	 *	stereo-match two gray cameras
	 */
	void match( CStereoConfiguration & configuration );

	/*
	*	Load camera calibration files
	*/
	void LoadLineBundleFromFile(
		std::string left_line_bundle_file_name,
		std::string right_line_bundle_file_name);

	/*
	*	load unwrapped phases from files
	*/
	void LoadUnwrappedPhaseFromFile(
		std::string left_horizontal_unwrapped_phase,
		std::string left_vertical_unwrapped_phase,
		std::string right_horizontal_unwrapped_phase,
		std::string right_vertical_unwrapped_phase);

	void LoadAmbientFromFile(std::string left_ambient, std::string right_ambient);
	//void LoadTextureAmbientFromFile(std::string left_texture, std::string left_ambient, std::string right_ambient);
	void LoadModulationFromFile(std::string left_ambient, std::string right_ambient);
	
	//classify low quality pixels as background, high quality pixels as foreground
	void _filter_noise(
		unsigned char ambient_threshold, 
		unsigned char modulataion_threshold);

	//write foreground, color, points to file
	void _points_to_obj(std::string filename);
	
protected:


	//convert pixel index to a point in Screen coordinates
	MeshLib::CPoint index2point(int i, int j);

	//map camera pixels to the projector image
	void _map_camera_to_projector(int id);

	//match the camera pixels registered to the same projector pixel
	void _match_one_projector_pixel(
		int u, int v,
		int disparity_threshold);


	//match camera pixel
	//left camera pixel (left_i,left_j)
	//projector pixel (u,v)
	void _match_one_camera_pixel(
		int left_i, int left_j,
		int u, int v,
		int disparity_threshold);

	//convert unwrapped phases (phase_x,phase_y) 
	//to projector coordinates (u,v)
	void _unwrapped_phase_to_projector_coord(
		double phase_x, double phase_y, 
		int& u, int& v);

protected:

	//horizaontal unwrapped phase
	double* m_pHorizontalUnwrappedPhase[2];
	//vertical unwrapped phase
	double* m_pVerticalUnwrappedPhase[2];

	unsigned char* m_pAmbient[2];
	unsigned char* m_pModulation[2];

	//visualizing the pixel classifications
	unsigned char* m_pBuffer[2];
	long m_width;
	long m_height;
	double m_disparity_threshold;
	
	MeshLib::CPoint* m_points;
	//MeshLib::CPoint* m_normals;
	std::string m_label;

	bool* m_foreground[2];
	CLineBundle * m_line_bundle[2];

	//the map from the projector image to the 
	//right image
	int  m_projector_width;
	int  m_projector_height;
	std::vector<int> * m_projector_image[2];

};

#endif


