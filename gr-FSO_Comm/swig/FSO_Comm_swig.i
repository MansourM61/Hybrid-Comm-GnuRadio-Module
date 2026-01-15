/* -*- c++ -*- */

#define FSO_COMM_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "FSO_Comm_swig_doc.i"

%{
#include "FSO_Comm/FogSmoke_Loss.h"
#include "FSO_Comm/Geometric_Loss.h"
#include "FSO_Comm/Pointing_Errors.h"
#include "FSO_Comm/Turbulence.h"
#include "FSO_Comm/Channel_Analyser.h"
%}

%include "FSO_Comm/FogSmoke_Loss.h"
GR_SWIG_BLOCK_MAGIC2(FSO_Comm, FogSmoke_Loss);
%include "FSO_Comm/Geometric_Loss.h"
GR_SWIG_BLOCK_MAGIC2(FSO_Comm, Geometric_Loss);
%include "FSO_Comm/Pointing_Errors.h"
GR_SWIG_BLOCK_MAGIC2(FSO_Comm, Pointing_Errors);
%include "FSO_Comm/Turbulence.h"
GR_SWIG_BLOCK_MAGIC2(FSO_Comm, Turbulence);

%include "FSO_Comm/Channel_Analyser.h"
GR_SWIG_BLOCK_MAGIC2(FSO_Comm, Channel_Analyser);
