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

class qa_Rx_Hard_Switch(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        NoS = 1000  # number of samples
        PacketSize = 50
        Thresh = 0
        SpB = (10, 5)

        Samp_1 = numpy.random.randint(0, 2, NoS, dtype=numpy.byte)
        Samp_2 = numpy.random.randint(0, 2, NoS, dtype=numpy.byte)
        Sel = numpy.random.randn(1, NoS)[0]
        Out = []
        for i, x in enumerate(Sel[::PacketSize]):
            Out.extend( Samp_1[i*PacketSize:(i + 1)*PacketSize] if(x < Thresh) else Samp_2[i*PacketSize:(i + 1)*PacketSize])
            pass

        Sig_1 = Samp_1.tolist()
        Sig_2 = Samp_2.tolist()
        Sig_Sel = Sel.tolist()

        src_1 = blocks.vector_source_b(Sig_1)
        src_2 = blocks.vector_source_b(Sig_2)
        src_sel = blocks.vector_source_f(Sig_Sel)
        testBlock = Hybrid_Comm.Rx_Hard_Switch(PacketSize, Thresh, SpB)
        dst = blocks.vector_sink_b()

        self.tb.connect((src_1, 0), (testBlock, 0))
        self.tb.connect((src_2, 0), (testBlock, 1))
        self.tb.connect((src_sel, 0), (testBlock, 2))
        self.tb.connect((testBlock, 0), (dst, 0))
        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of link 1 = ", len(Sig_1))
        print("Number of link 2 = ", len(Sig_2))
        print("Number of select = ", len(Sig_Sel))
        print()
        print("(Expeted, Caculated) = ", ["(" + str(x) + ", " + str(y) + "), " for x, y in zip(Out, resBlock)])

        self.assertFloatTuplesAlmostEqual(Out, resBlock, 0)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))    
    gr_unittest.run(qa_Rx_Hard_Switch)
