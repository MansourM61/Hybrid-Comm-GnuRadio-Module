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
#include "Link_Tester_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Link_Tester::sptr
    Link_Tester::make(int packetSize, float samp_rate)
    {
      return gnuradio::get_initial_sptr
        (new Link_Tester_impl(packetSize, samp_rate));
    }

    /*
     * The private constructor
     */
    Link_Tester_impl::Link_Tester_impl(int packetSize, float samp_rate)
      : gr::sync_block("Link Tester",
              gr::io_signature::make(4, 5, sizeof(char)),
              gr::io_signature::make(3, 3, sizeof(float)))
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Link_Tester_impl: Constructor called." << std::endl;
      #endif
      set_PacketSize(packetSize);  // set packet size
      set_SampRate(samp_rate);  // set sample rate
      ResetOutput();  // reset output parameters

      #ifdef _FLOW_MODE_
      std::cout << "Link_Tester_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }


    /*
     * Our virtual destructor.
     */
    Link_Tester_impl::~Link_Tester_impl()
    {
    }

    int
    Link_Tester_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Link_Tester_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *link_1 = (const char *) input_items[0];
      const char *link_2 = (const char *) input_items[1];
      const char *ctl = (const char *) input_items[2];
      const char *SpB = (const char *) input_items[3];

      const char *reset =  nullptr;      

      float *BER = (float *) output_items[0];
      float *LA = (float *) output_items[1];
      float *LT = (float *) output_items[2];

      if(input_items.size() == 5)  // reset input is connected
      {
        reset = (const char *) input_items[4];
      }

      bool bResetConnected = ( (reset != nullptr) ) ? true : false;  // if reset is to be transferred

      int NumOfPackets = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Link_Tester_impl: Work called." << std::endl;
      std::cout << "Link_Tester_impl: Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Link_Tester_impl: Packet size = " << iPacketSize << std::endl;
      std::cout << "Link_Tester_impl: Number of available packets = " << NumOfPackets << std::endl;
      std::cout << "Link_Tester_impl: Reset pin is " << ((bResetConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      bool ValidSample;  // valid sample flag
      int index_s_in_p;  // sample in package index

      double Bit_Rate;  // bit rate
      int NoB;  // number of received bits

      int dummy;
      // Do <+signal processing+>
      for(int index_p = 0; index_p < NumOfPackets; ++index_p)  // go through available packets
      {
        int SampPerBit = SpB[index_p * iPacketSize];  // samples per bit
        NoB = iPacketSize/SampPerBit;  // number of bits in the current packet
        Bit_Rate = fSampRate/SampPerBit;  // packet data rate
        fLT = (iNumBits_LT*fLT + NoB*Bit_Rate)/(iNumBits_LT + NoB);  // link throughput value
        iNumBits_LT += NoB;  // update number of received bits

        #ifdef _DEBUG_MODE_
        std::cout << "=============================================================" << std::endl;
        std::cout << "Link_Tester_impl: Packate index = " << index_p << " out of " << NumOfPackets - 1 << std::endl;
        std::cout << "Link_Tester_impl: Packet samples per bit = " << SampPerBit << std::endl;
        std::cout << "Link_Tester_impl: Packet received bits = " << NoB << std::endl;
        std::cout << "Link_Tester_impl: Total received bits for LT calculation = " << iNumBits_LT << std::endl;
        std::cout << "Link_Tester_impl: Packate bit rate = " << Bit_Rate << " bits per second"<< std::endl;
        std::cout << "Link_Tester_impl: Link Troughput (average data rate) = " << fLT << " bits per second"<< std::endl;
        #endif

        for(int index_s = 0; index_s < iPacketSize; index_s += SampPerBit)  // go through items with step of SpB, starting from first item in the packet
        {
          iTotRecBits_LA += 1;  // increment numner of total received bits

          index_s_in_p = index_s + (index_p*iPacketSize) + int(SampPerBit/2);  // index of sample in current packate

          ValidSample = IS_VALID(ctl[index_s_in_p]) && IS_BIT(link_1[index_s_in_p]) && IS_BIT(link_2[index_s_in_p]);  // if the sample is valid

          if (ValidSample)  // if sample is valid
          {
            iTotErrBits_BER += abs(link_1[index_s_in_p] - link_2[index_s_in_p]);  // update erroneous bits counter
            iTotValBits += 1;  // update total number of valid bits

          }

          fBER = (iTotValBits != 0) ? (iTotErrBits_BER*1.0)/iTotValBits : 0.0;  // BER value

          fLA = (iTotRecBits_LA != 0) ? (iTotValBits*100.0)/iTotRecBits_LA : 0.0;  // link availability value

          #ifdef _DEBUG_MODE_
          std::cout << "-------------------------------------------------------------" << std::endl;
          std::cout << "Link_Tester_impl: Sample index = " << index_s_in_p << std::endl;
          std::cout << "Link_Tester_impl: Bits of " << CPRN(link_1[index_s_in_p]) << " and " << CPRN(link_2[index_s_in_p]) << " for ctl of " << CPRN(ctl[index_s_in_p]) << std::endl;
          std::cout << "Link_Tester_impl: Total valid bits for LA calculation = " << iTotValBits << std::endl;
          std::cout << "Link_Tester_impl: Total received bits for LA calculation = " << iTotRecBits_LA << std::endl;
          std::cout << "Link_Tester_impl: Link availability = " << fLA << " %" << std::endl;
          std::cout << "Link_Tester_impl: Total erroneous received bits for BER calculation = " << iTotErrBits_BER << std::endl;
          std::cout << "Link_Tester_impl: Total valid received bits for BER calculation = " << iTotValBits << std::endl;
          std::cout << "Link_Tester_impl: BER = " << fBER << std::endl;
          #endif

          FillArray<float>((BER + index_s + (index_p*iPacketSize)), float(fBER), SampPerBit);  // update output BER array
          FillArray<float>((LA + index_s + (index_p*iPacketSize)), float(fLA), SampPerBit);  // update output LA array
        }

        FillArray<float>((LT + index_p*iPacketSize), float(fLT), iPacketSize);  // update output LT array
      }

      #ifdef _FLOW_MODE_
      std::cout << "Link_Tester_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

