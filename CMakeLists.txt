cmake_minimum_required(VERSION 3.1)

project(PhaseUnwrap)

set(3rdparty "${CMAKE_SOURCE_DIR}/3rdparty")

find_package( OpenCV REQUIRED )

include_directories("include"
                    "${3rdparty}/eigen-3.3.9"
                    "${3rdparty}/MeshLib/core"
                    "${3rdparty}/ScanLib"
                    "${3rdparty}/ScanLib/LightField"
                    "${3rdparty}/ScanLib/Stereo"
                    "${3rdparty}/ScanLib/phase"
                    "${OpenCV_INCLUDE_DIRS}"
                   )

file(GLOB SRCS
    "src/main.cpp"

    "${3rdparty}/ScanLib/phase/BasePhaseUnwrapper.cpp"
    "${3rdparty}/ScanLib/phase/CalibrationPhaseUnwrapper.cpp"
    "${3rdparty}/ScanLib/phase/CapturePhaseUnwrapper.cpp"
    "${3rdparty}/ScanLib/phase/TexturePhaseUnwrapper.cpp"
    "${3rdparty}/ScanLib/phase/unwrap2D.cpp"
    "${3rdparty}/ScanLib/phase/HorizontalVerticalMultipleWaveLengthPhaseUnwrapper.cpp"
    "${3rdparty}/ScanLib/LightField/LineBundle.cpp"
    "${3rdparty}/ScanLib/Stereo/HorizontalVerticalStereoMatcher.cpp"
    "${3rdparty}/ScanLib/Stereo/HorizontalVerticalTriCameraMatcher.cpp"
    "${3rdparty}/ScanLib/ColorFrameBuffer.cpp"
    "${3rdparty}/ScanLib/FrameBuffer.cpp"
    )

add_executable(PhaseUnwrap ${SRCS})

target_link_libraries(PhaseUnwrap ${OpenCV_LIBS})

install(TARGETS PhaseUnwrap DESTINATION ${CMAKE_SOURCE_DIR}/bin)
