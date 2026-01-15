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
#include "Tx_Soft_Switch_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Tx_Soft_Switch::sptr
    Tx_Soft_Switch::make(int packetSize, const std::vector<float>& thresh, const std::vector<char>& sampsPerPack_1, const std::vector<char>& sampsPerPack_2)
    {
      return gnuradio::get_initial_sptr
        (new Tx_Soft_Switch_impl(packetSize, thresh, sampsPerPack_1, sampsPerPack_2));
    }

    const std::vector<float> Tx_Soft_Switch_impl::defThresh = {0};  // default threshold
    const std::vector<char> Tx_Soft_Switch_impl::defSpP_1 = {1, 2};  // default link 1 SpP
    const std::vector<char> Tx_Soft_Switch_impl::defSpP_2 = {2, 1};  // default link 2 SpP

    /*
     * The private constructor
     */
    Tx_Soft_Switch_impl::Tx_Soft_Switch_impl(int packetSize, const std::vector<float>& thresh, const std::vector<char>& sampsPerPack_1, const std::vector<char>& sampsPerPack_2)
      : gr::sync_block("Tx Soft Switch",
              gr::io_signature::make2(2, 2, sizeof(char), sizeof(float)),
              gr::io_signature::make(2, 3, sizeof(char))), ptr_fThresh(nullptr), ptr_cSpP_1(nullptr), ptr_cSpP_2(nullptr)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Soft_Switch_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_Thresh(thresh);  // set threshold
      this->set_SampsPerPacket(sampsPerPack_1, 1);  // set link 1 samples per package
      this->set_SampsPerPacket(sampsPerPack_2, 2);  // set link 2 samples per package

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Soft_Switch_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Tx_Soft_Switch_impl::~Tx_Soft_Switch_impl()
    {
    }

    int
    Tx_Soft_Switch_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Tx_Soft_Switch_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *in = (const char *) input_items[0];
      const float *sel = (const float *) input_items[1];

      char *link_1 = (char *) output_items[0];
      char *link_2 = (char *) output_items[1];
      char* out_SpB = nullptr;

      if(output_items.size() == 3)  // if output SpB is connected
      {
        out_SpB = (char *) output_items[2];
      }

      bool bSpBConnected = ( (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int NoP = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Soft_Switch_impl: Work called." << std::endl;
      std::cout << "Tx_Soft_Switch_impl: Number of available packets = " << NoP << std::endl;      
      std::cout << "Tx_Soft_Switch_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Tx_Soft_Switch_impl: Samples per packets = " << iPacketSize << std::endl;      
      std::cout << "Tx_Soft_Switch_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      FillArray<char>(link_1, DEF_SIG_VAL, noutput_items);  // initialise link_1 array
      FillArray<char>(link_2, DEF_SIG_VAL, noutput_items);  // initialise link_2 array
      
      // Do <+signal processing+>
      for(int index = 0; index < NoP; ++index)  // go though all packets
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Tx_Soft_Switch_impl: Packet index " << index << " out of " << NoP - 1 << std::endl;
        #endif

        int Current_sel = sel[index*iPacketSize];  // curent selection
        int index_s = 0;  // spp index
        char SpP_1 = 1;  // link 1 samples per packets
        char SpP_2 = 1;  // link 2 samples per packets
        int NumOfSamp_1 = iPacketSize/2;  // link 1 number of samples
        int NumOfSamp_2 = iPacketSize/2;  // link 2 number of samples

        int Index_1_s = index*iPacketSize;  // link 1 start index
        int Index_1_e = Index_1_s;  // link 1 end index
        int Index_2_s = Index_1_s;  // link 2 start index
        int Index_2_e = (index + 1)*iPacketSize - 1;  // link 2 end index

        int InterpRate_1 = 1;  // link 1 interpolatin rate
        int InterpRate_2 = 1;  // link 1 interpolatin rate

        #ifdef _DEBUG_MODE_
        std::cout << "Tx_Soft_Switch_impl: Packet select value = " << Current_sel << std::endl;
        #endif

        if(Current_sel < ptr_fThresh[0])  // if in (-Inf, P0) range
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Tx_Soft_Switch_impl: " << Current_sel << " is in ( -Inf, " << ptr_fThresh[0] << " )" << std::endl;
          #endif
          index_s = 0;  // update spp index
        }
        else if(Current_sel >= ptr_fThresh[iNumOfThresh - 1])  // if in [Pn, +Inf) range
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Tx_Soft_Switch_impl: " << Current_sel << " is in [ " << ptr_fThresh[iNumOfThresh - 1] << ", +Inf )"  << std::endl;
          #endif
          index_s = iNumOfThresh;  // update spp index
        }
        else  // if in [P0, Pn) range
        {
          for(int index_p = 0; index_p < iNumOfThresh - 1; ++index_p)  // go through threshold
          {
            if((Current_sel >= ptr_fThresh[index_p]) && (Current_sel < ptr_fThresh[index_p + 1]))  // if in [Pi, Pi+1) range
            {
              #ifdef _DEBUG_MODE_
              std::cout << "Tx_Soft_Switch_impl: " << Current_sel << " is in [ " << ptr_fThresh[index_p] << ", " << ptr_fThresh[index_p + 1] << " )" << std::endl;
              #endif
              index_s = index_p + 1;  // update spp index 
              break;
            }
          }
        }

        SpP_1 = ptr_cSpP_1[index_s];  // update SpP_1
        SpP_2 = ptr_cSpP_2[index_s];  // update SpP_2

        NumOfSamp_1 = MAX(int(floor(SpP_2*iPacketSize/(SpP_1 + SpP_2))), 1);  // update link 1 number of samples
        NumOfSamp_2 = iPacketSize - NumOfSamp_1;  // update link 2 number of samples

        Index_1_e += NumOfSamp_1;  // update link 1 end index
        Index_2_s += NumOfSamp_1;  // update link 2 start index

        InterpRate_1 = int(floor(iPacketSize/NumOfSamp_1));  // update link 1 interpolatin rate
        InterpRate_2 = int(floor(iPacketSize/NumOfSamp_2));  // update link 2 interpolatin rate

        #ifdef _DEBUG_MODE_
        std::cout << "Tx_Soft_Switch_impl: Link 1 samples per packet " << CPRN(SpP_1) << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 2 samples per packet " << CPRN(SpP_2) << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 1 number of samples " << NumOfSamp_1 << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 2 number of samples " << NumOfSamp_2 << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 1 samples range = ( " << Index_1_s << ", " << Index_1_e << " )" << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 2 samples range = ( " << Index_2_s << ", " << Index_2_e << " )" << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 1 interpolatin rate = " << InterpRate_1 << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 2 interpolatin rate = " << InterpRate_2 << std::endl;
        #endif

        InterpArray<char>((in + Index_1_s), (link_1 + index*iPacketSize), NumOfSamp_1, InterpRate_1);  // fill link 1 array
        InterpArray<char>((in + Index_2_s), (link_2 + index*iPacketSize), NumOfSamp_2, InterpRate_2);  // fill link 2 array

        if(bSpBConnected == true)  // if SpB is to transferred
        {
          FillArray<char>((out_SpB + Index_1_s), NumOfSamp_1, noutput_items);  // fill SpB array
          FillArray<char>((out_SpB + Index_2_s), NumOfSamp_2, noutput_items);  // fill SpB array
        }

        #ifdef _ARRAY_MODE_
        std::cout << "Tx_Soft_Switch_impl: Link 1 = ";
        DisplayArray(link_1, noutput_items, 0);  // display input array
        std::cout << std::endl;
        std::cout << "Tx_Soft_Switch_impl: Link 2 = ";
        DisplayArray(link_2, noutput_items, 0);  // display input array
        std::cout << std::endl;
        #endif
      }

      #ifdef _ARRAY_MODE_
      std::cout << "Tx_Soft_Switch_impl: Input = ";
      DisplayArray<char>(in, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Tx_Soft_Switch_impl: Select = ";
      DisplayArray<float>(sel, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Tx_Soft_Switch_impl: Final link 1 = ";
      DisplayArray<char>(link_1, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Tx_Soft_Switch_impl: Final link 2 = ";
      DisplayArray<char>(link_2, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Tx_Soft_Switch_impl: Final SpB = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Soft_Switch_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

