#pragma once

#include<vector>
#include<algorithm>
#include<opencv2\opencv.hpp>
#include<ctime>
#include<cstdlib>

using namespace std;
using namespace cv;

class ScreenBroker {
private:
    vector<double> skinRateVector;
    Mat crackOrg, back;

public:
    ScreenBroker(const char *backPath) {
        skinRateVector.assign(5, 0.0);

        constexpr char* crackPath = "res/crack.png";
        crackOrg = imread(crackPath, cv::IMREAD_UNCHANGED);
        back = imread(backPath);

        namedWindow("back", WINDOW_NORMAL);
        moveWindow("back", 1920, 0);
        setWindowProperty("back", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
        imshow("back", back);
        srand(time(NULL));
    }

    void update_vector(double newSkinRate) {
        skinRateVector.erase(skinRateVector.begin());
        skinRateVector.push_back(newSkinRate);
        check_punch(skinRateVector);
    }

private:
    void check_punch(vector<double> vec) {
        constexpr double thValue = 0.25;
        constexpr int thIndex = 2;

        vector<double>::iterator minItr = min_element(vec.begin(), vec.end());
        vector<double>::iterator maxItr = max_element(vec.begin(), vec.end());

        size_t diffIndex = (maxItr > minItr) ? distance(minItr, maxItr) : 0;
        double diffValue = *maxItr - *minItr;
        //cout << diffValue << "," << diffIndex << endl;

        if ((diffIndex > thIndex) && (diffValue > thValue)) {
            //cout << "BANG" << endl;
            skinRateVector.assign(5, 1.0);//�A���Ŕ��������Ȃ�
            break_screen();
        }
    }

    void break_screen() {

        Mat alpha0(back.rows, back.cols, crackOrg.type());
        alpha0 = Scalar::all(0);

        Mat crack;
        double mag = 0.7 + (1.0 * rand() / RAND_MAX);
        resize(crackOrg, crack, Size(), mag, mag);//�g��,�k��

        Point2d ctr(crack.cols / 2, crack.rows / 2);//�O�i�摜�̉�]���S
        double angle = 360 * rand() / RAND_MAX;
        double tx = back.cols * rand() / RAND_MAX - crack.cols / 2;
        double ty = back.rows * rand() / RAND_MAX - crack.rows / 2;


        Mat mat = cv::getRotationMatrix2D(ctr, angle, 1.0);//��]�s��̍쐬
        cout << mag << "," << ty << endl;
        mat.at<double>(0, 2) += tx;//��]��̕��s�ړ���
        mat.at<double>(1, 2) += ty;//��]��̕��s�ړ���
        cv::warpAffine(crack, alpha0, mat, alpha0.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);

        //�`�����l���ɕ���
        vector<Mat> planes_rgb, planes_aaa, planes_rgba, planes_1ma;
        Mat crack_rgb, crack_aaa, back_1ma;
        int maxVal = pow(2, 8 * back.elemSize1()) - 1;
        split(alpha0, planes_rgba);

        //RGBA�摜��RGB�ɕϊ�   
        planes_rgb.push_back(planes_rgba[0]);
        planes_rgb.push_back(planes_rgba[1]);
        planes_rgb.push_back(planes_rgba[2]);
        merge(planes_rgb, crack_rgb);

        //RGBA�摜����A���t�@�`�����l�����o   
        planes_aaa.push_back(planes_rgba[3]);
        planes_aaa.push_back(planes_rgba[3]);
        planes_aaa.push_back(planes_rgba[3]);
        merge(planes_aaa, crack_aaa);

        //�w�i�p�A���t�@�`�����l��   
        planes_1ma.push_back(255 - planes_rgba[3]);
        planes_1ma.push_back(255 - planes_rgba[3]);
        planes_1ma.push_back(255 - planes_rgba[3]);
        merge(planes_1ma, back_1ma);

        back = crack_rgb.mul(crack_aaa, 1.0 / (double)maxVal) + back.mul(back_1ma, 1.0 / (double)maxVal);
        Mat backGau;
        GaussianBlur(back, backGau, cv::Size(51, 3), 80, 3); //�h�炷
        imshow("back", backGau);
        waitKey(100);
        imshow("back", back);

    }

};


