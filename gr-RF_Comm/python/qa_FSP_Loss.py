#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 gr-RF_Comm author.
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
import RF_Comm_swig as RF_Comm
import math

class qa_FSP_Loss(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        # test parameters
        src_data = (0, 1, 2, 3, 4)
        linklen = 1000
        freq = 0.01
        c0 = 299792458.0

        # calculate the loss
        L = (c0/(4*math.pi*linklen*freq*1.0e9))**2.0

        # create the blocks and connect flowgraph
        expected_result = (0, 1*L, 2*L, 3*L, 4*L)

        src = blocks.vector_source_f(src_data)
        sqr = RF_Comm.FSP_Loss(linklen, freq)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, sqr)
        self.tb.connect(sqr, dst)

        # set up fg
        self.tb.run ()
        # check data
        result_data = dst.data()

        print("***************************")
        print("Link length = ", linklen, " m")
        print("Frequency = ", freq, " GHz")
        print("Loss = ", L)      
        print("Test:")
        print("Expected Array = ", expected_result)
        print("Calculated Array = ", result_data)

        # check validaty of the output values from block
        self.assertFloatTuplesAlmostEqual(expected_result, result_data, 6)


if __name__ == '__main__':
    gr_unittest.run(qa_FSP_Loss)
