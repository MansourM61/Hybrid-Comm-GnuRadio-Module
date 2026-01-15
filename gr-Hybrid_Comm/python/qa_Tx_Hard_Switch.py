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

class qa_Tx_Hard_Switch(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        PacketSize = 10
        NoS = PacketSize*2  # number of samples
        Thresh = 0        
        SpB = (1, 2)

        Samp = numpy.random.randint(0, 2, NoS)
        Sig = Samp.tolist()

        Select = numpy.array([Thresh - 1, ]*PacketSize + [Thresh + 1, ]*PacketSize)

        src = blocks.vector_source_b(Sig)
        sel = blocks.vector_source_f(Select)
        testBlock = Hybrid_Comm.Tx_Hard_Switch(PacketSize, Thresh, SpB)
        dst_1 = blocks.vector_sink_b()
        dst_2 = blocks.vector_sink_b()

        self.tb.connect(src, (testBlock, 0))
        self.tb.connect(sel, (testBlock, 1))
        self.tb.connect((testBlock, 0), dst_1)
        self.tb.connect((testBlock, 1), dst_2)
        
        # set up fg
        self.tb.run()
        # check data
        resBlock_1 = dst_1.data()
        resBlock_2 = dst_2.data()

        Exp_Res_1 = numpy.ones(NoS)*0
        Exp_Res_1[Select < Thresh] = Samp[Select < Thresh]

        Exp_Res_2 = numpy.ones(NoS)*0
        Exp_Res_2[Select >= Thresh] = Samp[Select >= Thresh]

        print()        
        print("***************************")
        print("Number of input = ", NoS)
        print("Threshold = ", Thresh)
        print("Input samples = ", Sig)
        print("Selection signal = ", Select.tolist())
        print()
        print("Test:")
        print("Expected Output 1 = ", list(Exp_Res_1))
        print("Calculated Output 1 = ", list(resBlock_1))
        print("Expected Output 2 = ", list(Exp_Res_2))
        print("Calculated Output 2 = ", list(resBlock_2))

        self.assertFloatTuplesAlmostEqual(Exp_Res_1, resBlock_1, 0)
        self.assertFloatTuplesAlmostEqual(Exp_Res_2, resBlock_2, 0)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Tx_Hard_Switch)
