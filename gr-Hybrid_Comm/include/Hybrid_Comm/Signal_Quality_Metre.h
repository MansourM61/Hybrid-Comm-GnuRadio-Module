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

#ifndef INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_H
#define INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Signal Quality Estimator
     * \ingroup Hybrid_Comm
     * \brief The block assesses the incoming signal over the given sample window.
     * The merits of the signal can be:
     * 1- Signal power
     * 2- Noise power
     * 3- Signal-to-noise ratio (SNR)
     * 4- Q-factor
     */
    class HYBRID_COMM_API Signal_Quality_Metre : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Signal_Quality_Metre> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Signal_Quality_Metre.
       *
       * \param packetSize output packet size
       * \param sampsPerSymb header samples per bit
       * \param measType measurement type; 'Signal Power', 'Noise Power', 'Signal-to-noise Ratio (SNR)', 'Q-factor'
       * \param edgeFindingRange averaging window size for finding edge algorithm
       */
      static sptr make(int packetSize, int sampsPerSymb, std::string  measType, int edgeFindingRange);

      /*!
       * \brief Set samples per symbol
       * 
       * \param sampPerSymb 
       * samples per symbol
       */
      virtual void set_SampsPerSymb(int sampPerSymb) = 0;

      /*!
       * \brief Return samples per symbol
       */
      virtual int get_SampsPerSymb(void) = 0;

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
       * \brief Set measurement type
       * 
       * \param measType
       * measurement type
       * 0 = Signal power
       * 1 = Noise power
       * 2 = Signal-to-noise ratio (SNR)
       * 3 = Q-factor
       */
      virtual void set_MeasType(std::string  measType) = 0;

      /*!
       * \brief Return measurement type
       * 0 = Signal power
       * 1 = Noise power
       * 2 = Signal-to-noise ratio (SNR)
       * 3 = Q-factor
       */
      virtual std::string get_MeasType(void) = 0;

      /*!
       * \brief Set window range to find edge
       * 
       * \param edgeFindingRange
       * 
       */
      virtual void set_EdgeFindingRange(int edgeFindingRange) = 0;

      /*!
       * \brief Return window range to find edge
       * 
       */
      virtual int get_EdgeFindingRange(void) = 0;
      
    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SIGNAL_QUALITY_METRE_H */

