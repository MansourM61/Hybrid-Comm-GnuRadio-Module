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

class qa_Remove_Header(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None


    def test_001_t(self):  # test 1
        NumOfBits = 4
        SampPerBit = 1
        PacketSize = 3
        Preamble = (0, 1)
        Label = 'R'
        CounterLen = 16
        HeaderSpB = 1
        Delay = 3

        Bits = numpy.array(range(0, NumOfBits))
        Packet = self.rectpulse(Bits, SampPerBit)
        link = Packet.tolist()

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
        Output_arr = numpy.array(link[0:NumOfUsedData])
        for i in range(0, NumOfPackets):
            Header_str = Preamble_b + self.Char2Str(Label_b) + Counter_b[i]
            H_b = self.rectpulse(Header_str, HeaderSpB)
            H_arr = [ord(x) - ord('0') for x in H_b]

            Output_arr = numpy.insert(Output_arr, i*OutPacketSize, H_arr)
            pass

        Output_arr = numpy.insert(Output_arr, 0, [0,]*Delay)

        arr_Seq = Output_arr.tolist()

        src = blocks.vector_source_b(arr_Seq)
        testBlock = Hybrid_Comm.Remove_Header(PacketSize, Preamble, Label, HeaderSpB)
        dst_out = blocks.vector_sink_b()
        dst_sync = blocks.vector_sink_b()
        dst_counter = blocks.vector_sink_i()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst_out)
        self.tb.connect((testBlock, 1), dst_sync)
        self.tb.connect((testBlock, 2), dst_counter)

        # set up fg
        self.tb.run()
        # check data
        resBlock_out = dst_out.data()
        resBlock_sync = dst_sync.data()
        resBlock_counter = dst_counter.data()

        ValidRangeNum = (NumOfBits - (NumOfBits%2)) - ((NumOfBits - (NumOfBits%2)))%PacketSize
        ValidPacket = self.rectpulse(range(0, ValidRangeNum), SampPerBit)

        Res_data = numpy.sum(numpy.array(resBlock_out) - ValidPacket)
        Res_sync = list ( ( (resBlock_sync[-1::-1]) [(PacketSize - 1)::PacketSize]) [-1::-1] )
        Res_counter = list ( ( (resBlock_counter[-1::-1]) [(PacketSize - 1)::PacketSize]) [-1::-1] )

        print()        
        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Total number of header samples = ", HeaderSize)
        print("Total number of data packet samples = ", DataPacketSize)
        print("Total number of output packet samples = ", OutPacketSize)
        print("Total number of available blocks = ", NumOfPackets)
        print("Valid range numbre = ", ValidRangeNum)
        print("Preamble = ", Preamble)
        print("Label = ", Label, ' = ', Label_b)
        print("Counter = ", [str(i) + ': [' + x + ']' for i, x in enumerate(Counter_b)])
        print("Data = ", link)
        print("Source stream = ", arr_Seq)
        print()        
        print("Test 1:")
        print("Extracted data = ", list(resBlock_out))
        print("Sync pulse = ", list(resBlock_sync))
        print("Counter signal = ", list(resBlock_counter))

        self.assertAlmostEqual(Res_data, 0)
        self.assertAlmostEqual(sum(Res_sync)/len(Res_sync), 1)
        self.assertFloatTuplesAlmostEqual(Res_counter, range(0, round(ValidRangeNum/PacketSize)))


    def test_002_t(self):  # test 2
        NumOfBits = 400
        SampPerBit = 2
        PacketSize = 500
        Preamble = (0, 1)
        Label = 'R'
        CounterLen = 16
        HeaderSpB = 10
        Delay = 3

        Bits = numpy.array(range(0, NumOfBits))
        Packet = self.rectpulse(Bits, SampPerBit)
        link = Packet.tolist()

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
        Output_arr = numpy.array(link[0:NumOfUsedData])
        for i in range(0, NumOfPackets):
            Header_str = Preamble_b + self.Char2Str(Label_b) + Counter_b[i]
            H_b = self.rectpulse(Header_str, HeaderSpB)
            H_arr = [ord(x) - ord('0') for x in H_b]

            Output_arr = numpy.insert(Output_arr, i*OutPacketSize, H_arr)
            pass

        Output_arr = numpy.insert(Output_arr, 0, [0,]*Delay)

        src = blocks.vector_source_b(Output_arr)
        testBlock = Hybrid_Comm.Remove_Header(PacketSize, Preamble, Label, HeaderSpB)
        dst_out = blocks.vector_sink_b()
        dst_sync = blocks.vector_sink_b()
        dst_counter = blocks.vector_sink_i()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst_out)
        self.tb.connect((testBlock, 1), dst_sync)
        self.tb.connect((testBlock, 2), dst_counter)

        # set up fg
        self.tb.run()
        # check data
        resBlock_out = dst_out.data()
        resBlock_sync = dst_sync.data()
        resBlock_counter = dst_counter.data()

        ValidRangeNum = round(((NumOfBits*SampPerBit - (NumOfBits*SampPerBit%2)) - ((NumOfBits*SampPerBit - (NumOfBits*SampPerBit%2)))%PacketSize)/SampPerBit)
        ValidPacket = self.rectpulse(range(0, ValidRangeNum), SampPerBit)

        Res_data = numpy.sum(numpy.array(resBlock_out) - ValidPacket)
        Res_sync = list ( ( (resBlock_sync[-1::-1]) [(PacketSize - 1)::PacketSize]) [-1::-1] )

        print()        
        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Total number of header samples = ", HeaderSize)
        print("Total number of data packet samples = ", DataPacketSize)
        print("Total number of output packet samples = ", OutPacketSize)
        print("Total number of available blocks = ", NumOfPackets)
        print("Total stream size = ", numpy.size(Output_arr))
        print("Valid range number = ", ValidRangeNum)
        print("Preamble = ", Preamble)
        print("Label = ", Label, ' = ', Label_b)
        print()        
        print("Test 2:")

        self.assertAlmostEqual(Res_data, 0)
        self.assertAlmostEqual(sum(Res_sync)/len(Res_sync), 1)


    def test_003_t(self):  # test 3
        NumOfBits = 12850  # for more bits, change the counter bit length
        SampPerBit = 1
        PacketSize = 50
        Preamble = (0,1,0,1,0,1,0,1)
        Label = 'R'
        CounterLen = 16
        HeaderSpB = 1
        Delay = 0

        Bits = numpy.array(range(0, NumOfBits))
        Packet = self.rectpulse(Bits, SampPerBit)
        link = Packet.tolist()

        Data = list(range(0,100))*int(NumOfBits/100)

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
        Output_arr = numpy.array(Data, dtype=numpy.int8)
        for i in range(0, NumOfPackets):
            Header_str = Preamble_b + self.Char2Str(Label_b) + Counter_b[i]
            H_b = self.rectpulse(Header_str, HeaderSpB)
            H_arr = [ord(x) - ord('0') for x in H_b]

            Output_arr = numpy.insert(Output_arr, i*OutPacketSize, H_arr)
            pass

        Output_arr = numpy.insert(Output_arr, 0, [0,]*Delay)

        src = blocks.vector_source_b(Output_arr)
        testBlock = Hybrid_Comm.Remove_Header(PacketSize, Preamble, Label, HeaderSpB)
        dst_out = blocks.vector_sink_b()
        dst_sync = blocks.vector_sink_b()
        dst_counter = blocks.vector_sink_i()
        self.tb.connect(src, testBlock)
        self.tb.connect((testBlock, 0), dst_out)
        self.tb.connect((testBlock, 1), dst_sync)
        self.tb.connect((testBlock, 2), dst_counter)

        # set up fg
        self.tb.run()
        # check data
        resBlock_out = dst_out.data()
        resBlock_sync = dst_sync.data()
        resBlock_counter = dst_counter.data()

        ValidRangeNum = round(((NumOfBits*SampPerBit - (NumOfBits*SampPerBit%2)) - ((NumOfBits*SampPerBit - (NumOfBits*SampPerBit%2)))%PacketSize)/SampPerBit)
        ValidPacket = self.rectpulse(range(0, ValidRangeNum), SampPerBit)

        Res_data = sum([x - y for x, y in zip(Data, resBlock_out)])

        print()        
        print("***************************")
        print("Total number of source samples = ", NumOfBits*SampPerBit)
        print("Total number of header samples = ", HeaderSize)
        print("Total number of data packet samples = ", DataPacketSize)
        print("Total number of output packet samples = ", OutPacketSize)
        print("Total number of available blocks = ", NumOfPackets)
        print("Total stream size = ", numpy.size(Output_arr))
        print("Valid range number = ", ValidRangeNum)
        print("Preamble = ", Preamble)
        print("Label = ", Label, ' = ', Label_b)
        print()        
        print("Test 3:")
        print("Number of generated sync pulses = ", int(len(resBlock_sync)/sum(resBlock_sync)))

        self.assertAlmostEqual(Res_data, 0)


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
    gr_unittest.run(qa_Remove_Header)
