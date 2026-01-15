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

#ifndef INCLUDED_HYBRID_COMM_ADD_HEADER_IMPL_H
#define INCLUDED_HYBRID_COMM_ADD_HEADER_IMPL_H

#include <Hybrid_Comm/Add_Header.h>

namespace gr {
  namespace Hybrid_Comm {

    class Add_Header_impl : public Add_Header
    {
     private:
      // Nothing to declare in this block.
      char *ptr_cPreamble;  // preamble array
      int iPreambleLen;  // preamble array length
      char *ptr_cLabel;  // packet label array
      char *ptr_cLabelBits;  // packet label bit sequence
      int iLabelLen;  // packet label array length
      int iSamplesPerBit;  // samples per bit of the header
      int iPacketSize;  // packet size
      int iCounter;  // packet counter
      char *ptr_cPreambleSeq;  // preamble resampled bit sequence
      char *ptr_cLabelBitsSeq;  // label bits resampled bit sequence
      char *ptr_cCounterSeq;  // counter resampled bit sequence
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      char *ary_cCounterBits;  // counter bit sequence

      static const std::vector<char> defPreamb;  // default preamble
      static const std::string defLabel;  // default packet label
      static const int iCounterLen;  // counter field length
      static const int iLabelBitsPerLet;  // label bits per letters
      static const int iNumOfOutputMultiple;  // number of output items multiple

     public:
      Add_Header_impl(int packetSize = PACKET_SAMP_SIZE, const std::vector<char>& preamble = defPreamb, const std::string& label = defLabel, int samplesPerBit = DEF_SPB);
      ~Add_Header_impl();
      void ConfigureHeaderSize(void);  // configure header size based on the new pattern


      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);

      // Set preamble
      void set_Preamble(const std::vector<char>& preamble)
      {
        if (ptr_cPreamble != nullptr)  // if the array is free
        {
          delete[] ptr_cPreamble;  // release the array
          ptr_cPreamble = nullptr;  // label the array as empty
        }

        iPreambleLen = preamble.size();  // update new number of preamble samples

        ptr_cPreamble = new char [iPreambleLen];  // get the array memory

        for(int index = 0; index < iPreambleLen; ++index)  // go through the array
        {
          ptr_cPreamble[index] = preamble[index];  // update the element
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Preambles = [";
        for(int index = 0; index < iPreambleLen; ++index)  // go through the array
        {
          std::cout << CPRN(ptr_cPreamble[index])<< ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif

        this->ConfigureHeaderSize();  // configure header size based on the new pattern
      }

      // Get preamble
      void get_Preamble(std::vector<char>* preamble)
      {
        delete[] preamble;  // release the array
        preamble = new std::vector<char>;
        std::vector<char>::iterator it = preamble->begin();  // set the iterator to begining

        for(int index = 0; index < iPreambleLen; ++index)  // go through the array
        {
          it = preamble->insert(it, ptr_cPreamble[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Preambles = [";
        for(int index = 0; index < iPreambleLen; ++index)  // go through the array
        {
          std::cout << CPRN(ptr_cPreamble[index]) << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Set packet label
      void set_Label(const std::string& label)
      {
        if (ptr_cLabel != nullptr)  // if the array is free
        {
          delete[] ptr_cLabel;  // release the array
          ptr_cLabel = nullptr;  // label the array as empty

          delete[] ptr_cLabelBits;  // release the array
          ptr_cLabelBits = nullptr;  // label the array as empty
        }

        iLabelLen = label.length();  // update new number of preamble samples

        ptr_cLabel = new char [iLabelLen + 1];  // get the array memory
        ptr_cLabelBits = new char [iLabelLen*iLabelBitsPerLet];  // get the array memory


        std::strcpy(ptr_cLabel, label.c_str());  // copy the string to char array

        for(int index_l = 0; index_l < iLabelLen; ++index_l)  // go through the array
        {
          Num2Bits<char>(ptr_cLabel[index_l], ( ptr_cLabelBits + index_l*iLabelBitsPerLet ), iLabelBitsPerLet);  // convert the letter to bit sequence
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet label = \"";
        for(int index = 0; index < iLabelLen; ++index)  // go through the array
        {
          std::cout << ptr_cLabel[index];  // show the elements
        }
        std::cout << "\"" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet label bits = [";
        for(int index = 0; index < iLabelLen*8; ++index)  // go through the array
        {
          if((index % 8) == 0)  // if it is starting of a character
          {
            std::cout << "\"";
          }
          std::cout << CPRN(ptr_cLabelBits[index]);  // show the elements
          if(((index + 1) % 8) == 0)  // if it is end of a character
          {
            std::cout << "\", ";
          }
        }
        std::cout << "]" << std::endl;
        #endif

        this->ConfigureHeaderSize();  // configure header size based on the new pattern
      }

      // Get packet label
      void get_Label(std::string* label)
      {
        delete[] label;  // release the array
        label = new std::string(ptr_cLabel);  // create string object

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet label = \"";
        for(int index = 0; index < iLabelLen; ++index)  // go through the array
        {
          std::cout << ptr_cLabel[index];  // show the elements
        }
        std::cout << "\"" << std::endl;
        #endif

        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet label bits = [";
        for(int index = 0; index < iLabelLen*8; ++index)  // go through the array
        {
          if((index % 8) == 0)  // if it is starting of a character
          {
            std::cout << "\"";
          }
          std::cout << CPRN(ptr_cLabelBits[index]);  // show the elements
          if(((index + 1) % 8) == 0)  // if it is end of a character
          {
            std::cout << "\", ";
          }
        }
        std::cout << "]" << std::endl;
        #endif

      }

      // Set samples per bit
      void set_SamplesPerBit(int samplesPerBit)
      {
        #ifdef _THREAD_MUTEX_
        gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
        #endif
        iSamplesPerBit = CONSTRAIN(samplesPerBit, 1, INT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Samples per bit = " << iSamplesPerBit << std::endl;
        #endif

        this->ConfigureHeaderSize();  // configure header size based on the new pattern
      }

      // Get samples per bit
      int get_SamplesPerBit(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Samples per bit = " << iSamplesPerBit << std::endl;
        #endif
        return iSamplesPerBit;
      }

      // Set packet size
      void set_PacketSize(int packetsize)
      {
        #ifdef _THREAD_MUTEX_
        gr::thread::scoped_lock dummy(d_mutex_delay);  //  create a mutex lock for the current scope#
        #endif
        iPacketSize = CONSTRAIN(packetsize, 1, INT_MAX);
        this->set_output_multiple(iPacketSize);
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet size = " << iPacketSize << std::endl;
        #endif

        this->ConfigureHeaderSize();  // configure header size based on the new pattern
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Add_Header_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_ADD_HEADER_IMPL_H */

