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

#ifndef INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_H
#define INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Hysteresis Gate
     * \ingroup Hybrid_Comm
     * \brief The block sets the output to a value in range of [-1, +1] depending on the difference between two input signals.
     * If the inputs are 'Sig 1' and 'Sig 2', then output = f(Sig 1 - Sig 2).
     * The relation between the output and 'Sig 1 - Sig 2' is hysteresis.
     */
    class HYBRID_COMM_API Hysteresis_Gate : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Hysteresis_Gate> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Hysteresis_Gate.
       *
       * \param packetSize incoming packet size
       * \param forwardPoint hysteresis forward raising point
       * \param forwardSlope hysteresis forward raising slope
       * \param forwardState hysteresis forward raising saturation state
       * \param backwardPoint hysteresis backward raising point
       * \param backwardSlope hysteresis backward raising slope
       * \param backwardState hysteresis backward raising saturation state
       * \param mode hysteresis mode showing initial hysteresis state; 'Forward' or 'Backward' 
       */
      static sptr make(int packetSize, float forwardPoint, float forwardSlope, float forwardState, float backwardPoint, float backwardSlope, float backwardState, std::string mode);

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
       * \brief Set hysteresis forward raising point
       * 
       * \param forwardPoint 
       * hysteresis forward raising point
       */
      virtual void set_ForwardPoint(float forwardPoint) = 0;

      /*!
       * \brief Return hysteresis forward raising point
       */
      virtual float get_ForwardPoint(void) = 0;

      /*!
       * \brief Set hysteresis forward raising slope
       * 
       * \param forwardSlope 
       * hysteresis forward raising slope
       */
      virtual void set_ForwardSlope(float forwardSlope) = 0;

      /*!
       * \brief Return hysteresis forward raising slope
       */
      virtual float get_ForwardSlope(void) = 0;

      /*!
       * \brief Set hysteresis forward raising saturation state
       * 
       * \param forwardState 
       * hysteresis forward raising saturation state
       */
     virtual void set_ForwardState(float forwardState) = 0;

      /*!
       * \brief Return hysteresis forward raising saturation state
       */
      virtual float get_ForwardState(void) = 0;

      /*!
       * \brief Set hysteresis background falling point
       * 
       * \param backwardPoint 
       * hysteresis background falling point
       */
      virtual void set_BackwardPoint(float backwardPoint) = 0;

      /*!
       * \brief Return hysteresis background falling point
       */
      virtual float get_BackwardPoint(void) = 0;

      /*!
       * \brief Set hysteresis background falling slope
       * 
       * \param backwardSlope 
       * hysteresis background falling slope
       */
      virtual void set_BackwardSlope(float backwardSlope) = 0;

      /*!
       * \brief Return hysteresis background falling slope
       */
      virtual float get_BackwardSlope(void) = 0;

      /*!
       * \brief Set hysteresis background falling saturation state
       * 
       * \param backwardState 
       * hysteresis background falling saturation state
       */
      virtual void set_BackwardState(float backwardState) = 0;

      /*!
       * \brief Return hysteresis background falling saturation state
       */
      virtual float get_BackwardState(void) = 0;

      /*!
       * \brief Set hysteresis mode
       * 
       * \param mode 
       * hysteresis mode
       */
      virtual void set_Mode(std::string mode) = 0;

      /*!
       * \brief Return hysteresis mode
       */
      virtual std::string get_Mode(void) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_HYSTERESIS_GATE_H */

