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

#ifndef INCLUDED_FSO_COMM_TURBULENCE_H
#define INCLUDED_FSO_COMM_TURBULENCE_H

#include <FSO_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace FSO_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup FSO_Comm
     *
     */
    class FSO_COMM_API Turbulence : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Turbulence> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of FSO_Comm::Turbulence.
       *
       * To avoid accidental use of raw pointers, FSO_Comm::Turbulence's
       * constructor is in a private implementation
       * class. FSO_Comm::Turbulence::make is the public interface for
       * creating new instances.
       */
      static sptr make(float Cn2, float wavelength, float diaRx, float LinkLen, float tempCorr, float sampRate);

      /*!
       * \brief Set turbulence Cn2
       *
       * \param Cn2 refractive index structure constant (m^(-2/3))
       */
      virtual void set_Cn2(float Cn2) = 0;
      /*!
       * \brief Return current turbulence Cn2
       */
      virtual float get_Cn2(void) = 0;

      /*!
       * \brief Set turbulence wavelength
       *
       * \param wavelength optical beam wavelength (m)
       */
      virtual void set_Wavelength(float wavelength) = 0;
      /*!
       * \brief Return current turbulence wavelength
       */
      virtual float get_Wavelength(void) = 0;

      /*!
       * \brief Set turbulence link length
       *
       * \param linkLen FSO channel link length (m)
       */
      virtual void set_LinkLen(float linkLen) = 0;
      /*!
       * \brief Return current turbulence link length
       */
      virtual float get_LinkLen(void) = 0;

      /*!
       * \brief Set turbulence receiver apertture diameter
       *
       * \param diaRx receiver apertture diameter (m)
       */
      virtual void set_DiaRx(float diaRx) = 0;
      /*!
       * \brief Return current turbulence receiver aperture diameter
       */
      virtual float get_DiaRx(void) = 0;

      /*!
       * \brief Set turbulence temporal correlation
       *
       * \param tempCorr temppral correlation of turbulence channel (s)
       */
      virtual void set_TempCorr(float tempCorr) = 0;
      /*!
       * \brief Return current turbulence temporal correlation
       */
      virtual float get_TempCorr(void) = 0;

      /*!
       * \brief Set turbulence sampling rate
       *
       * \param sampRate sampling rate of signal (samples per second)
       */
      virtual void set_SampRate(float sampRate) = 0;
      /*!
       * \brief Return current turbulence sampling rate
       */
      virtual float get_SampRate(void) = 0;

    };

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_TURBULENCE_H */

