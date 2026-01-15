/* -*- c++ -*- */
/*
 * Copyright 2020 gr-Hybrid_Comm author.
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
#include "Signal_Quality_Metre_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Signal_Quality_Metre::sptr
    Signal_Quality_Metre::make(int packetSize, int sampsPerSymb, std::string measType, int edgeFindingRange)
    {
      return gnuradio::get_initial_sptr
        (new Signal_Quality_Metre_impl(packetSize, sampsPerSymb, measType, edgeFindingRange));
    }

    const std::string Signal_Quality_Metre_impl::strPs = PS_STR;
    const std::string Signal_Quality_Metre_impl::strPn = PN_STR;
    const std::string Signal_Quality_Metre_impl::strSNR = SNR_STR;
    const std::string Signal_Quality_Metre_impl::strQF = QF_STR;

    /*
     * The private constructor
     */
    Signal_Quality_Metre_impl::Signal_Quality_Metre_impl(int packetSize, int sampsPerSymb, std::string measType, int edgeFindingRange)
      : gr::sync_block("Signal Quality Metre",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 2, sizeof(float)))
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Signal_Quality_Metre_impl: Constructor called." << std::endl;
      #endif
      this->set_SampsPerSymb(sampsPerSymb);  // set samples per symbol
      this->set_PacketSize(packetSize);  // set packet size
      this->set_MeasType(measType);  // set measurement type
      this->set_EdgeFindingRange(edgeFindingRange);  // set window range to find edge

      #ifdef _FLOW_MODE_
      std::cout << "Signal_Quality_Metre_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Signal_Quality_Metre_impl::~Signal_Quality_Metre_impl()
    {
    }

    int
    Signal_Quality_Metre_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Signal_Quality_Metre_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      float *out_DC = nullptr;

      if(output_items.size() == 2)  // if DC output is connected
      {
        out_DC = (float *) output_items[1];
      }

      bool bDCConnected = (out_DC != nullptr) ? true : false;

      int numOfWin = noutput_items/iPacketSize;  // number of available windows in the input array

      #ifdef _DEBUG_MODE_
      std::cout << "Signal_Quality_Metre_impl: Work called." << std::endl;
      std::cout << "Signal_Quality_Metre_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Signal_Quality_Metre_impl: Number of total windows = " << numOfWin << std::endl;      
      std::cout << "Signal_Quality_Metre_impl: DC value pin is " << ((bDCConnected == true) ? "" : "not ") << std::endl;
      #endif

      // Do <+signal processing+>

      for (int winIndex = 0; winIndex < numOfWin; winIndex++)  // go through the available windows
      {
        int startIndex = winIndex*iPacketSize;  // first item index from the window
        int sampIndex = 0;  // sample index within the window
        #ifdef _DEBUG_MODE_
        std::cout << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window index = " << winIndex << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window start index = " << startIndex << std::endl;
        #endif

        // find the DC value of the signal
        float DC = DC_Value<float>(in, iPacketSize, startIndex);  // calculate DC value
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Window DC value = " << DC << std::endl;
        #endif

        // find the first edge of the signal
        int firstEdge = FindFirstEdge<float>(in, iPacketSize, startIndex, DC, iEdgeFindingRange);  // search for first edge
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Window first edge index = " << firstEdge << std::endl;
        #endif
        
        float arr_fRes[4];  // result array
        int arr_iLen[2];  // length array

        // sample the signal and extract levels 0 and 1, then calculate mean and variance of each level
        CalcMeanVar<float>(in, iPacketSize - firstEdge, firstEdge, DC, iSampsPerSymb, arr_iLen, arr_fRes);  // calculate mean and variance of levels 0 and 1

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
        std::cout << "Signal_Quality_Metre_impl: Window number of 0 = " << n_0 << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window mean of 0 = " << mean_0 << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window var of 0 = " << var_0 << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window number of 1 = " << n_1 << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window mean of 1 = " << mean_1 << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Window var of 1 = " << var_1 << std::endl;
        #endif

        // calculate the merits signal
        fSigPower = POW2((mean_1*n_1 - mean_0*n_0)/(n_0 + n_1));  // signal power
        fNoisePower = (var_0 + var_1)/2.0;  // noise power
        fSNR = 10*log10(fSigPower/fNoisePower);  // signal-to-noise ratio (SNR)
        fQ_fac = abs(mean_1 - mean_0)/(sqrt(var_0) + sqrt(var_1));  // Q-factor

        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Signal power = " << fSigPower << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Noise power = " << fNoisePower << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Signal-to-noise ratio = " << fSNR << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Q-factor = " << fQ_fac << std::endl;
        std::cout << "Signal_Quality_Metre_impl: Measured value = " << *ptr_fSigMerit << std::endl;
        #endif

        FillArray<float>((out + startIndex), *ptr_fSigMerit, iPacketSize);  // update output array

        if(bDCConnected == true)  // if DC is to transferred
        {
          FillArray<float>((out_DC + startIndex), DC, iPacketSize);  // insert DC into the output array
        }
      }

      #ifdef _FLOW_MODE_
      std::cout << "Signal_Quality_Metre_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

