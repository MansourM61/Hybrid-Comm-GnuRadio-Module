/* -*- c++ -*- */
/*
 * Copyright 2020 gr-RF_Comm author.
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

#ifndef INCLUDED_RF_COMM_FSP_LOSS_IMPL_H
#define INCLUDED_RF_COMM_FSP_LOSS_IMPL_H

#include <RF_Comm/FSP_Loss.h>

namespace gr {
  namespace RF_Comm {

    class FSP_Loss_impl : public FSP_Loss
    {
     private:
      // Nothing to declare in this block.
      float fLinkLen;  // link length (m)
      float fFreq;  // frequency (GHz)
      float fLoss;  // fog/smoke loss

      void CalcLoss(void);  // calculate loss

     public:
      FSP_Loss_impl(float linkLen = DEF_LINK_LEN, float freq = DEF_FREQ);
      ~FSP_Loss_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set link length
      void set_LinkLen(float linkLen)
      {
        fLinkLen = CONSTRAIN(linkLen, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get link length
      float get_LinkLen(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
        #endif

        return fLinkLen;
      }

      // Set frequenct
      void set_Freq(float freq)
      {
        fFreq = CONSTRAIN(freq, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Frequency = " << fFreq << " (GHz)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get frequenct
      float get_Freq(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Frequency = " << fFreq << " (GHz)" << std::endl;
        #endif

        return fFreq;
      }




    };

  } // namespace RF_Comm
} // namespace gr

#endif /* INCLUDED_RF_COMM_FSP_LOSS_IMPL_H */

