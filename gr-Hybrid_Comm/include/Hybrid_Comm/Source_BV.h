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

#ifndef INCLUDED_HYBRID_COMM_SOURCE_BV_H
#define INCLUDED_HYBRID_COMM_SOURCE_BV_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Bit-variant Source
     * \ingroup Hybrid_Comm
     * \brief The block generates fixed length output containing given number of bits.
     *
     */
    class HYBRID_COMM_API Source_BV : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Source_BV> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Source_BV.
       *
       * \param packetSize output packet size
       * \param signalType output signal type; 'Constant' or 'Random'
       */
      static sptr make(int packetSize, std::string signalType);

      /*!
       * \brief Set measurement packet size
       * 
       * \param packetSize
       * packet size
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return measurement packet size
       */
      virtual int get_PacketSize(void) = 0;

      /*!
       * \brief Set signal type
       * 
       * \param signalType
       * 0 = constant
       * 1 = random
       */
      virtual void set_SignalType(std::string signalType) = 0;

      /*!
       * \brief Return signal type
       */
      virtual std::string get_SignalType(void) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SOURCE_BV_H */

