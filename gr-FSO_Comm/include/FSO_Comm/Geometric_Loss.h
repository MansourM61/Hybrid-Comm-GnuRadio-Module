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

#ifndef INCLUDED_FSO_COMM_GEOMETRIC_LOSS_H
#define INCLUDED_FSO_COMM_GEOMETRIC_LOSS_H

#include <FSO_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace FSO_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup FSO_Comm
     *
     */
    class FSO_COMM_API Geometric_Loss : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Geometric_Loss> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of FSO_Comm::Geometric_Loss.
       *
       * To avoid accidental use of raw pointers, FSO_Comm::Geometric_Loss's
       * constructor is in a private implementation
       * class. FSO_Comm::Geometric_Loss::make is the public interface for
       * creating new instances.
       */
      static sptr make(float diaTx, float thetaTx, float diaRx, float linkLen);

      /*!
       * \brief Set geometric loss transmitter aperture diameter
       *
       * \param diaTx transmitter aperture diameter (m)
       */
      virtual void set_DiaTx(float diaTx) = 0;
      /*!
       * \brief Return current geometric loss transmitter aperture diameter
       */
      virtual float get_DiaTx(void) = 0;

      /*!
       * \brief Set geometric loss transmitter full-angle divergence angle
       *
       * \param thetaTx transmitter half-angle beam divergence (Deg)
       */
      virtual void set_ThetaTx(float thetaTx) = 0;
      /*!
       * \brief Return current geometric loss transmitter full-angle divergence angle (Deg)
       */
      virtual float get_ThetaTx(void) = 0;

      /*!
       * \brief Set geometric loss transmitter link length
       *
       * \param linkLen FSO channel link length (m)
       */
      virtual void set_LinkLen(float linkLen) = 0;
      /*!
       * \brief Return current geometric loss transmitter link length
       */
      virtual float get_LinkLen(void) = 0;

      /*!
       * \brief Set geometric loss receiver aperture diameter
       *
       * \param diaRx receiver aperture diameter (m)
       */
      virtual void set_DiaRx(float diaRx) = 0;
      /*!
       * \brief Return current geometric loss receiver aperture diameter
       */
      virtual float get_DiaRx(void) = 0;



    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_GEOMETRIC_LOSS_H */

