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

#ifndef INCLUDED_HYBRID_COMM_STEP_GATE_H
#define INCLUDED_HYBRID_COMM_STEP_GATE_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Step Gate
     * \ingroup Hybrid_Comm
     * The block maps the difference between two input signals to a piece-wise step value.
     * If the inputs are 'Sig 1' and 'Sig 2', then output = f(Sig 1 - Sig 2).
     * The relation between the output and 'Sig 1 - Sig 2' is based on piece-wise step function.
     */
    class HYBRID_COMM_API Step_Gate : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Step_Gate> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Step_Gate.
       *
       * \param packetSize output packet size
       * \param levels output levels array
       * \param hopPoints hopping points array
       */
      static sptr make(int packetSize, const std::vector<float>& levels, const std::vector<float>& hopPoints);

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
       * \brief Set levels values
       * 
       * \param levels
       * levels values
       */
      virtual void set_Levels(const std::vector<float>& levels) = 0;

      /*!
       * \brief Return levels values
       */
      virtual void get_Levels(std::vector<float>* levels) = 0;

      /*!
       * \brief Set hopping points
       * 
       * \param hopPoints
       * hopping points
       */
      virtual void set_HopPoints(const std::vector<float>& hopPoints) = 0;

      /*!
       * \brief Return hopping points
       */
      virtual void get_HopPoints(std::vector<float>* hopPoints) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_STEP_GATE_H */

