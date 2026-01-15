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
#include "Tx_Hard_Switch_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Tx_Hard_Switch::sptr
    Tx_Hard_Switch::make(int packetSize, float thresh, const std::vector<char>& sampPerBit)
    {
      return gnuradio::get_initial_sptr
        (new Tx_Hard_Switch_impl(packetSize, thresh, sampPerBit));
    }

    const std::vector<char> Tx_Hard_Switch_impl::defSpB = {1, 2};  // default samples per bit    

    /*
     * The private constructor
     */
    Tx_Hard_Switch_impl::Tx_Hard_Switch_impl(int packetSize, float thresh, const std::vector<char>& sampPerBit)
      : gr::sync_block("Tx Hard Switch",
              gr::io_signature::make2(2, 2, sizeof(char), sizeof(float)),
              gr::io_signature::make(2, 3, sizeof(char)))
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Hard_Switch_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_Thresh(thresh);  // set threshold
      this->set_SampsPerBit(sampPerBit);  // set samples per bit array

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Hard_Switch_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Tx_Hard_Switch_impl::~Tx_Hard_Switch_impl()
    {
    }

    int
    Tx_Hard_Switch_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Tx_Hard_Switch_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *in = (const char *) input_items[0];      
      const float *sel = (const float *) input_items[1];

      char *link_1 = (char *) output_items[0];
      char *link_2 = (char *) output_items[1];
      char *out_SpB =  nullptr;      

      if(output_items.size() == 3)  // if output SpB is connected
      {
        out_SpB = (char *) output_items[2];
      }

      bool bSpBConnected = ( (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int NoP = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Hard_Switch_impl: Work called." << std::endl;
      std::cout << "Tx_Hard_Switch_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Tx_Hard_Switch_impl: Number of available packets = " << NoP << std::endl;      
      std::cout << "Tx_Hard_Switch_impl: Samples per packets = " << iPacketSize << std::endl;      
      std::cout << "Tx_Hard_Switch_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      // Do <+signal processing+>
      for(int index_p = 0; index_p < NoP; ++index_p)  // go through available packets
      {
        if(bSpBConnected)  // if SpB is to be transferred
        {
          FillArray<char>((out_SpB + index_p*iPacketSize), sel[index_p*iPacketSize], iPacketSize);  // fill SpB with input value
        }

        if(sel[index_p*iPacketSize] < fThresh)  // if selection signal is less than threshold value; link 1 is active
        {
          CopyArrays<char>((in + index_p*iPacketSize), (link_1 + index_p*iPacketSize), iPacketSize);  // copy the input array to link 1
          FillArray<char>((link_2 + index_p*iPacketSize), DEF_SIG_VAL, iPacketSize);  // fill array with input value
        }
        else  // otherwise; link 2 is active
        {
          CopyArrays<char>((in + index_p*iPacketSize), (link_2 + index_p*iPacketSize), iPacketSize);  // copy the input array to link 2
          FillArray<char>((link_1 + index_p*iPacketSize), DEF_SIG_VAL, iPacketSize);  // fill array with input value          
        }
      }

      #ifdef _ARRAY_MODE_
      std::cout << "Tx_Hard_Switch_impl: Output_1 = ";
      DisplayArray<char>(link_1, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Tx_Hard_Switch_impl: Output_2 = ";
      DisplayArray<char>(link_2, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Tx_Hard_Switch_impl: Final SpB = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Hard_Switch_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

