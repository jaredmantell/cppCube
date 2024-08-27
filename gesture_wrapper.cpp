#include "gesture_wrapper.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

GestureWrapper::GestureWrapper() : cap(0), guard() {
    if (!cap.isOpened()) {
        throw std::runtime_error("Unable to open camera");
    }

    try {
        // Set Python home
        py::module::import("os").attr("environ")["PYTHONHOME"] = "C:/Users/Jared/AppData/Local/Programs/Python/Python310";

        // Import sys module
        auto sys = py::module::import("sys");

        // Add the current directory to Python path
        sys.attr("path").attr("append")(std::filesystem::current_path().string());

        // Add the directory containing gesture_recognition.py to Python path
        sys.attr("path").attr("append")(std::filesystem::current_path().parent_path().string());

        // Add Python site-packages to path
        sys.attr("path").attr("append")("C:/Users/Jared/AppData/Local/Programs/Python/Python310/Lib/site-packages");

        // Print Python path for debugging
        py::print("Python path:", sys.attr("path"));

        // Try to import the gesture_recognition module
        gesture_module = py::module::import("gesture_recognition");
    } catch (const py::error_already_set &e) {
        std::cerr << "Python error: " << e.what() << std::endl;
        throw;
    } catch (const std::exception &e) {
        std::cerr << "Error initializing Python: " << e.what() << std::endl;
        throw;
    }
}

GestureWrapper::~GestureWrapper() {
    cap.release();
}

std::pair<std::string, cv::Mat> GestureWrapper::run_single_frame() {
    cv::Mat frame;
    if (!cap.read(frame)) {
        return {"No gesture", cv::Mat()};
    }

    try {
        return process_frame(frame);
    } catch (const py::error_already_set &e) {
        std::cerr << "Python error while processing frame: " << e.what() << std::endl;
        return {"No gesture", frame};
    } catch (const std::exception &e) {
        std::cerr << "Error processing frame: " << e.what() << std::endl;
        return {"No gesture", frame};
    }
}

std::pair<std::string, cv::Mat> GestureWrapper::process_frame(cv::Mat &frame) const {
    if (frame.empty()) {
        return {"No gesture", frame};
    }

    // Create a py::array_t from the OpenCV Mat
    py::array_t<unsigned char> py_frame({frame.rows, frame.cols, 3}, frame.data);

    // Call the Python function
    auto result = gesture_module.attr("process_frame")(py_frame, frame.cols, frame.rows);

    // Extract the results
    std::string gesture = result.cast<py::tuple>()[0].cast<std::string>();
    py::bytes processed_bytes = result.cast<py::tuple>()[1].cast<py::bytes>();

    // Convert the processed frame back to OpenCV Mat
    std::string str = processed_bytes;
    std::vector<uchar> buf(str.begin(), str.end());
    cv::Mat processed_frame = cv::Mat(frame.rows, frame.cols, CV_8UC3, buf.data()).clone();

    return {gesture, processed_frame};
}