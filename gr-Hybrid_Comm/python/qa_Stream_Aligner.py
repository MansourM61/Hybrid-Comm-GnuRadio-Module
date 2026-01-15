#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 gr-Hybrid_Comm author.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import Hybrid_Comm_swig as Hybrid_Comm
import random
import numpy
import math
import time

class qa_Stream_Aligner(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):  # test 1
        N = 50
        PacketSize = 2
        Delay = 3
        MisAlign = 0
        Offset = 0
        LeadFlag = False

        NumOfPack = int(N/PacketSize)
        N = PacketSize*NumOfPack

        data_1 = numpy.array(range(0, N))
        data_1 = numpy.insert(data_1, 0, [0,]*Offset)
        data_2 = numpy.array(range(N, 2*N))
        data_2 = numpy.insert(data_2, 0, [0,]*Offset)

        sync_1 = numpy.zeros(N, dtype = numpy.byte)
        sync_1[MisAlign::PacketSize] = 1
        sync_1 = numpy.insert(sync_1, 0, [0,]*Offset)
        sync_2 = numpy.zeros(N, dtype = numpy.byte)
        sync_2[0::PacketSize] = 1
        sync_2 = numpy.insert(sync_2, 0, [0,]*Offset)

        dummy = numpy.zeros(N, dtype = numpy.int)
        dummy[::PacketSize] = range(Delay, NumOfPack + Delay)
        dummy = numpy.insert(dummy, 0, [0,]*MisAlign)
        if(MisAlign == 0):
            counter_1 = dummy
            pass
        else:
            counter_1 = dummy[0:-MisAlign]
            pass
        counter_1 = numpy.insert(counter_1, 0, [0,]*Offset)
        counter_2 = numpy.zeros(N, dtype = numpy.int)
        counter_2[::PacketSize] = range(0, NumOfPack)
        counter_2 = numpy.insert(counter_2, 0, [0,]*Offset)

        if (LeadFlag == True):
            src_data_1 = blocks.vector_source_b(data_1)
            src_sync_1 = blocks.vector_source_b(sync_1)
            src_counter_1 = blocks.vector_source_i(counter_1)

            src_data_2 = blocks.vector_source_b(data_2)
            src_sync_2 = blocks.vector_source_b(sync_2)
            src_counter_2 = blocks.vector_source_i(counter_2)

            Out_Exp_1 = numpy.ones(N, dtype = numpy.byte)*255
            Out_Exp_1[PacketSize*Delay:] = data_1[:-PacketSize*Delay]
            Out_Exp_2 = numpy.ones(N, dtype = numpy.byte)*255
            Out_Exp_2[PacketSize*Delay:] = data_2[PacketSize*Delay:]
            pass
        else:
            src_data_1 = blocks.vector_source_b(data_2)
            src_sync_1 = blocks.vector_source_b(sync_2)
            src_counter_1 = blocks.vector_source_i(counter_2)

            src_data_2 = blocks.vector_source_b(data_1)
            src_sync_2 = blocks.vector_source_b(sync_1)
            src_counter_2 = blocks.vector_source_i(counter_1)
            Out_Exp_1 = numpy.ones(N, dtype = numpy.byte)*255
            Out_Exp_1[PacketSize*Delay:] = data_2[PacketSize*Delay:]
            Out_Exp_2 = numpy.ones(N, dtype = numpy.byte)*255
            Out_Exp_2[PacketSize*Delay:] = data_1[:-PacketSize*Delay]
            pass

        testBlock = Hybrid_Comm.Stream_Aligner(PacketSize)
        dst_stream_1 = blocks.vector_sink_b()
        dst_stream_2 = blocks.vector_sink_b()
        dst_delay = blocks.vector_sink_b()
        self.tb.connect(src_data_1, (testBlock, 0))
        self.tb.connect(src_sync_1, (testBlock, 1))
        self.tb.connect(src_counter_1, (testBlock, 2))
        self.tb.connect(src_data_2, (testBlock, 3))
        self.tb.connect(src_sync_2, (testBlock, 4))
        self.tb.connect(src_counter_2, (testBlock, 5))
        self.tb.connect((testBlock, 0), dst_stream_1)
        self.tb.connect((testBlock, 1), dst_stream_2)
        self.tb.connect((testBlock, 2), dst_delay)
        
        # set up fg
        self.tb.run()
        # check data
        resBlock_stream_1 = dst_stream_1.data()
        resBlock_stream_2 = dst_stream_2.data()
        resBlock_delay = dst_delay.data()

        Res_1 = numpy.sum(Out_Exp_1 - numpy.array(resBlock_stream_1))
        Res_2 = numpy.sum(Out_Exp_2 - numpy.array(resBlock_stream_2))
        Res_3 = numpy.sum(numpy.array(Delay) - Delay)

        print()        
        print("***************************")
        print("Number of inputs = ", N + Offset)
        print("Packet size = ", PacketSize)
        print("Counter 1 to counter 2 packet delay = ", Delay)
        print("Sync 1 to sync 2 misalignment = ", MisAlign)
        print("Data 1    = ", data_1)
        print("Sync 1    = ", sync_1)
        print("Counter 1 = ", counter_1)
        print("Data 2    = ", data_2)
        print("Sync 2    = ", sync_2)
        print("Counter 2 = ", counter_2)
        print()
        print("Test 1:")
        print("Calculated stream 1 = ", resBlock_stream_1)
        print("Expected stream 1   = ", Out_Exp_1)
        print("Calculated stream 2 = ", resBlock_stream_2)
        print("Expected stream 2   = ", Out_Exp_2)
        print("Delay               = ", resBlock_delay)

        self.assertAlmostEqual(Res_1, 0)
        self.assertAlmostEqual(Res_2, 0)
        self.assertAlmostEqual(Res_3, 0)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Stream_Aligner)
