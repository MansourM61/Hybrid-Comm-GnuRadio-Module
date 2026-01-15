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
#include "Hysteresis_Gate_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Hysteresis_Gate::sptr
    Hysteresis_Gate::make(int packetSize, float forwardPoint, float forwardSlope, float forwardState, float backwardPoint, float backwardSlope, float backwardState, std::string mode)
    {
      return gnuradio::get_initial_sptr
        (new Hysteresis_Gate_impl(packetSize, forwardPoint, forwardSlope, forwardState, backwardPoint, backwardSlope, backwardState, mode));
    }

      const std::string Hysteresis_Gate_impl::strForward = FOR_STR;
      const std::string Hysteresis_Gate_impl::strBackward = BAK_STR;

    /*
     * The private constructor
     */
    Hysteresis_Gate_impl::Hysteresis_Gate_impl(int packetSize, float forwardPoint, float forwardSlope, float forwardState, float backwardPoint, float backwardSlope, float backwardState, std::string mode)
      : gr::sync_block("Hysteresis Gate",
              gr::io_signature::make(2, 2, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))), fForwardPoint(forwardPoint), fForwardSlope(forwardSlope), fForwardState(forwardState), 
              fBackwardPoint(backwardPoint), fBackwardSlope(backwardSlope), fBackwardState(backwardState)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Hysteresis_Gate_impl: Constructor called." << std::endl;
      #endif
      this->set_PacketSize(packetSize);  // set packet size
      this->set_ForwardPoint(forwardPoint);  // set hysteresis forward raising point
      this->set_ForwardSlope(forwardSlope);  // set hysteresis forward raising slope
      this->set_ForwardState(forwardState);  // set hysteresis forward raising saturation state
      this->set_BackwardPoint(backwardPoint);  // set hysteresis backward raising point
      this->set_BackwardSlope(backwardSlope);  // set hysteresis backward raising slope
      this->set_BackwardState(backwardState);  // set hysteresis backward raising saturation state
      this->set_Mode(mode);  // set hysteresis mode

      #ifdef _FLOW_MODE_
      std::cout << "Hysteresis_Gate_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Hysteresis_Gate_impl::~Hysteresis_Gate_impl()
    {
    }

    int
    Hysteresis_Gate_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Hysteresis_Gate_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const float *Sig_1 = (const float *) input_items[0];  // input signal 1
      const float *Sig_2 = (const float *) input_items[1];  // input signal 2
      float *Out = (float *) output_items[0];  // output signal

      int NoP = noutput_items/iPacketSize;  // number of available packets

      #ifdef _DEBUG_MODE_
      std::cout << "Hysteresis_Gate_impl: Work called." << std::endl;
      std::cout << "Hysteresis_Gate_impl: Number of output items = " << noutput_items << std::endl;      
      std::cout << "Hysteresis_Gate_impl: Number of available packets = " << NoP << std::endl;      
      std::cout << "Hysteresis_Gate_impl: Samples per packets = " << iPacketSize << std::endl;      
      #endif

      float diff = 0.0;  // signal difference
      float out_sel = 0.0;  // output select

      // Do <+signal processing+>
      for(int index = 0; index < NoP; ++index)  // go though all packets
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Hysteresis_Gate_impl: Packet index " << index << " out of " << NoP - 1 << std::endl;
        #endif

        diff = Sig_1[index*iPacketSize] - Sig_2[index*iPacketSize];  // update signal difference

        switch(cMode)  // which mode is the current hysteresis?
        {
          case 0:  // forward hysteresis
          {
            out_sel = CONSTRAIN(+1.0*(diff - fForwardPoint)*fForwardSlope + fBackwardState, fBackwardState, fForwardState);  // update output array
          
            cMode = EQUAL(out_sel, fForwardState) ? 1 : 0;  // if the output has passed the maximum forward point, set the mode to 1 = backward
            break;
          }
          case 1:  // backward hysteresis
          {
            out_sel = CONSTRAIN(-1.0*(diff - fBackwardPoint)*fBackwardSlope + fForwardState, fBackwardState, fForwardState);  // update output array

            cMode = EQUAL(out_sel, fBackwardState) ? 0 : 1;  // if the output has passed the maximum backward point, set the mode to 0 = forward
            break;
          }
          default:  // default hysteresis
          {
            out_sel = CONSTRAIN(0, fBackwardState, fForwardState);  // update output array
            break;
          }
        }
        FillArray<float>((Out + index*iPacketSize), out_sel, iPacketSize);  // fill output array
      }

      #ifdef _FLOW_MODE_
      std::cout << "Remove_Header_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

