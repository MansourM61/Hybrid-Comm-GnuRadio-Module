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

class qa_Add_Header(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):  # test 1
        NumOfBits = 10
        SampPerBit = 2
        PacketSize = 7
        Preamble = (0, 0, 1, 1)
        Label = 'L1'
        CounterLen = 16
        HeaderSpB = 1

        Bits = numpy.ones(NumOfBits, dtype=numpy.byte)
        Packet = self.rectpulse(Bits, SampPerBit)
        link = Packet.tolist()

        src = blocks.vector_source_b(link)
        testBlock = Hybrid_Comm.Add_Header(PacketSize, Preamble, Label, HeaderSpB)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()
        Output = numpy.array(list(resBlock))*1.0

        Preamble_b = self.Char2Str(Preamble)
        NumOfPackets = int(NumOfBits*SampPerBit/PacketSize)
        Label_b = [((bin(ord(x)).replace('b', '0'))[-1::-1])[0:8] for x in Label]

        Counter = range(0, NumOfPackets)  
        Counter_b = [self.Num2Bin(x, CounterLen)[-1::-1] for x in Counter]
        StringLength = (len(Preamble) + len(Label_b[0])*len(Label) + CounterLen) * HeaderSpB
        HeaderSize = StringLength
        DataPacketSize = PacketSize
        OutPacketSize = HeaderSize + DataPacketSize
        NumOfUsedData = NumOfBits*SampPerBit - ((NumOfBits*SampPerBit) % PacketSize)
        Output_arr = numpy.array(link[0:NumOfUsedData])*1.0

        for i in range(0, NumOfPackets):
            Header_str = Preamble_b + self.Char2Str(Label_b) + Counter_b[i]
            H_b = self.rectpulse(Header_str, HeaderSpB)
            H_arr = [ord(x) - ord('0') for x in H_b]

            Output_arr = numpy.insert(Output_arr, i*OutPacketSize, H_arr)
            pass

        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Total number of header samples = ", HeaderSize)
        print("Total number of data packet samples = ", DataPacketSize)
        print("Total number of output packet samples = ", OutPacketSize)
        print("Preamble = ", Preamble)
        print("Label = ", Label, ' = ', Label_b)
        print("Counter = ", [str(i) + ': [' + x + ']' for i, x in enumerate(Counter_b)])
        print("Data = ", link)
        print()        
        print("Test 1:")
        print("Expected output =   ", Output_arr.tolist())
        print("Calculated output = ", Output.tolist())
        print("Expected output length = ", len(Output_arr))
        print("Calculated output length = ", len(Output))
        print("(Expected, Calculated) pair = ", [(x, y) for x, y in zip(Output_arr.tolist(), Output.tolist())])

        self.assertFloatTuplesAlmostEqual(Output.tolist(), Output_arr.tolist())


    def test_002_t(self):  # test 2
        NumOfBits = 5000
        SampPerBit = 4
        PacketSize = 1000
        Preamble = (0, 0, 1, 1, 0, 1, 0, 1)
        Label = 'RF'
        CounterLen = 16
        HeaderSpB = 2

        Bits = numpy.ones(NumOfBits, dtype=numpy.byte)
        Packet = self.rectpulse(Bits, SampPerBit)
        link = Packet.tolist()

        src = blocks.vector_source_b(link)
        testBlock = Hybrid_Comm.Add_Header(PacketSize, Preamble, Label, HeaderSpB)
        dst = blocks.vector_sink_b()
        self.tb.connect(src, testBlock)
        self.tb.connect(testBlock, dst)

        # set up fg
        self.tb.run()
        # check data
        resBlock = dst.data()
        Output = numpy.array(list(resBlock))*1.0

        Preamble_b = self.Char2Str(Preamble)
        NumOfPackets = int(NumOfBits*SampPerBit/PacketSize)
        Label_b = [((bin(ord(x)).replace('b', '0'))[-1::-1])[0:8] for x in Label]
        Counter = range(0, NumOfPackets)  
        Counter_b = [self.Num2Bin(x, CounterLen)[-1::-1] for x in Counter]
        StringLength = (len(Preamble) + len(Label_b[0])*len(Label) + CounterLen) * HeaderSpB
        HeaderSize = StringLength
        DataPacketSize = PacketSize
        OutPacketSize = HeaderSize + DataPacketSize
        NumOfUsedData = NumOfBits*SampPerBit - ((NumOfBits*SampPerBit) % PacketSize)
        Output_arr = numpy.array(link[0:NumOfUsedData])*1.0
        for i in range(0, NumOfPackets):
            Header_str = Preamble_b + self.Char2Str(Label_b) + Counter_b[i]
            H_b = self.rectpulse(Header_str, HeaderSpB)
            H_arr = [ord(x) - ord('0') for x in H_b]

            Output_arr = numpy.insert(Output_arr, i*OutPacketSize, H_arr)
            pass

        Res = numpy.sum(Output - Output_arr)

        print()        
        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Total number of header samples = ", HeaderSize)
        print("Total number of data packet samples = ", DataPacketSize)
        print("Total number of output packet samples = ", OutPacketSize)
        print("Total number of available blocks = ", NumOfPackets)
        print("Preamble = ", Preamble)
        print("Label = ", Label, ' = ', Label_b)
        print("Counter = ", [str(i) + ': [' + x + ']' for i, x in enumerate(Counter_b)])
        print()        
        print("Test 2:")
        print("Expected and calculated results difference = ", Res)

        self.assertAlmostEqual(Res, 0)


    def rectpulse(self, inArray, SpS):
        flatten = lambda l: [item for sublist in l for item in sublist]
        outArray = [[bit,]*SpS for bit in inArray]
        outArray = flatten(outArray)
        return numpy.array(outArray)
        

    def Num2Bin(self, Number, Length = 8):
        Res = '{0:{width}{base}}'.format(Number, width = Length, base = 'b')
        return Res.replace(" ", "0")


    def Char2Str(self, inArg):
        Res = ""
        for x in inArg:
            Res += str(x)
            pass
        return Res



if __name__ == '__main__':
    numpy.random.seed(int(time.time()))
    gr_unittest.run(qa_Add_Header)
