/* -*- c++ -*- */

#define HYBRID_COMM_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "Hybrid_Comm_swig_doc.i"

%{
#include "Hybrid_Comm/Add_Header.h"
#include "Hybrid_Comm/Hysteresis_Gate.h"
#include "Hybrid_Comm/Rx_Parallel_Switch.h"
#include "Hybrid_Comm/Rx_Hard_Switch.h"
#include "Hybrid_Comm/Rx_Soft_Switch.h"
#include "Hybrid_Comm/Signal_Quality_Metre.h"
#include "Hybrid_Comm/Source_BV.h"
#include "Hybrid_Comm/Step_Gate.h"
#include "Hybrid_Comm/Tx_Parallel_Switch.h"
#include "Hybrid_Comm/Remove_Header.h"
#include "Hybrid_Comm/Stream_Aligner.h"
#include "Hybrid_Comm/Link_Tester.h"
#include "Hybrid_Comm/Slicer.h"
#include "Hybrid_Comm/Tx_Hard_Switch.h"
#include "Hybrid_Comm/Tx_Soft_Switch.h"

%}


%include "Hybrid_Comm/Add_Header.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Add_Header);
%include "Hybrid_Comm/Hysteresis_Gate.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Hysteresis_Gate);
%include "Hybrid_Comm/Rx_Parallel_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Rx_Parallel_Switch);
%include "Hybrid_Comm/Rx_Hard_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Rx_Hard_Switch);
%include "Hybrid_Comm/Rx_Soft_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Rx_Soft_Switch);
%include "Hybrid_Comm/Signal_Quality_Metre.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Signal_Quality_Metre);
%include "Hybrid_Comm/Source_BV.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Source_BV);
%include "Hybrid_Comm/Step_Gate.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Step_Gate);
%include "Hybrid_Comm/Tx_Parallel_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Tx_Parallel_Switch);
%include "Hybrid_Comm/Remove_Header.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Remove_Header);
%include "Hybrid_Comm/Stream_Aligner.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Stream_Aligner);
%include "Hybrid_Comm/Link_Tester.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Link_Tester);
%include "Hybrid_Comm/Slicer.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Slicer);
%include "Hybrid_Comm/Tx_Hard_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Tx_Hard_Switch);
%include "Hybrid_Comm/Tx_Soft_Switch.h"
GR_SWIG_BLOCK_MAGIC2(Hybrid_Comm, Tx_Soft_Switch);

