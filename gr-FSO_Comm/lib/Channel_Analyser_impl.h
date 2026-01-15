/* -*- c++ -*- */
/*
 * Copyright 2020 gr-FSO_Comm author.
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

#ifndef INCLUDED_FSO_COMM_CHANNEL_ANALYSER_IMPL_H
#define INCLUDED_FSO_COMM_CHANNEL_ANALYSER_IMPL_H

#include <FSO_Comm/Channel_Analyser.h>

namespace gr {
  namespace FSO_Comm {

    enum MeasType {Ps = 0, Pn = 1, SNR = 2, QF = 3, DC = 4, SI0 = 5, SI1 = 6, SIm = 7};    

    class Channel_Analyser_impl : public Channel_Analyser
    {
     private:
      // Nothing to declare in this block.
      // Nothing to declare in this block.
      int iSampsPerSymb;  // samples per symbol
      int iWinSize;  // measurement window size
      char cMeasType;  // measurement method
      int iEdgeFindingRange;  // window range to find edge
      float fSigPower;  // initial signal power
      float fNoisePower;  // initial noise power
      float fSNR;  // signal-to-noise ratio (SNR)
      float fQ_fac;  // Q-factor
      float fDC;  // DC value
      float fSI0;  // SI 0
      float fSI1;  // SI 1
      float fSIm;  // average SI
      const float *ptr_fSigMerit;  // signal quality merit

      static const std::string str_flags[8];
      static const char meas_types[8];
      const float *meas_val[8];

     public:
      Channel_Analyser_impl(int sampsPerSymb = DEF_SPB, int winSize = PACKET_SAMP_SIZE, std::string measType = str_flags[0], int edgeFindingRange = FIND_EDGE_WIN_SIZE);
      ~Channel_Analyser_impl();

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
        std::cout << "Sample per Symbol = " << iSampsPerSymb << std::endl;
        #endif
      }

      // Get samples per symbol
      int get_SampsPerSymb(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Sample per Symbol = " << iSampsPerSymb << std::endl;
        #endif
        return iSampsPerSymb;
      }

      // Set measurement window size
      void set_WinSize(int winSize)
      {
        iWinSize = CONSTRAIN(winSize, 1, INT_MAX);
        block::set_output_multiple(iWinSize);  // make sure the number of output array is a multiple of window size
        #ifdef _DEBUG_MODE_
        std::cout << "Window size = " << iWinSize << std::endl;
        #endif
      }

      // Get measurement window size
      int get_WinSize(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Window size = " << iWinSize << std::endl;
        #endif
        return iWinSize;
      }

      // Set measurement type
      void set_MeasType(std::string measType)
      {

        char index = 7;
        for( ;index > 0; --index)
        {
          if (measType == str_flags[index])
          {
            break;
          }
        }

        cMeasType = meas_types[index];  // set the pointer to measurement
        ptr_fSigMerit = meas_val[index];  // set the pointer to measurement value

        std::cout << "Index = " << CPRN(index) << std::endl;
        std::cout << "Index = " << *ptr_fSigMerit << std::endl;

        #ifdef _DEBUG_MODE_
        std::cout << "Measurement mode = " << CPRN(cMeasType) << std::endl;
        std::cout << "Measurement mode = " << str_flags[cMeasType] << std::endl;
        #endif
      }

      // Get measurement type
      std::string get_MeasType(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Measurement mode = " << CPRN(cMeasType) << std::endl;
        std::cout << "Measurement mode = " << str_flags[cMeasType] << std::endl;
        #endif
        
        return str_flags[cMeasType];
      }

      // Set window range to find edge
      void set_EdgeFindingRange(int edgeFindingRange)
      {
        iEdgeFindingRange = CONSTRAIN(edgeFindingRange, 2, INT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Window range to find edge = " << iEdgeFindingRange << std::endl;
        #endif
      }

      // Get window range to find edge
      int get_EdgeFindingRange(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Window range to find edge = " << iEdgeFindingRange << std::endl;
        #endif
        return iWinSize;
      }


    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_CHANNEL_ANALYSER_IMPL_H */

