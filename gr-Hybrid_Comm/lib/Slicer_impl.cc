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
#include "Slicer_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Slicer::sptr
    Slicer::make(int packetSize, int avgWinSize, float thresh)
    {
      return gnuradio::get_initial_sptr
        (new Slicer_impl(packetSize, avgWinSize, thresh));
    }


    /*
     * The private constructor
     */
    Slicer_impl::Slicer_impl(int packetSize, int avgWinSize, float thresh)
      : gr::sync_block("Slicer",
              gr::io_signature::make(1, 2, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(char))), ptr_fWeigths(nullptr)
      {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Slicer_impl: Constructor called." << std::endl;
      #endif
      this->set_AvgWinSize(avgWinSize);  // set averaging window size
      this->set_PacketSize(packetSize);  // set incoming packet size
      this->set_Thresh(thresh);  // set thresh

      #ifdef _FLOW_MODE_
      std::cout << "Slicer_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Slicer_impl::~Slicer_impl()
    {
    }

    int
    Slicer_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Slicer_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const float *in = (const float *) input_items[0];
      const float *in_thresh = nullptr;

      char *out = (char *) output_items[0];

      if(input_items.size() == 2)  // SpB input is connected
      {
        in_thresh = (const float *) input_items[1];
      }

      bool bThreshConnected = ( (in_thresh != nullptr) ) ? true : false;  // if thresh is to be transferred

      #ifdef _DEBUG_MODE_
      std::cout << "Slicer_impl: Work called." << std::endl;
      std::cout << "Slicer_impl: Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Slicer_impl: Tresh pin is " << ((bThreshConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      // Do <+signal processing+>
      float fMean = 0;  // sample mean value
      const float *ptr_Thresh;  // threshold value
      ptr_Thresh = (bThreshConnected) ? in_thresh : &fThresh;  // update threshold pointer

      for(int index = 0; index < noutput_items; ++index)  // go through the input array
      {
        fMean = WeightAverage<float>(in, ptr_fWeigths, index, noutput_items, iAvgWeightLen);  // calculate weighted mean value of sample

        out[index] = ( fMean < *ptr_Thresh ) ? 0 : 1;  // quantise the input

        ptr_Thresh += (bThreshConnected) ? 1 : 0;
      }

      #ifdef _ARRAY_MODE_
      std::cout << "Slicer_impl: Input = ";
      DisplayArray(in, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Slicer_impl: Output = ";
      DisplayArray<char>(out, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bThreshConnected == true)  // if Thresh is to transferred
      {
        std::cout << "Slicer_impl: Threshold = ";
        DisplayArray<float>(in_thresh, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Slicer_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif
      
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

