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

#ifndef INCLUDED_HYBRID_COMM_LINK_TESTER_H
#define INCLUDED_HYBRID_COMM_LINK_TESTER_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Link Tester
     * \ingroup Hybrid_Comm
     *   The blocks calculate the link merits based on two incoming bit streams, one directly from source and the other recovered from received signal.
     * The block does the calculattion based on availability of bit streams according to the control 'ctl' pin and the number samples per bit from 'SpB' pin.
     * The outputs are bit-error-rate (BER), link availability (LA), and link throughout (LT).
     * if 'ctl' value is less than 250, it is assumed to be links delay. Values greater than 250 represent error.
     */
    class HYBRID_COMM_API Link_Tester : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Link_Tester> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Link_Tester.
       *
       * \param packetSize incoming packet size
       * \param samp_rate sample rate (samples per seconds)
       */
      static sptr make(int packetSize, float samp_rate);

      /*!
       * \brief Set packet data samples length
       * 
       * \param packetSize
       * packet data samples length
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return packet data samples length
       */
      virtual int get_PacketSize(void) = 0;

      /*!
       * \brief Set sample rate
       * 
       * \param samp_rate
       * sample rate
       */
      virtual void set_SampRate(float samp_rate) = 0;

      /*!
       * \brief Return sample rate
       */
      virtual float get_SampRate(void) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_LINK_TESTER_H */

