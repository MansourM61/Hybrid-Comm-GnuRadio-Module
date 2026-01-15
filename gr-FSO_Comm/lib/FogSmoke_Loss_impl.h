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

#ifndef INCLUDED_FSO_COMM_FOGSMOKE_LOSS_IMPL_H
#define INCLUDED_FSO_COMM_FOGSMOKE_LOSS_IMPL_H

#include <FSO_Comm/FogSmoke_Loss.h>

namespace gr {
  namespace FSO_Comm {

    class FogSmoke_Loss_impl : public FogSmoke_Loss
    {
     private:
      // Nothing to declare in this block.
      float fLinkLen;  // link length (m)
      float fVisibility;  // visibility (km)
      float fWavelength;  // wavelength (nm)
      float fLoss;  // fog/smoke loss

      void CalcLoss(void);  // calculate loss

     public:
      FogSmoke_Loss_impl(float linkLen = DEF_LINK_LEN, float vis = DEF_VIS, float wavelength = DEF_WL);
      ~FogSmoke_Loss_impl();

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

      // Set visibility
      void set_Visibility(float vis)
      {
        fVisibility = CONSTRAIN(vis, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Visibility = " << fVisibility << " (km)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get visibility
      float get_Visibility(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Visibility = " << fVisibility << " (km)" << std::endl;
        #endif

        return fVisibility;
      }

      // Set wavelength
      void set_Wavelength(float wavelength)
      {
        fWavelength = CONSTRAIN(wavelength, FLT_MIN, FLT_MAX);
        #ifdef _DEBUG_MODE_
        std::cout << "Wavelength = " << fWavelength << " (nm)" << std::endl;
        #endif

        CalcLoss();
      }

      // Get wavelength
      float get_Wavelength(void)
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Wavelength = " << fWavelength << " (nm)" << std::endl;
        #endif

        return fWavelength;
      }

    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_FOGSMOKE_LOSS_IMPL_H */

