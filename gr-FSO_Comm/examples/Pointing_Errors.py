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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import gr
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
import FSO_Comm
from gnuradio import qtgui

class Pointing_Errors(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "Pointing_Errors")

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
        self.tx_dia = tx_dia = 3
        self.theta = theta = 0.1
        self.samp_rate = samp_rate = 32000
        self.rx_dia = rx_dia = 50
        self.linklen = linklen = 100
        self.jitter = jitter = 200

        ##################################################
        # Blocks
        ##################################################
        self._tx_dia_range = Range(0, 100, 0.01, 3, 200)
        self._tx_dia_win = RangeWidget(self._tx_dia_range, self.set_tx_dia, 'Transmitter aperture diameter (mm)', "counter_slider", float)
        self.top_grid_layout.addWidget(self._tx_dia_win)
        self._theta_range = Range(0, 1000, 0.01, 0.1, 200)
        self._theta_win = RangeWidget(self._theta_range, self.set_theta, 'Transmitter full angle divergence (mDeg)', "counter_slider", float)
        self.top_grid_layout.addWidget(self._theta_win)
        self._rx_dia_range = Range(0, 100, 1, 50, 200)
        self._rx_dia_win = RangeWidget(self._rx_dia_range, self.set_rx_dia, 'Receiver aperture diameter (mm)', "counter_slider", float)
        self.top_grid_layout.addWidget(self._rx_dia_win)
        self._linklen_range = Range(0, 1000, 1, 100, 200)
        self._linklen_win = RangeWidget(self._linklen_range, self.set_linklen, 'Link length (m)', "counter_slider", float)
        self.top_grid_layout.addWidget(self._linklen_win)
        self._jitter_range = Range(0, 1000, 0.01, 200, 200)
        self._jitter_win = RangeWidget(self._jitter_range, self.set_jitter, 'Jitter (mm)', "counter_slider", float)
        self.top_grid_layout.addWidget(self._jitter_win)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            1024, #size
            samp_rate, #samp_rate
            "", #name
            2 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Before', 'After', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
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
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_float*1, samp_rate,True)
        self.analog_sig_source_x_0 = analog.sig_source_f(samp_rate, analog.GR_COS_WAVE, 1000, 1, 0, 0)
        self.FSO_Comm_Pointing_Errors_0 = FSO_Comm.Pointing_Errors(jitter, tx_dia, theta, rx_dia, 10.0, linklen, samp_rate)



        ##################################################
        # Connections
        ##################################################
        self.connect((self.FSO_Comm_Pointing_Errors_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.FSO_Comm_Pointing_Errors_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.qtgui_time_sink_x_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "Pointing_Errors")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_tx_dia(self):
        return self.tx_dia

    def set_tx_dia(self, tx_dia):
        self.tx_dia = tx_dia

    def get_theta(self):
        return self.theta

    def set_theta(self, theta):
        self.theta = theta

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_rx_dia(self):
        return self.rx_dia

    def set_rx_dia(self, rx_dia):
        self.rx_dia = rx_dia

    def get_linklen(self):
        return self.linklen

    def set_linklen(self, linklen):
        self.linklen = linklen

    def get_jitter(self):
        return self.jitter

    def set_jitter(self, jitter):
        self.jitter = jitter
        self.FSO_Comm_Pointing_Errors_0.set_Jitter(self.jitter)



def main(top_block_cls=Pointing_Errors, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
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
