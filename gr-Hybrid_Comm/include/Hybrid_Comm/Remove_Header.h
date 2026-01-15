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

#ifndef INCLUDED_HYBRID_COMM_REMOVE_HEADER_H
#define INCLUDED_HYBRID_COMM_REMOVE_HEADER_H

#include <Hybrid_Comm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace Hybrid_Comm {

    /*!
     * \brief Remove Header
     * \ingroup Hybrid_Comm
     * The blocks removes the header from the incoming data array. It recognises the preamble as well as the label and the counter of the header.
     * After removing the header the data is sent to th output while the read counter is also made available at the 'Counter' pin.
     * A synchronisation pulse at 'sync' pin is used to show the beginning of the data section.
     * if 'Counter' pin value is -1, then the output data is not valid.
     * The pattern will be like:  [Preamble, Label, Counter, Data]
     * Counter field has 8 bits length.
     */
    class HYBRID_COMM_API Remove_Header : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<Remove_Header> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Hybrid_Comm::Remove_Header.
       *
       * \param packetSize output packet size
       * \param preamble preamble array of '0's and '1's
       * \param label label array of characters
       * \param samplesPerBit header samples per bit
       */
      static sptr make(int packetSize, const std::vector<char>& preamble, const std::string& label, int samplesPerBit);

      /*!
       * \brief Set preamble
       * 
       * \param preamble
       * preamble
       */
      virtual void set_Preamble(const std::vector<char>& preamble) = 0;

      /*!
       * \brief Return preamble
       */
      virtual void get_Preamble(std::vector<char>* preamble) = 0;

      /*!
       * \brief Set packet label
       * 
       * \param label
       * packet label
       */
      virtual void set_Label(const std::string& label) = 0;

      /*!
       * \brief Return packet label
       */
      virtual void get_Label(std::string* label) = 0;

      /*!
       * \brief Set packet data samples length
       * 
       * \param packetSize
       * packet data samples length
       */
      virtual void set_PacketSize(int packetSize) = 0;

      /*!
       * \brief Return packet data samples length
       */
      virtual int get_PacketSize(void) = 0;

      /*!
       * \brief Set samples per bit
       * 
       * \param samplesPerBit
       * samples per bit
       */
      virtual void set_SamplesPerBit(int samplesPerBit) = 0;

      /*!
       * \brief Return samples per bit
       */
      virtual int get_SamplesPerBit(void) = 0;

    };

  } // namespace Hybrid_Comm
} // namespace gr

#endif /* INCLUDED_HYBRID_COMM_REMOVE_HEADER_H */

