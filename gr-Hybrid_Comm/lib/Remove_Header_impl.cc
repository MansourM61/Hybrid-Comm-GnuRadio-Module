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
#include "Remove_Header_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Remove_Header::sptr
    Remove_Header::make(int packetSize, const std::vector<char>& preamble, const std::string& label, int samplesPerBit)
    {
      return gnuradio::get_initial_sptr
        (new Remove_Header_impl(packetSize, preamble, label, samplesPerBit));
    }

    const std::vector<int> Remove_Header_impl::iov = {sizeof(char), sizeof(char), sizeof(int), sizeof(char)};  // io signature

    const std::vector<char> Remove_Header_impl::defPreamb = {0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0};  // default preamble
    const std::string Remove_Header_impl::defLabel = "L1";  // default packet label
    const int Remove_Header_impl::iCounterLen = sizeof(char)*COUNTER_BITS;  // counter field length
    const int Remove_Header_impl::iLabelBitsPerLet = sizeof(char)*BITS_PER_LETT;  // label bits per letters
    const int Remove_Header_impl::iNumOfOutputMultiple = OUT_NUM_MP;  // number of output items multiple

    /*
     * The private constructor
     */
    Remove_Header_impl::Remove_Header_impl(int packetSize, const std::vector<char>& preamble, const std::string& label, int samplesPerBit)
      : gr::block("Remove Header",
              gr::io_signature::make(1, 2, sizeof(char)),
              gr::io_signature::makev(3, 4, iov)),
              iPreambleLen(1), iLabelLen(1), iSamplesPerBit(1), iPacketSize(1), iCounter(0), iStoredSampLen(0), bStorageLoaded(false),
              ptr_cPreamble(nullptr), ptr_cLabel(nullptr), ptr_cLabelBits(nullptr), ary_cCounterBits(nullptr),
              ptr_cPreambleSeq(nullptr), ptr_cLabelBitsSeq(nullptr), ptr_cCounterSeq(nullptr),
              ptr_cAuxStorage(nullptr), ptr_cHeaderPattern(nullptr), ptr_cAuxPattern(nullptr)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Constructor called." << std::endl;
      #endif
      this->set_Preamble(preamble);  // set packet preamble
      this->set_Label(label);  // set packet label
      this->set_SamplesPerBit(samplesPerBit);  // set samples per bits bit
      this->set_PacketSize(packetSize);  // set packet size

      this->set_history(1);

      ary_cCounterBits = new char [iCounterLen];  // allocate memory

      #ifdef _FLOW_MODE_
      std::cout << "Remove_Header_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Remove_Header_impl::~Remove_Header_impl()
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

      if (ptr_cAuxStorage != nullptr)  // if the array is free
      {
        delete[] ptr_cAuxStorage;  // release the array
        ptr_cAuxStorage = nullptr;  // label the array as empty
      }

      if (ptr_cHeaderPattern != nullptr)  // if the array is free
      {
        delete[] ptr_cHeaderPattern;  // release the array
        ptr_cHeaderPattern = nullptr;  // label the array as empty

        delete[] ptr_cAuxPattern;  // release the array
        ptr_cAuxPattern = nullptr;  // label the array as empty
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
    void Remove_Header_impl::ConfigureHeaderSize(void)
    {
      #ifdef _THREAD_MUTEX_
      gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Configure header size called." << std::endl;
      #endif
      int N_out = iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range
      int N_header = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit;
      int N_in = N_header + iPacketSize;  // calculate number of input samples for each sample
      
      block::set_output_multiple(N_out);
//      block::set_max_noutput_items(N_out*iNumOfOutputMultiple);

      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Number of output block samples = " << N_out << std::endl;
      std::cout << "Remove_Header_impl: Maximum number of output samples = " << N_out*iNumOfOutputMultiple << std::endl;
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

      if (ptr_cAuxStorage != nullptr)  // if the array is free
      {
        delete[] ptr_cAuxStorage;  // release the array
        ptr_cAuxStorage = nullptr;  // label the array as empty
      }
      ptr_cAuxStorage = new char [N_in];  // get the array memory
      FillArray<char>(ptr_cAuxStorage, 0, N_in);  // initialise the array with 0

      int N_headerPattern = (iPreambleLen + iLabelLen*iLabelBitsPerLet) * iSamplesPerBit;  // number of samples in header pattern (preamble + label)
      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Number of header pattern sample = " << N_headerPattern << std::endl;
      #endif
      if (ptr_cHeaderPattern != nullptr)  // if the array is free
      {
        delete[] ptr_cHeaderPattern;  // release the array
        ptr_cHeaderPattern = nullptr;  // label the array as empty

        delete[] ptr_cAuxPattern;  // release the array
        ptr_cAuxPattern = nullptr;  // label the array as empty
      }
      if ((ptr_cPreambleSeq != nullptr) && (ptr_cLabelBitsSeq != nullptr))
      {
        ptr_cHeaderPattern = new char [N_headerPattern];  // get the array memory
        ptr_cAuxPattern = new char [N_headerPattern];  // get the array memory
        CopyArrays<char>(ptr_cPreambleSeq, ptr_cHeaderPattern, iPreambleLen*iSamplesPerBit);  // insert preamble sequence
        CopyArrays<char>(ptr_cLabelBitsSeq, (ptr_cHeaderPattern + iPreambleLen*iSamplesPerBit), iLabelLen*iLabelBitsPerLet*iSamplesPerBit);  // insert label sequence
      }

      #ifdef _DEBUG_MODE_
      if (ptr_cHeaderPattern != nullptr)
      {
        std::cout << "Remove_Header_impl: Header pattern sequence = [";
        for(int index = 0; index < N_headerPattern; )  // go through the array
        {
          std::cout << CPRN(ptr_cHeaderPattern[index]);  // show the elements
          ++index;  // increment the coutner
          if((index % 4) == 0)  // if 4 bit has been printed out
          {
            std::cout << ", ";  // show the elements
          }
        }
        std::cout << "]" << std::endl;
      }
      #endif
    }


    void
    Remove_Header_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      #ifdef _DEBUG_MODE_
      std::cout << "Add_Header_impl: Forecast called." << std::endl;
      std::cout << "Remove_Header_impl: Number of output samples = " << noutput_items << std::endl;
      #endif

      int N_out = iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range
      int Multiple_out = noutput_items/N_out;  // calculate number of output blocks
      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Number of output samples per block = " << N_out << std::endl;
      std::cout << "Remove_Header_impl: Expected number of blocks = " << iNumOfOutputMultiple << std::endl;
      std::cout << "Remove_Header_impl: Available number of blocks = " << Multiple_out << std::endl;
      #endif

      int N_header = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit;
      int N_in = N_header + iPacketSize;  // calculate number of input samples for each sample 

      for (int index = 0; index < ninput_items_required.size(); index++)  // go through all inputs
      {
        ninput_items_required[index] = N_in*Multiple_out;  // set the number of required inputs equal to number of required sample
        #ifdef _DEBUG_MODE_
        std::cout << "Remove_Header_impl: Index = " << index <<" Required number of input = " << ninput_items_required[index] << std::endl;
        #endif
      }
    }


    int
    Remove_Header_impl::general_work (int noutput_items,
                      gr_vector_int &ninput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Remove_Header_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *in = (const char *) input_items[0];
      const char *in_SpB =  nullptr;
      char *out = (char *) output_items[0];
      char *sync = (char *) output_items[1];
      int *counter = (int *) output_items[2];
      char *out_SpB =  nullptr;      

      if(input_items.size() == 2)  // SpB input is connected
      {
        in_SpB = (const char *) input_items[1];
      }

      if(output_items.size() == 4)  // SpB output is connected
      {
        out_SpB = (char *) output_items[3];
      }

      bool bSpBConnected = ( (in_SpB != nullptr) && (out_SpB != nullptr) ) ? true : false;  // if SpB is to be transferred

      int N_header = (iPreambleLen + iLabelLen*iLabelBitsPerLet + iCounterLen) * iSamplesPerBit;
      int N_in = N_header + iPacketSize;  // calculate number of input samples for each sample 

      int N_out = iPacketSize;  // calculate number of output samples for each sample burst
      N_out = CONSTRAIN(N_out, 1 ,INT_MAX);  // check the range
      int Multiple_out = noutput_items/N_out;  // calculate number of ouput blocks
      int numInput = ninput_items[0];  // number of available input samples

      int N_headerPattern = (iPreambleLen + iLabelLen*iLabelBitsPerLet) * iSamplesPerBit;  // number of samples in header pattern (preamble + label)
      int index_M = MatchBitSeq<char>(in, ptr_cHeaderPattern, ptr_cAuxPattern, numInput, N_headerPattern); // find first matching index
      int Index_PacketStart;  // index of processed element in the current packet; by default all the elements are analysed.
      int totNumInput = N_in*Multiple_out;  // total number of inputs

      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Work called." << std::endl;
      std::cout << "Remove_Header_impl: Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Remove_Header_impl: Number of output samples per block = " << N_out << std::endl;
      std::cout << "Remove_Header_impl: Number of header samples = " << N_header << std::endl;
      std::cout << "Remove_Header_impl: Expected number of blocks = " << iNumOfOutputMultiple << std::endl;
      std::cout << "Remove_Header_impl: Available number of blocks = " << Multiple_out << std::endl;
      std::cout << "Remove_Header_impl: Expected number of input samples = " << totNumInput << std::endl;
      std::cout << "Remove_Header_impl: Available number of input samples = " << numInput << std::endl;
      std::cout << "Remove_Header_impl: Samples per bit pin is " << ((bSpBConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      FillArray<char>(out, INV_SIG_VAL, noutput_items);  // initialise out array
      FillArray<char>(sync, DEF_SIG_VAL, noutput_items);  // initialise sync array
      FillArray<int>(counter, DEF_SIG_VAL, noutput_items);  // initialise short array

      // Do <+signal processing+>
      if (index_M != -1)  // if a match is found
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Remove_Header_impl: Match found" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Remove_Header_impl: First pattern and array matching index = " << index_M << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << std::endl << "Remove_Header_impl: DSP main loop: Start" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        #endif

        int NumOfAvPack = Multiple_out - int(index_M/N_in);  // number of available blocks ready for DSP
        #ifdef _DEBUG_MODE_
        std::cout << "Remove_Header_impl: Number of available input blocks for DSP = " << NumOfAvPack << std::endl;
        #endif

        Index_PacketStart = index_M;  // index of first element in the current packet

        for (int index_B = 0; index_B < NumOfAvPack; ++index_B)  // go through available blocks
        {

          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Input packet index = " << index_B << " out of " << NumOfAvPack - 1 << std::endl;
          #endif

          sync[index_B*N_out] = 1;  // set sync pulse

          Index_PacketStart += (iPreambleLen + iLabelLen*iLabelBitsPerLet)*iSamplesPerBit;  // update packet index to point to counter section

          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Preamble and label parsed!" << std::endl;
          #endif

          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Packet index = " << Index_PacketStart << std::endl;
          #endif

          CopyArrays<char>((in + Index_PacketStart), ptr_cCounterSeq, iCounterLen*iSamplesPerBit);  // insert counter into the array
          DecimatArray<char>(ptr_cCounterSeq, ary_cCounterBits, iCounterLen*iSamplesPerBit, iSamplesPerBit);  // decimate the counter array
          int counterValue = Bits2Num<int>(ary_cCounterBits, iCounterLen);  // convert the counter bits to equivalent number
          counter[index_B*N_out] = counterValue;  // set counter output
          Index_PacketStart += iCounterLen*iSamplesPerBit;  // update packet index to point to data section

          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Counter parsed!" << std::endl;
          #endif
          
          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Packet index = " << Index_PacketStart << std::endl;
          std::cout << "Remove_Header_impl: Counter bit sequence = [";
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
          std::cout << "Remove_Header_impl: Counter = " << counterValue << std::endl;;
          #endif

          CopyArrays<char>((in + Index_PacketStart), (out + index_B*N_out), N_out);  // insert data into the output array

          if(bSpBConnected == true)  // if SpB is to transferred
          {
            CopyArrays<char>((in_SpB + Index_PacketStart), (out_SpB + index_B*N_out), N_out);  // insert SpB into the output array
          }

          Index_PacketStart += N_out;  // update packet index to point to end of packet

          #ifdef _DEBUG_MODE_
          std::cout << "Remove_Header_impl: Packet index = " << Index_PacketStart << std::endl;
          #endif

          #ifdef _ARRAY_MODE_
          std::cout << "Remove_Header_impl: Output = ";
          DisplayArray<char>(out, noutput_items, 0);  // display input array
          std::cout << std::endl;
          if(bSpBConnected == true)  // if SpB is to transferred
          {
            std::cout << "Remove_Header_impl: SpB = ";
            DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
            std::cout << std::endl;
          }
          #endif
          
          index_M = MatchBitSeq<char>((in + Index_PacketStart), ptr_cHeaderPattern, ptr_cAuxPattern, numInput - Index_PacketStart, N_headerPattern); // find first matching index
          if(index_M == -1)  // no more match is found
          {
            break;  // leave the loop
          }
          Index_PacketStart += index_M;  // adjust the input index
          if((totNumInput - Index_PacketStart) < N_in)  // if no more packet is available
          {
            break;  // leave the loop
          }

        }

        #ifdef _DEBUG_MODE_
        std::cout << "------------------------------------------" << std::endl;
        std::cout << std::endl << "Remove_Header_impl: DSP main loop: End" << std::endl;
        #endif

      }
      else  // if a match is not found
      {

        #ifdef _DEBUG_MODE_
        std::cout << "Remove_Header_impl: No match found!" << std::endl;
        #endif
        FillArray<int>(counter, -1, noutput_items);  // indicate loss of sync

        Index_PacketStart = numInput;  // total number of items being analysed
      }

      #ifdef _DEBUG_MODE_
      std::cout << "Remove_Header_impl: Total processed item = " << Index_PacketStart << ", " << numInput << std::endl;
      #endif

      #ifdef _ARRAY_MODE_
      std::cout << "Remove_Header_impl: Final output = ";
      DisplayArray<char>(out, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bSpBConnected == true)  // if SpB is to transferred
      {
        std::cout << "Remove_Header_impl: Final SpB = ";
        DisplayArray<char>(out_SpB, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      // Tell runtime system how many input items we consumed on
      // each input stream.

      consume_each (totNumInput);

      #ifdef _FLOW_MODE_
      std::cout << "Remove_Header_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

