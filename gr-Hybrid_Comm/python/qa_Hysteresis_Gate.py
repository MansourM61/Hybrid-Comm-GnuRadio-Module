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

class qa_Hysteresis_Gate(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):  # test 1: no hysteresis
        NoS = 100000

        Scale = 1;

        P_r = -1.0
        S_r = +1.0
        V_r = +1.0
        P_f = +1.0
        S_f = -1.0
        V_f = -1.0
        Mode = 'Forward'
        PacketSize = 100

        Samp_1 = numpy.random.rand(1, NoS)[0] * Scale
        Samp_2 = numpy.random.rand(1, NoS)[0] * Scale
        Sig_1 = Samp_1.tolist()
        Sig_2 = Samp_2.tolist()

        Out_Sig = []
        for x, y in zip(Sig_1[::PacketSize], Sig_2[::PacketSize]):
            val = max( min( ((x - y) - P_r)*S_r + V_f, V_r), V_f)
            Out_Sig.extend([val, ]*PacketSize)
            pass

        src_1 = blocks.vector_source_f(Sig_1)
        src_2 = blocks.vector_source_f(Sig_2)
        testBlock = Hybrid_Comm.Hysteresis_Gate(PacketSize, P_r, S_r, V_r, P_f, S_f, V_f, Mode)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of link 1 = ", len(Sig_1))
        print("Number of link 2 = ", len(Sig_2))
        print("Number of select = ", len(Out_Sig))
        print()

        self.assertFloatTuplesAlmostEqual(Out_Sig, resBlock, 5)


    def test_002_t(self):  # test 1: with forward hysteresis
        NoS = 100000

        Scale = 2;

        P_r = -0.0
        S_r = +1.0
        V_r = +1.0
        P_f = +0.0
        S_f = -1.0
        V_f = -1.0
        Mode = 'Forward'
        PacketSize = 100

        Samp_1 = numpy.random.rand(1, NoS)[0] * Scale
        Samp_2 = numpy.random.rand(1, NoS)[0] * Scale
        Sig_1 = Samp_1.tolist()
        Sig_2 = Samp_2.tolist()

        Out_Sig = []
        for x, y in zip(Sig_1[::PacketSize], Sig_2[::PacketSize]):
            val = self.Hysteresis((P_r, S_r, V_r, P_f, S_f, V_f, Mode), [x,], [y,])
            Out_Sig.extend([val[0], ]*PacketSize)
            pass
        
        src_1 = blocks.vector_source_f(Sig_1)
        src_2 = blocks.vector_source_f(Sig_2)
        testBlock = Hybrid_Comm.Hysteresis_Gate(PacketSize, P_r, S_r, V_r, P_f, S_f, V_f, Mode)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of link 1 = ", len(Sig_1))
        print("Number of link 2 = ", len(Sig_2))
        print("Number of select = ", len(Out_Sig))
        print()

        self.assertFloatTuplesAlmostEqual(Out_Sig, resBlock, 5)


    def test_003_t(self):  # test 1: with backward hysteresis
        NoS = 100000

        Scale = 2;

        P_r = -0.0
        S_r = +1.0
        V_r = +1.0
        P_f = +0.0
        S_f = -1.0
        V_f = -1.0
        Mode = 'Backward'
        PacketSize = 100

        Samp_1 = numpy.random.rand(1, NoS)[0] * Scale
        Samp_2 = numpy.random.rand(1, NoS)[0] * Scale
        Sig_1 = Samp_1.tolist()
        Sig_2 = Samp_2.tolist()

        Out_Sig = []
        for x, y in zip(Sig_1[::PacketSize], Sig_2[::PacketSize]):
            val = self.Hysteresis((P_r, S_r, V_r, P_f, S_f, V_f, Mode), [x,], [y,])
            Out_Sig.extend([val[0], ]*PacketSize)
            pass
        
        src_1 = blocks.vector_source_f(Sig_1)
        src_2 = blocks.vector_source_f(Sig_2)
        testBlock = Hybrid_Comm.Hysteresis_Gate(PacketSize, P_r, S_r, V_r, P_f, S_f, V_f, Mode)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of link 1 = ", len(Sig_1))
        print("Number of link 2 = ", len(Sig_2))
        print("Number of select = ", len(Out_Sig))
        print()

        self.assertFloatTuplesAlmostEqual(Out_Sig, resBlock, 5)


    def test_004_t(self):  # test 1: with hysteresis
        NoS = 17

        Scale = 2;

        P_r = -0.0
        S_r = +1.0
        V_r = +1.0
        P_f = +0.0
        S_f = -1.0
        V_f = -1.0
        Mode = 'Forward'
        PacketSize = 17

        Samp_1 = [4.0,] * NoS
        Samp_2 = [4.0, 3.0, 2.0, 1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 7.0, 6.0, 5.0, 4.0]
        Sig_1 = Samp_1
        Sig_2 = Samp_2

        Out_Sig = []
        for x, y in zip(Sig_1[::PacketSize], Sig_2[::PacketSize]):
            val = self.Hysteresis((P_r, S_r, V_r, P_f, S_f, V_f, Mode), [x,], [y,])
            Out_Sig.extend([val[0], ]*PacketSize)
            pass
        
        src_1 = blocks.vector_source_f(Sig_1)
        src_2 = blocks.vector_source_f(Sig_2)
        testBlock = Hybrid_Comm.Hysteresis_Gate(PacketSize, P_r, S_r, V_r, P_f, S_f, V_f, Mode)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        print()        
        print("***************************")
        print("Number of link 1 = ", len(Sig_1))
        print("Number of link 2 = ", len(Sig_2))
        print("Number of select = ", len(Out_Sig))
        print()

        self.assertFloatTuplesAlmostEqual(Out_Sig, resBlock, 5)


    def Hysteresis(self, Param, Sig_1, Sig_2):
        P_f, S_f, V_f, P_b, S_b, V_b, Mode = Param

        Sig = [0,]*len(Sig_1)

        for i, (x, y) in enumerate(zip(Sig_1, Sig_2)):
            if(Mode == 'Forward'):
                Sig[i] = max( min( +1.0*((x - y) - P_f)*S_f + V_b, V_f), V_b)

                Mode = 'Backward' if (abs(Sig[i] - V_f) <= 1.0e-10) else 'Forward'
                pass

            else:
                Sig[i] = max( min( -1.0*((x - y) - P_b)*S_b + V_f, V_f), V_b)

                Mode = 'Forward' if (abs(Sig[i] - V_b) <= 1.0e-10) else 'Backward'
                pass

            pass

        return Sig


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Hysteresis_Gate)