// Copyright (c) 2022, CJackW
// All rights reserved.

// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. 

#include <iostream>
#include <string>
#include <cmath>

#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QColor>
#include <Qt>
#include <QSlider>
#include <QWidget>
#include <QVBoxLayout>
#include <QObject>
#include <QString>

int maxIterations = 1000;

float map(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
    float slope = 1.0 * (outputEnd - outputStart) / (inputEnd - inputStart);
    return outputStart + slope * (value - inputStart);
}

QSlider& createSlider(int min, int max)
{
    QSlider* sld = new QSlider(Qt::Horizontal);
    sld->setTickInterval(0.001);
    sld->setMinimum(min);
    sld->setMaximum(max);

    return *sld;
}

void draw(float scaleMinimum, float scaleMaximum, int windowWidth, int windowHeight, QImage& img)
{   
    for(auto i = 0; i < windowWidth; ++i)
    {
        for(auto j = 0; j < windowHeight; ++j)
        {
            float xValue = map(i, 0, windowWidth, scaleMinimum, scaleMaximum);
            float yValue = map(j, 0, windowHeight, scaleMinimum, scaleMaximum);

            float xStart = xValue;
            float yStart = yValue;

            int numberOfIterations = 0;

            while (numberOfIterations < maxIterations)
            {
                float aValue = xValue * xValue - yValue * yValue;
                float bValue = 2 * xValue * yValue;

                xValue = aValue + xStart;
                yValue = bValue + yStart;

                if(xValue * xValue + yValue * yValue > 16)
                {
                    break;
                }

                ++numberOfIterations;
            }

            float bright = map(numberOfIterations, 0, maxIterations, 0, 1);
            bright = map(sqrt(bright), 0, 1, 0, 255);

            if(numberOfIterations == maxIterations)
            {
                bright = 0;
            }

            img.setPixelColor(i, j, QColor(bright, bright, bright));
        }
    }
}

int main(int argc, char** argv)
{
    if (argc == 1) { maxIterations = 1000; }
    else if(argc > 2) { std::cerr << "Too many arguments passed in!" << std::endl; return 1; }
    else { maxIterations = std::stoi(argv[1]); }

    // Create the app
    QApplication app(argc, argv);

    int windowWidth = 1280;
    int windowHeight = 1280;

    // Create the image
    QImage img(windowWidth, windowHeight, QImage::Format::Format_RGB32);

    // --- Main Code ---

    float scaleMinimum = -2.5;
    float scaleMaximum = 2.5;

    draw(scaleMinimum, scaleMaximum, windowWidth, windowHeight, img);

    // -----------------

    // Display the image
    QLabel display;
    display.setPixmap(QPixmap::fromImage(img));

    display.window()->setWindowTitle(QString("Mandelbrot Set (%1 Iterations)").arg(QString::number(maxIterations)));

    display.show();

    // // Adjustable Sliders
    // QWidget sliderHolder;
    // QVBoxLayout sliderLayout(&sliderHolder);

    // QSlider& minimumSlider = createSlider(-2.5, 0);
    // QSlider& maximumSlider = createSlider(0, 2.5);

    // QLabel minimumLabel("Minimum");
    // QLabel maximumLabel("Maximum");

    // sliderLayout.addWidget(&minimumLabel);
    // sliderLayout.addWidget(&minimumSlider);
    // sliderLayout.addWidget(&maximumLabel);
    // sliderLayout.addWidget(&maximumSlider);

    // QObject::connect(&minimumSlider, &QSlider::sliderReleased, [&](){ scaleMinimum = minimumSlider.value(); draw(scaleMinimum, scaleMaximum, windowWidth, windowHeight, img); display.setPixmap(QPixmap::fromImage(img)); });
    // QObject::connect(&maximumSlider, &QSlider::sliderReleased, [&](){ scaleMaximum = maximumSlider.value(); draw(scaleMinimum, scaleMaximum, windowWidth, windowHeight, img); display.setPixmap(QPixmap::fromImage(img)); });
    
    // sliderHolder.show();

    // Run the app
    return app.exec();
}