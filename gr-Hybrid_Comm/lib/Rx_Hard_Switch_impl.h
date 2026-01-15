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

#ifndef INCLUDED_HYBRID_COMM_RX_HARD_SWITCH_IMPL_H
#define INCLUDED_HYBRID_COMM_RX_HARD_SWITCH_IMPL_H

#include <Hybrid_Comm/Rx_Hard_Switch.h>

namespace gr {
  namespace Hybrid_Comm {

    class Rx_Hard_Switch_impl : public Rx_Hard_Switch
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      float fThresh;  // threshold
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      char cSpB[2];  // samples per bit array

      static const std::vector<char> defSpB;  // default samples per bit

     public:
      Rx_Hard_Switch_impl(int packetSize = PACKET_SAMP_SIZE, float thresh = DEF_THRESH, const std::vector<char>& sampsPerBit = defSpB);
      ~Rx_Hard_Switch_impl();

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
        std::cout << "Rx_Hard_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Hard_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set link selection threshold
      void set_Thresh(float thresh)
      {
        fThresh = thresh;
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Hard_Switch_impl: Selection threshold = " << fThresh << std::endl;
        #endif
      }

      // Get link selection threshold
      float get_Thresh(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Hard_Switch_impl: Selection threshold = " << fThresh << std::endl;
        #endif
        return fThresh;
      }

      // Set samples per bit
      void set_SampsPerBit(const std::vector<char>& SpB)
      {
        cSpB[0] = CONSTRAIN(SpB[0], 1, +UCHAR_MAX);  // update 2ns element
        cSpB[1] = CONSTRAIN(SpB[1], 1, +UCHAR_MAX);  // update 2ns element

        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Hard_Switch_impl: Samples per bit = [";
        std::cout << "(Link 1: " << CPRN(cSpB[0]) << "), ";  // show the elements
        std::cout << "(Link 2: " << CPRN(cSpB[1]) << "), ";  // show the elements
        std::cout << "]" << std::endl;
        #endif
      }

      // Get samples per bit
      void get_SampsPerBit(std::vector<char>* SpB)
      {
        delete[] SpB;  // release the array
        SpB = new std::vector<char>;
        std::vector<char>::iterator it = SpB->begin();  // set the iterator to begining

        for(int index = 0; index < 2; ++index)  // go through the array
        {
          it = SpB->insert(it, cSpB[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Hard_Switch_impl: Samples per bit = [";
        std::cout << "(Link 1: " << CPRN(cSpB[0]) << "), ";  // show the elements
        std::cout << "(Link 2: " << CPRN(cSpB[1]) << "), ";  // show the elements
        std::cout << "]" << std::endl;
        #endif
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_RX_HARD_SWITCH_IMPL_H */

