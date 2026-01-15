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

class qa_Signal_Quality_Metre(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):  # signal power measurement
        amp = 1
        DC = 0
        N = 100000
        SNR_dB = 10.0
        SpS = 4
        WinSize = 10000
        EdgeRange = 2
        MeasType = 'Signal Power'  # signal power

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        Q_fact = math.sqrt(SNR)
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + DC
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Signal_Quality_Metre(WinSize, SpS, MeasType, EdgeRange)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("SNR = ", SNR_dB, "dB")
        print("Signal Power = ", P_S)
        print("Noise Power = ", P_N)
        print("Q-factor = ", Q_fact)
        print()        
        print("Signal power test:")
        print("Expected value = ", P_S)
        print("Measured value = ", sum(resBlock)*1.0/len(resBlock))
        print()
        
        self.assertAlmostEqual(P_S, sum(resBlock)*1.0/len(resBlock), 2)
        

    def test_002_t(self):  # noise power measurement
        amp = 1
        DC = 0
        N = 100000
        SNR_dB = 10.0
        SpS = 4
        WinSize = 10000
        EdgeRange = 2
        MeasType = 'Noise Power'  # noise power

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        Q_fact = math.sqrt(SNR)
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + DC
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Signal_Quality_Metre(WinSize, SpS, MeasType, EdgeRange)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("SNR = ", SNR_dB, "dB")
        print("Signal Power = ", P_S)
        print("Noise Power = ", P_N)
        print("Q-factor = ", Q_fact)
        print()        
        print("Noise power test:")
        print("Expected value = ", P_N)
        print("Measured value = ", sum(resBlock)*1.0/len(resBlock))
        print()
        
        self.assertAlmostEqual(P_N, sum(resBlock)*1.0/len(resBlock), 2)


    def test_003_t(self):  # signal-to-noise ratio (SNR) measurement
        amp = 1
        DC = 0
        N = 100000
        SNR_dB = 10.0
        SpS = 4
        WinSize = 10000
        EdgeRange = 2
        MeasType = 'Signal-to-noise Ratio (SNR)'  # signal-to-noise ratio (SNR)

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        Q_fact = math.sqrt(SNR)
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + DC
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Signal_Quality_Metre(WinSize, SpS, MeasType, EdgeRange)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("SNR = ", SNR_dB, "dB")
        print("Signal Power = ", P_S)
        print("Noise Power = ", P_N)
        print("Q-factor = ", Q_fact)
        print()        
        print("Signal-to-noise ratio (SNR) test:")
        print("Expected value = ", SNR_dB)
        print("Measured value = ", sum(resBlock)*1.0/len(resBlock))
        print()
        
        self.assertAlmostEqual(SNR_dB, sum(resBlock)*1.0/len(resBlock), 0)


    def test_004_t(self):  # Q-factor measurement
        amp = 1
        DC = 0
        N = 100000
        SNR_dB = 10.0
        SpS = 4
        WinSize = 10000
        EdgeRange = 2
        MeasType = 'Q-factor'  # Q-factor

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        Q_fact = math.sqrt(SNR)
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + DC
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Signal_Quality_Metre(WinSize, SpS, MeasType, EdgeRange)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("SNR = ", SNR_dB, "dB")
        print("Signal Power = ", P_S)
        print("Noise Power = ", P_N)
        print("Q-factor = ", Q_fact)
        print()        
        print("Q-factor test:")
        print("Expected value = ", Q_fact)
        print("Measured value = ", sum(resBlock)*1.0/len(resBlock))
        print()
        
        self.assertAlmostEqual(Q_fact, sum(resBlock)*1.0/len(resBlock), 0)


    def test_005_t(self):  # DC value measurement
        amp = 1
        DC = 3
        N = 100000
        SNR_dB = 10.0
        SpS = 4
        WinSize = 10000
        EdgeRange = 2
        MeasType = 'Signal Power'

        P_S = (amp/2.0)*(amp/2.0)
        SNR = 10.0**(SNR_dB/10.0)
        P_N = P_S/SNR
        Q_fact = math.sqrt(SNR)
        rawBits = numpy.random.randint(0, 2, N)
        Bits = self.rectpulse(rawBits, SpS)
        X = amp*(Bits - 0.5) + DC
        noise = numpy.random.normal(loc = 0.0, scale = 1.0, size = (1,N*SpS))*math.sqrt(P_N)
        Y = X + noise
        Sig = Y.tolist()[0]

        src = blocks.vector_source_f(Sig)
        testBlock = Hybrid_Comm.Signal_Quality_Metre(WinSize, SpS, MeasType, EdgeRange)
        dst_1 = blocks.vector_sink_f()
        dst_2 = blocks.vector_sink_f()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst_1)
        self.tb.connect((testBlock, 1), dst_2)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst_2.data()

        print()        
        print("***************************")
        print("DC value test:", DC)
        print("Expected value = ", sum(resBlock)*1.0/len(resBlock))
        print()
        
        self.assertAlmostEqual(DC, sum(resBlock)*1.0/len(resBlock), 0)


    def rectpulse(self, inArray, SpS):
        flatten = lambda l: [item for sublist in l for item in sublist]
        outArray = [[bit,]*SpS for bit in inArray]
        outArray = flatten(outArray)
        return numpy.array(outArray)
        

if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Signal_Quality_Metre)
