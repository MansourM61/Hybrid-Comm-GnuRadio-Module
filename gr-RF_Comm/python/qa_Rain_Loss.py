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

class qa_Rain_Loss(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t (self):
        # test parameters
        src_data = (0, 1, 2, 3, 4)
        linklen = 1000
        prep = 64.8

        # calculate the loss
        L = 10.0**(-(linklen * 1e-3 * 1.076 * (prep**(2.0/3.0)))/10.0)

        # create the blocks and connect flowgraph
        expected_result = (0, 1*L, 2*L, 3*L, 4*L)

        src = blocks.vector_source_f(src_data)
        sqr = RF_Comm.Rain_Loss(linklen, prep)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, sqr)
        self.tb.connect(sqr, dst)

        # set up fg
        self.tb.run ()
        # check data
        result_data = dst.data()

        print("***************************")
        print("Link length = ", linklen, " m")
        print("Precipitation = ", prep, " mm/h")
        print("Loss = ", L)      
        print("Test:")
        print("Expected Array = ", expected_result)
        print("Calculated Array = ", result_data)

        # check validaty of the output values from block
        self.assertFloatTuplesAlmostEqual(expected_result, result_data, 6)



if __name__ == '__main__':
    gr_unittest.run(qa_Rain_Loss)
