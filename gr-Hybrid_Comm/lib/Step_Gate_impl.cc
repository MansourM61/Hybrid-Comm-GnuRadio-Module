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
#include "Step_Gate_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Step_Gate::sptr
    Step_Gate::make(int packetSize, const std::vector<float>& levels, const std::vector<float>& hopPoints)
    {
      return gnuradio::get_initial_sptr
        (new Step_Gate_impl(packetSize, levels, hopPoints));
    }

    const std::vector<float> Step_Gate_impl::defLev = {-1, 0, +1};  // default levels
    const std::vector<float> Step_Gate_impl::defPnt = {-1, +1};  // default levels

    /*
     * The private constructor
     */
    Step_Gate_impl::Step_Gate_impl(int packetSize, const std::vector<float>& levels, const std::vector<float>& hopPoints)
      : gr::sync_block("Step Gate",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))), ptr_fLevels(nullptr), ptr_fPoints(nullptr)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Step_Gate_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_Levels(levels);  // set levels array
      this->set_HopPoints(hopPoints);  // set hopping points array

      #ifdef _FLOW_MODE_
      std::cout << "Step_Gate_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Step_Gate_impl::~Step_Gate_impl()
    {
    }

    int
    Step_Gate_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Step_Gate_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const float *Sig_1 = (const float *) input_items[0];  // input signal 1
      const float *Sig_2 = (const float *) input_items[1];  // input signal 2
      float *Out = (float *) output_items[0];  // output signal

      int NoP = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Step_Gate_impl: Work called." << std::endl;
      std::cout << "Step_Gate_impl: Number of output items = " << noutput_items << std::endl;
      std::cout << "Step_Gate_impl: Number of available packets = " << NoP << std::endl;      
      std::cout << "Step_Gate_impl: Samples per packets = " << iPacketSize << std::endl;      
      #endif

      // Do <+signal processing+>
      float diff = 0;  // signal difference
      int index_l = 0;  // level index

      // Do <+signal processing+>
      for(int index = 0; index < NoP; ++index)  // go though all packets
      {
        diff = Sig_1[index*iPacketSize] - Sig_2[index*iPacketSize];  // update signal difference

        if(diff < ptr_fPoints[0])  // if in (-Inf, P0) range
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Step_Gate_impl: " << diff << " is in ( -Inf, " << ptr_fPoints[0] << " ) : Level = " << ptr_fLevels[0] << std::endl;
          #endif
          index_l = 0;  // update level index
        }
        else if(diff >= ptr_fPoints[iNumOfLevs - 2])  // if in [Pn, +Inf) range
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Step_Gate_impl: " << diff << " is in [ " << ptr_fPoints[iNumOfLevs - 2] << ", +Inf ) : Level = " << ptr_fLevels[iNumOfLevs - 1] << std::endl;
          #endif
          index_l = iNumOfLevs - 1;  // update level index
        }
        else  // if in [P0, Pn) range
        {
          for(int index_p = 0; index_p < iNumOfLevs - 1; ++index_p)  // go through hopping points
          {
            if((diff >= ptr_fPoints[index_p]) && (diff < ptr_fPoints[index_p + 1]))  // if in [Pi, Pi+1) range
            {
              #ifdef _DEBUG_MODE_
              std::cout << "Step_Gate_impl: " << diff << " is in [ " << ptr_fPoints[index_p] << ", " << ptr_fPoints[index_p + 1] << " ) : Level = " << ptr_fLevels[index_p + 1] << std::endl;
              #endif
              index_l = index_p + 1;  // update level index 
              break;
            }
          }
        }

        FillArray<float>((Out + index*iPacketSize), ptr_fLevels[index_l], iPacketSize);  // fill output array
      }

      #ifdef _FLOW_MODE_
      std::cout << "Step_Gate_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

