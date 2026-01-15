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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Stream_Aligner_impl.h"

namespace gr {
  namespace Hybrid_Comm {

    Stream_Aligner::sptr
    Stream_Aligner::make(int packetSize)
    {
      return gnuradio::get_initial_sptr
        (new Stream_Aligner_impl(packetSize));
    }

    const std::vector<int> Stream_Aligner_impl::iov = {sizeof(char), sizeof(char), sizeof(int), sizeof(char), sizeof(char), sizeof(int)};  // io signature
    const int Stream_Aligner_impl::iMaxInBufCoeff = 2;  // maximum input items buffer coefficient; this code is only writen for upto 2 packets


    /*
     * The private constructor
     */
    Stream_Aligner_impl::Stream_Aligner_impl(int packetSize)
      : gr::sync_block("Stream Aligner",
              gr::io_signature::makev(6, 6, iov),
              gr::io_signature::make(2, 3, sizeof(char))), Buffer(packetSize), bBufStored(false)
    {
      #ifdef _FLOW_MODE_
      iFlowCounter = 0;
      #endif

      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: Constructor called." << std::endl;
      #endif
      set_PacketSize(packetSize);  // set packet size

      #ifdef _FLOW_MODE_
      std::cout << "Stream_Aligner_impl: Packet size = " << iPacketSize << std::endl;
      #endif
    }

    /*
     * Our virtual destructor.
     */
    Stream_Aligner_impl::~Stream_Aligner_impl()
    {
    }

    int
    Stream_Aligner_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      #ifdef _FLOW_MODE_
      std::cout << "Stream_Aligner_impl: Entry: Counter = " << iFlowCounter << std::endl;
      #endif

      const char *data_1 = (const char *) input_items[0];
      const char *sync_1 = (const char *) input_items[1];
      const int *counter_1 = (const int *) input_items[2];
      const char *data_2 = (const char *) input_items[3];
      const char *sync_2 = (const char *) input_items[4];
      const int *counter_2 = (const int *) input_items[5];

      char *stream_1 = (char *) output_items[0];
      char *stream_2 = (char *) output_items[1];
      char *ctl = (char *) output_items[2];

      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: Work called." << std::endl;
      std::cout << "Stream_Aligner_impl: Available total number of output samples = " << noutput_items << std::endl;
      std::cout << "Stream_Aligner_impl: Packet size = " << iPacketSize << std::endl;
      #endif

      FillArray<char>(ctl, INV_SIG_VAL, noutput_items);  // fill output with misalignment


      // Do <+signal processing+>
      #ifdef _DEBUG_MODE_
      std::cout << "----------------------------------------------------" << std::endl;
      #endif
      int index_s_1 = 0;  // first sync pulse index for data stream 1
      for(int index = 0; index < iPacketSize; ++index)  // go through first iPacketSize item from data_1
      {
        if(sync_1[index] == 1)  // if a sync pulse is found
        {
          index_s_1 = index;  // update first sync pulse index
          break;
        }
      }
      int c_1 = counter_1[index_s_1];  // first counter for data stream 1
      int n_v_p_1 = (index_s_1 == 0) ? int(noutput_items/iPacketSize) : int(noutput_items/iPacketSize) - 1;  // number of valid packets
      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: First sync pulse index for data stream 1  = " << index_s_1 << std::endl;
      std::cout << "Stream_Aligner_impl: First counter value for data stream 1  = " << c_1 << std::endl;
      std::cout << "Stream_Aligner_impl: Number of valid packets for data stream 1  = " << n_v_p_1 << std::endl;
      #endif

      int index_s_2 = 0;  // first sync pulse index for data stream 2
      for(int index = 0; index < iPacketSize; ++index)  // go through first iPacketSize item from data_2
      {
        if(sync_2[index] == 1)  // if a sync pulse is found
        {
          index_s_2 = index;  // update first sync pulse index
          break;
        }
      }
      int c_2 = counter_2[index_s_2];  // first counter for data stream 2
      int n_v_p_2 = (index_s_2 == 0) ? int(noutput_items/iPacketSize) : int(noutput_items/iPacketSize) - 1;  // number of valid packets
      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: First sync pulse index for data stream 2  = " << index_s_2 << std::endl;
      std::cout << "Stream_Aligner_impl: First counter value for data stream 2  = " << c_2 << std::endl;
      std::cout << "Stream_Aligner_impl: Number of valid packets for data stream 2  = " << n_v_p_2 << std::endl;
      #endif

      char streams_delay = c_1 - c_2;  // calculate delay
      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: Streams delay  = " << streams_delay << std::endl;
      #endif

      int iNumOfProdOutput = noutput_items;  // number of produced outputs

      if(index_s_1 != index_s_2)  // if the sync pulses are not aligned
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Stream_Aligner_impl: Sync pulses are misaligned." << std::endl;
        #endif

        FillArray<char>(stream_1, PULSE_MISAL_ERR, noutput_items);  // fill output with -1 as error
        FillArray<char>(stream_2, PULSE_MISAL_ERR, noutput_items);  // fill output with -1 as error
        FillArray<char>(ctl, index_s_1 - index_s_2, noutput_items);  // fill output with misalignment
        iNumOfProdOutput = noutput_items;  // update number of produced outputs

        if(bBufStored == true)  // if there is stored buffer
        {
          Buffer.clear();  // clear the buffer
          bBufStored = false;  // set the buffer state
        }
      }    
      else  // otherwise; the sync pulses are aligned
      {
        #ifdef _DEBUG_MODE_
        std::cout << "Stream_Aligner_impl: Sync pulses are aligned." << std::endl;
        #endif

        if(index_s_1 != 0)  // if sync pulses are not at zero index
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Stream_Aligner_impl: Sync pulses are not at index zero." << std::endl;
          #endif

          FillArray<char>(stream_1, NONZERO_IND_ERR, index_s_1);  // fill output with -2 as error
          FillArray<char>(stream_2, NONZERO_IND_ERR, index_s_1);  // fill output with -2 as error
          FillArray<char>(ctl, index_s_1, index_s_1);  // fill output with offset
          iNumOfProdOutput = index_s_1;  // update number of produced outputs

          if(bBufStored == true)  // if there is stored buffer
          {
            Buffer.clear();  // clear the buffer
            bBufStored = false;  // set the buffer state
          }
        }
        else  // otherwise; sync pulses are at zero index
        {
          #ifdef _DEBUG_MODE_
          std::cout << "Stream_Aligner_impl: Sync pulses are at index zero." << std::endl;
          #endif

          if(abs(streams_delay) > Buffer.BankSize)  // if delay is longer than available buffer
          {
            #ifdef _DEBUG_MODE_
            std::cout << "Stream_Aligner_impl: Delay period is longer than available buffer bank." << std::endl;
            #endif
            FillArray<char>(stream_1, LONG_DELAY_ERR, noutput_items);  // fill output with -3 as error
            FillArray<char>(stream_2, LONG_DELAY_ERR, noutput_items);  // fill output with -3 as error
            FillArray<char>(ctl, streams_delay, noutput_items);  // fill output with delay
            iNumOfProdOutput = noutput_items;  // update number of produced outputs

            if(bBufStored == true)  // if there is stored buffer
            {
              Buffer.clear();  // clear the buffer
              bBufStored = false;  // set the buffer state
            }
          }
          else if(streams_delay == 0)  // if there is no delay
          {
            #ifdef _DEBUG_MODE_
            std::cout << "Stream_Aligner_impl: Signals are synced." << std::endl;
            #endif

            CopyArrays<char>(data_1, stream_1, noutput_items);  // fill output with input
            CopyArrays<char>(data_2, stream_2, noutput_items);  // fill output with input
            FillArray<char>(ctl, streams_delay, noutput_items);  // fill output with delay
            iNumOfProdOutput = noutput_items;  // update number of produced outputs

            if(bBufStored == true)  // if there is stored buffer
            {
              Buffer.clear();  // clear the buffer
              bBufStored = false;  // set the buffer state
            }
          }          
          else  // otherwise; if (1: stream_1 is ahead of stream_2) or (2: stream_2 is ahead of stream_3)
          {

            const char* input_lead;  // lead input signal
            const char* input_lag;  // lag signal

            char* output_lead;  // lead output signal
            char* output_lag;  // lag output signal

            char id_lead;  // lead signal id
            char id_lag;  // lag signal ids

            int index_i_lead = 0;  // index of input lead stream
            int index_i_lag = 0;  // index of input lag stream

            int index_o_lead = 0;  // index of output lead stream
            int index_o_lag = 0;  // index of output lag stream

            if(streams_delay > 0)  // 1: stream_1 is ahead of stream_2; stream_1 is lead; stream_2 is lag
            {
              input_lead = data_1;  // set input lead array pointer
              output_lead = stream_1;  // set output lead array pointer
              id_lead = '1';  // set lead signal id

              input_lag = data_2;  // set input lag array pointer
              output_lag = stream_2;  // set output lag array pointer
              id_lag = '2';  // set lag signal id
            }
            else  // otherwise; 2: stream_2 is ahead of stream_1 stream_2 is lead; stream_1 is lag
            {
              input_lead = data_2;  // set input lead array pointer
              output_lead = stream_2;  // set output lead array pointer
              id_lead = '2';  // set lead signal id

              input_lag = data_1;  // set input lag array pointer
              output_lag = stream_1;  // set output lag array pointer
              id_lag = '1';  // set lag signal id

              streams_delay *= -1;  // make it positive
            }

            bBufStored = (streams_delay == Buffer.get_TakenSlots()) ? true : false;  // set the flag to stored status

            #ifdef _DEBUG_MODE_
            std::cout << "Stream_Aligner_impl: Stream " << id_lead << " is ahead of stream " << id_lag << std::endl;
            #endif

            if(streams_delay < n_v_p_1)  // if the delayed signal is within the input stream; note that n_v_p_1 == n_v_p_2
            {
              #ifdef _DEBUG_MODE_
              std::cout << "Stream_Aligner_impl: Delayed stream is within the input data." << std::endl;
              #endif

              if(bBufStored == true)  // if there is stored buffer
              {
                #ifdef _DEBUG_MODE_
                std::cout << "Stream_Aligner_impl: Stored buffer is available." << std::endl;
                #endif

                // stream lead
                for(int index_p = 0; index_p < streams_delay; ++index_p)  // go through stored input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is recovered from buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " output index = " << index_o_lead << std::endl;
                  #endif
                  Buffer.pop((output_lead + index_o_lead));  // put the previously saved packet to output
                  index_o_lead += iPacketSize;  // update output index
                }

                for(int index_p = 0; index_p < n_v_p_1 - streams_delay; ++index_p)  // go through available input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is streamed from input." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " output index = " << index_o_lead << std::endl;
                  #endif
                  CopyArrays<char>((input_lead + index_i_lead), (output_lead + index_o_lead), iPacketSize);  // fill output with input
                  index_i_lead += iPacketSize;  // update input index
                  index_o_lead += iPacketSize;  // update output index
                }

                for(int index_p = 0; index_p < streams_delay; ++index_p)  // go through extra input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is stored into buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  #endif
                  Buffer.push((input_lead + index_i_lead));  // put the extra input packet into buffer
                  index_i_lead += iPacketSize;  // update output index
                }

                // stream lag
                CopyArrays<char>(input_lag, output_lag, noutput_items);  // fill output with input
                FillArray<char>(ctl, streams_delay, noutput_items);  // fill output with delay
                iNumOfProdOutput = noutput_items;  // update number of produced outputs
              }
              else  // if there is no stored buffer
              {
                #ifdef _DEBUG_MODE_
                std::cout << "Stream_Aligner_impl: Stored buffer is not available." << std::endl;
                #endif

                Buffer.clear();  // clear the buffer
                
                // fill the unavailable section with 'filling buffer' status
                FillArray<char>(output_lead, FILLING_BUF_ERR, iPacketSize*streams_delay);  // fill output with FILLING_BUF_ERR as error
                FillArray<char>(output_lag, FILLING_BUF_ERR, iPacketSize*streams_delay);  // fill output with FILLING_BUF_ERR as error

                index_o_lead += iPacketSize*streams_delay;  // update output index
                index_i_lag += iPacketSize*streams_delay;  // update input index
                index_o_lag += iPacketSize*streams_delay;  // update output index

                // stream lead
                for(int index_p = 0; index_p < n_v_p_1 - streams_delay; ++index_p)  // go through available input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is streamed from input." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " output index = " << index_o_lead << std::endl;
                  #endif
                  CopyArrays<char>((input_lead + index_i_lead), (output_lead + index_o_lead), iPacketSize);  // fill output with input
                  index_i_lead += iPacketSize;  // update input index
                  index_o_lead += iPacketSize;  // update output index
                }

                for(int index_p = 0; index_p < streams_delay; ++index_p)  // go through extra input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is stored into buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  #endif
                  Buffer.push((input_lead + index_i_lead));  // put the extra input packet into buffer
                  index_i_lead += iPacketSize;  // update input index
                }

                // stream lag
                CopyArrays<char>((input_lag + index_i_lag), (output_lag + index_o_lag), (n_v_p_1 - streams_delay)*iPacketSize);  // fill output with input
                FillArray<char>(ctl, streams_delay, noutput_items);  // fill output with delay
                iNumOfProdOutput = noutput_items;  // update number of produced outputs
              }

            }
            else  // otherwise; if the delayed signal is not within the input stream
            {
              #ifdef _DEBUG_MODE_
              std::cout << "Stream_Aligner_impl: Delayed stream is not within the input data." << std::endl;
              #endif

              if(bBufStored == true)  // if there is stored buffer
              {
                #ifdef _DEBUG_MODE_
                std::cout << "Stream_Aligner_impl: Stored buffer is available." << std::endl;
                #endif

                // stream lead
                for(int index_p = 0; index_p < n_v_p_1; ++index_p)  // go through stored input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is recovered from buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " output index = " << index_o_lead << std::endl;
                  #endif
                  Buffer.pop((output_lead + index_o_lead));  // put the previously saved packet to output
                  index_o_lead += iPacketSize;  // update output index
                }

                for(int index_p = 0; index_p < n_v_p_1; ++index_p)  // go through all input packets
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is stored into buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  #endif
                  Buffer.push((input_lead + index_i_lead));  // put the extra input packet into buffer
                  index_i_lead += iPacketSize;  // update output index
                }

                // stream lag
                CopyArrays<char>(input_lag, output_lag, noutput_items);  // fill output with input
                iNumOfProdOutput = noutput_items;  // update number of produced outputs
              }
              else  // if there is no stored buffer
              {
                #ifdef _DEBUG_MODE_
                std::cout << "Stream_Aligner_impl: Stored buffer is not available." << std::endl;
                #endif

                int iNumOfConsumed = 0;  // number of consumed input packets

                for(iNumOfConsumed = 0; iNumOfConsumed < n_v_p_1;)  // go through all input packets and store them in the buffer till the delay can be compensated
                {
                  #ifdef _DEBUG_MODE_
                  std::cout << "Stream_Aligner_impl: Data is stored into buffer." << std::endl;
                  std::cout << "Stream_Aligner_impl: Stream " << id_lead << " input index = " << index_i_lead << std::endl;
                  #endif
                  Buffer.push((input_lead + index_i_lead));  // put the extra input packet into buffer
                  index_i_lead += iPacketSize;  // update output index

                  ++iNumOfConsumed;  // update number of consumed input packets

                  if(Buffer.get_TakenSlots() == streams_delay)  // if enough packets are stored
                  {
                    #ifdef _DEBUG_MODE_
                    std::cout << "Stream_Aligner_impl: Enough data is stored." << std::endl;
                    #endif
                    break;  // leave the loop
                  }
                }
                #ifdef _DEBUG_MODE_
                std::cout << "Stream_Aligner_impl: Number of consumed input packets = " << iNumOfConsumed << std::endl;
                #endif

                // fill the unavailable section with 'filling buffer' status
                FillArray<char>((output_lead + index_o_lead), FILLING_BUF_ERR, iNumOfConsumed*iPacketSize);  // fill output with -4 as error
                FillArray<char>((output_lag + index_o_lag), FILLING_BUF_ERR, iNumOfConsumed*iPacketSize);  // fill output with -4 as error
                iNumOfProdOutput = iNumOfConsumed*iPacketSize;  // update number of produced outputs
              }
            }
          }
        }
      }
      // Tell runtime system how many output items we produced.

      #ifdef _DEBUG_MODE_
      std::cout << "Stream_Aligner_impl: Consumed output = " << iNumOfProdOutput << std::endl;
      #endif

      #ifdef _FLOW_MODE_
      std::cout << "Stream_Aligner_impl: Exit: Counter = " << iFlowCounter++ << std::endl;
      #endif

      return iNumOfProdOutput;
    }

  } /* namespace Hybrid_Comm */
} /* namespace gr */

