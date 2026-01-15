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

class qa_Source_BV(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None



    def test_001_t(self):  # constant source
        Val_0 = 0
        Val_1 = 1
        NumOfChunk = 10
        PacketSize = 1000
        N = PacketSize * NumOfChunk
        SigType = "Constant"  # constant signal
        BpW_elem = (5, 2)  # bits per window

        bpw_Ar = numpy.random.randint(0, 2, N, dtype=numpy.int8) * (BpW_elem[1] - BpW_elem[0]) + BpW_elem[0]
        BPW = bpw_Ar.tolist()

        src = blocks.vector_source_b(BPW)
        testBlock = Hybrid_Comm.Source_BV(PacketSize, SigType)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Total number of samples = ", N)
        print()        
        print("Constant source test:")
        print("Total number of samples = ", len(resBlock))
        print()
        
        self.assertAlmostEqual(Val_1, sum(resBlock)*1.0/len(resBlock), 2)


    def test_002_t(self):  # random source test 1
        Val_0 = 0
        Val_1 = 1
        NumOfChunk = 2
        PacketSize = 12
        N = PacketSize * NumOfChunk
        SigType = "Random"  # random signal
        BpW_elem = (6, 3)  # bits per window

        bpw_Ar = numpy.random.randint(0, 2, N, dtype=numpy.int8) * (BpW_elem[1] - BpW_elem[0]) + BpW_elem[0]
        BPW = bpw_Ar.tolist()

        src = blocks.vector_source_b(BPW)
        testBlock = Hybrid_Comm.Source_BV(PacketSize, SigType)
        dst = blocks.vector_sink_b()
        dst_spb = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst)
        self.tb.connect((testBlock, 1), dst_spb)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()
        spbBlock = dst_spb.data()

        print()        
        print("***************************")
        print("Samples per bit = ", BPW)
        print()
        print("Random source test:")
        print("Output = ", resBlock)
        print("SpB = ", spbBlock)
        print()

        self.assertAlmostEqual(N*1.0, len(resBlock)*1.0, 2)


    def test_003_t(self):  # random source test 2
        Val_0 = 0
        Val_1 = 1
        NumOfChunk = 100
        PacketSize = 1000
        N = PacketSize * NumOfChunk
        SigType = "Random"  # constant signal
        BpW_elem = (20, 50)  # bits per window

        bpw_Ar = numpy.random.randint(0, 2, N, dtype=numpy.int8) * (BpW_elem[1] - BpW_elem[0]) + BpW_elem[0]
        BPW = bpw_Ar.tolist()
        Mu_e = (Val_1 + Val_0)/2.0
        sig_e = math.sqrt( ((Val_1 - Mu_e)**2 + (Val_0 - Mu_e)**2)/2 )

        src = blocks.vector_source_b(BPW)
        testBlock = Hybrid_Comm.Source_BV(PacketSize, SigType)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()
        Mu_c = sum(resBlock)*1.0/len(resBlock)
        sig_c = math.sqrt( sum([(x - Mu_c)**2 for x in resBlock] ) / len(resBlock) )

        print()        
        print("***************************")
        print("Total number of samples = ", N)
        print("Expected mean value = ", Mu_e)
        print("Expected std value = ", sig_e)
        print()        
        print("Random source test:")
        print("Total number of samples = ", len(resBlock))
        print("Calculated mean value = ", Mu_c)
        print("Calculated std value = ", sig_c)
        print()

        self.assertAlmostEqual(Mu_e, Mu_c, 0)
        self.assertAlmostEqual(sig_e, sig_c, 0)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Source_BV)
