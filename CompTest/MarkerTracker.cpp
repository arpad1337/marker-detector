//
//  MarkerTracker.cpp
//  CompTest
//
//  Created by Árpád Kiss on 2012.12.07..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#include "MarkerTracker.h"

MarkerTracker::MarkerTracker(IplImage *firstFrame)
{
    previousFrame = Mat(Mat::zeros(firstFrame->height, firstFrame->width, CV_8UC3));
    currentFrame = Mat(Mat::zeros(firstFrame->height, firstFrame->width, CV_8UC3));
    outImage = Mat(firstFrame);
    currentFrame = outImage;
    //cvtColor(outImage,currentFrame,CV_RGB2GRAY);
    
    // SNN
    markerDetector = new MarkerDetector((firstFrame->width - 1 )*(firstFrame->height - 1),firstFrame->width,firstFrame->height);
    
    // Kuwahara-Nagao (3*stride-1 + 3*height-1)
    //markerDetector = new MarkerDetector((3*firstFrame->width-1) * (firstFrame->height-1),firstFrame->width,firstFrame->height);
    
    markerDetector->setCurrentFrame(currentFrame);
    markerDetector->preprocessImage();
   
    
    vector<Marker> foundedMarkers = markerDetector->findPossibleMarkers();
    for(int i = 0; i < foundedMarkers.size(); i++)
    {
        lastMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(0));
        lastMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(1));
        lastMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(2));
        lastMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(3));
    }
}

void MarkerTracker::ProcessFrame(IplImage *newFrame)
{
    vector<Point2f> trackedPoints;
    recognizedMarkerCornerPoints = vector<Point2f>();
    previousFrame.release();
    outImage.release();
    previousFrame = Mat(currentFrame);
    
    // currentFrame.release();
    
    outImage = Mat(newFrame);
    //cvtColor(outImage,currentFrame,CV_RGB2GRAY);
    
    currentFrame = Mat(newFrame);
    
    //calcOpticalFlowFarneback(previousFrame,currentFrame,flowImage,0.5,3,5,5,3,1.1,0);
    
    //drawOptFlowMap(flowImage, outImage, 20, CV_RGB(0,255,0));
    
    markerDetector->setCurrentFrame(currentFrame);
        
    markerDetector->preprocessImage();
    
    //currentFrame.release();
    
    vector<Marker> foundedMarkers = markerDetector->findPossibleMarkers();
    vector<Marker> correctedMarkers;
    
    /*if(!trackSuccess)
    {*/
    
    int j;
    for(int i = 0; i < foundedMarkers.size(); i++)
    {
        j = 0;
        
        recognizedMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(0));
        recognizedMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(1));
        recognizedMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(2));
        recognizedMarkerCornerPoints.push_back(foundedMarkers.at(i).CornerPointsAt(3));
        
        
        // TODO: tömbök Mergelése, és koordináták frissítése
         
        while(j<recognizedMarkers.size() &&
              !(recognizedMarkers.at(j).Hash() == foundedMarkers.at(i).Hash()))
        {
            j++;
        }
        
        correctedMarkers.push_back(foundedMarkers.at(i));
        DrawMarkerLines(correctedMarkers.back());
        
        if(j < recognizedMarkers.size() )
        {
            recognizedMarkers.erase(recognizedMarkers.begin() + j);
        }
    }
    
    //JAVÍTANI
    
    if( recognizedMarkers.size() > 0 )
    {
        
        if(lastMarkerCornerPoints.size() > 0 )
        {
            calcOpticalFlowPyrLK(previousFrame,currentFrame,lastMarkerCornerPoints,trackedPoints,trackSuccess,trackError);
        
            int age;
            for(int i = 0; i< recognizedMarkers.size() && i < trackedPoints.size() / 4; i++)
            {
                age = recognizedMarkers.at(i).Age();
                if(age < 30) {
                    recognizedMarkers.at(i).setCornerPointAt(0,trackedPoints.at(i*4));
                    recognizedMarkers.at(i).setCornerPointAt(1,trackedPoints.at(i*4+1));
                    recognizedMarkers.at(i).setCornerPointAt(2,trackedPoints.at(i*4+2));
                    recognizedMarkers.at(i).setCornerPointAt(3,trackedPoints.at(i*4+3));
                    std::cout << "Hiba optical flow-al korigálva" << std::endl;
                
                    recognizedMarkers.at(i).setAge(age + 1);
                    correctedMarkers.push_back(recognizedMarkers.at(i));
                    DrawMarkerLines(correctedMarkers.back());
                    recognizedMarkers.erase(recognizedMarkers.begin() + i);
                }
            }
                
        }
    }
    
    
    /*
    }
    else
    {
        int j=0;
        for(int i = 0; i < recognizedMarkers.size(); i++)
        {
            recognizedMarkers.at(i).CornerPointsAt(0) = recognizedMarkerCornerPoints.at(j++);
            recognizedMarkers.at(i).CornerPointsAt(1) = recognizedMarkerCornerPoints.at(j++);
            recognizedMarkers.at(i).CornerPointsAt(2) = recognizedMarkerCornerPoints.at(j++);
            recognizedMarkers.at(i).CornerPointsAt(3) = recognizedMarkerCornerPoints.at(j++);
        }
    }
    */
    
    if(foundedMarkers.size() > 0)
    {
        //if(recognizedMarkers.size() >
        lastMarkerCornerPoints = recognizedMarkerCornerPoints;
    }
    else
    {
        lastMarkerCornerPoints = trackedPoints;
    }
    
    
    recognizedMarkers = vector<Marker>(correctedMarkers);
    
    std::cout << "Azonosított markerek száma: " << recognizedMarkers.size() << std::endl;

}

void MarkerTracker::RenderImage()
{
    
    
}

Mat MarkerTracker::OutImage() const
{
    return outImage; //markerDetector->CurrentFrame();
}


void MarkerTracker::DrawMarkerLines(Marker m)
{
    line( outImage, m.CornerPointsAt(0), m.CornerPointsAt(1), CV_RGB(255,0,0),2);
    line( outImage, m.CornerPointsAt(1), m.CornerPointsAt(2), CV_RGB(0,255,0),2);
    line( outImage, m.CornerPointsAt(2), m.CornerPointsAt(3), CV_RGB(0,0,255),2);
    line( outImage, m.CornerPointsAt(3), m.CornerPointsAt(0), CV_RGB(255,255,0),2);
}


void MarkerTracker::drawOptFlowMap(const cv::Mat& flow,
                    cv::Mat& cflowmap,
                    int step,
                    const cv::Scalar& color
                    )
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
            cv::line(cflowmap,
                     cv::Point(x,y),
                     cv::Point(cvRound(x+fxy.x),cvRound(y+fxy.y)),
                     color);
            cv::circle(cflowmap, cv::Point(x,y), 2, color, -1);
        }
}