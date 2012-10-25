//
//  MarkerDetector.cpp
//  CompTest
//
//  Created by Árpád Kiss on 2012.10.24..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#include "MarkerDetector.h"

MarkerDetector::MarkerDetector()
{
    MarkerDetector::init();
}

MarkerDetector::MarkerDetector(int len, int str, int hei)
{
    length = len;
    stride = str;
    height = hei;
    MarkerDetector::init();
}

void MarkerDetector::preprocessImage(Mat* image)
{
    preprocessedFrame.release();
    preprocessedFrame = Mat::zeros(height, stride, CV_8UC1);
    differenceEdgeDetectionWithThresh(image);
}

void MarkerDetector::differenceEdgeDetectionWithThresh(Mat* from)
{
    z = 0;
    ptr = (uchar*)(from->data);
    ptr_2 = (uchar*)(preprocessedFrame.data);
    i = stride + 1;
    for(; i < length; i++)
    {
        _max = 0;
        variances[0] = abs(ptr[i-1 -stride] - ptr[i+1 + stride]);
        variances[1] = abs(ptr[i - stride] - ptr[i + stride]);
        variances[2] = abs(ptr[i+1 - stride] - ptr[i - 1 + stride]);
        variances[3] = abs(ptr[i - 1] - ptr[i + 1]);
        
        for(z = 0; z<4; z++)
        {
            if(variances[z] > variances[_max])
            {
                _max = z;
            }
        }
        ptr_2[i] = (variances[_max] > tresh)?255:0;
    }
}

void MarkerDetector::findPossibleMarkers()
{
    findContours( preprocessedFrame, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    
    vector<Point> approxCurve;
    
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        if ( 100< contours[idx].size() && contours[idx].size()<700  )
        {
            approxPolyDP( contours[idx], approxCurve, double ( contours[idx].size() ) * 0.05 , true );
            
            if ( approxCurve.size() ==4 && isContourConvex ( Mat(approxCurve)) )
            {
                Scalar color( rand()&255, rand()&255, rand()&255 );
                drawContours( currentFrame, contours, idx, color, CV_FILLED, 8, hierarchy );
            }
        }
    }
}

/*IplImage* MarkerDetector::CurrentFrame()
{
    return new IplImage(currentFrame);
}*/

Mat MarkerDetector::CurrentFrame() const
{
    return currentFrame;
}

Mat MarkerDetector::PreprocessedFrame() const
{
    return preprocessedFrame;
}

void MarkerDetector::setCurrentFrame(Mat value)
{
    currentFrame.release();
    currentFrame = Mat(value);
}

//void MarkerDetector::set

void MarkerDetector::init()
{
    std::cout << "Object <MarkerDetector> Created" << std::endl;
}

int MarkerDetector::Length() const
{
    return length;
}

int MarkerDetector::Stride() const
{
    return stride;
}

void MarkerDetector::setLength(const int value)
{
    length = value;
}

void MarkerDetector::setStride(const int value)
{
    stride = value;
}

int MarkerDetector::Height() const
{
    return height;
}

void MarkerDetector::setHeight(const int value)
{
    height = value;
}
