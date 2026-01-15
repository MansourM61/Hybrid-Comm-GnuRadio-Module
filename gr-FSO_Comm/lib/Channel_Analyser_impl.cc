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
#include "Channel_Analyser_impl.h"

namespace gr {
  namespace FSO_Comm {

    Channel_Analyser::sptr
    Channel_Analyser::make(int sampsPerSymb, int winSize, std::string measType, int edgeFindingRange)
    {
      return gnuradio::get_initial_sptr
        (new Channel_Analyser_impl(sampsPerSymb, winSize, measType, edgeFindingRange));
    }

    const std::string Channel_Analyser_impl::str_flags[8] = {PS_STR, PN_STR, SNR_STR, QF_STR, DC_STR, SI0_STR, SI1_STR, SIm_STR};
    const char Channel_Analyser_impl::meas_types[8] = {Ps, Pn, SNR, QF, DC, SI0, SI1, SIm};

    /*
     * The private constructor
     */
    Channel_Analyser_impl::Channel_Analyser_impl(int sampsPerSymb, int winSize, std::string measType, int edgeFindingRange)
      : gr::sync_block("Channel_Analyser",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))), ptr_fSigMerit(nullptr), fSigPower(0), fNoisePower(1), fSNR(2), fQ_fac(3),  fDC(4), fSI0(5), fSI1(6), fSIm(7)
    {
      #ifdef _DEBUG_MODE_
      std::cout << "Channel_Analyser_impl: Constructor called." << std::endl;
      #endif

      meas_val[0] = &fSigPower;
      meas_val[1] = &fNoisePower;
      meas_val[2] = &fSNR;
      meas_val[3] = &fQ_fac;
      meas_val[4] = &fDC;
      meas_val[5] = &fSI0;
      meas_val[6] = &fSI1;
      meas_val[7] = &fSIm;

      this->set_SampsPerSymb(sampsPerSymb);  // set samples per symbol
      this->set_WinSize(winSize);  // set measurement window size
      this->set_MeasType(measType);  // set measurement type
      this->set_EdgeFindingRange(edgeFindingRange);  // set window range to find edge
    }

    /*
     * Our virtual destructor.
     */
    Channel_Analyser_impl::~Channel_Analyser_impl()
    {
    }

    int
    Channel_Analyser_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      int numOfWin = noutput_items/iWinSize;  // number of available windows in the input array

      #ifdef _DEBUG_MODE_
      std::cout << "Channel_Analyser_impl: Work called." << std::endl;
      std::cout << "Measurement mode = " << CPRN(cMeasType) << std::endl;
      std::cout << "Measurement mode = " << str_flags[cMeasType] << std::endl;
      std::cout << "Number of output items = " << noutput_items << std::endl;      
      std::cout << "Number of total windows = " << numOfWin << std::endl;      
      #endif

      // Do <+signal processing+>
      #ifdef _DEBUG_MODE_
      std::cout << "DSP Main Loop: Start" << std::endl;
      std::cout << "===========================================" << std::endl;
      std::cout  << std::endl;
      #endif
      for (int winIndex = 0; winIndex < numOfWin; winIndex++)  // go through the available windows
      {
        int startIndex = winIndex*iWinSize;  // first item index from the window
        int sampIndex = 0;  // sample index within the window
        #ifdef _DEBUG_MODE_
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Window index = " << winIndex << std::endl;
        std::cout << "Window start index = " << startIndex << std::endl;
        #endif

        // find the DC value of the signal
        float DC = DC_Value<float>(in, iWinSize, startIndex);  // calculate DC value
        #ifdef _DEBUG_MODE_
        std::cout << "Window DC value = " << DC << std::endl;
        #endif

        // find the first edge of the signal
        int firstEdge = FindFirstEdge<float>(in, iWinSize, startIndex, DC, iEdgeFindingRange);  // search for first edge
        firstEdge = (firstEdge > (iWinSize/2)) ? 0 : firstEdge;  // is the signal seems to be DC, set the first edge

        #ifdef _DEBUG_MODE_
        std::cout << "Window first edge index = " << firstEdge << std::endl;
        #endif
        
        float arr_fRes[4];  // result array
        int arr_iLen[2];  // length array

        // sample the signal and extract levels 0 and 1, then calculate mean and variance of each level
        CalcMeanVar<float>(in, iWinSize - firstEdge, firstEdge, DC, iSampsPerSymb, arr_iLen, arr_fRes);  // calculate mean and variance of levels 0 and 1

        int n_0 = 0;  // number of level 0
        int n_1 = 0;  // number of level 1
        float mean_0 = 0.0;  // mean value of level 0
        float mean_1 = 0.0;  // mean value of level 1
        float var_0 = 0.0;  // variance value of level 0
        float var_1 = 0.0;  // variance value of level 1

        // update length varible
        n_0 = arr_iLen[0];
        n_1 = arr_iLen[1];

        // update mean and variance variable
        mean_0 = arr_fRes[0];  // calculate mean value of level 0
        mean_1 = arr_fRes[1];  // calculate mean value of level 1
        var_0 = arr_fRes[2];  // calculate variance value of level 0
        var_1 = arr_fRes[3];  // calculate variance value of level 1

        #ifdef _DEBUG_MODE_
        std::cout << "Window number of 0 = " << n_0 << std::endl;
        std::cout << "Window mean of 0 = " << mean_0 << std::endl;
        std::cout << "Window var of 0 = " << var_0 << std::endl;
        std::cout << "Window number of 1 = " << n_1 << std::endl;
        std::cout << "Window mean of 1 = " << mean_1 << std::endl;
        std::cout << "Window var of 1 = " << var_1 << std::endl;
        std::cout << "Window average = " << DC << std::endl;
        #endif

        // calculate the merits signal
        fSigPower = POW2((mean_1*n_1 - mean_0*n_0)/(n_0 + n_1));  // initial signal power
        fNoisePower = (var_0 + var_1)/2.0;  // initial noise power
        fSNR = 10*log10(fSigPower/fNoisePower);  // signal-to-noise ratio (SNR)
        fQ_fac = abs(mean_1 - mean_0)/(sqrt(var_0) + sqrt(var_1));  // Q-factor
        fDC = DC;  // DC value
        fSI0 = var_0/POW2(mean_0);  // SI 0
        fSI1 = var_1/POW2(mean_1);  // SI 1
        fSIm = (fSI0 + fSI1)/2.0;  // SIm

        #ifdef _DEBUG_MODE_
        std::cout << "Signal power = " << fSigPower << std::endl;
        std::cout << "Noise power = " << fNoisePower << std::endl;
        std::cout << "Signal-to-noise ratio = " << fSNR << std::endl;
        std::cout << "Q-factor = " << fQ_fac << std::endl;
        std::cout << "DC value = " << DC << std::endl;
        std::cout << "Level 0 SI = " << fSI0 << std::endl;
        std::cout << "Level 1 SI = " << fSI1 << std::endl;
        std::cout << "Average SI = " << fSIm << std::endl;
        std::cout << "Measured value = " << *ptr_fSigMerit << std::endl;        
        #endif

       FillArray<float>((out + startIndex), *ptr_fSigMerit, iWinSize);  // update output array

      }
      #ifdef _DEBUG_MODE_
      std::cout << "===========================================" << std::endl;
      std::cout << "DSP Main Loop: End" << std::endl;
      std::cout  << std::endl;
      #endif

      std::cout << "Measured value = " << *ptr_fSigMerit << std::endl;        

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace FSO_Comm */
} /* namespace gr */

