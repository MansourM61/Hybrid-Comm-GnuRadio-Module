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

#ifndef INCLUDED_HYBRID_COMM_TX_HARD_SWITCH_H
#define INCLUDED_HYBRID_COMM_TX_HARD_SWITCH_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Tx Switch Module for Hard Selection
     * \ingroup Hybrid_Comm
     * \brief The block is used to act as a switch at transmitter with feature of hard selection from two inputs.
     */
    class HYBRID_COMM_API Tx_Hard_Switch : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Tx_Hard_Switch> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Tx_Hard_Switch.
       *
       * \param packetSize output packet size
       * \param thresh switching threshold value
       * \param sampPerBit header samples per bit
       */
      static sptr make(int packetSize, float thresh, const std::vector<char>& sampPerBit);

      /*!
       * \brief Set packet size
       * 
       * \param packetSize
       * Packet size
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return packet size
       */
      virtual int get_PacketSize() = 0;

      /*!
       * \brief Set threshold
       * 
       * \param threshold
       * threshold
       */
      virtual void set_Thresh(float threshold) = 0;

      /*!
       * \brief Return threshold
       */
      virtual float get_Thresh(void) = 0;

      /*!
       * \brief Set samples per bit
       * 
       * \param SpB 
       * samples per bit
       */
      virtual void set_SampsPerBit(const std::vector<char>& SpB) = 0;

      /*!
       * \brief Return samples per bit
       */
      virtual void get_SampsPerBit(std::vector<char>* SpB) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_TX_HARD_SWITCH_H */

