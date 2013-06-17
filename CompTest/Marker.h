//
//  Marker.h
//  CompTest
//
//  Created by Árpád Kiss on 2012.12.05..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#ifndef __CompTest_Marker__
#define __CompTest_Marker__

#include "cv.h"
#include <iostream>

using namespace cv;

class Marker
{
public:
    Marker(Point2f const newCornerPoints[4],long newHash,bool const newBinaryCode[6][6]);
    Point2f CornerPointsAt(int idx) const;
    Mat RotationMatrix() const;
    Mat TranslationMatrix() const;
    void setRotationMatrix(Mat const newRotationMatrix);
    void setTranslationMatrix(Mat const newTranslationMaxtrix);
    void setCornerPointAt(int idx, Point2f const newPoint);
    int Age() const;
    long Hash() const;
    void setAge(int const newAge);
private:
    Point2f cornerPoints[4];
    Mat rotationMatrix;
    Mat translationMatrix;
    long hash;
    bool binaryCode[6][6];
    bool calculatePosition();
    int age;
    int currentOrientation;
};


#endif
