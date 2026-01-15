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

#ifndef INCLUDED_HYBRID_COMM_STREAM_ALIGNER_IMPL_H
#define INCLUDED_HYBRID_COMM_STREAM_ALIGNER_IMPL_H

#include <Hybrid_Comm/Stream_Aligner.h>

namespace gr {
  namespace Hybrid_Comm {

    class Stream_Aligner_impl : public Stream_Aligner
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      Bank_Buff<char> Buffer;  // buffer
      bool bBufStored;  // flag to show buffer has been stored
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::vector<int> iov;  // io signature
      static const int iMaxInBufCoeff;  // maximum input items buffer coefficient 

     public:
      Stream_Aligner_impl(int packetSize = PACKET_SAMP_SIZE);
      ~Stream_Aligner_impl();

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
        Buffer.set_SlotSize(iPacketSize);  // set new slot size
        this->set_output_multiple(iPacketSize);  // make sure there are complete number of packets in the incoming data
        this->set_max_noutput_items(iPacketSize*Buffer.BankSize*iMaxInBufCoeff - 1);  // set the maximum number of items

        #ifdef _DEBUG_MODE_
        std::cout << "Stream_Aligner_impl: Packet size = " << iPacketSize << std::endl;
        #endif

        bBufStored = false;  // set the buffer storage flag
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Stream_Aligner_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_STREAM_ALIGNER_IMPL_H */

