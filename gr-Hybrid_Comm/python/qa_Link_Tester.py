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

class qa_Link_Tester(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None


    def test_001_t(self):  # test: BER 1
        NumOfBits = 1000
        SampPerBit = 5
        PacketSize = 500
        samp_rate = 1e3

        Bits_1 = numpy.random.randint(0, 2, NumOfBits, dtype=numpy.byte)
        Packet_1 = self.rectpulse(Bits_1, SampPerBit)
        link_1 = Packet_1.tolist()

        Bits_2 = numpy.random.randint(0, 2, NumOfBits, dtype=numpy.byte)
        Packet_2 = self.rectpulse(Bits_2, SampPerBit)
        link_2 = Packet_2.tolist()

        ctl_sig = [1]*NumOfBits*SampPerBit
        spb_sig = [SampPerBit]*NumOfBits*SampPerBit

        l1 = blocks.vector_source_b(link_1)
        l2 = blocks.vector_source_b(link_2)
        ctl = blocks.vector_source_b(ctl_sig)
        sbp = blocks.vector_source_b(spb_sig)
        testBlock = Hybrid_Comm.Link_Tester(PacketSize, samp_rate)
        dst_BER = blocks.vector_sink_f()
        dst_LA = blocks.vector_sink_f()
        dst_LT = blocks.vector_sink_f()
        self.tb.connect(l1, (testBlock, 0))
        self.tb.connect(l2, (testBlock, 1))
        self.tb.connect(ctl, (testBlock, 2))
        self.tb.connect(sbp, (testBlock, 3))
        self.tb.connect((testBlock, 0), dst_BER)
        self.tb.connect((testBlock, 1), dst_LA)
        self.tb.connect((testBlock, 2), dst_LT)

        # set up fg
        self.tb.run()
        # check data
        resBlock_BER = dst_BER.data()
        resBlock_LA = dst_LA.data()
        resBlock_LT = dst_LT.data()

        BER = resBlock_BER[-1]

        Exp_BER = numpy.sum( numpy.abs( numpy.array(Bits_1) - numpy.array(Bits_2) ) ) / NumOfBits

        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Packet size = ", PacketSize)
        print("Sample rate = ", samp_rate)
        print()        
        print("Test BER 1:")
        print("Expected BER = ", Exp_BER)
        print("Calculated BER = ", BER)

        self.assertAlmostEqual(BER, Exp_BER, 3)


    def test_002_t(self):  # test: BER 2
        NumOfBits = 100
        SampPerBit = 5
        PacketSize = 10
        NoE = 5
        samp_rate = 1e3

        link_1 = numpy.array([1]*NumOfBits*SampPerBit)
        link_1 = numpy.insert(link_1, 0, [0]*NoE*SampPerBit)

        link_2 = numpy.array([1]*NumOfBits*SampPerBit)
        link_2 = numpy.insert(link_2, 0, [1]*NoE*SampPerBit)

        ctl_sig = [1]*(NumOfBits + NoE)*SampPerBit
        spb_sig = [SampPerBit]*(NumOfBits + NoE)*SampPerBit

        l1 = blocks.vector_source_b(link_1)
        l2 = blocks.vector_source_b(link_2)
        ctl = blocks.vector_source_b(ctl_sig)
        sbp = blocks.vector_source_b(spb_sig)
        testBlock = Hybrid_Comm.Link_Tester(PacketSize, samp_rate)
        dst_BER = blocks.vector_sink_f()
        dst_LA = blocks.vector_sink_f()
        dst_LT = blocks.vector_sink_f()
        self.tb.connect(l1, (testBlock, 0))
        self.tb.connect(l2, (testBlock, 1))
        self.tb.connect(ctl, (testBlock, 2))
        self.tb.connect(sbp, (testBlock, 3))
        self.tb.connect((testBlock, 0), dst_BER)
        self.tb.connect((testBlock, 1), dst_LA)
        self.tb.connect((testBlock, 2), dst_LT)

        # set up fg
        self.tb.run()
        # check data
        resBlock_BER = dst_BER.data()
        resBlock_LA = dst_LA.data()
        resBlock_LT = dst_LT.data()

        BER = resBlock_BER[-1]

        Exp_BER = NoE/(NumOfBits + NoE)

        print(link_1)
        print(link_2)

        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Packet size = ", PacketSize)
        print("Sample rate = ", samp_rate)
        print()        
        print("Test BER 2:")
        print("Expected BER = ", Exp_BER)
        print("Calculated BER = ", BER)

        self.assertAlmostEqual(BER, Exp_BER, 3)


    def test_003_t(self):  # test: BER 3
        NumOfRepeatation = 10
        PacketSize = 5
        NumOfInval = 10
        SampPerBit = 5
        samp_rate = 1e3

        Dummy = numpy.random.randint(0, 2, PacketSize - 1, dtype = numpy.byte)
        Dummy_1 = numpy.insert(Dummy, 0, [0])
        Dummy_2 = numpy.insert(Dummy, 0, [1])

        Bits_1 = Bits_1_org = numpy.repeat(Dummy_1, NumOfRepeatation)
        Bits_1 = numpy.insert(Bits_1, 0, numpy.random.randint(0, 2, NumOfInval*PacketSize))
        Packet_1 = self.rectpulse(Bits_1, SampPerBit)
        link_1 = Packet_1.tolist()

        Bits_2 = Bits_2_org = numpy.repeat(Dummy_2, NumOfRepeatation)
        Bits_2 = numpy.insert(Bits_2, 0, numpy.random.randint(0, 2, NumOfInval*PacketSize))
        Packet_2 = self.rectpulse(Bits_2, SampPerBit)
        link_2 = Packet_2.tolist()

        ctl_sig_array = numpy.array([1]*NumOfRepeatation*PacketSize)
        ctl_sig_arr = numpy.insert(ctl_sig_array, 0, [255,]*NumOfInval*PacketSize)
        ctl_sig_ar = self.rectpulse(ctl_sig_arr, SampPerBit)
        ctl_sig = ctl_sig_ar.tolist()

        spb_sig = [SampPerBit]*(NumOfRepeatation*PacketSize + NumOfInval*PacketSize)
        spb_sig_ar = self.rectpulse(spb_sig, SampPerBit)
        spb_sig = spb_sig_ar.tolist()
        
        l1 = blocks.vector_source_b(link_1)
        l2 = blocks.vector_source_b(link_2)
        ctl = blocks.vector_source_b(ctl_sig)
        sbp = blocks.vector_source_b(spb_sig)
        testBlock = Hybrid_Comm.Link_Tester(PacketSize, samp_rate)
        dst_BER = blocks.vector_sink_f()
        dst_LA = blocks.vector_sink_f()
        dst_LT = blocks.vector_sink_f()
        self.tb.connect(l1, (testBlock, 0))
        self.tb.connect(l2, (testBlock, 1))
        self.tb.connect(ctl, (testBlock, 2))
        self.tb.connect(sbp, (testBlock, 3))
        self.tb.connect((testBlock, 0), dst_BER)
        self.tb.connect((testBlock, 1), dst_LA)
        self.tb.connect((testBlock, 2), dst_LT)

        # set up fg
        self.tb.run()
        # check data
        resBlock_BER = dst_BER.data()
        resBlock_LA = dst_LA.data()
        resBlock_LT = dst_LT.data()

        BER = resBlock_BER[-1]

        Exp_BER = numpy.sum( numpy.abs( numpy.array(Bits_1_org) - numpy.array(Bits_2_org) ) ) / (NumOfRepeatation*PacketSize)

        print("***************************")
        print("Total number of source samples = ", NumOfRepeatation*PacketSize)
        print("Packet size = ", PacketSize)
        print("Sample rate = ", samp_rate)
        print("Number of invalid bits = ", NumOfInval*PacketSize)
        print()        
        print("Test BER 3:")
        print("Expected BER = ", Exp_BER)
        print("Calculated BER = ", BER)

        self.assertAlmostEqual(BER, Exp_BER, 3)


    def test_004_t(self):  # test: LT
        NumOfBits = 1000000
        SampPerBit = 5
        PacketSize = 500
        samp_rate = 1e3

        Bits_1 = numpy.random.randint(0, 2, NumOfBits, dtype=numpy.byte)
        Packet_1 = self.rectpulse(Bits_1, SampPerBit)
        link_1 = Packet_1.tolist()

        Bits_2 = numpy.random.randint(0, 2, NumOfBits, dtype=numpy.byte)
        Packet_2 = self.rectpulse(Bits_2, SampPerBit)
        link_2 = Packet_2.tolist()

        ctl_sig = [1]*NumOfBits*SampPerBit
        spb_sig = [SampPerBit]*NumOfBits*SampPerBit

        l1 = blocks.vector_source_b(link_1)
        l2 = blocks.vector_source_b(link_2)
        ctl = blocks.vector_source_b(ctl_sig)
        sbp = blocks.vector_source_b(spb_sig)
        testBlock = Hybrid_Comm.Link_Tester(PacketSize, samp_rate)
        dst_BER = blocks.vector_sink_f()
        dst_LA = blocks.vector_sink_f()
        dst_LT = blocks.vector_sink_f()
        self.tb.connect(l1, (testBlock, 0))
        self.tb.connect(l2, (testBlock, 1))
        self.tb.connect(ctl, (testBlock, 2))
        self.tb.connect(sbp, (testBlock, 3))
        self.tb.connect((testBlock, 0), dst_BER)
        self.tb.connect((testBlock, 1), dst_LA)
        self.tb.connect((testBlock, 2), dst_LT)

        # set up fg
        self.tb.run()
        # check data
        resBlock_BER = dst_BER.data()
        resBlock_LA = dst_LA.data()
        resBlock_LT = dst_LT.data()

        LT = resBlock_LT[-1]

        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Packet size = ", PacketSize)
        print("Sample rate = ", samp_rate)
        print()        
        print("Test LT:")
        print("Calculated LT = ", LT)
        print("Expected LT = ", samp_rate/SampPerBit)

        self.assertAlmostEqual(LT, samp_rate/SampPerBit)        


    def rectpulse(self, inArray, SpS):
        flatten = lambda l: [item for sublist in l for item in sublist]
        outArray = [[bit,]*SpS for bit in inArray]
        outArray = flatten(outArray)
        return numpy.array(outArray)
        

if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Link_Tester)
