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

#ifndef INCLUDED_HYBRID_COMM_RX_PARALLEL_SWITCH_IMPL_H
#define INCLUDED_HYBRID_COMM_RX_PARALLEL_SWITCH_IMPL_H

#include <Hybrid_Comm/Rx_Parallel_Switch.h>

namespace gr {
  namespace Hybrid_Comm {

    class Rx_Parallel_Switch_impl : public Rx_Parallel_Switch
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      float fThresh;  // threshold
      int iSampsPerBit;  // samples per bit
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif
      
     public:
      Rx_Parallel_Switch_impl(int packetSize = PACKET_SAMP_SIZE, float thresh = DEF_THRESH, int sampsPerBit = DEF_SPB);
      ~Rx_Parallel_Switch_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set packet size
      void set_PacketSize(int packetsize)
      {
        #ifdef _THREAD_MUTEX_
        gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
        #endif
        iPacketSize = CONSTRAIN(packetsize, 1, INT_MAX);
        this->set_output_multiple(iPacketSize);
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set link selection threshold
      void set_Thresh(float thresh)
      {
        fThresh = thresh;
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Link selection threshold = " << fThresh << std::endl;
        #endif
      }

      // Get link selection threshold
      float get_Thresh(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Link selection threshold = " << fThresh << std::endl;
        #endif
        return fThresh;
      }

      // Set samples per bit
      void set_SampsPerBit(float sampsPerBit)
      {
        iSampsPerBit = CONSTRAIN(sampsPerBit, 1, INT_MAX);;
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Samples per bit = " << iSampsPerBit << std::endl;
        #endif
      }

      // Get samples per bit
      float get_SampsPerBit(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Parallel_Switch_impl: Samples per bit = " << iSampsPerBit << std::endl;
        #endif
        return iSampsPerBit;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_RX_PARALLEL_SWITCH_IMPL_H */

