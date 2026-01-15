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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "FogSmoke_Loss_impl.h"

namespace gr {
  namespace FSO_Comm {

    FogSmoke_Loss::sptr
    FogSmoke_Loss::make(float linkLen, float vis, float wavelength)
    {
      return gnuradio::get_initial_sptr
        (new FogSmoke_Loss_impl(linkLen, vis, wavelength));
    }


    /*
     * The private constructor
     */
    FogSmoke_Loss_impl::FogSmoke_Loss_impl(float linkLen, float vis, float wavelength)
      : gr::sync_block("FogSmoke_Loss",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 2, sizeof(float))), fLoss(0)
    {
      #ifdef _DEBUG_MODE_
      std::cout << "FogSmoke_Loss_impl: Constructor called." << std::endl;
      #endif
      this->set_LinkLen(linkLen);  // set link length
      this->set_Visibility(vis);  // set visibility
      this->set_Wavelength(wavelength);  // set wavelength
    }

    void
    FogSmoke_Loss_impl::CalcLoss(void)
    {
      float q = 0;   // q parameter

      if(fVisibility > 50.0)  // select proper 'q' value based on Kim model
      {
        q = 1.6;
      }
      else if (fVisibility <= 50.0 && fVisibility > 6.0)
      {
        q = 1.3;
      }
      else if (fVisibility <= 6.0 && fVisibility > 1.0)
      {
        q = 0.67*fVisibility + 0.34;
      }
      else if (fVisibility <= 1.0 && fVisibility > 0.5)
      {
        q = fVisibility + 0.5;
      } 
      else
      {
        q = 0.0;
      }

      float fbeta_l = (-log(T_TH_FS)/fVisibility) * pow(fWavelength/LAMBDA_0, -q);  // calculate the attenuation coefficient

      fLoss = CONSTRAIN(exp(-fbeta_l*fLinkLen/1.0e3), 0.0, 1.0);  // calculate the loss coefficient

      #ifdef _DEBUG_MODE_
      std::cout << "FogSmoke_Loss_impl: CalcLoss called." << std::endl;
      std::cout << "beta = " << fbeta_l << std::endl;
      std::cout << "Loss = " << fLoss << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    FogSmoke_Loss_impl::~FogSmoke_Loss_impl()
    {
    }

    int
    FogSmoke_Loss_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      float *out_loss = nullptr;

      if(output_items.size() == 2)  // SpB output is connected
      {
        out_loss = (float *) output_items[1];
      }

      bool bLossConnected = ( (out_loss != nullptr) ) ? true : false;  // if loss is to be transferred

      #ifdef _DEBUG_MODE_
      std::cout << "FogSmoke_Loss_impl: Work called." << std::endl;
      std::cout << "Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
      std::cout << "Visibility = " << fVisibility << " (km)" << std::endl;
      std::cout << "Wavelength = " << fWavelength << " (nm)" << std::endl;
      std::cout << "Fog/smoke loss = " << fLoss << ", " << 10.0*log10(fLoss) << " dB" << std::endl;
      std::cout << "Loss pin is " << ((bLossConnected == true) ? "" : "not ") << "connected." << std::endl;
      #endif

      // Do <+signal processing+>
      LinearMap<float>(in, out, noutput_items, fLoss, 0.0);  // apply the loss
      if(bLossConnected == true)  // if SpB is to transferred
      {
        FillArray<float>(out_loss, fLoss, noutput_items);  // fill loss array
      }


      #ifdef _ARRAY_MODE_
      std::cout << "Input = ";
      DisplayArray<float>(in, noutput_items, 0);  // display input array
      std::cout << std::endl;
      std::cout << "Output = ";
      DisplayArray<float>(out, noutput_items, 0);  // display input array
      std::cout << std::endl;
      if(bLossConnected == true)  // if SpB is to transferred
      {
        std::cout << "Final loss = ";
        DisplayArray<float>(out_loss, noutput_items, 0);  // display input array
        std::cout << std::endl;
      }
      #endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace FSO_Comm */
} /* namespace gr */
