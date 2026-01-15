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
#include "Add_Header_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Add_Header::sptr
    Add_Header::make(int packetSize, const std::vector<char>& preamble, const std::string& label, int samplesPerBit)
    {
      return gnuradio::get_initial_sptr
        (new Add_Header_impl(packetSize, preamble, label, samplesPerBit));
    }

    const std::vector<char> Add_Header_impl::defPreamb = {0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0};  // default preamble
    const std::string Add_Header_impl::defLabel = "L1";  // default packet label
    const int Add_Header_impl::iCounterLen = sizeof(char)*COUNTER_BITS;  // counter field length
    const int Add_Header_impl::iLabelBitsPerLet = sizeof(char)*BITS_PER_LETT;  // label bits per letters
    const int Add_Header_impl::iNumOfOutputMultiple = OUT_NUM_MP;  // number of output items multiple

    /*
     * The private constructor
     */
    Add_Header_impl::Add_Header_impl(int packetSize, const std::vector<char>& preamble, const std::string& label, int samplesPerBit)
      : gr::block("Add Header",
              gr::io_signature::make(1, 2, sizeof(char)),
              gr::io_signature::make(1, 2, sizeof(char))),
              iPreambleLen(1), iLabelLen(1), iSamplesPerBit(1), iPacketSize(1), iCounter(0),
              ptr_cPreamble(nullptr), ptr_cLabel(nullptr), ptr_cLabelBits(nullptr),
              ptr_cPreambleSeq(nullptr), ptr_cLabelBitsSeq(nullptr), ptr_cCounterSeq(nullptr), ary_cCounterBits(nullptr)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Constructor called." << std::endl;
      #endif
      this->set_Preamble(preamble);  // set packet preamble
      this->set_Label(label);  // set packet label
      this->set_SamplesPerBit(samplesPerBit);  // set samples per bits bit
      this->set_PacketSize(packetSize);  // set packet size

      this->set_history(1);

      #ifdef _FLOW_MODE_
      std::cout << "Add_Header_impl: Packet size = " << iPacketSize << std::endl;
      #endif

      ary_cCounterBits = new char [iCounterLen];  // allocate memory
    }

    /*
     * Our virtual destructor.
     */
    Add_Header_impl::~Add_Header_impl()
    {
      if (ptr_cPreamble != nullptr)  // if the array is free
      {
        delete[] ptr_cPreamble;  // release the array
        ptr_cPreamble = nullptr;  // label the array as empty
      }

      if (ptr_cLabel != nullptr)  // if the array is free
      {
        delete[] ptr_cLabel;  // release the array
        ptr_cLabel = nullptr;  // label the array as empty
      }

      if (ptr_cLabelBits != nullptr)  // if the array is free
      {
        delete[] ptr_cLabelBits;  // release the array
        ptr_cLabelBits = nullptr;  // label the array as empty
      }

      if (ptr_cPreambleSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cPreambleSeq;  // release the array
        ptr_cPreambleSeq = nullptr;  // label the array as empty
      }

      if (ptr_cLabelBitsSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cLabelBitsSeq;  // release the array
        ptr_cLabelBitsSeq = nullptr;  // label the array as empty
      }

      if (ptr_cCounterSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cCounterSeq;  // release the array
        ptr_cCounterSeq = nullptr;  // label the array as empty
      }

      if (ary_cCounterBits != nullptr)  // if the array is free
      {
        delete[] ary_cCounterBits;  // release the array
        ary_cCounterBits = nullptr;  // label the array as empty
      }

    }

    /*
     * Configure header size based on the new pattern
     */
    void Add_Header_impl::ConfigureHeaderSize(void)
    {
      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Configure header size called." << std::endl;
      #endif
      int N_out = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit + iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range

      block::set_output_multiple(N_out);
      block::set_max_noutput_items(N_out*iNumOfOutputMultiple);

      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Number of output samples per block = " << N_out << std::endl;
      std::cout << "Add_Header_impl: Maximum number of output samples = " << N_out*iNumOfOutputMultiple << std::endl;
      #endif

      if (ptr_cPreambleSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cPreambleSeq;  // release the array
        ptr_cPreambleSeq = nullptr;  // label the array as empty
      }
      if (ptr_cPreamble != nullptr)
      {
        ptr_cPreambleSeq = new char [iPreambleLen*iSamplesPerBit];  // get the array memory
        InterpArray<char>(ptr_cPreamble, ptr_cPreambleSeq, iPreambleLen, iSamplesPerBit);  // resample the array
      }

      if (ptr_cLabelBitsSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cLabelBitsSeq;  // release the array
        ptr_cLabelBitsSeq = nullptr;  // label the array as empty
      }
      if (ptr_cLabelBits != nullptr)
      {
        ptr_cLabelBitsSeq = new char [iLabelLen*iLabelBitsPerLet*iSamplesPerBit];  // get the array memory
        InterpArray<char>(ptr_cLabelBits, ptr_cLabelBitsSeq, iLabelLen*iLabelBitsPerLet, iSamplesPerBit);  // resample the array
      }

      if (ptr_cCounterSeq != nullptr)  // if the array is free
      {
        delete[] ptr_cCounterSeq;  // release the array
        ptr_cCounterSeq = nullptr;  // label the array as empty
      }
      ptr_cCounterSeq = new char [iCounterLen*iSamplesPerBit];  // get the array memory
      FillArray<char>(ptr_cCounterSeq, 0, iCounterLen*iSamplesPerBit);  // initialise the array with 0

    }

    void
    Add_Header_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Add_Header_impl: Forecast called." << std::endl;
      std::cout << "Add_Header_impl: Number of output samples = " << noutput_items << std::endl;
      #endif

      int N_header = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit;
      int N_out = N_header + iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range
      int Multiple_out = noutput_items/N_out;  // calculate output to packet size multiple
      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Number of output samples per block = " << N_out << std::endl;
      std::cout << "Add_Header_impl: Number of header samples = " << N_header << std::endl;
      std::cout << "Add_Header_impl: Expected number of blocks = " << iNumOfOutputMultiple << std::endl;
      std::cout << "Add_Header_impl: Available number of blocks = " << Multiple_out << std::endl;
      #endif

      int N_in = iPacketSize;  // number of input samples for each sample burst

      for (int index = 0; index < ninput_items_required.size(); index++)  // go through all inputs
      {
        ninput_items_required[index] = N_in*Multiple_out;  // set the number of required inputs equal to number of required sample
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Index = " << index <<" Required number of input = " << ninput_items_required[index] << std::endl;
        #endif
      }
    }

    int
    Add_Header_impl::general_work (int noutput_items,
                      gr_vector_int &ninput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
  {
      #ifdef _FLOW_MODE_
      std::cout << "Add_Header_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *in = (const char *) input_items[0];
      const char *in_SpB =  nullptr;
      char *out = (char *) output_items[0];
      char *out_SpB =  nullptr;      

      if(input_items.size() == 2)  // SpB input is connected
      {
        in_SpB = (const char *) input_items[1];
      }

      if(output_items.size() == 2)  // SpB output is connected
      {
        out_SpB = (char *) output_items[1];
      }

      bool bSpBConnected = ( (in_SpB != nullptr) && (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int N_header = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit;
      int N_out = N_header + iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range
      int Multiple_out = noutput_items/N_out;  // calculate output to packet size multiple
      int numInput = ninput_items[0];  // number of available input samples

      int Index_PacketStart = 0;  // packet start element index

      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Add_Header_impl: Work called." << std::endl;
      std::cout << "Add_Header_impl: Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Add_Header_impl: Number of output samples per block = " << N_out << std::endl;
      std::cout << "Add_Header_impl: Number of header samples = " << N_header << std::endl;
      std::cout << "Add_Header_impl: Expected number of blocks = " << iNumOfOutputMultiple << std::endl;
      std::cout << "Add_Header_impl: Available number of blocks = " << Multiple_out << std::endl;
      std::cout << "Add_Header_impl: Expected number of input samples = " << iPacketSize*Multiple_out << std::endl;
      std::cout << "Add_Header_impl: Available number of input samples = " << numInput << std::endl;
      std::cout << "Add_Header_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << std::endl << "Add_Header_impl: DSP main loop: start" << std::endl;
      std::cout << "------------------------------------------" << std::endl;
      #endif

      FillArray<char>(out, 0, noutput_items);  // initialise the array
      if(output_items.size() == 2)  // SpB output is connected
      {
        FillArray<char>(out_SpB, 0, noutput_items);  // initialise the array
      }

      // Do <+signal processing+>
      for(int index = 0; index < Multiple_out; ++index, ++iCounter)  // go through availabler input packets
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Input packet index = " << index << " out of " << Multiple_out - 1 << std::endl;
        #endif

        if(bSpBConnected == true)  // if SpB is to transferred
        {
          FillArray<char>((out_SpB + index*N_out), iSamplesPerBit, N_header);  // fill SpB with input value
        }

        CopyArrays<char>(ptr_cPreambleSeq, (out + Index_PacketStart), iPreambleLen*iSamplesPerBit);  // insert preamble into the array
        Index_PacketStart += iPreambleLen*iSamplesPerBit;  // update packet index

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Preamble inserted!" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Preamble bit sequence = [";
        for(int index = 0; index < iPreambleLen; )  // go through the array
        {
          std::cout << CPRN(ptr_cPreamble[index]);  // show the elements
          ++index;  // increment the coutner
          if((index % 4) == 0)  // if 4 bit has been printed out
          {
            std::cout << ", ";  // show the elements
          }
        }
        std::cout << "]" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet index = " << Index_PacketStart << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Add_Header_impl: Output = ";
        DisplayArray(out, noutput_items, 0);  // display input array
        std::cout << std::endl;
        #endif

        CopyArrays<char>(ptr_cLabelBitsSeq, (out + Index_PacketStart), iLabelLen*iLabelBitsPerLet*iSamplesPerBit);  // insert label into the array
        Index_PacketStart += iLabelLen*iLabelBitsPerLet*iSamplesPerBit;  // update packet index

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Label inserted!" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Label = [";
        for(int index = 0; index < iLabelLen*iLabelBitsPerLet; )  // go through the array
        {
          std::cout << CPRN(ptr_cLabelBits[index]);  // show the elements
          ++index;  // increment the coutner
          if((index % 4) == 0)  // if 4 bit has been printed out
          {
            std::cout << ", ";  // show the elements
          }
        }
        std::cout << "]" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet index = " << Index_PacketStart << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Add_Header_impl: Output = ";
        DisplayArray(out, noutput_items, 0);  // display input array
        std::cout << std::endl;
        #endif

        Num2Bits<char>(iCounter, ary_cCounterBits, iCounterLen);  // convert the letter to bit sequence
        InterpArray<char>(ary_cCounterBits, ptr_cCounterSeq, iCounterLen, iSamplesPerBit);  // resample the array
        CopyArrays<char>(ptr_cCounterSeq, (out + Index_PacketStart), iCounterLen*iSamplesPerBit);  // insert counter into the array
        Index_PacketStart += iCounterLen*iSamplesPerBit;  // update packet index

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Counter inserted!" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Current counter = " << iCounter << std::endl;
        std::cout << "Add_Header_impl: Counter bit sequence = [";
        for(int index = 0; index < iCounterLen; )  // go through the array
        {
          std::cout << CPRN(ary_cCounterBits[index]);  // show the elements
          ++index;  // increment the coutner
          if((index % 4) == 0)  // if 4 bit has been printed out
          {
            std::cout << ", ";  // show the elements
          }
        }
        std::cout << "]" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet index = " << Index_PacketStart << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Add_Header_impl: Output = ";
        DisplayArray(out, noutput_items, 0);  // display input array
        std::cout << std::endl;
        #endif

        CopyArrays<char>((in + index*iPacketSize), (out + Index_PacketStart), iPacketSize);  // insert counter into the array
        Index_PacketStart += iPacketSize;  // update packet index

        if(bSpBConnected == true)  // if SpB is to transferred
        {
          CopyArrays<char>((in_SpB + index*iPacketSize), (out_SpB + index*N_out + N_header), iPacketSize);  // insert SpB into the output array
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Data inserted!" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet index = " << Index_PacketStart << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Add_Header_impl: Output = ";
        DisplayArray(out, noutput_items, 0);  // display input array
        std::cout << std::endl;
        if(bSpBConnected == true)  // if SpB is to transferred
        {
          std::cout << "Add_Header_impl: SpB = ";
          DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
          std::cout << std::endl;
        }
        std::cout << "__________________________________________" << std::endl;
        #endif

      }

      #ifdef _DEBUG_MODE_
      std::cout << "------------------------------------------" << std::endl;
      std::cout << std::endl << "Add_Header_impl: DSP main loop: end" << std::endl;
      #endif

      #ifdef _ARRAY_MODE_
      std::cout << "Add_Header_impl: Input = ";
      DisplayArray(in, numInput, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Add_Header_impl: Output = ";
      DisplayArray<char>(out, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Add_Header_impl: Final SpB = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (iPacketSize*Multiple_out);

      #ifdef _FLOW_MODE_
      std::cout << "Add_Header_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

