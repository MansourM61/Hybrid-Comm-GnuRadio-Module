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

#ifndef INCLUDED_HYBRID_COMM_SLICER_IMPL_H
#define INCLUDED_HYBRID_COMM_SLICER_IMPL_H

#include <Hybrid_Comm/Slicer.h>

namespace gr {
  namespace Hybrid_Comm {

    class Slicer_impl : public Slicer
    {
     private:
      // Nothing to declare in this block.
      int iAvgWinSize;  // averaging window size
      int iAvgWeightLen;  // averaging weight array length
      int iPacketSize;  // incoming packet size
      float fThresh;  // threshold
      float *ptr_fWeigths;  // averaging weights
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

     public:
      Slicer_impl(int packetSize = PACKET_SAMP_SIZE, int avgWinSize = AVG_WIN_SIZE, float thresh = DEF_THRESH);
      ~Slicer_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set averaging window size
      void set_AvgWinSize(int avgWinSize)
      {
        iAvgWinSize = CONSTRAIN(avgWinSize, 1, INT_MAX);
        if(iAvgWinSize > 1)  // if averaging is required
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Slicer_impl: Averaging is needed." << std::endl;
          #endif

          iAvgWinSize += (iAvgWinSize % 2 == 0) ? 1 : 0;  // make sure it is odd value

          if(ptr_fWeigths != nullptr)  // if array is no empty
          {
            delete[] ptr_fWeigths;  // clean array
            ptr_fWeigths = nullptr;  // mark it as empty
          }

          iAvgWeightLen = (iAvgWinSize + 1) / 2;

          ptr_fWeigths = new float[iAvgWeightLen];

          float del_x = 5.0/(iAvgWeightLen - 1);

          for(int index = 0; index < iAvgWeightLen; ++index)
          {
            ptr_fWeigths[index] = exp(-POW2(del_x*index))/sqrt(2*M_PI); 
          }
        }
        else  // if averaging is not needed
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Slicer_impl: Averaging is not needed." << std::endl;
          #endif

          ptr_fWeigths = new float[1];
          ptr_fWeigths[0] = 1.0;
          iAvgWeightLen = 1;
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Slicer_impl: Averaging window size = " << iAvgWinSize << std::endl;
        std::cout << "Slicer_impl: Weight array = ";
        DisplayArray<float>(ptr_fWeigths, iAvgWeightLen, 0);
        std::cout << std::endl;
        #endif
      }

      // Get measurement window size
      int get_AvgWinSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Window size = " << iAvgWinSize << std::endl;
        #endif
        return iAvgWinSize;
      }

      // Set incoming packet size
      void set_PacketSize(int packetSize)
      {
        iPacketSize = CONSTRAIN(packetSize, 1, INT_MAX);
        this->set_output_multiple(iPacketSize);  // set output array size
        #ifdef _DEBUG_MODE_
        std::cout << "Slicer_impl: Incoming packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get incoming packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Slicer_impl: Incoming packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set link selection threshold
      void set_Thresh(float thresh)
      {
        fThresh = thresh;
        #ifdef _DEBUG_MODE_
        std::cout << "Slicer_impl: Slicing threshold = " << fThresh << std::endl;
        #endif
      }

      // Get link selection threshold
      float get_Thresh(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Slicer_impl: Slicing threshold = " << fThresh << std::endl;
        #endif
        return fThresh;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SLICER_IMPL_H */

