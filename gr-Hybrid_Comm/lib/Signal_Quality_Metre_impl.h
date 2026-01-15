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

#ifndef INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_IMPL_H
#define INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_IMPL_H

#include <Hybrid_Comm/Signal_Quality_Metre.h>

namespace gr {
  namespace Hybrid_Comm {

    enum MeasType {Ps = 0, Pn = 1, SNR = 2, QF = 3};    

    class Signal_Quality_Metre_impl : public Signal_Quality_Metre
    {
     private:
      // Nothing to declare in this block.
      int iSampsPerSymb;  // samples per symbol
      int iPacketSize;  // packet size
      char cMeasType;  // measurement method
      int iEdgeFindingRange;  // window range to find edge
      float fSigPower;  // initial signal power
      float fNoisePower;  // initial noise power
      float fSNR;  // signal-to-noise ratio (SNR)
      float fQ_fac;  // Q-factor
      float *ptr_fSigMerit;  // signal quality merit
      #ifdef _FLOW_MODE_
      int iFlowCounter;  // flow debugging counter
      #endif

      static const std::string strPs;
      static const std::string strPn;
      static const std::string strSNR;
      static const std::string strQF;

     public:
      Signal_Quality_Metre_impl(int winSize = PACKET_SAMP_SIZE, int sampsPerSymb = DEF_SPB, std::string measType = strPs, int edgeFindingRange = FIND_EDGE_WIN_SIZE);
      ~Signal_Quality_Metre_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );


      // Set samples per symbol
      void set_SampsPerSymb(int sampPerSymb)
      {
        iSampsPerSymb = CONSTRAIN(sampPerSymb, 1, INT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Sample per Symbol = " << iSampsPerSymb << std::endl;
        #endif
      }

      // Get samples per symbol
      int get_SampsPerSymb(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Sample per Symbol = " << iSampsPerSymb << std::endl;
        #endif
        return iSampsPerSymb;
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
        std::cout << "Signal_Quality_Metre_impl: Packet size = " << iPacketSize << std::endl;
        #endif
      }

      // Get packet size
      int get_PacketSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Packet size = " << iPacketSize << std::endl;
        #endif
        return iPacketSize;
      }

      // Set measurement type
      void set_MeasType(std::string measType)
      {

        if(measType == strPs)  // if measurement is signal power
        {
          cMeasType = Ps;
          ptr_fSigMerit = &fSigPower;  // set the pointer to signal power
        }
        else if(measType == strPn)  // if measurement is noise power
        {
          cMeasType = Pn;
          ptr_fSigMerit = &fNoisePower;  // set the pointer to noise power
        }
        else if(measType == strSNR)  // if measurement is signal-to-noise ratio (SNR)
        {
          cMeasType = SNR;
          ptr_fSigMerit = &fSNR;  // set the pointer to signal-to-noise ratio (SNR)
        }
        else if(measType == strQF)  // if measurement is Q-factor
        {
          cMeasType = QF;
          ptr_fSigMerit = &fQ_fac;  // set the pointer to Q-factor
        }
        else  // otherwise
        {
          cMeasType = Ps;
          ptr_fSigMerit = &fQ_fac;  // set the pointer to signal power
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Measurement mode = " << CPRN(cMeasType) << std::endl;
        #endif
      }

      // Get measurement type
      std::string get_MeasType(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Measurement mode = " << CPRN(cMeasType) << std::endl;
        #endif

        switch(cMeasType)
        {
          case Ps:
            return strPs;
          case Pn:
            return strPn;
          case SNR:
            return strSNR;
          case QF:
            return strQF;
          default:
            return strPs;
        }
        return strPs;
      }

      // Set window range to find edge
      void set_EdgeFindingRange(int edgeFindingRange)
      {
        iEdgeFindingRange = CONSTRAIN(edgeFindingRange, 2, INT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Window range to find edge = " << iEdgeFindingRange << std::endl;
        #endif
      }

      // Get window range to find edge
      int get_EdgeFindingRange(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Signal_Quality_Metre_impl: Window range to find edge = " << iEdgeFindingRange << std::endl;
        #endif
        return iEdgeFindingRange;
      }

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_IMPL_H */

