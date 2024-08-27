#ifndef GESTURE_WRAPPER_H
#define GESTURE_WRAPPER_H

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <opencv2/opencv.hpp>
#include <string>

namespace py = pybind11;

class GestureWrapper {
private:
    cv::VideoCapture cap;
    py::scoped_interpreter guard;
    py::module gesture_module;

    // Private method to process a single frame
    std::pair<std::string, cv::Mat> process_frame(cv::Mat &frame) const;

public:
    GestureWrapper();
    ~GestureWrapper();

    // Public method to run a single frame and return the gesture and processed frame
    std::pair<std::string, cv::Mat> run_single_frame();
};

#endif // GESTURE_WRAPPER_H