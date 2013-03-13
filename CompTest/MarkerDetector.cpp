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
    rstride = str*3;
    MarkerDetector::init();
}

void MarkerDetector::preprocessImage()
{
    preprocessedFrame.release();
    preprocessedFrame = Mat(Mat::zeros(height, stride, CV_8UC1));
    preprocessedFrameColor.release();
    preprocessedFrameColor = Mat(Mat::zeros(height, stride, CV_8UC3));
    //differenceEdgeDetectionWithThresh();
    //kuwaharaNagaoFilter();
    differenceEdgeDetectionWithThresh();
    //cv::Canny(currentFrame,preprocessedFrame,thresh*8,thresh * 20, 3);
    //houghBasedEdgeDetection();
}

void MarkerDetector::houghBasedEdgeDetection()
{
    cv::Canny(currentFrame,preprocessedFrame,thresh*8,thresh * 20, 3);
    //vector<Vec2f> lines;
    /*cv::HoughLines(preprocessedFrame, lines, 1, CV_PI/180, 100, 0, 0 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( preprocessedFrameColor, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }*/
    vector<Vec4i> lines;
    HoughLinesP(preprocessedFrame, lines, 1, CV_PI/180, 20, 20, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( preprocessedFrameColor, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255,255,255), 1, CV_8U);
    }
    //cvtColor(preprocessedFrameColor, preprocessedFrame, CV_RGB2GRAY);
    preprocessedFrame = preprocessedFrameColor > 0;
    
    cvtColor(preprocessedFrame, preprocessedFrame, CV_RGB2GRAY);

    //cv::imshow("burgonya",preprocessedFrame);
    //highgui::imshow("burgonya",preprocessedFrame);
}

void MarkerDetector::differenceEdgeDetectionWithThresh()
{
    //preprocessedFrame.release();
    //preprocessedFrame = Mat(Mat::zeros(height, stride, CV_8UC1));
    z = 0;
    ptr = (uchar*)(currentFrameGray.data);
    ptr_2 = (uchar*)(preprocessedFrame.data);
    i = rstride + 1;
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
        ptr_2[i] = (variances[_max] > thresh)?255:0;
    }
}

void MarkerDetector::kuwaharaNagaoFilter()
{
    //int sum = 0;

    //Mat x = Mat(Mat::zeros(height, stride, CV_8UC1));
    //ptr = (uchar*)(from.data);
    ptr_2 = (uchar*)(preprocessedFrame.data);
    
    //std::cout<<ptr[0]<<", azután:";
    
    //ptr+=i;
    //std::cout<<ptr[0]<<" ez";
    
    // 1 ciklussal nem lehet..........
    //for(i = rstride + 1; i < length; ++i)
    //{
    for(i=1;i<height-2;++i){for(j=1;j<stride-2;++j){
        for(z=0;z<3;z++) // by color channels
        {
            /*means[0][j] = ptr[i*3+j-3 -rstride];
            means[0][j] += ptr[i*3+j -rstride];
            means[0][j] += ptr[i*3+j-3];
            means[0][j] += ptr[i*3+j];
            means[0][j] = means[0][j] / 4;
            
            means[1][j] = ptr[i*3+j-rstride];
            means[1][j] += ptr[i*3+j+3 -rstride];
            means[1][j] += ptr[i*3+j];
            means[1][j] += ptr[i*3+j+3];
            means[1][j] = means[1][j] / 4;
            
            means[2][j] = ptr[i*3+j-3];
            means[2][j] += ptr[i*3+j];
            means[2][j] += ptr[i*3+j-3 + rstride];
            means[2][j] += ptr[i*3+j + rstride];
            means[2][j] = means[2][j] / 4;
            
            means[3][j] = ptr[i*3+j];
            means[3][j] += ptr[i*3+j+3];
            means[3][j] += ptr[i*3+j+rstride];
            means[3][j] += ptr[i*3+j+3+rstride];
            means[3][j] = means[3][j] / 4;*/
            
        
            //std::cout << from.at<Vec3b>(i,j)[0] << " R " << from.at<Vec3b>(i,j)[0] << " G  " << from.at<Vec3b>(i,j)[0] << " B where I: " << i << "and J: " << j;
            
            means[0][z] = kuwaharaLut_2[currentFrame.at<Vec3b>(i-1,j-1)[z]
                                        + currentFrame.at<Vec3b>(i,j-1)[z]
                                        + currentFrame.at<Vec3b>(i-1,j)[z]
                                        + currentFrame.at<Vec3b>(i,j)[z]];
            means[1][z] = kuwaharaLut_2[currentFrame.at<Vec3b>(i-1,j)[z]
                                        + currentFrame.at<Vec3b>(i,j)[z]
                                        + currentFrame.at<Vec3b>(i-1,j+1)[z]
                                        + currentFrame.at<Vec3b>(i,j+1)[z]];
            means[2][z] = kuwaharaLut_2[currentFrame.at<Vec3b>(i,j-1)[z]
                                        + currentFrame.at<Vec3b>(i+1,j-1)[z]
                                        + currentFrame.at<Vec3b>(i,j)[z]
                                        + currentFrame.at<Vec3b>(i,j+1)[z]];
            means[3][z] = kuwaharaLut_2[currentFrame.at<Vec3b>(i,j)[z]
                                        + currentFrame.at<Vec3b>(i,j+1)[z]
                                        + currentFrame.at<Vec3b>(i,j+1)[z]
                                        + currentFrame.at<Vec3b>(i+1,j+1)[z]];
        }
        
        _min = 10000;
        k=0;
        for(z=0;z<4;z++)
        {
            _dist = getEucledianDistanceByColors(currentFrame.at<Vec3b>(i,j)[0],
                                                 currentFrame.at<Vec3b>(i,j)[1],
                                                 currentFrame.at<Vec3b>(i,j)[2],
                                                 means[z][0],
                                                 means[z][1],
                                                 means[z][2]);
            if(_dist < _min)
            {
                _min = _dist;
                k = z;
            }
        }
        
        //ptr_2[i*stride+j] = (uchar)kuwaharaLut_1[minColors[0] + minColors[1] + minColors[2]];
        
        //sum += kuwaharaLut_1[means[k][0]+means[k][1]+means[k][2]];
        
        /*if(kuwaharaLut_1[means[k][0]+means[k][1]+means[k][2]] > 130)
        {
            std::cout << kuwaharaLut_1[means[k][0]+means[k][1]+means[k][2]] << "  , index: " << means[k][0]+means[k][1]+means[k][2] << std::endl;;
        }*/
        
        
        
        //ptr_2[i*stride+j] = kuwaharaLut_1[means[k][0]+means[k][1]+means[k][2]];
        
        ptr_2[i*stride+j] = (_dist > thresh)?255:0;
        
        
        //ptr+=3;
        //}
        //ptr
        
            //_dist = (r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2);
    
    }}
    
    //std::cout << "Sum: " << sum << " ";
    //cvtColor(preprocessedFrameColor,preprocessedFrame,CV_RGB2GRAY);

}

double MarkerDetector::getEucledianDistanceByColors(int r1, int g1, int b1, int r2, int g2, int b2)
{
    return (r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2);
}

vector<Marker> MarkerDetector::findPossibleMarkers()
{
    segmentsFrame.release();
    segmentsFrame = Mat::zeros(height, stride, CV_8UC3);
    
    vector<Marker> realMarkers;
    
    findContours( preprocessedFrame, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    
    vector<Point> approxCurve;
    
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
    {
    //    Scalar color( rand()&255, rand()&255, rand()&255 );
        
      //  drawContours( segmentsFrame, contours, idx, color, CV_FILLED, 8, hierarchy );
        
        if ( 80< contours[idx].size() && contours[idx].size()<1000  )
        {
            approxPolyDP( contours[idx], approxCurve, double ( contours[idx].size() ) * 0.2 , true );
            
            if ( approxCurve.size() ==4 && isContourConvex ( Mat(approxCurve)) )
            {
                Point2f p[4];
                Point2f q[4];
                
                p[0].x= approxCurve[0].x;
                p[0].y= approxCurve[0].y;
                p[1].x= approxCurve[1].x;
                p[1].y= approxCurve[1].y;
                
                p[2].x= approxCurve[2].x;
                p[2].y= approxCurve[2].y;
                p[3].x= approxCurve[3].x;
                p[3].y= approxCurve[3].y;
                
                // távolságok vizsgálata, túl közeli objektumok kiszűrése
                if( sqrt((approxCurve[0].x-approxCurve[1].x)*(approxCurve[0].x-approxCurve[1].x)
                          + (approxCurve[0].y-approxCurve[1].y)*(approxCurve[0].y-approxCurve[1].y)) > 50 &&
                     sqrt((approxCurve[1].x-approxCurve[2].x)*(approxCurve[1].x-approxCurve[2].x)
                          + (approxCurve[1].y-approxCurve[2].y)*(approxCurve[1].y-approxCurve[2].y)) > 50 &&
                     sqrt((approxCurve[2].x-approxCurve[3].x)*(approxCurve[2].x-approxCurve[3].x)
                          + (approxCurve[2].y-approxCurve[3].y)*(approxCurve[2].y-approxCurve[3].y)) > 50 &&
                     sqrt((approxCurve[3].x-approxCurve[0].x)*(approxCurve[3].x-approxCurve[0].x)
                          + (approxCurve[3].y-approxCurve[0].y)*(approxCurve[3].y-approxCurve[0].y)) > 50
                    )
                {
                
                    q[0].x= (float) 0;
                    q[0].y= (float) 0;
                    q[1].x= (float)300;
                    q[1].y= (float) 0;
                
                    q[2].x= (float)300;
                    q[2].y= (float)300;
                    q[3].x= (float) 0;
                    q[3].y= (float)300;
                    
                    p[0].x= approxCurve[0].x;
                    p[0].y= approxCurve[0].y;
                    p[1].x= approxCurve[1].x;
                    p[1].y= approxCurve[1].y;
                
                    p[2].x= approxCurve[2].x;
                    p[2].y= approxCurve[2].y;
                    p[3].x= approxCurve[3].x;
                    p[3].y= approxCurve[3].y;
                
                    //Calculate Perspective matrix
                    Mat warpMatrix = getPerspectiveTransform(p,q);
                    
                    //Rect boundRect = boundingRect(approxCurve);
                    
                    //Mat cuttedImage = Mat(currentFrame,boundRect);
                    
                    Mat currentMarker = Mat();
                    Mat temp = Mat();
                
                    warpPerspective(currentFrameGray, temp, warpMatrix, Size(300,300),INTER_NEAREST);
                    
                    threshold(temp, currentMarker, 0, 255, THRESH_OTSU);
                    
                    bool binaryCode[6][6];
                    
                    double confidence = extractDataFromBinaryMarker(currentMarker, binaryCode);
                    
                    if( confidence < 0.80)
                    {
                        std::cout << "Valószínűleg rossz marker" << std::endl;
                    
                    }
                    else
                    {
                        if(validateBinaryMatrix(binaryCode))
                        {
                            Marker m = Marker(p,hashBinaryMatrix(binaryCode),binaryCode);
                            realMarkers.push_back(m);
                            
                            //std::cout << "Hash: "<< hashBinaryMatrix(binaryCode) << std::endl;
                        }
                        else{
                            std::cout << "Nem valós markert reprezentáló bináris mátrix" << std::endl;
                        }
                    }
                    
                    //currentMarker = temp;
                
                    //currentMarker = cuttedImage;
                
                }
                
              //  drawContours( currentFrame, contours, idx, color, CV_FILLED, 8, hierarchy );
            }
        }
    }
    
    return realMarkers;
}

double MarkerDetector::extractDataFromBinaryMarker(Mat possibleMarker,bool binaryCode[6][6])
{
    Mat currentBlock = Mat(40,40,CV_8UC1);
    double confidence = 1600;
    double blockConfidence,whitePixels;
    std::cout << "Marker start: " << std::endl;
    for( int i = 0; i < 6; i++ )
    {
        for( int j = 0; j < 6; j++ )
        {
            // 50x50 méretű blokkok 5-5 pixel ráhagyással
            
            currentBlock = Mat(possibleMarker,Rect((i*50 + 5), (j*50 + 5),40,40));
            
            whitePixels = (sum(currentBlock)[0] / 255);
            
            binaryCode[i][j] = (whitePixels > 800);
            
            blockConfidence = (binaryCode[i][j])?whitePixels:1600-whitePixels;
            
            std::cout << binaryCode[i][j] << " | ";
            
            if(blockConfidence < confidence){
                confidence = blockConfidence;
            }
        }
        std::cout << std::endl;
    }
    confidence = confidence / 1600;
    std::cout << "Konfidencia szint: " << confidence << std::endl;
    return confidence;
    
}

bool MarkerDetector::validateBinaryMatrix(bool binaryCode[6][6])
{
    bool hasValue = false;
    bool blackBorder = false;
    for( int i = 0; i < 6; i++ )
    {
        blackBorder |= binaryCode[0][i];
        blackBorder |= binaryCode[i][0];
        blackBorder |= binaryCode[5][i];
        blackBorder |= binaryCode[i][5];
        for(int j = 0;j<6;j++)
        {
            hasValue |= binaryCode[i][j];
        }
    }
    return !blackBorder && hasValue;
}

long MarkerDetector::hashBinaryMatrix(bool binaryCode[6][6])
{
    //bool rotate90[4][4],rotate180[4][4],rotate270[4][4];
    
    //char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    
   // char bm[4],rotate90[4],rotate180[4],rotate270[4];
    
    long hash = 0;
    
    string bmc="",r90="",r180="",r270="";
    for( int i = 1; i < 5; i++ )
    {
        for( int j = 1; j < 5; j++ )
        {
            bmc += (binaryCode[i][j])?"0":"1";
            r90 += (binaryCode[j][5-i])?"0":"1";
            r180 += (binaryCode[5-i][5-j])?"0":"1";
            r270 += (binaryCode[5-j][i])?"0":"1";
        }
        
        //char const *byte = bmc.c_str();
        
        //std::cout << std::hex << bmc << std::endl;
       // std::cout << hex_chars[ ( bmc.c_str() & 0x0F ) >> 0 ];
       // bm[i] = bmc[0];
        //rotate90[i] = r90[0];
        //rotate180[i] = r180[0];
        //rotate270[i] = r270[0];
    }
    
    std::cout << bmc << std::endl;
    std::cout << r90 << std::endl;
    std::cout << r180 << std::endl;
    std::cout << r270 << std::endl;
    
   // return std::atoi(bm) + std::atoi(rotate90) + std::atoi(rotate180) + std::atoi(rotate270);
    
    //hash = Integer.parseInt(
    
    //return std::strtol(bmc.c_str(), & ptr, 2) + std::strtol(r90.c_str(), & ptr, 2) + std::strtol(r180.c_str(), & ptr, 2) + std::strtol(r270.c_str(), & ptr, 2);
    
    char *ptr;
    
    hash ^= std::strtol(bmc.c_str(), & ptr, 2) / 64 - 1;

    *ptr = NULL;
    
    hash ^= std::strtol(r90.c_str(), & ptr, 2) / 64 - 1;
    
    *ptr = NULL;
    
    hash ^= std::strtol(r180.c_str(), & ptr, 2) / 64 - 1;
    
    *ptr = NULL;
    
    hash ^= std::strtol(r270.c_str(), & ptr, 2) / 64 - 1;
    
    return hash % 1337;
}



/*IplImage* MarkerDetector::CurrentFrame()
{
    return new IplImage(currentFrame);
}*/

Mat MarkerDetector::CurrentFrame() const
{
    return currentFrame;
}

Mat MarkerDetector::CurrentFrameGray() const
{
    return currentFrameGray;
}

Mat MarkerDetector::PreprocessedFrame() const
{
    return preprocessedFrame;
}
Mat MarkerDetector::PreprocessedFrameColor() const
{
    return preprocessedFrameColor;
}
Mat MarkerDetector::SegmentsFrame() const
{
    return segmentsFrame;
}

void MarkerDetector::setCurrentFrame(Mat value)
{
    currentFrame.release();
    currentFrame = value;
    currentFrameGray.release();
    currentFrameGray = Mat();
    cvtColor(currentFrame, currentFrameGray, CV_RGB2GRAY);
}

//void MarkerDetector::set

void MarkerDetector::init()
{
    Mat warpMatrix = Mat(3,3,CV_32FC1);
    currentFrameGray = Mat(Mat::zeros(height, stride, CV_8UC1));
    preprocessedFrame = Mat(Mat::zeros(height, stride, CV_8UC1));
    preprocessedFrameColor = Mat(Mat::zeros(height, stride, CV_8UC3));
    int j = 0;
    for(int i = 0; i< 768 ; i+=3)
    {
        kuwaharaLut_1[i] = j;
        kuwaharaLut_1[i+1] = j;
        kuwaharaLut_1[i+2] = j;
        j++;
    }
    j = 0;
    for(int i = 0; i< 1024 ; i+=4)
    {
        kuwaharaLut_2[i] = j;
        kuwaharaLut_2[i+1] = j;
        kuwaharaLut_2[i+2] = j;
        kuwaharaLut_2[i+3] = j;
        j++;
    }
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
    rstride = value*3;
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
