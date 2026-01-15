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

#ifndef INCLUDED_HYBRID_COMM_STREAM_ALIGNER_H
#define INCLUDED_HYBRID_COMM_STREAM_ALIGNER_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Stream Aligner
     * \ingroup Hybrid_Comm
     *   The blocks aligns two input streams based on the sync pulses.
     * It also checks the counter input of each stream and makes sure the output streams are from the same packet.
     * If the sync pulses are not aligned, 'delay' will be equal to misalignment value and both outputs are set to -1.
     * If the sync pulses indices have non-zero offset, 'delay' will be equal to offset value and both outputs are set to -2.
     * If the counter values difference is larger than available buffer bank, 'delay' will be equal to counter difference value and both outputs are set to -3.
     * If the block is filling the buffer, 'delay' will be equal to counter difference value and both outputs are set to -4.
     * If the sync pulses are aligned, zero-offset with counter difference within buffer bank capacity,
     * the output 'delay' is the difference between counter values of input stream and the outputs are aligned signals.
     */
    class HYBRID_COMM_API Stream_Aligner : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<Stream_Aligner> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Stream_Aligner.
       *
       * \param packetSize output packet size
       */
      static sptr make(int packetSize);

      /*!
       * \brief Set packet size
       * 
       * \param packetSize
       * packet size
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return packet size
       */
      virtual int get_PacketSize(void) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_STREAM_ALIGNER_H */

