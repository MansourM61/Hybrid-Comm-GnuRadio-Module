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

#ifndef INCLUDED_FSO_COMM_POINTING_ERRORS_IMPL_H
#define INCLUDED_FSO_COMM_POINTING_ERRORS_IMPL_H

#include <FSO_Comm/Pointing_Errors.h>

namespace gr {
  namespace FSO_Comm {

    class Pointing_Errors_impl : public Pointing_Errors
    {
     private:
      // Nothing to declare in this block.
      float fJitter;  // pointing errors jitter (mm)
      float fDiaTx;  // Tx aperture diameter (mm)
      float fThetaTx;  // Tx beam full-divergence angle (mDeg)
      float fLinkLen;  // Rx aperture diameter (mm)
      float fDiaRx;  // Link length (m)
      float fTempCorr;  // pointing errors temporal correlation (ms)
      float fSampRate;  // sample rate (bps)
      float fW2_eq_PE;  // pointing error equivalent beam size squared (m)
      int iSig2CCRatio;  // number of signal samples to number of channel coefficients ratio
      Norm_Rand_Gen RandGen;

      void CalcParam(void);  // calculate channel coefficient parameters

     public:
      Pointing_Errors_impl(float jitter = DEF_JITTER, float diaTx = DEF_DIA_TX, float thetaTx = DEF_THETA_TX,
                          float diaRx = DEF_DIA_RX, float linkLen = DEF_LINK_LEN, float tempCorr = PE_TEMP_CORR, float sampRate = SAMP_RATE);
      ~Pointing_Errors_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set jitter (mm)
      void set_Jitter(float jitter)
      {
        fJitter = CONSTRAIN(jitter, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Jitter = " << fJitter << " (mm)" << std::endl;
        #endif

        CalcParam();
      }

      // Get jitter (mm)
      float get_Jitter(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Jitter = " << fJitter << " (mm)" << std::endl;
        #endif

        return fJitter;
      }

      // Set Tx aperture diameter (mm)
      void set_DiaTx(float diaTx)
      {
        fDiaTx = CONSTRAIN(diaTx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Tx aperture diameter = " << fDiaTx << " (mm)" << std::endl;
        #endif

        CalcParam();
      }

      // Get Tx aperture diameter (mm)
      float get_DiaTx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Tx aperture diameter = " << fDiaTx << " (mm)" << std::endl;
        #endif

        return fDiaTx;
      }

      // Set transmitter half-angle beam divergence (mDeg)
      void set_ThetaTx(float thetaTx)
      {
        fThetaTx = CONSTRAIN(thetaTx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Tx half-angle beam divergence = " << fThetaTx << " (Deg)" << std::endl;
        #endif

        CalcParam();
      }

      // Get transmitter half-angle beam divergence (mDeg)
      float get_ThetaTx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Tx half-angle beam divergence = " << fThetaTx << " (Deg)" << std::endl;
        #endif

        return fThetaTx;
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

      // Set temporal correlation of pointing errors channel
      void set_TempCorr(float tempcorr)
      {
        fTempCorr = CONSTRAIN(tempcorr, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Temporal correlation of pointing errors channel = " << fTempCorr << " (ms)" << std::endl;
        #endif

        CalcParam();
      }

      // Get temporal correlation of pointing errors channel (ms)
      float get_TempCorr(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Temporal correlation of pointing errors channel = " << fTempCorr << " (ms)" << std::endl;
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

#endif /* INCLUDED_FSO_COMM_POINTING_ERRORS_IMPL_H */

