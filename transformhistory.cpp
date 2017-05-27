#include "transformhistory.h"
#include <QDebug>
#include <QMessageBox>

//TODO: Add a window that shows the transforms currently applied (in the correct order)
//TODO: Export list of transforms as a module that a rostopic can run through?
//  So we don't just transform a dataset but rather a real-time-ish feed

TransformHistory::TransformHistory()
{
    //A data structure to store list of transforms

    //A function to apply all transforms to an image (pass image as parameter)
}

void TransformHistory::updateHistory(_effect settings) {
    this->history.append(settings);
    //qDebug() << boost::get<blur_effect>(this->history.last()).kernel << '\n';
}

void TransformHistory::undo() {
    if(!this->history.isEmpty()) {
        this->history.remove(this->history.size()-1);
    }
}

cv::Mat TransformHistory::recalculateAll(cv::Mat image) {
    cv::Mat new_image = image;
    _effect current;
    for(int i=0;i<this->history.size();i++) {
        current = this->history.at(i);
        switch(current.which()) {
        case 0: {//blur
            blur_effect b = boost::get<blur_effect>(current);
            new_image = this->blur(b.type,new_image,b.kernel);
            break;
        }
        case 1: {//distortion
            distortion_effect d = boost::get<distortion_effect>(current);
            new_image = this->distort(d.type,new_image,d.level);
            break;
        }
        case 2: {//lighting
            lighting_effect l = boost::get<lighting_effect>(current);
            new_image = this->light(new_image,l.contrast,l.brightness);
            break;
        }
        case 3: {//resolution
            resolution_effect r = boost::get<resolution_effect>(current);
            new_image = this->resolution(new_image,r.horizontal,r.vertical);
            break;
        }
        default:

            break;
        }
    }
    return new_image;
}

cv::Mat TransformHistory::blur(QString type, cv::Mat src, int kernel) {
    cv::Mat blurredImage = src;
    if(type == "Averaging") {
        cv::blur(src,blurredImage,cv::Size(kernel,kernel));
    }
    else if(type == "Gaussian") {
        if(kernel % 2 == 0) {
            QMessageBox message;
            message.critical(0,"Kernel Size","Please select an odd number for your kernel size!");
            return blurredImage;
        }
        cv::GaussianBlur(src,blurredImage,cv::Size(kernel,kernel),0,0);
    }
    else if(type == "Median") {
        if(kernel % 2 == 0) {
            QMessageBox message;
            message.critical(0,"Kernel Size","Please select an odd number for your kernel size!");
            return blurredImage;
        }
        cv::medianBlur(src,blurredImage,kernel);
    }
    else {
        QMessageBox message;
        message.critical(0,"Blur Type","Not a valid type! Not sure how you managed that...");
    }
    return blurredImage;
}

cv::Mat TransformHistory::distort(QString type, cv::Mat src, double level) {
    cv::Mat distImage = src.clone();
    if(type=="Gaussian") {
        cv::Mat mSrc = src;
        cv::Mat mSrc_16SC;
        cv::Mat mGaussian_noise = cv::Mat(mSrc.size(),CV_16SC3);
        cv::randn(mGaussian_noise,cv::Scalar::all(0.0), cv::Scalar::all(level));

        mSrc.convertTo(mSrc_16SC,CV_16SC3);
        cv::addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
        mSrc_16SC.convertTo(distImage,mSrc.type());
    }
    else if(type=="Salt and Pepper") {
        if(level>1) {
            QMessageBox message;
            message.critical(0,"Salt and Pepper level","Please select a level between 0 and 1!");
            return distImage;
        }
        int totalPix = src.rows*src.cols;
        int pixToChange = level*totalPix;
        for(int i=0;i<pixToChange;i++) {
            int targetRow = rand() % src.rows;
            int targetCol = rand() % src.cols;
            int value = 0; //Pepper
            if(i%2==0) {//Salt
                value = 255;
            }
            distImage.at<cv::Vec3b>(targetRow,targetCol)[0] = value;
            distImage.at<cv::Vec3b>(targetRow,targetCol)[1] = value;
            distImage.at<cv::Vec3b>(targetRow,targetCol)[2] = value;
        }
    }
    else {
        QMessageBox message;
        message.critical(0,"Noise Type","Not a valid type! Not sure how you managed that...");
    }
    return distImage;
}

cv::Mat TransformHistory::light(cv::Mat src, double contrast, int brightness) {
    //Derived from http://docs.opencv.org/2.4/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
    cv::Mat lightImage = cv::Mat::zeros(src.size(),src.type());

    for( int y = 0; y < src.rows; y++ )
    { for( int x = 0; x < src.cols; x++ )
        { for( int c = 0; c < 3; c++ )
            {
                lightImage.at<cv::Vec3b>(y,x)[c] =
                        cv::saturate_cast<uchar>( contrast*( src.at<cv::Vec3b>(y,x)[c] ) + brightness );
            }
        }
    }
    return lightImage;
}

cv::Mat TransformHistory::resolution(cv::Mat src, int horizontal, int vertical) {
    cv::Mat resolutionImage = cv::Mat::zeros(horizontal,vertical,src.type());

    cv::resize(src,resolutionImage,resolutionImage.size(),0,0,cv::INTER_LANCZOS4);

    return resolutionImage;
}
