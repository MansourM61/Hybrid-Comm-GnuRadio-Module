#ifndef INCLUDED_DEFAULTS_H
#define INCLUDED_DEFAULTS_H


#define VAL_0                               (0)                                         // value representing bit 0
#define VAL_1                               (1)                                         // value representing bit 1
#define MIN_MATCH_VAL                       (75.0)                                      // minimum acceptable match (%)
#define MIN_VALID_VAL                       (0)                                         // minimum acceptable delay value
#define MAX_VALID_VAL                       (250)                                       // maximum acceptable delay value
#define BUFF_SIZE			                (5)                 						// stream aligner buffer size
#define DEF_VIS                             (20.0)                                      // default visibility (km)
#define DEF_VIS                             (20.0)                                      // default visibility (km)
#define DEF_WL                              (850.0)                                     // default wavelength (nm)
#define DEF_LINK_LEN                        (100.0)                                     // default link length (m)
#define LAMBDA_0                            (550.0)                                     // green laser wavelength (nm)
#define T_TH_FS                             (2.0/100.0)                                 // contrast threshold (2%)
#define DEF_DIA_TX                          (5.0)                                       // Tx aperture diameter (mm)
#define DEF_THETA_TX                        (0.1)                                       // Tx beam full-divergence angle (mDeg)
#define DEF_DIA_RX                          (50.0)                                      // Rx aperture diameter (mm)
#define DEF_JITTER                          (1.0)                                       // default jitter (mm)
#define PE_TEMP_CORR                        (10.0)                                      // pointing errors temporal correlation (ms)
#define SAMP_RATE                           (32000)                                     // sampling rate (bps)
#define DEF_CN2                             (1.0e-20)                                   // the Refractive Index Structure Coefficient (m^-2/3)
#define T_TEMP_CORR                         (10.0)                                      // turbulence temporal correlation (ms)
#define DEF_SPB                             (1)                                         // default samples per bit
#define FIND_EDGE_WIN_SIZE                  (2)                                         // finding first edge search window size
#define PACKET_SAMP_SIZE                    (1000)                                      // number of samples in a packet
#define PS_STR                              ("Signal Power")                            // signal power flag string
#define PN_STR                              ("Noise Power")                             // noise power flag string
#define SNR_STR                             ("Signal-to-noise Ratio (SNR)")             // signal-to-noise Ratio (SNR) flag string
#define QF_STR                              ("Q-factor")                                // Q-factor flag string
#define DC_STR                              ("DC Level")                                // DC level flag string
#define SI0_STR                             ("Level 0 SI")                              // Level 0 SI flag string
#define SI1_STR                             ("Level 1 SI")                              // Level 1 SI flag string
#define SIm_STR                             ("Average SI")                              // Average SI flag string

#endif /* INCLUDED_DEFAULTS_H */
