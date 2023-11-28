#ifndef _CALIBRATION_DEBUGGER_H_
#define _CALIBRATION_DEBUGGER_H_

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>

//if we want to view all the intermediate process
//set _DEBUGGER_ to be 1 otherwise 0
//#define _DEBUGGER_ 0
#define _DEBUGGER_ 1

inline void _line(cv::Mat img, cv::Point start, cv::Point end)
{
    int thickness = 2;
    int lineType = cv::LINE_8;
    cv::line(img,
        start,
        end,
        cv::Scalar(0, 0, 255),
        thickness,
        lineType);
}

inline void __visualize(unsigned char* pMod, int height, int width, int channel, std::string title, int center_x, int center_y )
{
    if (!_DEBUGGER_) return;

    if (channel == 1)
    {
        cv::Mat pModulation(height, width, CV_8UC1, pMod, 0);
        cv::Mat cimg;
        cv::cvtColor(pModulation, cimg, cv::COLOR_GRAY2BGR);

        _line(cimg, cv::Point(center_x-10, center_y), cv::Point(center_x+10, center_y));
        _line(cimg, cv::Point(center_x, center_y-10), cv::Point(center_x, center_y+10));
        cv::imshow(title.c_str(), cimg);
        cv::waitKey(0);
    }
    if (channel == 3)
    {
        cv::Mat pModulation(height, width, CV_8UC3, pMod, 0);
        cv::imshow(title.c_str(), pModulation);
        cv::waitKey(0);
    }
};

inline void __visualize(unsigned char* pMod, int height, int width, int channel, std::string title)
{
    if (!_DEBUGGER_) return;

    if (channel == 1)
    {
        cv::Mat pModulation(height, width, CV_8UC1, pMod, 0);
        cv::imshow(title.c_str(), pModulation);
        cv::waitKey(0);
    }
    if (channel == 3)
    {
        cv::Mat pModulation(height, width, CV_8UC3, pMod, 0);
        cv::imshow(title.c_str(), pModulation);
        cv::waitKey(0);
    }
};

inline void __visualize(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = pMod[i * width + j] / 255;
        }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);
};

inline void __visualize(double* pMod, int height, int width, double max, double min, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = (pMod[i * width + j] - min) / (max - min);
        }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);
};

inline void __visualize_ellipse(double* pMod, int height, int width, std::vector<std::vector<double>>& ellipses)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = pMod[i * width + j] / 255;
        }

    for (size_t i = 0; i < ellipses.size(); i++)
    {
        std::vector<double>& para = ellipses[i];
        cv::Point center(para[0] / 1.25, para[1] / 1.25);
        cv::Size size(para[2] / 1.25, para[3] / 1.25);
        double angle = para[4];
        cv::Scalar rgb(255, 0, 0);

        cv::ellipse(pModulation, center, size, angle, 0, 360, rgb, -1);
    }

    std::stringstream ss;
    ss << "Ellipse " << height << " " << width;
    cv::imshow(ss.str(), pModulation);
    cv::waitKey(0);
};

inline void __visualize(cv::Mat& image, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::imshow(title.c_str(), image);
    cv::waitKey(0);
}

inline void __visualize_normalize(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = pMod[i * width + j];
        }

    double max = -1e+10;
    double min = +1e+10;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            double v = pModulation.at<double>(i, j);
            max = (max > v) ? max : v;
            min = (min < v) ? min : v;
            //std::cout << max << " " << min << std::endl;
        }
    if (fabs(max - min) > 1e-8)
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                pModulation.at<double>(i, j) = (pMod[i * width + j] - min) / (max - min);
            }
    }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);
};

inline void __visualize_color_normalize(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64FC3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            for( int k = 0; k < 3; k ++ )
                pModulation.at<cv::Vec3d>(i, j)[k] = pMod[(i * width + j)*3+k];
        }
/*
    double max = -1e+10;
    double min = +1e+10;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            cv::Vec3f v = pModulation.at<cv::Vec3d>(i, j);
            for (int k = 0; k < 3; k++) {
                max = (max > v[k]) ? max : v[k];
                min = (min < v[k]) ? min : v[k];
            }
            //std::cout << max << " " << min << std::endl;
        }
    if (fabs(max - min) > 1e-8)
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                for( int k = 0; k < 3; k ++ )
                pModulation.at<cv::Vec3d>(i, j)[k] = (pMod[(i * width + j)*3+k] - min) / (max - min);
            }
    }
*/    
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);

    cv::Mat pOutput(height, width, CV_8U);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
                int r = (int)(pModulation.at<cv::Vec3d>(i, j)[0] * 255 + 0.5);
                if (r > 255) r = 255;
                if (r < 0)  r = 0;
                uchar c = r;
                pOutput.at<uchar>(i, j) = c;
        }

    cv::imwrite(title, pOutput);
};

inline void __visualize_texture_color_normalize(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64FC3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < 3; k++)
                pModulation.at<cv::Vec3d>(i, j)[k] = pMod[(i * width + j) * 3 + k];
        }
    //cv::namedWindow(title.c_str(), cv::WINDOW_AUTOSIZE);
    cv::namedWindow(title.c_str(), cv::WINDOW_NORMAL);
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);

    cv::Mat pOutput(height, width, CV_8UC3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            cv::Vec3d d = pModulation.at<cv::Vec3d>(i, j);
            cv::Vec3b c;
            for (int k = 0; k < 3; k++) {
                int r = (int)(d.val[k] * 255.0 + 0.5);
                if (r > 255) r = 255;
                if (r < 0)  r = 0;
                c.val[k] = r;
            }
            pOutput.at<cv::Vec3b>(i, width-1-j) = c;

        }

    cv::imshow(title.c_str(), pOutput);
    cv::waitKey(0);

    cv::imwrite(title, pOutput);
};

inline void __visualize_and_save(double* pMod,
    int height,
    int width,
    double max,
    double min,
    std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = (pMod[i * width + j] - min) / (max - min);
        }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);

    cv::Mat img(height, width, CV_8UC1);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            int c = (int)(pModulation.at<double>(i, j) * 255.0 + 0.5);
            c = (c > 255) ? 255 : c;
            img.at<uchar>(i, j) = (uchar)c;
        }
    cv::imwrite(title.c_str(), img);
};

inline void __visualize_and_save(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = pMod[i * width + j] / 255;
        }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);
    cv::Mat img(height, width, CV_8UC1);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            img.at<uchar>(i, j) = (uchar)pMod[i * width + j];
        }

    cv::imwrite(title.c_str(), img);
};

inline void __visualize_normalize_and_save(
    double* pMod,
    int height,
    int width,
    std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64F);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            pModulation.at<double>(i, j) = pMod[i * width + j];
        }

    double max = -1e+10;
    double min = +1e+10;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            double v = pModulation.at<double>(i, j);
            max = (max > v) ? max : v;
            min = (min < v) ? min : v;
            //std::cout << max << " " << min << std::endl;
        }
    if (fabs(max - min) > 1e-8)
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                pModulation.at<double>(i, j) = (pMod[i * width + j] - min) / (max - min);
            }
    }
    cv::imshow(title.c_str(), pModulation);

    cv::Mat img(height, width, CV_8UC1);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            img.at<uchar>(i, j) = (uchar)(pModulation.at<double>(i, j) * 255);
            if (j == 306 && i == 741) {
                std::cout << img.at<uchar>(i, j) << std::endl;
            }
        }
    cv::imwrite(title.c_str(), img);
    cv::waitKey(0);
};

/*
*   written by Chen Wang 08/23/2020
*/
inline void display_image_full_screen(std::string image_name)
{
    cv::Mat Image = cv::imread(image_name);

    if (Image.empty())
    {
        std::cout << "Image load error\n" << std::endl;
        system("pause");
        exit(-1);
    }
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::setWindowProperty("Image", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::imshow("Image", Image);
    cv::waitKey(0);
};


inline void _color_visualize_and_save(double* pMod, int height, int width, std::string title)
{
    if (!_DEBUGGER_) return;

    cv::Mat pModulation(height, width, CV_64FC3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for (int k = 0; k < 3; k++)
            {
                pModulation.at<cv::Vec3d>(i, j)[k] = pMod[(i * width + j)*3 + k] / 255;
            }
    cv::imshow(title.c_str(), pModulation);
    cv::waitKey(0);
    cv::Mat img(height, width, CV_8UC3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for( int k = 0; k < 3; k ++ )
            {
                img.at<cv::Vec3b>(i, j)[k] = (uchar)pMod[(i * width + j)*3 + k];
            }

    cv::imwrite(title.c_str(), img);
};

#endif
