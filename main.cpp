#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "common.h"
#include "cube.h"
#include "pyramid.h"
#include "donut.h"
#include "gesture_wrapper.h"
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <memory>
#include <chrono>
#include <thread>

// Shape interface
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

class CubeShape : public Shape {
public:
    void draw() override { drawCube(); }
};

class PyramidShape : public Shape {
public:
    void draw() override { drawPyramid(); }
};

class DonutShape : public Shape {
public:
    void draw() override { drawDonut(); }
};

std::unique_ptr<Shape> createShape(const std::string& gesture) {
    if (gesture == "Rock") {
        return std::make_unique<DonutShape>();
    } else if (gesture == "Paper") {
        return std::make_unique<CubeShape>();
    } else if (gesture == "Scissors") {
        return std::make_unique<PyramidShape>();
    } else {
        return nullptr;
    }
}

void clearScreen() {
    system("cls");
}

void renderShape(const std::unique_ptr<Shape>& shape) {
    if (!shape) return;

    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0, width * height * sizeof(float));

    shape->draw();

    clearScreen();
    for(int k = 0; k < width * height; k++) {
        putchar(k % width ? buffer[k] : 10);
    }
}

int main() {
    try {
        GestureWrapper gesture_wrapper;
        cv::namedWindow("Gesture Recognition", cv::WINDOW_NORMAL);

        std::unique_ptr<Shape> currentShape;
        std::string lastGesture = "No gesture";

        while (true) {
            auto [gesture, frame] = gesture_wrapper.run_single_frame();

            // Update the shape if the gesture has changed
            if (gesture != lastGesture) {
                auto newShape = createShape(gesture);
                if (newShape) {
                    currentShape = std::move(newShape);
                    lastGesture = gesture;
                }
            }

            // Render the current shape
            if (currentShape) {
                renderShape(currentShape);
                A += 0.05f;
                B += 0.05f;
            }

            // Display the processed frame with hand landmarks
            cv::imshow("Gesture Recognition", frame);

            // Check for ESC key to exit
            if (cv::waitKey(30) == 27) break;

            // Small delay to control frame rate
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}