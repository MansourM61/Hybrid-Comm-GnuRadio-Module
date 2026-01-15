#ifndef INCLUDED_DEFAULTS_H
#define INCLUDED_DEFAULTS_H


#define VAL_0                               (0)                                         // value representing bit 0
#define VAL_1                               (1)                                         // value representing bit 1
#define MIN_MATCH_VAL                       (75.0)                                      // minimum acceptable match (%)
#define MIN_VALID_VAL                       (0)                                         // minimum acceptable delay value
#define MAX_VALID_VAL                       (250)                                       // maximum acceptable delay value
#define INV_SIG_VAL                         (255)                                       // use this value to label a signal as invalid
#define DEF_SIG_VAL                         (0)                                         // use this when initialisng the signal to default value
#define PULSE_MISAL_ERR                     (251)                                       // pulses misalignment error
#define NONZERO_IND_ERR                     (252)                                       // non zero index alignment error
#define LONG_DELAY_ERR                      (253)                                       // delay is too long for buffer
#define FILLING_BUF_ERR                     (255)                                       // filling buffer error
#define COUNTER_BITS                        (16)                                        // number of bits allocated for counter 
#define BITS_PER_LETT                       (8)                                         // number of bits per letter; for label allocation within the packet header
#define OUT_NUM_MP                          (5)                                         // number of output items multiple
#define PACKET_SAMP_SIZE                    (1000)                                      // number of samples in a packet
#define SAMPLE_RATE                         (3200)                                      // default sample rate
#define DEF_THRESH                          (0.0)                                       // default threshold
#define FIND_EDGE_WIN_SIZE                  (2)                                         // finding first edge search window size
#define AVG_WIN_SIZE                        (10)                                        // averaging window size
#define DEF_SPB                             (1)                                         // default samples per bit
#define DEF_FOR_PT                          (-1.0)                                      // default forward point
#define DEF_FOR_SL                          (+1.0)                                      // default forward slope
#define DEF_FOR_SS                          (+1.0)                                      // default forward saturation state
#define DEF_BAK_PT                          (+1.0)                                      // default backward point
#define DEF_BAK_SL                          (-1.0)                                      // default backward slope
#define DEF_BAK_SS                          (-1.0)                                      // default backward saturation state
#define FOR_STR                             ("Forward")                                 // forward flag string
#define BAK_STR                             ("Backward")                                // backward flag string
#define PS_STR                              ("Signal Power")                            // signal power flag string
#define PN_STR                              ("Noise Power")                             // noise power flag string
#define SNR_STR                             ("Signal-to-noise Ratio (SNR)")             // signal-to-noise Ratio (SNR) flag string
#define QF_STR                              ("Q-factor")                                // Q-factor flag string
#define CNT_STR                             ("Constant")                                // constant flag string
#define RND_STR                             ("Random")                                  // random flag string
#define BUFF_SIZE			                (5)                 						// stream aligner buffer size


#endif /* INCLUDED_DEFAULTS_H */
