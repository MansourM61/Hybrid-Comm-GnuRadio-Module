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
#include "Tx_Parallel_Switch_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Tx_Parallel_Switch::sptr
    Tx_Parallel_Switch::make(int packetSize, int sampsPerBit)
    {
      return gnuradio::get_initial_sptr
        (new Tx_Parallel_Switch_impl(packetSize, sampsPerBit));
    }


    /*
     * The private constructor
     */
    Tx_Parallel_Switch_impl::Tx_Parallel_Switch_impl(int packetSize, int sampsPerBit)
      : gr::sync_block("Tx Parallel Switch",
              gr::io_signature::make2(1, 2, sizeof(char), sizeof(float)),
              gr::io_signature::make(2, 3, sizeof(char)))
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_SampsPerBit(sampsPerBit);

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Tx_Parallel_Switch_impl::~Tx_Parallel_Switch_impl()
    {
    }

    int
    Tx_Parallel_Switch_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *in = (const char *) input_items[0];
      const float *sel = nullptr;  // not used in this module

      char *link_1 = (char *) output_items[0];  // link 1 output
      char *link_2 = (char *) output_items[1];  // link 2 output
      char *out_SpB =  nullptr;      

      if(output_items.size() == 3)  // if output SpB is connected
      {
        out_SpB = (char *) output_items[2];
      }

      bool bSpBConnected = ( (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int NoP = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Work called." << std::endl;
      std::cout << "Tx_Parallel_Switch_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Tx_Parallel_Switch_impl: Number of available packets = " << NoP << std::endl;      
      std::cout << "Tx_Parallel_Switch_impl: Samples per packets = " << iPacketSize << std::endl;      
      std::cout << "Tx_Parallel_Switch_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      // Do <+signal processing+>
      CopyArrays<char>(in, link_1, noutput_items);  // copy the input array to link 1
      CopyArrays<char>(in, link_2, noutput_items);  // copy the input array to link 2      

      if(bSpBConnected == true)  // if SpB is to transferred
      {
        FillArray<char>(out_SpB, iSampsPerBit, noutput_items);  // fill SpB with input value
      }

      #ifdef _ARRAY_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Output_1 = ";
      DisplayArray<char>(link_1, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Tx_Parallel_Switch_impl: Output_2 = ";
      DisplayArray<char>(link_2, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Tx_Parallel_Switch_impl: Final SpB = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Tx_Parallel_Switch_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

