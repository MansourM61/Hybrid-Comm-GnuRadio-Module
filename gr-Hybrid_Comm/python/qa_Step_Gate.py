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

class qa_Step_Gate(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None


    def test_001_t(self):  # test 1
        N = 10
        NoL = 5
        Levels = (-2, -1, 0, +1, +2)
        Points = (-2, -1, +1, +2)
        PacketSize = N

        Link_1 = [0,]*N
        Link_2 = list(range(-math.floor(N/2), math.ceil(N/2)))

        src_1 = blocks.vector_source_f(Link_1)
        src_2 = blocks.vector_source_f(Link_2)
        testBlock = Hybrid_Comm.Step_Gate(PacketSize, Levels, Points)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        diff = [x - y for x, y in zip(Link_1, Link_2)]
        Out = []

        for x in diff[::PacketSize]:
            if x < Points[0]:
                print(x, ' is in (-Inf, ', Points[0], ')')
                Out.extend([Levels[0], ]*PacketSize)
                pass
            elif x >= Points[-1]:
                print(x, ' is in [', Points[-1], ', +Inf)')
                Out.extend([Levels[-1], ]*PacketSize)
                pass
            else:
                for j in range(0,len(Points)):
                    if ((x >= Points[j]) and (x < Points[j + 1])):
                        print(x, ' is in [', Points[j], ', ', Points[j + 1], ')')
                        Out.extend([LevelsLevels[j + 1], ]*PacketSize)
                        break
                    pass
                pass
            pass

        print()        
        print("***************************")
        print("Total number of samples = ", N)
        print("Levels = ", Levels)
        print("Hopping points = ", Points)
        print("Link 1 = ", Link_1)
        print("Link 2 = ", Link_2)
        print()        
        print("Test 1:")
        print("Expected output = ", Out)
        print("Calculated output = ", resBlock)
        print()

        self.assertFloatTuplesAlmostEqual(Out, resBlock, 5)


    def test_002_t(self):  # test 2
        N = 5
        NoL = 11
        Levels = list(range(-math.floor(NoL/2), math.ceil(NoL/2)))
        Points = Levels[:-1]
        PacketSize = N

        mean_p = sum(Points)*1.0/len(Points)
        std_p = sum( [ (x - mean_p)**2 for x in Points] )*1.0/len(Points)

        sig_1 = numpy.random.normal(loc = mean_p, scale = std_p*1.5, size = (1,N))
        sig_2 = numpy.random.normal(loc = mean_p, scale = std_p*1.5, size = (1,N))
        Link_1 = sig_1.tolist()[0]
        Link_2 = sig_2.tolist()[0]

        src_1 = blocks.vector_source_f(Link_1)
        src_2 = blocks.vector_source_f(Link_2)
        testBlock = Hybrid_Comm.Step_Gate(PacketSize, Levels, Points)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        diff = [x - y for x, y in zip(Link_1, Link_2)]
        Out = []

        for x in diff[::PacketSize]:
            if x < Points[0]:
                print(round(x, 2), ' is in ( -Inf, ', Points[0], ') : Level = ', round(Levels[0], 2))
                Out.extend([Levels[0], ]*PacketSize)
                pass
            elif x >= Points[-1]:
                print(round(x, 2), ' is in [ ', Points[-1], ', +Inf ) : Level = ', round(Levels[-1], 2))
                Out.extend([Levels[-1], ]*PacketSize)
                pass
            else:
                for j in range(0,len(Points)):
                    if ((x >= Points[j]) and (x < Points[j + 1])):
                        print(round(x, 2), ' is in [ ', Points[j], ', ', Points[j + 1], ' ) : Level = ', round(Levels[j + 1], 2))
                        Out.extend([Levels[j + 1], ]*PacketSize)
                        break
                    pass
                pass
            pass

        print()        
        print("***************************")
        print("Total number of samples = ", N)
        print("Levels = ", [round(s, 2) for s in Levels])
        print("Hopping points = ", [round(s, 2) for s in Points])
        print("Link 1 = ", [round(s, 2) for s in Link_1])
        print("Link 2 = ", [round(s, 2) for s in Link_2])
        print("Difference = ", [round(s, 2) for s in diff])
        print()        
        print("Test 2:")
        print("Expected output = ", [round(s, 2) for s in Out])
        print("Calculated output = ", [round(s, 2) for s in resBlock])
        print()

        self.assertFloatTuplesAlmostEqual(Out, resBlock, 5)


    def test_003_t(self):  # test 3
        N = 5000
        NoL = 11
        Levels = list(range(-math.floor(NoL/2), math.ceil(NoL/2)))
        Points = Levels[:-1]
        PacketSize = 100

        mean_p = sum(Points)*1.0/len(Points)
        std_p = sum( [ (x - mean_p)**2 for x in Points] )*1.0/len(Points)

        sig_1 = numpy.random.normal(loc = mean_p, scale = std_p*1.5, size = (1,N))
        sig_2 = numpy.random.normal(loc = mean_p, scale = std_p*1.5, size = (1,N))
        Link_1 = sig_1.tolist()[0]
        Link_2 = sig_2.tolist()[0]

        src_1 = blocks.vector_source_f(Link_1)
        src_2 = blocks.vector_source_f(Link_2)
        testBlock = Hybrid_Comm.Step_Gate(PacketSize, Levels, Points)
        dst = blocks.vector_sink_f()
        self.tb.connect(src_1, (testBlock, 0))
        self.tb.connect(src_2, (testBlock, 1))
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()

        diff = [x - y for x, y in zip(Link_1, Link_2)]
        Out = []

        for x in diff[::PacketSize]:
            if x < Points[0]:
                Out.extend([Levels[0], ]*PacketSize)
                pass
            elif x >= Points[-1]:
                Out.extend([Levels[-1], ]*PacketSize)
                pass
            else:
                for j in range(0,len(Points)):
                    if ((x >= Points[j]) and (x < Points[j + 1])):
                        Out.extend([Levels[j + 1], ]*PacketSize)
                        break
                    pass
                pass
            pass

        print()        
        print("***************************")
        print("Total number of samples = ", N)
        print("Levels = ", [round(s, 2) for s in Levels])
        print("Hopping points = ", [round(s, 2) for s in Points])
        print()        
        print("Test 2:")
        print()

        self.assertFloatTuplesAlmostEqual(Out, resBlock, 5)


if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Step_Gate)
