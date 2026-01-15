/* -*- c++ -*- */
/*
 * Copyright 2020 gr-RF_Comm author.
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

#ifndef INCLUDED_RF_COMM_RAIN_LOSS_H
#define INCLUDED_RF_COMM_RAIN_LOSS_H

#include <RF_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace RF_Comm {

    /*!
     * \brief <+description of block+>
     * \ingroup RF_Comm
     *
     */
    class RF_COMM_API Rain_Loss : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Rain_Loss> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of RF_Comm::Rain_Loss.
       *
       * To avoid accidental use of raw pointers, RF_Comm::Rain_Loss's
       * constructor is in a private implementation
       * class. RF_Comm::Rain_Loss::make is the public interface for
       * creating new instances.
       */
      static sptr make(float linkLen, float prep);

      /*!
       * \brief Set link length
       * 
       * \param linkLen
       * link length (m)
       */
      virtual void set_LinkLen(float linkLen) = 0;

      /*!
       * \brief Return link length (m)
       */
      virtual float get_LinkLen(void) = 0;

      /*!
       * \brief Set precipitation rate
       * 
       * \param prep
       * precipitation rate (mm/h)
       */
      virtual void set_Prep(float prep) = 0;

      /*!
       * \brief Return precipitation rate (mm/h)
       */
      virtual float get_Prep(void) = 0;

    };

  } // namespace RF_Comm
} // namespace gr

#endif /* INCLUDED_RF_COMM_RAIN_LOSS_H */

