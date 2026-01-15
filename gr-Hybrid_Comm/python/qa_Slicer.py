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

class qa_Slicer(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):  # test 1
        AvgWinSize = 25
        PacketSize = 50
        Thresh = 3
        amp = 1
        N = 100
        SpS = 5
        SNR_dB = 20.0

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + Thresh
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Slicer(PacketSize, AvgWinSize, Thresh)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Test 1:")
        print("DC value = ", Thresh)
        print("Input length = ", len(Sig))
        print("Results length = ", len(resBlock))
        print()

        self.assertFloatTuplesAlmostEqual(Bits, resBlock, 0)


    def test_002_t(self):  # test 2
        AvgWinSize = 25
        PacketSize = 1000
        Thresh = 3
        amp = 1
        N = 10000
        SpS = 5
        SNR_dB = 10.0

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + Thresh
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Slicer(PacketSize, AvgWinSize, Thresh)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        Exp_res = sum(Bits)/len(Bits)
        Cal_res = sum(resBlock)/len(resBlock)

        print()        
        print("***************************")
        print("Test 2:")
        print("DC value = ", Thresh)
        print("Input length = ", len(Sig))
        print("Results length = ", len(resBlock))
        print("Expected mean value = ", Exp_res)
        print("Calculated mean value = ", Cal_res)
        print()
        
        self.assertAlmostEqual(Exp_res, Cal_res, 1)
        

    def rectpulse(self, inArray, SpS):
        flatten = lambda l: [item for sublist in l for item in sublist]
        outArray = [[bit,]*SpS for bit in inArray]
        outArray = flatten(outArray)
        return numpy.array(outArray)
        

if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Slicer)
