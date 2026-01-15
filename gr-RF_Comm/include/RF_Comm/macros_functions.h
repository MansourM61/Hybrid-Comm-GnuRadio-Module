#ifndef INCLUDED_MACROS_FUNCTIONS_H
#define INCLUDED_MACROS_FUNCTIONS_H


#include <cmath>
#include <climits>
#include <ctime>


#ifdef _DEBUG_MODE_
#include <iostream>
#endif

#ifdef _ARRAY_MODE_
#include <iostream>
#endif

#ifdef _THREAD_MUTEX_
#include <gnuradio/thread/thread.h>
#endif

#include "defaults.h"


#define MIN(x, y)                           (((x) < (y)) ? (x) : (y))                   // minimum value
#define MAX(x, y)                           (((x) > (y)) ? (x) : (y))                   // maximum value
#define CONSTRAIN(x, lowLim, highLim)       (MIN(MAX((x), (lowLim)), (highLim)))        // return a constraint value of x within [lowlim, highLim] range*/
#define POW2(x)                             ((x)*(x))                                   // power of 2
#define EQUAL(x, y)                         (abs((x) - (y)) <= (10.0*FLT_MIN))          // if two float values are the almost same
#define CPRN(x)                             (int(x))                                    // character print version
#define CH2ASC(x)                           (char(x + '0'))                             // character number to ascii character
#define IS_VALID(x)                         (((x) <= MAX_VALID_VAL) && \
                                             ((x) >= MIN_VALID_VAL))                     // checks if control signal shows a vlid signal input    


namespace gr {
    namespace RF_Comm {
        
        // normal random number/bit generator
        class Norm_Rand_Gen
        {
            private:
            float fGaussGenArray[2];  // random array storage
            int iGaussGenCounter;  // generator counter

            public:
            Norm_Rand_Gen();
            ~Norm_Rand_Gen();
            float GaussNormNumGen(void);  // normally distributed number generator
            void NormDistArray(float *ptr_fInArray, const int iArrayLen = 0);  // fill input array with normally distributed numbers
            void UniformBinary(char *ptr_cInArray, const int iArrayLen = 0);  // fill input array with normally distributed binary numbers (0, 1)
        };

        // bank buffer class
        template <class T>
        class Bank_Buff
        {
            private:
            int iSlotSize;  // bank buffer slot size
            T **ptr_Banks;  // elements banks
            int *ptr_Order;  // bank order
            int iTakenSlots;  // taken slots counter

            public:
            static const int BankSize;  // number of empty slots
            Bank_Buff();  // default constructor
            Bank_Buff(const int size);  // constructor
            ~Bank_Buff();  // destructor

            void set_SlotSize(const int slotSize);  // setter: iSlotSize
            int get_SlotSize(void)  // getter: iSlotSize
            {
                return iSlotSize;
            }

            int get_TakenSlots(void)  // getter: iTakenSlots
            {
                return iTakenSlots;
            }

            void push(const T* inArray);  // insert given array after the oldest bank
            int pop(T* outArray);  // extract the oldest array from the bank; return -1 if failed
            void clear(void);  // reset the bank to empty state
        };


        template <class T>
        void DisplayArray(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0);  // display the array items

        template <class T>
        float DC_Value(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0);  // find DC value of the signal

        template <class T>
        int FindFirstEdge(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0, const float fThresh = 0, const int edgeFindingRange = 2);  // find the first edge index

        template <class T>
        int FindFirstRisingEdge(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0, const float fThresh = 0, const int edgeFindingRange = 2);  // find the first rising edge index

        template <class T>
        int FindFirstFallingEdge(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0, const float fThresh = 0, const int edgeFindingRange = 2);  // find the first falling edge index

        template <class T>
        void CalcMeanVar(const T *ptr_inArray, const int iArrayLen = 0, const int iOffset = 0, const float fThresh = 0, const int iSpS = 1, int *ptr_iLen = nullptr, float *prt_fRes = nullptr);  // calculate mean and variance at levels 0 and 1

        template <class T>
        void FillArray(T *ptr_array, const T value = 0, const int iArrayLen = 0);  // fill the array with given value

        template <class T>
        void InterpArray(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen = 0, const int iInterRatio = 1);  // interpolate of input array

        template <class T>
        void CopyArrays(const T *ptr_srcArray, T *ptr_destArray, const int iArrayLen = 0);  // copies 'iArrayLen' contents of 'ptr_srcArray' to 'ptr_destArray'

        template <class T>
        void Num2Bits(const T inArg, char *ptr_destArray, const int iSeqLen = 8);  // converts input number to equivalent bit sequence

        template <class T>
        void AddArrays(const T *ptr_array_1, const T *ptr_array_2, T *ptr_fOutArray, const int iArrayLen = 0);  // copies 'iArrayLen' contents of 'ptr_array_1' + 'ptr_array_2' to 'ptr_destArray'

        template <class T>
        void SubtractArrays(const T *ptr_array_1, const T *ptr_array_2, T *ptr_fOutArray, const int iArrayLen = 0);  // copies 'iArrayLen' contents of 'ptr_array_1' - 'ptr_array_2' to 'ptr_destArray'

        template <class T>
        void LinearMap(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen = 0, const T fA = 1, const T fB = 0);  // apply linear map to input array

        template <class T>
        int MatchBitSeq(const T *ptr_inArray, const T *ptr_inPattern, T *ptr_auxPattern = nullptr, const int iArrayLen = 0, const int iPatternLen = 0);  // find index of first matching of pattern and the input array

        template <class T>
        void DecimatArray(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen = 0, const  int iDecimatRatio = 1);  // decimation of input array

        template <class T>
        T Bits2Num(const char *ptr_inArray, const int iSeqLen = 8);  // converts input bit sequence to equivalent number

        template <class T, class U, class V>
        T ApplySum(const U *ptr_inArray, V (*f)(V), const int iArrayLen = 0);  // apply given function to each element in the array and then sum them

        template <class T>
        T Sum(const T *ptr_inArray, const int iArrayLen = 0);  // returns summation of array elements

        template <class T, class U>
        void Apply(const T *ptr_inArray, T *ptr_outArray, U (*f)(U), const int iArrayLen = 0);  // returns summation of array elements

        template <class T>
        float WeightAverage(const T *ptr_inArray, const float *prt_fWeights, const int iCentreIndex = 0, const int iArrayLen = 0, const int iWeightLen = 0);  // returns weighted average of input

  } // namespace RF_Comm
} // namespace gr

#endif /* INCLUDED_MACROS_FUNCTIONS_H */
