/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#include <iostream>
#include <math.h>
#include "Biquad.h"

//==============================================================================
Biquad::Biquad()
{
    this->Fs = 44100;
    setBiquad(Biquad::FilterType::lowpass, 10000, 0.7071, 0.0);
    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    x1 = x2 = y1 = y2 = 0.0;
}

Biquad::Biquad(double fs, FilterType type, double Fc, double Q, double peakGainDB)
{
    this->Fs = fs;
    setBiquad(type, Fc, Q, peakGainDB);
    x1 = x2 = y1 = y2 = 0.0;
}

Biquad::~Biquad()
{
    //
}

//==============================================================================
void Biquad::setFs(double fs)
{
    this->Fs = fs;
}

void Biquad::setBiquad(FilterType type, double Fc, double Q, double peakGainDB)
{
    // TODO: invalid input edge cases
    //std::cout << "Biquad::setBiquad(" << type << ", " << Fc << ", " << Q << ")" << std::endl;
    this->type = type;
    this->Fc = Fc;
    this->Q = Q;
    this->peakGain = peakGainDB;
    calcBiquad();
}

void Biquad::setType(FilterType type)
{
    this->type = type;
    calcBiquad();
}

void Biquad::setFc(double fc)
{
    this->Fc = fc;
    calcBiquad();
}

void Biquad::setQ(double q)
{
    this->Q = q;
    calcBiquad();
}

void Biquad::setPeakGain(double db)
{
    this->peakGain = db;
    calcBiquad();
}

//==============================================================================
void Biquad::calcBiquad(void)
{
    float w0 = 2 * M_PI * Fc/Fs;
    float sinw0 = sin(w0);
    float cosw0 = cos(w0);
    switch (type) {
        case Biquad::FilterType::lowpass: {
            float alpha = sinw0 / 2.0 * Q;
            a0 = 1 + alpha;
            a1 = -2 * cosw0;
            a2 = 1 - alpha;
            b1 = 1 - cosw0;
            b2 = (1 - cosw0) / 2.0;
            break;
        }
        case Biquad::FilterType::highpass: {
            float alpha = sinw0 / 2.0 * Q;
            a0 = 1 + alpha;
            a1 = -2 * cosw0;
            a2 = 1 - alpha;
            b1 = -(1 + cosw0);
            b2 = (1 + cosw0) / 2.0;
            break;
        }
        default:
            return;
    }
    //std::cout << "Biquad::calcBiquad\na0 " << a0 << "\na1 " << a1 << "\na2 " << a2 << "\nb1 " << b1 << "\nb2 " << b2 << std::endl;
}

//==============================================================================
// y[n] = (b₀/a₀) * x[n] + (b₁/a₀) * x[n-1] + (b₂/a₀) * x[n-2]
//                       - (a₁/a₀) * y[n-1] - (a₂/a₀) * y[n-2]
float Biquad::process(float x)
{
    // Direct form 1
    // x[n] == the current input sample
    // x[n-1] == the previous input sample
    // x[n-2] == the previous previous input sample
    // y[n-1] == the previous output sample
    // y[n-2] == the previous previous output sample
    float y = x + b1/a0*x1 + b2/a0*x2 - a1/a0*y1 - a2/a0*y2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    return y;
}
