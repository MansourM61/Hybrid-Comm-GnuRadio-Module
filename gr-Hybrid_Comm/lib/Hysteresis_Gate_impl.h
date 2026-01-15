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

#ifndef INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_IMPL_H
#define INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_IMPL_H

#include <Hybrid_Comm/Hysteresis_Gate.h>

namespace gr {
  namespace Hybrid_Comm {

    enum HystType {Forward = 0, Backward = 1};

    class Hysteresis_Gate_impl : public Hysteresis_Gate
    {
     private:
      // Nothing to declare in this block.
      int iPacketSize;  // packet size
      float fForwardPoint;  // hysteresis forward raising point
      float fForwardSlope;  // hysteresis forward raising slope
      float fForwardState;  // hysteresis forward raising saturation state
      float fBackwardPoint;  // hysteresis backward raising point
      float fBackwardSlope;  // hysteresis backward raising slope
      float fBackwardState;  // hysteresis backward raising saturation state
      char cMode;  // current mode 0 = forward, 1 = backward
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::string strForward;
      static const std::string strBackward;

     public:
      Hysteresis_Gate_impl(int packetSize = PACKET_SAMP_SIZE,
                           float forwardPoint = DEF_FOR_PT, float forwardSlope = DEF_FOR_SL,
                           float forwardState = DEF_FOR_SS, float backwardPoint = DEF_BAK_PT,
                           float backwardSlope = DEF_BAK_SL, float backwardState = DEF_BAK_SS,
                           std::string mode = strForward);
      ~Hysteresis_Gate_impl();

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
        std::cout << "Hysteresis_Gate_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set hysteresis forward raising point
      void set_ForwardPoint(float forwardPoint)
      {
        float P0 = -(fBackwardState - fForwardState)/fBackwardSlope + fBackwardPoint;  // end point of backward falling 

        fForwardPoint = CONSTRAIN(forwardPoint, P0, +FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling end point = " << P0 << std::endl;
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising point = " << fForwardPoint << std::endl;
        #endif
      }

      // Get hysteresis forward raising point
      float get_ForwardPoint(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising point = " << fForwardPoint << std::endl;
        #endif
        return fForwardPoint;
      }

      // Set hysteresis forward raising slope
      void set_ForwardSlope(float forwardSlope)
      {
        fForwardSlope = CONSTRAIN(forwardSlope, 0, +FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising slope = " << fForwardSlope << std::endl;
        #endif
      }

      // Get hysteresis forward raising slope
      float get_ForwardSlope(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising slope = " << fForwardSlope << std::endl;
        #endif
        return fForwardSlope;
      }

      // Set hysteresis forward raising saturation state
      void set_ForwardState(float forwardState)
      {
        fForwardState = CONSTRAIN(forwardState, fBackwardState, +FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising saturation state = " << fForwardState << std::endl;
        #endif
      }

      // Get hysteresis forward raising saturation state
      float get_ForwardState(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward raising saturation state = " << fForwardState << std::endl;
        #endif
        return fForwardState;
      }

      // Set hysteresis backward falling point
      void set_BackwardPoint(float backwardPoint)
      {
        float P0 = (fForwardState - fBackwardState)/fForwardSlope + fForwardPoint;  // end point of forward raising

        fBackwardPoint = CONSTRAIN(backwardPoint, -FLT_MAX, P0);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis forward rising end point = " << P0 << std::endl;
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling point = " << fBackwardPoint << std::endl;
        #endif
      }

      // Get hysteresis backward falling point
      float get_BackwardPoint(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling point = " << fBackwardPoint << std::endl;
        #endif
        return fBackwardPoint;
      }

      // Set hysteresis backward falling slope
      void set_BackwardSlope(float backwardSlope)
      {
        fBackwardSlope = CONSTRAIN(backwardSlope, -FLT_MAX, 0);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling slope = " << fBackwardSlope << std::endl;
        #endif
      }

      // Get hysteresis backward falling slope
      float get_BackwardSlope(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling slope = " << fBackwardSlope << std::endl;
        #endif
        return fBackwardSlope;
      }

      // Set hysteresis backward falling saturation state
      void set_BackwardState(float backwardState)
      {
        fBackwardState = CONSTRAIN(backwardState, -FLT_MAX, fForwardState);
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling saturation state = " << fBackwardState << std::endl;
        #endif
      }

      // Get hysteresis backward falling saturation state
      float get_BackwardState(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis backward falling saturation state = " << fBackwardState << std::endl;
        #endif
        return fBackwardState;
      }

      // Set hysteresis mode
      void set_Mode(std::string mode)
      {
        cMode = (mode == strForward) ? Forward : Backward;
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis mode = " << CPRN(cMode) << std::endl;
        #endif
      }

      // Get hysteresis mode
      std::string get_Mode(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Hysteresis mode = " << CPRN(cMode) << std::endl;
        #endif
        return (cMode == Forward) ? strForward : strBackward;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_IMPL_H */

