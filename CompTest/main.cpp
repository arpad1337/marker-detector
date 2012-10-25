//
//  main.cpp
//  Interactive-Coursebook_v4
//
//  Created by Árpád Kiss on 2012.10.20..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

// CV bugfix
#define use_speed_ 0

#include <cv.h>
#include <highgui.h>
#include <features2d.hpp>
#include <objdetect.hpp>
#include <iostream>

using namespace cv;

int variances[4];
int _max,length,stride,labres,tresh = 10;
int i,z = 0;
uchar* ptr,* ptr_2;

void differenceEdgeDetectionWithThresh(IplImage *frame, IplImage *frame_new)
{
    z = 0;
    ptr = (uchar*)(frame->imageData);
    ptr_2 = (uchar*)(frame_new->imageData);
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

int main(int argc, const char * argv[])
{
    CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }
    // Create a window in which the captured images will be presented
    cvNamedWindow( "CameraView", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "CameraViewEdgeDetected", CV_WINDOW_AUTOSIZE );
    
    // Fukin blob staff
    
    /*
    vector<KeyPoint> keyPoints;
    vector< vector <Point> > contours;
    vector< vector <Point> > approxContours;
    
    SimpleBlobDetector::Params params;
    params.minThreshold = 40;
    params.maxThreshold = 60;
    params.thresholdStep = 5;
    
    params.minArea = 100;
    params.minConvexity = 0.3;
    params.minInertiaRatio = 0.01;
    
    params.maxArea = 8000;
    params.maxConvexity = 10;
    
    params.filterByColor = false;
    params.filterByCircularity = false;
    
    SimpleBlobDetector blobDetector( params );
    blobDetector.create("SimpleBlob");
     */
    
    // Show the image captured from the camera in the window and repeat
    while ( 1 ) {
        // Get one frame
        IplImage* frame = cvQueryFrame( capture );
        CvSize frameSize = cvGetSize(frame);
        if ( !frame ) {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        
        
        
        length = (frame->width - 1 )*(frame->height - 1);
        stride = frame->width;
        
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        IplImage *frame_gray = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        IplImage *frame_binary = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        
        Mat dst = Mat::zeros(frame->height, frame->width, CV_8UC3);
        Mat src = Mat(frame_binary);
        src = src < 255;
        
        //IplImage *frame_label = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        
        cvCvtColor(frame,frame_gray,CV_RGB2GRAY);
        
        differenceEdgeDetectionWithThresh( frame_gray , frame_binary );
        
        findContours( src, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
        
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
                    drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
                }
            }
        }
        
        
        /*blobDetector.detect( frame_binary, keyPoints );
        
        std::cout << "Keypoints " << keyPoints.size() << std::endl;
        
        for (KeyPoint t : keyPoints)
        {
            std::cout << t.pt << std::endl;
        }*/
        //drawKeypoints( frame, keyPoints, frame_binary, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        
        cvShowImage( "CameraView", frame );
        imshow( "CameraViewEdgeDetected", dst );
        
        cvReleaseImage(&frame_gray);
       // cvReleaseImage(&frame_label);
        cvReleaseImage(&frame_binary);
        
        //cvShowImage( "CameraView", frame_gray );
        
        // Do not release the frame!
        //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
        //remove higher bits using AND operator
        if ( (cvWaitKey(10) & 255) == 27 ) break;
    }
    // Release the capture device housekeeping
    cvReleaseCapture( &capture );
    cvDestroyWindow( "CameraView" );
    std::cout << "Maybe running...\n";
    return 0;
}



