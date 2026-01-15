#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 gr-FSO_Comm author.
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
import FSO_Comm_swig as FSO_Comm
import math

class qa_Turbulence(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t(self):
        # test parameters
        Cn2 = 1e-12
        wavelen = 850
        linklen = 200
        Rx_Dia = 50
        Time_Correlation = 25
        SampleRate = 32e3

        # calculate turbulence channel coeeficient
        k = 2.0*math.pi/(wavelen*1e-9)  # wave number (rad/m)
        sig2_R = 1.23*Cn2*pow(k, 7.0/6.0)*pow(linklen, 11.0/6.0)  # Rytov variance
        D_AAF = math.sqrt(k*(Rx_Dia*1e-3*Rx_Dia*1e-3)/(4.0*linklen))  # auxilary parameter for aperture averaging effect
        AAF = pow(1.0 + 1.062*(D_AAF*D_AAF), -7.0/6.0)  # aperture averaging factor
        sig_x = math.sqrt(AAF*sig2_R/4.0)  # sigma parameter weak turbulence
        mu_x = -AAF*sig2_R/4.0  # mu parameter weak turbulence
        mean_val_c = math.exp(mu_x + 0.5*sig_x**2.0)
        var_val_c = 4.0*(math.exp(sig_x**2.0) - 1.0) * math.exp(2.0*mu_x + sig_x**2.0)
        
        # create blocks and connect flowgraph
        src_data = (1, )*2000000

        src = blocks.vector_source_f(src_data)
        sqr = FSO_Comm.Turbulence(Cn2, wavelen, Rx_Dia, linklen, Time_Correlation, SampleRate)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, sqr)
        self.tb.connect(sqr, dst)

        # set up fg
        self.tb.run ()
        # check data
        result_data = dst.data()

        mean_val = sum(result_data)/len(result_data)
        var_val = sum((x - mean_val) ** 2 for x in result_data) / len(result_data)

        print("***************************")
        print("Link length = ", linklen, " m")
        print("Cn2 = ", Cn2, " m^(-2/3)")
        print("Wavelength = ", wavelen, " nm")
        print("Rx diameter = ", Rx_Dia, " mm")
        print("Time correlation = ", Time_Correlation, " ms")
        print("Test:")
        print("Expected mean value = ", mean_val_c)
        print("Calculated mean value = ", mean_val)
        print("Expected var value = ", var_val_c)
        print("Calculated var value = ", var_val)

        # check accuracy of calculated values from blocks 
        self.assertAlmostEqual(round(mean_val_c), round(mean_val), 0)
        self.assertAlmostEqual(round(var_val_c*1e2), round(var_val*1e2), 0)	



if __name__ == '__main__':
    gr_unittest.run(qa_Turbulence)
