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

class qa_Rx_Soft_Switch(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None


    def test_001_t(self):
        NoS = 200  # number of samples
        PacketSize = 50
        Thresh = (5, 8)
        SpP_1 = (1, 1, 2)
        SpP_2 = (2, 1, 1)
        
        Sig = range(0, NoS)
        SpP = range(0, NoS)

        NoP = int(NoS/PacketSize)

        In_1 = []
        In_2 = []

        for index_p in range(0, NoP):
            if SpP[index_p*PacketSize] < Thresh[0]:
                Index_T = 0
                pass
            if SpP[index_p*PacketSize] >= Thresh[-1]:
                Index_T = len(Thresh)
                pass
            else:
                for index_th in range(0, len(Thresh) - 1):
                    if (SpP[index_p*PacketSize] >= Thresh[index_th]) and (SpP[index_p*PacketSize] < Thresh[index_th + 1]):
                        Index_T = index_t
                        break
                    pass
                pass

            Index_1_s = index_p*PacketSize
            Index_1_e = Index_1_s
            Index_2_s = Index_1_s
            Index_2_e = (index_p + 1)*PacketSize

            NumOfSamp_1 = int(math.floor(SpP_2[Index_T]*PacketSize/(SpP_1[Index_T] + SpP_2[Index_T])))
            NumOfSamp_2 = PacketSize - NumOfSamp_1

            InterpRate_1 = max(int(math.floor(PacketSize/NumOfSamp_1)), 1)
            InterpRate_2 = max(int(math.floor(PacketSize/NumOfSamp_2)), 1)

            Index_1_e += int(NumOfSamp_1)
            Index_2_s += int(NumOfSamp_1)

            In_1.extend(self.rectpulse(Sig[Index_1_s:Index_1_e], InterpRate_1))
            In_2.extend(self.rectpulse(Sig[Index_2_s:Index_2_e], InterpRate_2))

            Extra_1 = (index_p + 1)*PacketSize - len(In_1)
            In_1.extend([0,]*Extra_1)

            Extra_2 = (index_p + 1)*PacketSize - len(In_2)
            In_2.extend([0,]*Extra_2)

            pass

        src_1 = blocks.vector_source_b(In_1)
        src_2 = blocks.vector_source_b(In_2)
        src_spp = blocks.vector_source_f(SpP)
        testBlock = Hybrid_Comm.Rx_Soft_Switch(PacketSize, Thresh, SpP_1, SpP_2)
        dst = blocks.vector_sink_b()

        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(src_spp, (testBlock, 2))
        self.tb.connect((testBlock, 0), dst)
        
        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of input = ", NoS)
        print("Input 1 = ", In_1)
        print("Input 2 = ", In_2)
        print("Link 1 samples per packet = ", SpP_1)
        print("Link 2 samples per packet = ", SpP_2)
        print("Threshold = ", Thresh)
        print()
        print("Test:")
        print("Expected output = ", list(Sig))
        print("Calculated output = ", resBlock)

        self.assertFloatTuplesAlmostEqual(Sig, resBlock, 0)


    def rectpulse(self, inArray, SpS):
        flatten = lambda l: [item for sublist in l for item in sublist]
        outArray = [[bit,]*SpS for bit in inArray]
        outArray = flatten(outArray)
        return numpy.array(outArray)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Rx_Soft_Switch)
