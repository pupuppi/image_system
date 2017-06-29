#pragma once

#include<opencv2\opencv.hpp>


class FrameAnalyzer {
private:
    cv::CascadeClassifier cc;
    cv::Ptr<cv::BackgroundSubtractorMOG2> bgs;

public:
    struct Result {
        double skinRate;
        cv::Point center;
    };

    FrameAnalyzer() {
        constexpr char* cascadePath = "haarcascade_frontalface_default.xml";
        if (!cc.load(cascadePath)) {
            exit(2);
        }

        bgs = cv::createBackgroundSubtractorMOG2();
    }

    Result analyze(const cv::Mat& frame) {
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
            const Point c(face.x + face.width / 2, face.y + face.height);
            ellipse(target, c, Size(1.1*face.width / 2, 1.1*face.height), 0, 0, 360, Scalar(0, 0, 0), -1);
        }

        Mat fore, masked;
        bgs->apply(target, fore);
        target.copyTo(masked, fore);

        cvtColor(masked, masked, COLOR_BGR2HSV);
        Mat skin;
        inRange(masked, Scalar(0, 30, 40), Scalar(40, 240, 240), skin);

        Result result;
        
        result.skinRate = mean(skin)[0]/255;

        const auto m = moments(skin, true);
        result.center = Point(m.m10 / m.m00, m.m01 / m.m00);
        circle(skin, result.center, 10, Scalar(130), -1);

        imshow("skin", skin);

        return result;
    }

};