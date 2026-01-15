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

#ifndef INCLUDED_HYBRID_COMM_LINK_TESTER_IMPL_H
#define INCLUDED_HYBRID_COMM_LINK_TESTER_IMPL_H

#include <Hybrid_Comm/Link_Tester.h>

namespace gr {
  namespace Hybrid_Comm {

    class Link_Tester_impl : public Link_Tester
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet data samples length
      double fSampRate;  // sample rate
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      unsigned long iTotValBits;  // total valid bits for BER and LA calculation

      unsigned long iTotErrBits_BER;  // total erroneous bits for BER calculation
      double fBER;  // bit-error-rate (BER)

      unsigned long iTotRecBits_LA;  // total received bits for LA calculation
      double fLA;  // link availability (%)

      unsigned long iNumBits_LT;  // number of received bits for LT calculation
      double fLT;  // link throughput (bps)

      // reset measurement parameters
      void ResetOutput(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Output parameters are reset." << std::endl;
        #endif

        iTotValBits = 0;
        iTotErrBits_BER = 0;
        fBER = 0.0;
        iTotRecBits_LA = 0;
        fLA = 0.0;
        iNumBits_LT = 0;
        fLT = 0.0;
      }

     public:
      Link_Tester_impl(int packetSize = PACKET_SAMP_SIZE, float samp_rate = SAMPLE_RATE);
      ~Link_Tester_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set packet data samples length
      void set_PacketSize(int packetSize)
      {
        #ifdef _THREAD_MUTEX_
        gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
        #endif
        iPacketSize = CONSTRAIN(packetSize, 1, INT_MAX);
        this->set_output_multiple(iPacketSize);
        #ifdef _DEBUG_MODE_
        std::cout << "Link_Tester_impl: Packet data samples length = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet data samples length
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Link_Tester_impl: Packet data samples length = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set sample rate
      void set_SampRate(float samp_rate)
      {
        fSampRate = CONSTRAIN(samp_rate, 0, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Link_Tester_impl: Sample rate = " << fSampRate << std::endl;
        #endif
      }

      // Get sample rate
      float get_SampRate(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Link_Tester_impl: Sample rate = " << fSampRate << std::endl;
        #endif
        return fSampRate;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_LINK_TESTER_IMPL_H */

