#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.8.0.0

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import blocks
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import Hybrid_Comm
from gnuradio import qtgui

class Remove_Header(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "Remove_Header")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000
        self.Preamble = Preamble = (2, 2, 2, 2)
        self.LengthOffset = LengthOffset = 0
        self.Label_str = Label_str = "R"
        self.Label_bit = Label_bit = (0,1,0,0,1,0,1,0,)
        self.Data = Data = range(0, 50)
        self.Counter = Counter = (1, 1, 0, 0, 0, 0, 0, 0)

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            1024, #size
            samp_rate, #samp_rate
            "", #name
            3 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(True)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Data', 'Sync', 'Counter', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['green', 'cyan', 'red', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [0, 1, 7, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(3):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.blocks_vector_source_x_0_0_0_0 = blocks.vector_source_b(Data, True, 1, [])
        self.blocks_vector_source_x_0_0_0 = blocks.vector_source_b(Counter, True, 1, [])
        self.blocks_vector_source_x_0_0 = blocks.vector_source_b(Label_bit, True, 1, [])
        self.blocks_vector_source_x_0 = blocks.vector_source_b(Preamble, True, 1, [])
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_stream_mux_1 = blocks.stream_mux(gr.sizeof_char*1, [len(Preamble)+len(Label_bit)+len(Counter)+len(Data) + LengthOffset, ]*4)
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_char*1, [len(Preamble), len(Label_bit), len(Counter), len(Data) + LengthOffset])
        self.blocks_short_to_float_0 = blocks.short_to_float(1, 1)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_ff(1.0/len(Data))
        self.blocks_char_to_float_0_0 = blocks.char_to_float(1, 1)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1)
        self.Hybrid_Comm_Remove_Header_0 = Hybrid_Comm.Remove_Header(Preamble, Label_str, 1, len(Data)*4)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.Hybrid_Comm_Remove_Header_0, 0), (self.blocks_char_to_float_0, 0))
        self.connect((self.Hybrid_Comm_Remove_Header_0, 1), (self.blocks_char_to_float_0_0, 0))
        self.connect((self.Hybrid_Comm_Remove_Header_0, 2), (self.blocks_short_to_float_0, 0))
        self.connect((self.blocks_char_to_float_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_char_to_float_0_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_short_to_float_0, 0), (self.qtgui_time_sink_x_0, 2))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_stream_mux_1, 2))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_stream_mux_1, 0))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_stream_mux_1, 1))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_stream_mux_1, 3))
        self.connect((self.blocks_stream_mux_1, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.Hybrid_Comm_Remove_Header_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_mux_0, 0))
        self.connect((self.blocks_vector_source_x_0_0, 0), (self.blocks_stream_mux_0, 1))
        self.connect((self.blocks_vector_source_x_0_0_0, 0), (self.blocks_stream_mux_0, 2))
        self.connect((self.blocks_vector_source_x_0_0_0_0, 0), (self.blocks_stream_mux_0, 3))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "Remove_Header")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_Preamble(self):
        return self.Preamble

    def set_Preamble(self, Preamble):
        self.Preamble = Preamble
        self.Hybrid_Comm_Remove_Header_0.set_Preamble(self.Preamble)
        self.blocks_vector_source_x_0.set_data(self.Preamble, [])

    def get_LengthOffset(self):
        return self.LengthOffset

    def set_LengthOffset(self, LengthOffset):
        self.LengthOffset = LengthOffset

    def get_Label_str(self):
        return self.Label_str

    def set_Label_str(self, Label_str):
        self.Label_str = Label_str
        self.Hybrid_Comm_Remove_Header_0.set_Label(self.Label_str)

    def get_Label_bit(self):
        return self.Label_bit

    def set_Label_bit(self, Label_bit):
        self.Label_bit = Label_bit
        self.blocks_vector_source_x_0_0.set_data(self.Label_bit, [])

    def get_Data(self):
        return self.Data

    def set_Data(self, Data):
        self.Data = Data
        self.Hybrid_Comm_Remove_Header_0.set_PacketSampLen(len(self.Data)*4)
        self.blocks_multiply_const_vxx_0.set_k(1.0/len(self.Data))
        self.blocks_vector_source_x_0_0_0_0.set_data(self.Data, [])

    def get_Counter(self):
        return self.Counter

    def set_Counter(self, Counter):
        self.Counter = Counter
        self.blocks_vector_source_x_0_0_0.set_data(self.Counter, [])



def main(top_block_cls=Remove_Header, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start(100)
    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
