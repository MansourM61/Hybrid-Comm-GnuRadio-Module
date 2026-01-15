/* -*- c++ -*- */
/*
 * Copyright 2020 gr-FSO_Comm author.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_FSO_COMM_TURBULENCE_IMPL_H
#define INCLUDED_FSO_COMM_TURBULENCE_IMPL_H

#include <FSO_Comm/Turbulence.h>

namespace gr {
  namespace FSO_Comm {

    class Turbulence_impl : public Turbulence
    {
     private:
      // Nothing to declare in this block.
      float fCn2;  // the Refractive Index Structure Coefficient (m^-2/3)
      float fWavelength;  // wavelength (nm)
      float fThetaTx;  // Tx beam full-divergence angle (mDeg)
      float fLinkLen;  // Rx aperture diameter (mm)
      float fDiaRx;  // Link length (m)
      float fTempCorr;  // pointing errors temporal correlation (ms)
      float fSampRate;  // sample rate (bps)
    	float fsig_x;  // sigma parameter weak turbulence
    	float fmu_x;  // mu parameter weak turbulence
      int iSig2CCRatio;  // number of signal samples to number of channel coefficients ratio
      Norm_Rand_Gen RandGen;

      void CalcParam(void);  // calculate channel coefficient parameters

     public:
      Turbulence_impl(float Cn2 = DEF_CN2, float wavelength = DEF_WL, float diaRx = DEF_DIA_RX,
                      float LinkLen = DEF_LINK_LEN, float tempCorr = T_TEMP_CORR, float sampRate = SAMP_RATE);
      ~Turbulence_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set the Refractive Index Structure Coefficient (m^-2/3)
      void set_Cn2(float Cn2)
      {
        fCn2 = CONSTRAIN(Cn2, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "The Refractive Index Structure Coefficient) = " << fCn2 << " (m^-2/3" << std::endl;
        #endif

        CalcParam();
      }

      // Get the Refractive Index Structure Coefficient (m^-2/3)
      float get_Cn2(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "The Refractive Index Structure Coefficient = " << fCn2 << " (m^-2/3" << std::endl;
        #endif

        return fCn2;
      }

      // Set wavelength
      void set_Wavelength(float wavelength)
      {
        fWavelength = CONSTRAIN(wavelength, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Wavelength = " << fWavelength << " (nm)" << std::endl;
        #endif

        CalcParam();
      }

      // Get wavelength
      float get_Wavelength(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Wavelength = " << fWavelength << " (nm)" << std::endl;
        #endif

        return fWavelength;
      }

      // Set link length
      void set_LinkLen(float linkLen)
      {
        fLinkLen = CONSTRAIN(linkLen, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
        #endif

        CalcParam();
      }

      // Get link length
      float get_LinkLen(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
        #endif

        return fLinkLen;
      }

      // Set Rx aperture diameter (mm)
      void set_DiaRx(float diaRx)
      {
        fDiaRx = CONSTRAIN(diaRx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Rx aperture diameter = " << fDiaRx << " (mm)" << std::endl;
        #endif

        CalcParam();
      }

      // Get Rx aperture diameter (mm)
      float get_DiaRx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx aperture diameter = " << fDiaRx << " (mm)" << std::endl;
        #endif

        return fDiaRx;
      }

      // Set temporal correlation of turbulence channel
      void set_TempCorr(float tempcorr)
      {
        fTempCorr = CONSTRAIN(tempcorr, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Temporal correlation of turbulence channel = " << fTempCorr << " (ms)" << std::endl;
        #endif

        CalcParam();
      }

      // Get temporal correlation of turbulence channel (ms)
      float get_TempCorr(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Temporal correlation of turbulence channel = " << fTempCorr << " (ms)" << std::endl;
        #endif

        return fTempCorr;
      }

      // Set sample rate
      void set_SampRate(float sampRate)
      {
        fSampRate = CONSTRAIN(sampRate, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Sample rate = " << fSampRate << " (bps)" << std::endl;
        #endif

        CalcParam();
      }

      // Get sample rate (bps)
      float get_SampRate(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Sample rate = " << fSampRate << " (bps)" << std::endl;
        #endif

        return fSampRate;
      }

    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_TURBULENCE_IMPL_H */

