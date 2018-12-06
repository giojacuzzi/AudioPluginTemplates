/* Copyright (c) 2018, Gio Jacuzzi (giojacuzzi@gmail.com) */

#pragma once

#include <stdio.h>

/**
 * A biquad is a widely used two-pole, two-zero digital filter. "Biquad" is an
 * abbreviation of "biquadratic", which refers to the fact that in the Z domain,
 * its transfer function is the ratio of two quadratic functions. The transfer
 * function of the biquad can be defined as:
 *
 *           b₀ + b₁*z⁻¹ + b₂*z⁻²
 *   H(z) = ----------------------
 *           a₀ + a₁*z⁻¹ + a₂*z⁻²
 *
 * Often, the coefficients are normalized such that a₀ = b₀ = 1, collecting them into an overall
 * gain coefficient. The most straightforward implementation becomes the "Direct Form 1":
 *
 *   y[n] = (b₀/a₀) * x[n] + (b₁/a₀) * x[n-1] + (b₂/a₀) * x[n-2]
 *                         - (a₁/a₀) * y[n-1] - (a₂/a₀) * y[n-2]
 *
 *
 */
class Biquad {
public:
    //==============================================================================
    // Biquad filter types.
    enum FilterType
    {
        lowpass = 0,
        highpass//,
        //    bandpass,
        //    notch,
        //    peak,
        //    lowshelf,
        //    highshelf
    };

    //==============================================================================
    Biquad();
    Biquad(double fs, FilterType type, double fc, double q, double peakGainDb);
    ~Biquad();

    //==============================================================================
    void setFs(double fs);
    void setBiquad(FilterType type, double fc, double q, double peakGainDb);
    void setType(FilterType type);
    void setFc(double fc);
    void setQ(double q);
    void setPeakGain(double db);

    //==============================================================================
    float process(float in);

private:
    //==============================================================================
    void calcBiquad(void);

    //==============================================================================
    FilterType type;
    double Fs; // Sampling frequency
    double a0, a1, a2, b1, b2; // Biquad coefficients
    double Fc; // Center (or "cutoff") frequency
    double Q; // Resonance
    double peakGain; // Used only for peaking and shelving filters
    double x1, x2, y1, y2; // Delays
};
