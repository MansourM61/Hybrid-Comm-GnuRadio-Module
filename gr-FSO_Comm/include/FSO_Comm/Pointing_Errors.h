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

#ifndef INCLUDED_FSO_COMM_POINTING_ERRORS_H
#define INCLUDED_FSO_COMM_POINTING_ERRORS_H

#include <FSO_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace FSO_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup FSO_Comm
     *
     */
    class FSO_COMM_API Pointing_Errors : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Pointing_Errors> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of FSO_Comm::Pointing_Errors.
       *
       * To avoid accidental use of raw pointers, FSO_Comm::Pointing_Errors's
       * constructor is in a private implementation
       * class. FSO_Comm::Pointing_Errors::make is the public interface for
       * creating new instances.
       */
      static sptr make(float jitter, float diaTx, float thetaTx, float diaRx, float linkLen, float tempCorr, float sampRate);

      /*!
       * \brief Set pointing errors jitter
       *
       * \param jitter vibration displacement (mm)
       */
      virtual void set_Jitter(float jitter) = 0;
      /*!
       * \brief Return current pointing errors jitter
       */
      virtual float get_Jitter(void) = 0;

      /*!
       * \brief Set pointing errors link length
       *
       * \param linkLen FSO channel link length (m)
       */
      virtual void set_LinkLen(float linkLen) = 0;
      /*!
       * \brief Return current pointing errors link length
       */
      virtual float get_LinkLen(void) = 0;

      /*!
       * \brief Set pointing errors transmitter aperture diameter
       *
       * \param diaTx transmitter aperture diameter (mm)
       */
      virtual void set_DiaTx(float diaTx) = 0;
      /*!
       * \brief Return current pointing errors transmitter aperture diameter
       */
      virtual float get_DiaTx(void) = 0;

      /*!
       * \brief Set pointing errors transmitter beam divergence
       *
       * \param thetaTx transmitter beam half-angle divergence angle (mDeg)
       */
      virtual void set_ThetaTx(float thetaTx) = 0;
      /*!
       * \brief Return current pointing errors transmitter beam divergence
       */
      virtual float get_ThetaTx(void) = 0;

      /*!
       * \brief Set pointing errors receiver aperture diameter
       *
       * \param diaRx receiver aperture diameter (mm)
       */
      virtual void set_DiaRx(float diaRx) = 0;
      /*!
       * \brief Return current pointing errors receiver aperture diameter
       */
      virtual float get_DiaRx(void) = 0;

      /*!
       * \brief Set pointing errors temporal correlation
       *
       * \param tempCorr temporal correlation of pointing errors channel (s)
       */
      virtual void set_TempCorr(float tempCorr) = 0;
      /*!
       * \brief Return current pointing errors temporal correlation
       */
      virtual float get_TempCorr(void) = 0;

      /*!
       * \brief Set pointing errors sampling rate
       *
       * \param sampRate sampling rate of signal (samples per second)
       */
      virtual void set_SampRate(float sampRate) = 0;
      /*!
       * \brief Return current pointing errors sampling rate
       */
      virtual float get_SampRate(void) = 0;

    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_POINTING_ERRORS_H */

