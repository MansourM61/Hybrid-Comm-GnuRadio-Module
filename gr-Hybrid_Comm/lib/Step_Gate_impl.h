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

#ifndef INCLUDED_HYBRID_COMM_STEP_GATE_IMPL_H
#define INCLUDED_HYBRID_COMM_STEP_GATE_IMPL_H

#include <Hybrid_Comm/Step_Gate.h>

namespace gr {
  namespace Hybrid_Comm {

    class Step_Gate_impl : public Step_Gate
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      int iNumOfLevs;  // number of levels
      float *ptr_fLevels;  // levels array
      float *ptr_fPoints;  // hopping points array
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::vector<float> defLev;  // default levels
      static const std::vector<float> defPnt;  // default levels

     public:
      Step_Gate_impl(int packetSize = PACKET_SAMP_SIZE, const std::vector<float>& levels = defLev, const std::vector<float>& hopPoints = defPnt);
      ~Step_Gate_impl();

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
        std::cout << "Step_Gate_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set levels values
      void set_Levels(const std::vector<float>& levels)
      {
        if (ptr_fLevels != nullptr)  // if the array is free
        {
          delete[] ptr_fLevels;  // release the array
          ptr_fLevels = nullptr;  // label the array as empty
        }

        iNumOfLevs = levels.size();  // update new number of levels
        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Number of levels = " << iNumOfLevs << std::endl;
        #endif

        ptr_fLevels = new float [iNumOfLevs];  // get the array memory

        ptr_fLevels[0] = levels[0];  // update first element
        ptr_fLevels[iNumOfLevs - 1] = CONSTRAIN(levels[iNumOfLevs - 1], levels[0], +FLT_MAX);  // update last element

        for(int index = 1; index < iNumOfLevs - 1; ++index)  // go through the array
        {
          ptr_fLevels[index] = CONSTRAIN(levels[index], ptr_fLevels[index - 1], +FLT_MAX);  // update the element
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Levels = [";
        for(int index = 0; index < iNumOfLevs; ++index)  // go through the array
        {
          std::cout << ptr_fLevels[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Get levels values
      void get_Levels(std::vector<float>* levels)
      {
        delete[] levels;  // release the array
        levels = new std::vector<float>;
        std::vector<float>::iterator it = levels->begin();  // set the iterator to begining

        for(int index = 0; index < iNumOfLevs; ++index)  // go through the array
        {
          it = levels->insert(it, ptr_fLevels[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Levels = [";
        for(int index = 0; index < iNumOfLevs; ++index)  // go through the array
        {
          std::cout << ptr_fLevels[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Set hopping points
      void set_HopPoints(const std::vector<float>& hopPoints)
      {
        if (ptr_fPoints != nullptr)  // if the array is free
        {
          delete[] ptr_fPoints;  // release the array
          ptr_fPoints = nullptr;  // label the array as empty
        }

        iNumOfLevs = hopPoints.size() + 1;  // update new number of levels
        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Number of levels = " << iNumOfLevs << std::endl;
        #endif

        ptr_fPoints = new float [iNumOfLevs];  // get the array

        ptr_fPoints[0] = hopPoints[0];  // update first element
        ptr_fPoints[iNumOfLevs - 2] = CONSTRAIN(hopPoints[iNumOfLevs - 2], hopPoints[0], +FLT_MAX);  // update last element

        for(int index = 1; index < iNumOfLevs - 2; ++index)  // go through the array
        {
          ptr_fPoints[index] = CONSTRAIN(hopPoints[index], ptr_fPoints[index - 1], +FLT_MAX);  // update the element
        }
        
        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Hopping points = [";
        for(int index = 0; index < iNumOfLevs - 1; ++index)  // go through the array
        {
          std::cout << ptr_fPoints[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

      // Get hopping points
      void get_HopPoints(std::vector<float>* hopPoints)
      {
        delete[] hopPoints;  // release the array
        hopPoints = new std::vector<float>;
        std::vector<float>::iterator it = hopPoints->begin();  // set the iterator to begining

        for(int index = 1; index < iNumOfLevs - 1; ++index)  // go through the array
        {
          it = hopPoints->insert(it, ptr_fPoints[index]);  // insert the element and update iterator
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Step_Gate_impl: Hopping points = [";
        for(int index = 0; index < iNumOfLevs - 1; ++index)  // go through the array
        {
          std::cout << ptr_fPoints[index] << ", ";  // show the elements
        }
        std::cout << "]" << std::endl;
        #endif
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_STEP_GATE_IMPL_H */

