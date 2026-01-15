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

#ifndef INCLUDED_HYBRID_COMM_SOURCE_BV_IMPL_H
#define INCLUDED_HYBRID_COMM_SOURCE_BV_IMPL_H

#include <Hybrid_Comm/Source_BV.h>

namespace gr {
  namespace Hybrid_Comm {

    enum SigType {Constant = 0, Random = 1};
    
    class Source_BV_impl : public Source_BV
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      char cSignalType;  // signal type
      int iBitsPerPack;  // current bits per package
      char *ptr_cRandSig;  // random signal array
      #ifdef _THREAD_MUTEX_
      gr::thread::mutex d_mutex_delay;  // thread safety mutex
      #endif
      Norm_Rand_Gen RandGen;
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::string strConstant;
      static const std::string strRandom;

     public:
      Source_BV_impl(int packetSize = PACKET_SAMP_SIZE, std::string signalType = strConstant);
      ~Source_BV_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );


      // Set packet size
      void set_PacketSize(int packetSize)
      {
        #ifdef _THREAD_MUTEX_
        gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
        #endif
        iPacketSize = CONSTRAIN(packetSize, 1, INT_MAX);
        this->set_output_multiple(iPacketSize);  // make sure the number of output array is a multiple of window size
        #ifdef _DEBUG_MODE_
        std::cout << "Source_BV_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Source_BV_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set signal type
      void set_SignalType(std::string signalType)
      {
        cSignalType = (signalType == strConstant) ? Constant : Random;
        #ifdef _DEBUG_MODE_
        std::cout << "Source_BV_impl: Signal mode = " << CPRN(cSignalType) << std::endl;
        #endif
      }

      // Get signal type
      std::string get_SignalType(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Source_BV_impl: Signal mode = " << CPRN(cSignalType) << std::endl;
        #endif
        return (cSignalType == Constant) ? strConstant : strRandom;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SOURCE_BV_IMPL_H */

