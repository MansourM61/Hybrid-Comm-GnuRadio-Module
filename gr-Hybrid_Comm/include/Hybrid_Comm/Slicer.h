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

#ifndef INCLUDED_HYBRID_COMM_SLICER_H
#define INCLUDED_HYBRID_COMM_SLICER_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup Hybrid_Comm
     *
     */
    class HYBRID_COMM_API Slicer : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Slicer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Slicer.
       *
       * \param packetSize output packet size
       * \param avgWinSize averaging window size
       * \param thresh switching threshold value
       */
      static sptr make(int packetSize, int avgWinSize, float thresh);

      /*!
       * \brief Set averaging window size
       * 
       * \param avgWinSize
       * averaging window size
       */
      virtual void set_AvgWinSize(int avgWinSize) = 0;

      /*!
       * \brief Return averaging window size
       */
      virtual int get_AvgWinSize(void) = 0;

      /*!
       * \brief Set incoming packet size
       * 
       * \param packetSize
       * incoming packet size
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return incoming packet size
       */
      virtual int get_PacketSize(void) = 0;

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

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_SLICER_H */

