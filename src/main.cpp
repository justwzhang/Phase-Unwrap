#include "HorizontalVerticalMultipleWaveLengthPhaseUnwrapper.h"
#include "HorizontalVerticalStereoMatcher.h"
#include "StereoConfiguration.h"

/*
*	gray camera, phase unwrap, horizontal 7 periods, vertical 7 periods; 
*   each period has 3 fringe images
*/
void horizontal_vertical_multiple_wavelength_phase_unwrap(
	int width, int height,
	char* fringe_image_files,
	char* label)
{
	CHorizontalVerticalMultipleWaveLengthPhaseUnwrapper mw_phase_unwrapper(width, height,14);
	mw_phase_unwrapper.phase_unwrap(fringe_image_files, label);
}

/*!
*	gray camera stereo matching
*/
void horizontal_vertical_stereo_match(
	int width, int height,
	std::string configuration_file)
{
	CStereoConfiguration configure;
	configure.read_from_file(configuration_file);
	CHorizontalVerticalStereoMatcher matcher(width, height);
	matcher.match(configure);
	matcher._points_to_obj(configure.output_obj);
}



int main(int argc, char* argv[]) {

	int width = 1600, height = 1200;
	std::string cmd(argv[1]);

	//capture step 1
	if (cmd == "-horizontal_vertical_multiple_unwrap_phase") {
		std::cout << argv[0] << " -horizontal_vertical_multiple_unwrap_phase fringe_image_list.txt unwrapped_phase_label [width height]" << std::endl;

		if (argc == 6) {
			width = atoi(argv[4]);
			height = atoi(argv[5]);
		}

		horizontal_vertical_multiple_wavelength_phase_unwrap(width, height, argv[2], argv[3]);
		return 0;
	}


	//capture step 2
	if (cmd == "-horizontal_vertical_stereo_match")
	{
		std::cout << argv[0] << " -horizontal_vertical_stereo_match "
			<< "left_ambient.png right_ambient.png "
			<< "left_modulation.png right_modulation.png "
			<< "left_horizontal_unwrapped_phase.bin left_vertical_unwrapped_phase.bin "
			<< "right_horizontal_unwrapped_phase.bin right_vertical_unwrapped_phase.bin "
			<< "left_line_bundle.bin right_line_bundle.bin "
			<< "reconstructed_mesh.obj "
			<< "reconstructed_mesh.bin "
			<< "ambient_threshold "
			<< "reliability_threshold ";

		if (argc == 5) {
			width = atoi(argv[3]);
			height = atoi(argv[4]);
		}

		horizontal_vertical_stereo_match(
			width, height, argv[2]);

		return 0;
	}

	return 0;
}