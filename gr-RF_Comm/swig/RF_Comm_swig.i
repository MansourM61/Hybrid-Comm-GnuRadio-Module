/* -*- c++ -*- */

#define RF_COMM_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "RF_Comm_swig_doc.i"

%{
#include "RF_Comm/Rain_Loss.h"
#include "RF_Comm/FSP_Loss.h"
%}

%include "RF_Comm/Rain_Loss.h"
GR_SWIG_BLOCK_MAGIC2(RF_Comm, Rain_Loss);
%include "RF_Comm/FSP_Loss.h"
GR_SWIG_BLOCK_MAGIC2(RF_Comm, FSP_Loss);
