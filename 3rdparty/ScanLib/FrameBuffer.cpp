#include "FrameBuffer.h"

CFrameBuffer::CFrameBuffer(int gray_rows, int gray_cols, int numImages ) :ROWS(gray_rows), COLS(gray_cols), m_numImages(numImages)
{
    for (int i = 0; i < m_numImages; i++)
    {
        m_pBuffer[i] = new unsigned char[ROWS * COLS];
        if (m_pBuffer[i] == NULL) {
            std::cerr << "Error in memory allocation" << std::endl;
            return;
        }
    }
}

CFrameBuffer::~CFrameBuffer()
{
    for (int i = 0; i < m_numImages; i++)
    {
        delete []m_pBuffer[i];
    }
}

void CFrameBuffer::load_gray_calibration_fringe_images( const std::string  filename )
{
    //gray images for the left side
    cv::Mat  _image;
    std::ifstream nameFileout;
    nameFileout.open(filename);
    std::string line;
    
   
    std::getline(nameFileout, line);
    std::stringstream ss(line);
    ss >> m_center_x >> m_center_y;
    std::cout << "Center (" << m_center_x << "," << m_center_y << ")" << std::endl;
    

    int i = 0;
    while (std::getline(nameFileout, line))
    {
        std::cout << "line:" << line << std::endl;
        // TODO: assign item_name based on line (or if the entire line is 
        // the item name, replace line with item_name in the code above)

        _image = cv::imread(line.c_str(), cv::IMREAD_GRAYSCALE);
        cv::imshow(line.c_str(), _image);
        // Blur the image with 5x5 Gaussian kernel
        cv::Mat image_blurred;
        cv::GaussianBlur(_image, image_blurred, cv::Size(7, 7), 0);
        //memcpy((void*)RecordBuffer[i], (void*)_image.data, sizeof(unsigned char) * ROWS * COLS);
        memcpy((void*)m_pBuffer[i], (void*)image_blurred.data, sizeof(unsigned char) * ROWS * COLS);
        __visualize(m_pBuffer[i], ROWS, COLS, 1, line.c_str(), m_center_x, m_center_y);
        i++;
        if (i == m_numImages) break;
    }
}


void CFrameBuffer::load_rgb_calibration_fringe_images(const std::string  rgb_filename)
{
    //color images for the left side
    cv::Mat  _rgb_image;
    //gray  images
    cv::Mat  _image;

    std::ifstream nameFileout;
    nameFileout.open(rgb_filename);
    std::string line;


    std::getline(nameFileout, line);
    std::stringstream ss(line);
    ss >> m_center_x >> m_center_y;
    std::cout << "Center (" << m_center_x << "," << m_center_y << ")" << std::endl;


    int i = 0;
    while (std::getline(nameFileout, line))
    {
        std::cout << "line:" << line << std::endl;
        // TODO: assign item_name based on line (or if the entire line is 
        // the item name, replace line with item_name in the code above)

        _rgb_image = cv::imread(line.c_str(), cv::IMREAD_COLOR);
        cv::imshow(line.c_str(), _rgb_image);

        cv::cvtColor(_rgb_image, _image, cv::COLOR_BGR2GRAY);

        // Blur the image with 5x5 Gaussian kernel
        cv::Mat image_blurred;
        cv::GaussianBlur(_image, image_blurred, cv::Size(7, 7), 0);
        //memcpy((void*)RecordBuffer[i], (void*)_image.data, sizeof(unsigned char) * ROWS * COLS);
        memcpy((void*)m_pBuffer[i], (void*)image_blurred.data, sizeof(unsigned char) * ROWS * COLS);
        __visualize(m_pBuffer[i], ROWS, COLS, 1, line.c_str(), m_center_x, m_center_y);
        i++;
        //for calibration, we need 13 images
        if (i == m_numImages) break;
    }
}

void CFrameBuffer::load_rgb_capture_fringe_images(const std::string  filename)
{
    //color images for the left side
    cv::Mat _rgb_image;
    //gray images for the left side
    cv::Mat  _image;
    std::ifstream nameFileout;
    nameFileout.open(filename);
    std::string line;


    int i = 0;
    while (std::getline(nameFileout, line))
    {
        std::cout << "line:" << line << std::endl;
        // TODO: assign item_name based on line (or if the entire line is 
        // the item name, replace line with item_name in the code above)

        _rgb_image = cv::imread(line.c_str(), cv::IMREAD_COLOR);
        cv::imshow(line.c_str(), _rgb_image);
        cv::waitKey(0);
        cv::cvtColor(_rgb_image, _image, cv::COLOR_BGR2GRAY);

        memcpy((void*)m_pBuffer[i], (void*)_image.data, sizeof(unsigned char) * ROWS * COLS);
        i++;

        if (i == m_numImages) break;
    }
}

void CFrameBuffer::load_gray_capture_fringe_images(const std::string  filename)
{
    //gray images for the left side
    cv::Mat  _image;
    std::ifstream nameFileout;
    nameFileout.open(filename);
    std::string line;

    int i = 0;
    while (std::getline(nameFileout, line))
    {
        std::cout << "line:" << line << std::endl;

        _image = cv::imread(line.c_str(), cv::IMREAD_GRAYSCALE);
        cv::imshow(line.c_str(), _image);
        cv::waitKey(0);

        memcpy((void*)m_pBuffer[i], (void*)_image.data, sizeof(unsigned char) * ROWS * COLS);
        i++;
        if (i == m_numImages) break;
    }
}
