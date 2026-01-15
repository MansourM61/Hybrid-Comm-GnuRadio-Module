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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Pointing_Errors_impl.h"

namespace gr {
  namespace FSO_Comm {

    Pointing_Errors::sptr
    Pointing_Errors::make(float jitter, float diaTx, float thetaTx, float diaRx, float linkLen, float tempCorr, float sampRate)
    {
      return gnuradio::get_initial_sptr
        (new Pointing_Errors_impl(jitter, diaTx, thetaTx, diaRx, linkLen, tempCorr, sampRate));
    }


    /*
     * The private constructor
     */
    Pointing_Errors_impl::Pointing_Errors_impl(float jitter, float diaTx, float thetaTx, float diaRx, float linkLen, float tempCorr, float sampRate)
      : gr::sync_block("Pointing_Errors",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 2, sizeof(float))), RandGen()
    {
      #ifdef _DEBUG_MODE_
      std::cout << "Pointing_Errors_impl: Constructor called." << std::endl;
      #endif
      this->set_Jitter(jitter);  // set jitter (mm)
      this->set_DiaTx(diaTx);  // set Tx diameter (mm)
      this->set_ThetaTx(thetaTx);  // set Tx theta (mDeg)
      this->set_LinkLen(linkLen);  // set link length (m)
      this->set_DiaRx(diaRx);  // set Rx diameter (mm)
      this->set_TempCorr(tempCorr);  // set temporal correlation of pointing errors channel
      this->set_SampRate(sampRate);  // set sample rate
    }

    void
    Pointing_Errors_impl::CalcParam(void)
    {
      float fW_Rx = fDiaTx*1e-3/2.0 + fLinkLen*tan(fThetaTx*M_PI/360.0);  // beam radius at Rx side
      float v_PE = sqrt(M_PI/2.0)*(fDiaRx*1e-3/(2.0*fW_Rx));  // aperture to vertical beam size ratio
      fW2_eq_PE = POW2(fW_Rx)*sqrt(M_PI)*erf(v_PE)/(2.0*v_PE*exp(-POW2(v_PE)));  // equivalent vertical beam size at receiver side

      float fF_t = 1.0/(fTempCorr*1e-3);  // pointing errors fading maximum frequency (Hz)

      iSig2CCRatio = MAX(int(ceil(fSampRate*1.0/fF_t)), 1);  // number of signal samples to number of channel coefficients ratio

      sync_block::set_output_multiple(iSig2CCRatio);

      #ifdef _DEBUG_MODE_
      std::cout << "Pointing_Errors_impl: CalcParam called." << std::endl;
      std::cout << "Pointing errors fading maximum frequency = " << fF_t << " Hz" << std::endl;
      std::cout << "Pointing errors equivalent beam radius squared = " << fW2_eq_PE << " m" << std::endl;
      std::cout << "Number of signal samples to number of channel coefficients ratio = " << iSig2CCRatio << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Pointing_Errors_impl::~Pointing_Errors_impl()
    {
    }

    int
    Pointing_Errors_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      float *out_h = nullptr;

      if(output_items.size() == 2)  // h output is connected
      {
        out_h = (float *) output_items[1];
      }

      int NumOfSeg = noutput_items/iSig2CCRatio;  // available number of segments

      bool bhConnected = ( (out_h != nullptr) ) ? true : false;  // if loss is to be transferred

      #ifdef _DEBUG_MODE_
      std::cout << "Pointing_Errors_impl: Work called." << std::endl;
      std::cout << "Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
      std::cout << "Jitter = " << fJitter << " (mm)" << std::endl;
      std::cout << "Tx aperture diameter = " << fDiaTx << " (mm)" << std::endl;
      std::cout << "Tx half-angle beam divergence = " << fThetaTx << " (Deg)" << std::endl;
      std::cout << "Rx aperture diameter = " << fDiaRx << " (mm)" << std::endl;
      std::cout << "Temporal correlation of pointing errors channel = " << fTempCorr << " (ms)" << std::endl;
      std::cout << "Sample rate = " << fSampRate << " (bps)" << std::endl;
      std::cout << "Number of signal samples to number of channel coefficients ratio = " << iSig2CCRatio << std::endl;
      std::cout << "Available number of segments = " << NumOfSeg << std::endl;
      std::cout << "Channel coefficient pin is " << ((bhConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      float ChannCoeff;  // channel coefficient

      // Do <+signal processing+>
      for (int index_s = 0; index_s < NumOfSeg; ++index_s)  // go through available segments
      {
        ChannCoeff = RandGen.RayleighNumGen(fJitter*1e-3, fW2_eq_PE);  // generate a pointing errors channel coefficient based on Rayleigh distribution
        #ifdef _DEBUG_MODE_
        std::cout << "Index = " << index_s << " out of " << NumOfSeg - 1 << std::endl;
        std::cout << "Current channel coefficient = " << ChannCoeff << std::endl;
        #endif

        LinearMap<float>((in + index_s*iSig2CCRatio), (out + index_s*iSig2CCRatio), iSig2CCRatio, ChannCoeff, 0.0);  // apply channel coefficient

        if(bhConnected == true)  // if h is to transferred
        {
          FillArray<float>((out_h + index_s*iSig2CCRatio), ChannCoeff, iSig2CCRatio);  // fill channel coefficient array
        }
      }

      #ifdef _ARRAY_MODE_
      std::cout << "Input = ";
      DisplayArray<float>(in, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Output = ";
      DisplayArray<float>(out, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bhConnected == true)  // if h is to transferred
      {
        std::cout << "Final h = ";
        DisplayArray<float>(out_h, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace FSO_Comm */
} /* namespace gr */

