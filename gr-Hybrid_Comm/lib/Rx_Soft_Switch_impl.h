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

#ifndef INCLUDED_HYBRID_COMM_RX_SOFT_SWITCH_IMPL_H
#define INCLUDED_HYBRID_COMM_RX_SOFT_SWITCH_IMPL_H

#include <Hybrid_Comm/Rx_Soft_Switch.h>

namespace gr {
  namespace Hybrid_Comm {

    class Rx_Soft_Switch_impl : public Rx_Soft_Switch
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      float* ptr_fThresh;  // threshold array
      int iNumOfThresh;  // number of thresholds
      char* ptr_cSpP_1;  // link 1 samples per package array
      char* ptr_cSpP_2;  // link 2 samples per package array
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::vector<float> defThresh;  // default threshold
      static const std::vector<char> defSpP_1;  // default link 1 samples per package array
      static const std::vector<char> defSpP_2;  // default link 2 samples per package array

     public:
      Rx_Soft_Switch_impl(int packetSize = PACKET_SAMP_SIZE, const std::vector<float>& thresh = defThresh, const std::vector<char>& sampsPerPack_1 = defSpP_1,
                         const std::vector<char>& sampsPerPack_2 = defSpP_2);
      ~Rx_Soft_Switch_impl();

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
        std::cout << "Rx_Soft_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set thresholds values
      void set_Thresh(const std::vector<float>& threshold)
      {
        if (ptr_fThresh != nullptr)  // if the array is free
        {
          delete[] ptr_fThresh;  // release the array
          ptr_fThresh = nullptr;  // label the array as empty
        }

        iNumOfThresh = threshold.size();  // update new number of threshold
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Number of threshold = " << iNumOfThresh << std::endl;
        #endif

        ptr_fThresh = new float [iNumOfThresh];  // get the array memory

        ptr_fThresh[0] = threshold[0];  // update first element
        ptr_fThresh[iNumOfThresh - 1] = CONSTRAIN(threshold[iNumOfThresh - 1], threshold[0], +FLT_MAX);  // update last element

        for(int index = 1; index < iNumOfThresh - 1; ++index)  // go through the array
        {
          ptr_fThresh[index] = CONSTRAIN(threshold[index], ptr_fThresh[index - 1], +FLT_MAX);  // update the element
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Thresholds = [";
        for(int index = 0; index < iNumOfThresh; ++index)  // go through the array
        {
          std::cout << ptr_fThresh[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Get thresholds values
      void get_Thresh(std::vector<float>* threshold)
      {
        delete[] threshold;  // release the array
        threshold = new std::vector<float>;
        std::vector<float>::iterator it = threshold->begin();  // set the iterator to begining

        for(int index = 0; index < iNumOfThresh; ++index)  // go through the array
        {
          it = threshold->insert(it, ptr_fThresh[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Thresholds = [";
        for(int index = 0; index < iNumOfThresh; ++index)  // go through the array
        {
          std::cout << ptr_fThresh[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Set link samples per package values
      void set_SampsPerPacket(const std::vector<char>& SpP, char linkID)
      {
        char *ptr_cSpP = (linkID == 1) ? ptr_cSpP_1: ptr_cSpP_2;  // update pointer

        if (ptr_cSpP != nullptr)  // if the array is free
        {
          delete[] ptr_cSpP;  // release the array
          ptr_cSpP = nullptr;  // label the array as empty
        }

        iNumOfThresh = SpP.size() - 1;  // update new number of SpP
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Link " << ((linkID == 1) ? '1': '2') << " number of samples per packet = " << iNumOfThresh + 1 << std::endl;
        #endif

        ptr_cSpP = new char [iNumOfThresh + 1];  // get the array memory

        if(linkID == 1)  // if link 1
        {
          ptr_cSpP_1 = ptr_cSpP;  // set link 1 pointer
        }
        else  // otherwise; if link 2
        {
          ptr_cSpP_2 = ptr_cSpP;  // set link 2 pointer
        }

        ptr_cSpP[0] = SpP[0];  // update first element
        ptr_cSpP[iNumOfThresh] = CONSTRAIN(SpP[iNumOfThresh], 1, +INT_MAX);  // update last element

        for(int index = 1; index < iNumOfThresh; ++index)  // go through the array
        {
          ptr_cSpP[index] = CONSTRAIN(SpP[index], 1, +INT_MAX);  // update the element
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Link " << ((linkID == 1) ? '1': '2') << " samples per packet values = [";
        for(int index = 0; index < iNumOfThresh + 1; ++index)  // go through the array
        {
          std::cout << CPRN(ptr_cSpP[index]) << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Get link samples per package values
      void get_SampsPerPacket(std::vector<char>* SpP, char linkID)
      {
        char *ptr_cSpP = (linkID == 1) ? ptr_cSpP_1: ptr_cSpP_2;  // update pointer

        delete[] SpP;  // release the array
        SpP = new std::vector<char>;
        std::vector<char>::iterator it = SpP->begin();  // set the iterator to begining

        for(int index = 0; index < iNumOfThresh + 1; ++index)  // go through the array
        {
          it = SpP->insert(it, ptr_cSpP[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Rx_Soft_Switch_impl: Link " << ((linkID == 1) ? '1': '2') << " samples per packet values = [";
        for(int index = 0; index < iNumOfThresh + 1; ++index)  // go through the array
        {
          std::cout << CPRN(ptr_cSpP[index]) << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }    
    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_RX_SOFT_SWITCH_IMPL_H */

