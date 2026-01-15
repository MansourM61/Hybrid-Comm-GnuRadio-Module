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
#include "Source_BV_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Source_BV::sptr
    Source_BV::make(int winSize, std::string signalType)
    {
      return gnuradio::get_initial_sptr
        (new Source_BV_impl(winSize, signalType));
    }

    const std::string Source_BV_impl::strConstant = CNT_STR;
    const std::string Source_BV_impl::strRandom = RND_STR;

    /*
     * The private constructor
     */
    Source_BV_impl::Source_BV_impl(int packetSize, std::string signalType)
      : gr::sync_block("Source_BV",
              gr::io_signature::make(1, 1, sizeof(char)),
              gr::io_signature::make(1, 2, sizeof(char))), iBitsPerPack(1), RandGen()
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Source_BV_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_SignalType(signalType);  // set signal type

      #ifdef _FLOW_MODE_
      std::cout << "Source_BV_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Source_BV_impl::~Source_BV_impl()
    {
    }

    int
    Source_BV_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Source_BV_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      #ifdef _THREAD_MUTEX_
      gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
      #endif

      const char *SpB = (const char *) input_items[0];
      char *out = (char *) output_items[0];
      char *out_SpB = nullptr;

      if(output_items.size() == 2)  // if SpB is connected
      {
        out_SpB = (char *) output_items[1];
        
      }

      bool bSpBConnected = ( (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int NoP = noutput_items/iPacketSize;  // number of packates

      #ifdef _DEBUG_MODE_
      std::cout << "Source_BV_impl: Work called." << std::endl;
      std::cout << "Source_BV_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Source_BV_impl: Package size = " << iPacketSize << std::endl;
      std::cout << "Source_BV_impl: Number of packets = " << NoP << std::endl;      
      std::cout << "Source_BV_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      // Do <+signal processing+>
      if(cSignalType == Constant)  // if the signal type is constant
      {
        FillArray<char>(out, VAL_1, noutput_items);  // fill the output array with constant

        for(int index = 0; index < NoP; ++index)  // go through the packets
        {
          if(bSpBConnected == true)  // if SpB is to transferred
          {
            FillArray<char>((out_SpB + index*iPacketSize), SpB[index*iPacketSize], iPacketSize);  // fill SpB with first input value of packet
          }
        }
      }
      else if(cSignalType == Random)  // if the signal type is random
      {
        int InterpRatio = 1;  // window size to number of required bits ratio
        int Index_Out = 0;  // starting index of output chunck

        ptr_cRandSig = new char [iPacketSize];  // create dummy array assuming 'InterpRatio = 1'

        for(int index = 0; index < NoP; ++index)  // go through the packets
        {
          iBitsPerPack = iPacketSize/SpB[index*iPacketSize];  // update bits per window based on the 1st element in the input 'samples per bit' array

          InterpRatio = iPacketSize/iBitsPerPack;  // number of required bits (source sample)
          Index_Out = index*iPacketSize;  // update starting index of output chunck

          #ifdef _DEBUG_MODE_
          std::cout << "---------------------------------------------------------------" << std::endl;
          std::cout << "Source_BV_impl: Samples per bit = " << CPRN(SpB[index*iPacketSize]) << std::endl;      
          std::cout << "Source_BV_impl: Current number of bits per package = " << iBitsPerPack << std::endl;      
          std::cout << "Source_BV_impl: Window size to number of bits ratio = " << InterpRatio << std::endl;      
          std::cout << "Source_BV_impl: Starting index of output chunck = " << Index_Out << std::endl;      
          #endif

          RandGen.UniformBinary(ptr_cRandSig, iBitsPerPack);  // create array of random binaries (VAL_0, VAL_1)
          #ifdef _ARRAY_MODE_
          std::cout << "Source_BV_impl: Uniform binary = ";
          DisplayArray<char>(ptr_cRandSig, iBitsPerPack, 0);
          std::cout << std::endl;
          #endif

          InterpArray<char>(ptr_cRandSig, (out + Index_Out), iBitsPerPack, InterpRatio);  // resample the array according to required interpolation ratio
          #ifdef _ARRAY_MODE_
          std::cout << "Source_BV_impl: Resampled array = ";
          DisplayArray<char>((out + Index_Out), iPacketSize, 0);
          std::cout << std::endl;
          #endif

          if(bSpBConnected == true)  // if SpB is to transferred
          {
            FillArray<char>((out_SpB + index*iPacketSize), SpB[index*iPacketSize], iPacketSize);  // fill SpB with input value
          }
        }

        delete[] ptr_cRandSig;  // release dummy array
      }
      else  // otherwise
      {
        FillArray<char>(out, VAL_0, noutput_items);  // fill the output array with VAL_0

        for(int index = 0; index < NoP; ++index)  // go through the packets
        {
          if(bSpBConnected == true)  // if SpB is to transferred
          {
            FillArray<char>((out_SpB + index*iPacketSize), SpB[index*iPacketSize], iPacketSize);  // fill SpB with input value
          }
        }
      }

      #ifdef _DEBUG_MODE_
      std::cout << "Source_BV_impl: Number of generated outputs = " << noutput_items << std::endl;
      #endif

      #ifdef _ARRAY_MODE_
      std::cout << "Source_BV_impl: Output array = ";
      DisplayArray<char>(out, noutput_items, 0);
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Source_BV_impl: Bits per window = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);
        std::cout << std::endl;
      }
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Source_BV_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

