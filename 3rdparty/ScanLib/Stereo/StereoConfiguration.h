#ifndef _STEREO_CONFIGURATION_H_
#define _STEREO_CONFIGURATION_H_

#include <string>
#include <iostream>
#include <fstream>

class CStereoConfiguration
{
public:
	std::string ambient_image[2];
	std::string modulation_image[2];
	std::string horizontal_unwrapped_phase[2];
	std::string vertical_unwrapped_phase[2];
	std::string line_bundle[2];
	std::string output_obj;
	std::string output_bin;

	std::string texture_image;

public:
	int    ambient_threshold; //10
	double reliability_threshold; //4.5
	double modulation_threshold;  //5.0
	double disparity_threshold;   //1.5

public:
	void read_from_file(std::string file_name)
	{
		std::ifstream myfile(file_name);

		if (!myfile.is_open())
		{
			std::cerr << "Unable to open configuration file: " << file_name << std::endl;
			return;
		}

		while (myfile) {
			std::string key;
			myfile >> key;
			if (key == "left_ambient_image") {
				myfile >> ambient_image[0];
				continue;
			}
			if (key == "right_ambient_image") {
				myfile >> ambient_image[1];
				continue;
			}
			if (key == "left_modulation_image") {
				myfile >> modulation_image[0];
				continue;
			}
			if (key == "right_modulation_image") {
				myfile >> modulation_image[1];
				continue;
			}

			if (key == "left_horizontal_unwrapped_phase") {
				myfile >> horizontal_unwrapped_phase[0];
				continue;
			}

			if (key == "right_horizontal_unwrapped_phase") {
				myfile >> horizontal_unwrapped_phase[1];
				continue;
			}

			if (key == "left_vertical_unwrapped_phase") {
				myfile >> vertical_unwrapped_phase[0];
				continue;
			}

			if (key == "right_vertical_unwrapped_phase") {
				myfile >> vertical_unwrapped_phase[1];
				continue;
			}

			if (key == "left_line_bundle") {
				myfile >> line_bundle[0];
				continue;
			}

			if (key == "right_line_bundle") {
				myfile >> line_bundle[1];
				continue;
			}

			if (key == "output_obj_file") {
				myfile >> output_obj;
				continue;
			}

			if (key == "output_bin_file") {
				myfile >> output_bin;
				continue;
			}

			if (key == "ambient_threshold") {
				myfile >> ambient_threshold;
				continue;
			}
			if (key == "reliability_threshold") {
				myfile >> reliability_threshold;
				continue;
			}
			if (key == "modulation_threshold") {
				myfile >> modulation_threshold;
				continue;
			}
			if (key == "disparity_threshold") {
				myfile >> disparity_threshold;
				continue;
			}
			if (key == "texture_image") {
				myfile >> texture_image;
				continue;
			}
		}
		myfile.close();
	}
};
#endif
