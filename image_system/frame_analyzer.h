#pragma once

#include<opencv2\opencv.hpp>


class FrameAnalyzer {
private:
    cv::CascadeClassifier cc;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgs;

public:
    FrameAnalyzer() {
        constexpr char* cascadePath = "haarcascade_frontalface_default.xml";
        if (!cc.load(cascadePath)) {
            exit(2);
        }

        bgs = cv::createBackgroundSubtractorMOG2();
    }

    auto analyze(const cv::Mat& frame) -> double {
        using namespace cv;
        using namespace std;

        Mat target;
        resize(frame, target, Size(640, 480));
        medianBlur(target, target, 5);

        Mat gray;
        cvtColor(target, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        vector<Rect> faces;
        cc.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));
        for (auto& face : faces) {
            const Point c(face.x + face.width / 2, face.y + face.height / 2);
            ellipse(target, c, Size(1.1*face.width / 2, 1.1*face.height / 2), 0, 0, 360, Scalar(0, 0, 0), -1);
        }

        Mat fore, masked;
        bgs->apply(target, fore);
        target.copyTo(masked, fore);

        Mat masked2;
        cvtColor(masked, masked2, COLOR_BGR2HSV);
        Mat skin;
        inRange(masked2, Scalar(0, 30, 40), Scalar(40, 240, 240), skin);

        imshow("target", target);
        imshow("masked", masked);
        imshow("skin", skin);
        
        double skinRate = 0;
        int key = waitKey(2);
        if (key == 27) {
            skinRate = -1;
        } else if (key == 'a') {
            int k = waitKey(0);
            if (k == 's') {
                imwrite("target.png", target);
                imwrite("masked.png", masked);
                imwrite("skin.png", skin);
            }
        }
        
        //const double skinRate = mean(skin)[0]/255;
        return skinRate;
    }

};