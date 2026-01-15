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

class qa_Tx_Parallel_Switch(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        NoS = 100000  # number of samples
        SpB = 10
        PacketSize = 10000

        Samp = numpy.random.randint(0, 2, NoS)
        Sig = Samp.tolist()

        src = blocks.vector_source_b(Sig)
        testBlock = Hybrid_Comm.Tx_Parallel_Switch(PacketSize, SpB)
        dst_1 = blocks.vector_sink_b()
        dst_2 = blocks.vector_sink_b()

        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst_1)
        self.tb.connect((testBlock, 1), dst_2)
        
        # set up fg
        self.tb.run()
        # check data
        resBlock_1 = dst_1.data()
        resBlock_2 = dst_2.data()

        print()        
        print("***************************")
        print("Number of input = ", NoS)
        print("Number of link 1 = ", len(resBlock_1))
        print("Number of link 2 = ", len(resBlock_2))
        print()

        self.assertFloatTuplesAlmostEqual(Sig, resBlock_1, 0)
        self.assertFloatTuplesAlmostEqual(Sig, resBlock_2, 0)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Tx_Parallel_Switch)
