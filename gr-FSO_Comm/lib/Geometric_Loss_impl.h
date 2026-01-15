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

#ifndef INCLUDED_FSO_COMM_GEOMETRIC_LOSS_IMPL_H
#define INCLUDED_FSO_COMM_GEOMETRIC_LOSS_IMPL_H

#include <FSO_Comm/Geometric_Loss.h>

namespace gr {
  namespace FSO_Comm {

    class Geometric_Loss_impl : public Geometric_Loss
    {
     private:
      // Nothing to declare in this block.
      float fDiaTx;  // Tx aperture diameter (mm)
      float fThetaTx;  // Tx beam full-divergence angle (Deg)
      float fLinkLen;  // Rx aperture diameter (mm)
      float fDiaRx;  // Link length (m)
      float fLoss;  // fog/smoke loss

      void CalcLoss(void);  // calculate loss

     public:
      Geometric_Loss_impl(float diaTx = DEF_DIA_TX, float thetaTx = DEF_THETA_TX, float diaRx = DEF_DIA_RX, float linkLen = DEF_LINK_LEN);
      ~Geometric_Loss_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

      // Set Tx aperture diameter (mm)
      void set_DiaTx(float diaTx)
      {
        fDiaTx = CONSTRAIN(diaTx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Tx aperture diameter = " << fDiaTx << " (mm)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get Tx aperture diameter (mm)
      float get_DiaTx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Tx aperture diameter = " << fDiaTx << " (mm)" << std::endl;
        #endif

        return fDiaTx;
      }

      // Set transmitter half-angle beam divergence (Deg)
      void set_ThetaTx(float thetaTx)
      {
        fThetaTx = CONSTRAIN(thetaTx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Tx half-angle beam divergence = " << fThetaTx << " (Deg)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get transmitter half-angle beam divergence (Deg)
      float get_ThetaTx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Tx half-angle beam divergence = " << fThetaTx << " (Deg)" << std::endl;
        #endif

        return fThetaTx;
      }

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

      // Set Rx aperture diameter (mm)
      void set_DiaRx(float diaRx)
      {
        fDiaRx = CONSTRAIN(diaRx, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Rx aperture diameter = " << fDiaRx << " (mm)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get Rx aperture diameter (mm)
      float get_DiaRx(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Rx aperture diameter = " << fDiaRx << " (mm)" << std::endl;
        #endif

        return fDiaRx;
      }

    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_GEOMETRIC_LOSS_IMPL_H */

