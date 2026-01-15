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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "FSP_Loss_impl.h"

namespace gr {
  namespace RF_Comm {

    FSP_Loss::sptr
    FSP_Loss::make(float linkLen, float freq)
    {
      return gnuradio::get_initial_sptr
        (new FSP_Loss_impl(linkLen, freq));
    }


    /*
     * The private constructor
     */
    FSP_Loss_impl::FSP_Loss_impl(float linkLen, float freq)
      : gr::sync_block("FSP_Loss",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 2, sizeof(float))), fLoss(0)
    {
      #ifdef _DEBUG_MODE_
      std::cout << "FSP_Loss_impl: Constructor called." << std::endl;
      #endif
      this->set_LinkLen(linkLen);  // set link length
      this->set_Freq(freq);  // set frequency
    }


    void
    FSP_Loss_impl::CalcLoss(void)
    {
      fLoss = CONSTRAIN(POW2( C_0 / (4.0 * M_PI * fLinkLen * fFreq * 1.0e9) ), 0.0, 1.0);  // free space propagation loss

      #ifdef _DEBUG_MODE_
      std::cout << "FSP_Loss_impl: CalcLoss called." << std::endl;
      std::cout << "Loss = " << fLoss << std::endl;
      #endif
    }


    /*
     * Our virtual destructor.
     */
    FSP_Loss_impl::~FSP_Loss_impl()
    {
    }

    int
    FSP_Loss_impl::work(int noutput_items,
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
      std::cout << "FSP_Loss_impl: Work called." << std::endl;
      std::cout << "Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Link length = " << fLinkLen << " (m)" << std::endl;
      std::cout << "Frequency = " << fFreq << " (GHz)" << std::endl;
      std::cout << "FSP loss = " << fLoss << ", " << 10.0*log10(fLoss) << " dB" << std::endl;
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

  } /* namespace RF_Comm */
} /* namespace gr */

