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

#ifndef INCLUDED_FSO_COMM_CHANNEL_ANALYSER_H
#define INCLUDED_FSO_COMM_CHANNEL_ANALYSER_H

#include <FSO_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace FSO_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup FSO_Comm
     *
     */
    class FSO_COMM_API Channel_Analyser : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Channel_Analyser> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of FSO_Comm::Channel_Analyser.
       *
       * To avoid accidental use of raw pointers, FSO_Comm::Channel_Analyser's
       * constructor is in a private implementation
       * class. FSO_Comm::Channel_Analyser::make is the public interface for
       * creating new instances.
       */
      static sptr make(int sampsPerSymb, int winSize, std::string  measType, int edgeFindingRange);

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
       * \brief Set measurement window size
       * 
       * \param winSize
       * window size
       */
      virtual void set_WinSize(int winSize) = 0;

      /*!
       * \brief Return measurement window size
       */
      virtual int get_WinSize(void) = 0;

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

  } // namespace FSO_Comm
} // namespace gr

#endif /* INCLUDED_FSO_COMM_CHANNEL_ANALYSER_H */

