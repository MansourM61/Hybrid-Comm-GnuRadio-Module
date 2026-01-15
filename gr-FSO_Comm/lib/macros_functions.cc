#include <FSO_Comm/macros_functions.h>

namespace gr {
    namespace FSO_Comm {


    Norm_Rand_Gen::Norm_Rand_Gen()
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Norm_Rand_Gen: Constructor called." << std::endl;
        #endif        
        fGaussGenArray[0] = 0.0;  // initialise first 2 random
        fGaussGenArray[1] = 0.0;
        iGaussGenCounter = 0;  // set counter to 0
        srand(time(0));  // initialise the random number generator
    }


    Norm_Rand_Gen::~Norm_Rand_Gen()
    {
    }


    float Norm_Rand_Gen::GaussNormNumGen(float mean, float std)  // normally distributed number generator
    {
        // the code is adopted from 'https://rosettacode.org/wiki/Statistics/Normal_distribution#C'
        ++iGaussGenCounter;

        // Marsaglia polar method
        // https://en.wikipedia.org/wiki/Marsaglia_polar_method
        if (iGaussGenCounter%2 != 0) {  // generating random numbers
            float x, y, rsq, f;  // temporary variables
            do {
                x = 2.0 * rand() / (float)RAND_MAX - 1.0;  // x point calculation
                y = 2.0 * rand() / (float)RAND_MAX - 1.0;  // y point calculation
                rsq = x * x + y * y;  // calculate distance
            } while(rsq >= 1.0 || rsq == 0.0);  // repeat the calculation till the points are in the square

            f = sqrt(-2.0*log(rsq)/rsq);  // auxiliary value
            fGaussGenArray[0] = x*f;  // first random value
            fGaussGenArray[1] = y*f;  // second random value

            return fGaussGenArray[0]*std + mean;
        }
        else {  // returning already calculated number
            return fGaussGenArray[1]*std + mean;
        }
    }

    float Norm_Rand_Gen::RayleighNumGen(float mult_cnt, float div_cnt)  // Rayleigh distributed number generator
    {
        float x, y;

        x = GaussNormNumGen(0.0, 1.0)*mult_cnt;
        y = GaussNormNumGen(0.0, 1.0)*mult_cnt;

        float r = sqrt( POW2(x) + POW2(y) );  // calculate random radius based on the normal distribution
        
        return exp( -2.0*POW2(r) / div_cnt );  // calculate Log-Normal random sequence
    }

    float Norm_Rand_Gen::LogNormalNumGen(float mu_x, float sig_x)  // log-normal distributed number generator
    {
        return exp( 2.0*GaussNormNumGen(mu_x, sig_x) );  // calculate Log-Normal random sequence
    }

    void Norm_Rand_Gen::NormDistArray(float *ptr_fInArray, int iArrayLen)  // fill input array with normally distributed numbers
    {
        for(int index = 0; index < iArrayLen; ++index)  // go through all elements
        {
            ptr_fInArray[index] = this->GaussNormNumGen();  // update the element
        }
    }

    void Norm_Rand_Gen::RayleighDistArray(float *ptr_fInArray, float p1, float p2, const int iArrayLen)  // fill input array with Rayleigh distributed numbers
    {
        for(int index = 0; index < iArrayLen; ++index)  // go through all elements
        {
            ptr_fInArray[index] = this->RayleighNumGen(p1, p2);  // update the element
        }
    }

    void Norm_Rand_Gen::UniformBinary(char *ptr_cInArray, int iArrayLen)  // fill input array with normally distributed binary numbers (VAL_0, VAL_1)
    {
        for(int index = 0; index < iArrayLen; ++index)  // go through all elements
        {
            ptr_cInArray[index] = (this->GaussNormNumGen() < 0.0) ? VAL_0 : VAL_1;  // update the element
        }
    }


    template <class T>
    const int Bank_Buff<T>::BankSize = BUFF_SIZE;  // number of empty slots


    template <class T>
    Bank_Buff<T>::Bank_Buff()  // default constructor
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: Default constructor called." << std::endl;
        #endif        
    }


    template <class T>
    Bank_Buff<T>::Bank_Buff(const int size)  // constructor
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: Constructor called." << std::endl;
        #endif

        ptr_Banks = nullptr;  // mark pointer as empty
        ptr_Order = nullptr;  // mark pointer as empty
        iTakenSlots = 0;  // set bank to empty

        ptr_Order = new int[BankSize];  // allocate memory for order array

        ptr_Banks = new T* [BankSize];  // allocate memory for bank slots
        for(int index = 0; index < BankSize; ++index)  // go through available bank slots
        {
            ptr_Banks[index] = nullptr;  // mark the slot as empty
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Order array is initialised" << std::endl;
        #endif
        FillArray<int>(ptr_Order, -1, BankSize);  // initialise the order array

        this->set_SlotSize(size);  // set buffer size
    }


    template <class T>
    Bank_Buff<T>::~Bank_Buff()  // destructor
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: Destructor called." << std::endl;
        #endif

        for(int index = 0; index < BankSize; ++index)  // go through available bank slots
        {
            if(ptr_Banks[index] != nullptr)  // if bank slot is not empty
            {
                delete[] ptr_Banks[index];  // release memory of each bank slot
                ptr_Banks[index]= nullptr;  // mark the slot as empty
            }
        }
        delete[] ptr_Banks;  // release the bank
        ptr_Banks = nullptr;  // mark the bank as empty

        delete[] ptr_Order;  // release the order array
        ptr_Order = nullptr;  // mark the order as empty

        iTakenSlots = 0;  // mark bank as empty
    }


    template <class T>
    void Bank_Buff<T>::set_SlotSize(const int slotSize)  // setter: iSlotSize
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: Bank buffer slot size = " << slotSize << std::endl;
        #endif

        iSlotSize = slotSize;  // update buffer size

        for(int index = 0; index < BankSize; ++index)  // go through available bank slots
        {
            if(ptr_Banks[index] != nullptr)  // if bank slot is not empty
            {
                delete[] ptr_Banks[index];  // release memory of each bank slot
                ptr_Banks[index]= nullptr;  // mark the slot as empty
            }
            ptr_Banks[index] = new T [iSlotSize];  // allocate slots memory
        }

        #ifdef _DEBUG_MODE_
        std::cout << "Order array is initialised" << std::endl;
        #endif
        FillArray<int>(ptr_Order, -1, BankSize);  // initialise the order array

        iTakenSlots = 0;  // mark bank as empty
    }


    template <class T>
    void Bank_Buff<T>::push(const T* inArray)  // insert given array after the oldest bank
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: push called." << std::endl;
        #endif

        int index_s = 0;  // available slot index
        if(iTakenSlots != BankSize)  // if the bank is not full
        {
            #ifdef _DEBUG_MODE_
            std::cout << "Bank_Buff: push: bank is not full." << std::endl;
            #endif
            for(int index = 0; index < BankSize; ++index)  // go through orders
            {
                if(ptr_Order[index] == -1)  // if the slot is empty
                {
                    index_s = index;  // update available slot index
                    break;
                }
            }
            ptr_Order[index_s] = iTakenSlots;  // update order array

            ++iTakenSlots;  // update number of taken slots
        }
        else  // if bank is full
        {
            #ifdef _DEBUG_MODE_
            std::cout << "Bank_Buff: push: bank is full." << std::endl;
            #endif
             for(int index = 0; index < BankSize; ++index)  // go through orders
            {
                if(ptr_Order[index] == 0)  // if the slot is oldest
                {
                    index_s = index;  // update available slot index
                    break;
                }
            }
            for(int index = 0; index < BankSize; ++index)  // go through orders
            {
                if(ptr_Order[index] != -1)  // if the slot is oldest
                {
                    --ptr_Order[index];
                }
            }
            ptr_Order[index_s] = BankSize - 1;  // update order array
        }
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: push: index_s = " << index_s << std::endl;
        std::cout << "Bank_Buff: push: iTakenSlots = " << iTakenSlots << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Order = ";
        DisplayArray(ptr_Order, BankSize);
        std::cout << std::endl;
        #endif
        CopyArrays<T>(inArray, ptr_Banks[index_s], iSlotSize);  // insert the array into the available slot
    }


    template <class T>
    int Bank_Buff<T>::pop(T* outArray) // extract the oldest array from the bank; return -1 if failed
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: pop called." << std::endl;
        #endif

        if(iTakenSlots == 0)  // no slot available
        {
            return -1;  // return error code
        }

        int index_s = 0;  // oldest slot index

        for(int index = 0; index < BankSize; ++index)  // go through orders
        {
            if(ptr_Order[index] == 0)  // if the slot is oldest
            {
                index_s = index;  // update available slot index
                break;
            }
        }
        for(int index = 0; index < BankSize; ++index)  // go through orders
        {
            if(ptr_Order[index] != -1)  // if the slot is oldest
            {
                --ptr_Order[index];
            }
        }
        
        --iTakenSlots;  // update number of taken slots

        ptr_Order[index_s] = -1;  // mark the slot as empty

        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: push: index_s = " << index_s << std::endl;
        std::cout << "Bank_Buff: push: iTakenSlots = " << iTakenSlots << std::endl;
        #endif

        #ifdef _ARRAY_MODE_
        std::cout << "Order = ";
        DisplayArray(ptr_Order, BankSize);
        std::cout << std::endl;
        #endif
        CopyArrays<T>(ptr_Banks[index_s], outArray, iSlotSize);  // insert the slot into the array

        return index_s;  // return index of exchanged slot
    }


    template <class T>
    void Bank_Buff<T>::clear(void) // reset the bank to empty state
    {
        #ifdef _DEBUG_MODE_
        std::cout << "Bank_Buff: clear called." << std::endl;
        #endif

        if (ptr_Order != nullptr)  // if the order array is not empty
        {
            FillArray<int>(ptr_Order, -1, BankSize);  // set the order array to empty
        }

        iTakenSlots = 0;  // mark bank as empty
    }


    template <class T>
    void DisplayArray(const T *ptr_inArray, const int iArrayLen, const int iOffset)  // display the array items
    {
        #ifdef _ARRAY_MODE_
        std::cout << "[";
        for(int index = 0; index < iArrayLen; index++)  // go through the samples within the window
        {
            if(sizeof(T) == sizeof(char))  // if tempelate is 'character'
            {
                std::cout << CPRN(ptr_inArray[index + iOffset]) << ", ";
            }
            else  //otherwise
            {
                std::cout << ptr_inArray[index + iOffset] << ", ";
            }            
        }
        std::cout << "]";
        #endif
    }


    template <class T> 
    float DC_Value(const T *ptr_inArray, const int iArrayLen, const int iOffset)  // find DC value of the signal
    {
        float DC = 0.0;  // inital DC value

        for(int index = 0; index < iArrayLen; index++)  // go through the samples within the window
        {
            DC += ptr_inArray[iOffset + index];  // update summation
        }
        return DC / iArrayLen;  // return DC value
    }


    template <class T> 
    int FindFirstEdge(const T *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int edgeFindingRange)  // find the first edge index
    {
        int firstEdge = 0;  // initial edge index
        for(int index = 0; index < iArrayLen - (edgeFindingRange - 1); index++)  // go through the samples within the window
        {
            if(((ptr_inArray[iOffset + index] < fThresh) &&
                (ptr_inArray[iOffset + index + (edgeFindingRange - 1)] >= fThresh)) ||
                ((ptr_inArray[iOffset + index] > fThresh) &&
                (ptr_inArray[iOffset + index + (edgeFindingRange - 1)] <= fThresh)))  // if it is an edge
            {
            firstEdge = index++;  // update edge index
            break;  // leave the loop
            }
        }

        return firstEdge;  // return index of first edge
    }


    template <class T> 
    int FindFirstRisingEdge(const T *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int edgeFindingRange)  // find the first rising edge index
    {
        int firstRisingEdge = 0;  // initial rising edge index
        for(int index = 0; index < iArrayLen - (edgeFindingRange - 1); index++)  // go through the samples within the window
        {
            if((ptr_inArray[iOffset + index] < fThresh) &&
            (ptr_inArray[iOffset + index + (edgeFindingRange - 1)] >= fThresh))  // if it is a rising edge
            {
            firstRisingEdge = index++;  // update rising edge index
            break;  // leave the loop
            }
        }

        return firstRisingEdge;  // return index of first rising edge
    }


    template <class T> 
    int FindFirstFallingEdge(const T *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int edgeFindingRange)  // find the first falling edge index
    {
        int firstFallingEdge = 0;  // initial falling edge index
        for(int index = 0; index < iArrayLen - (edgeFindingRange - 1); index++)  // go through the samples within the window
        {
            if((ptr_inArray[iOffset + index] > fThresh) &&
            (ptr_inArray[iOffset + index + (edgeFindingRange - 1)] <= fThresh))  // if it is a falling edge
            {
            firstFallingEdge = index++;  // update falling edge index
            break;  // leave the loop
            }
        }

        return firstFallingEdge;  // return index of first falling edge
    }


    template <class T> 
    void CalcMeanVar(const T *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int iSpS, int *ptr_iLen, float *prt_fRes)  // calculate mean and variance at levels 0 and 1
    {
        float x2_0 = 0.0;  // noise variance for level 0 dummy variable
        float x2_1 = 0.0;  // noise variance for level 1 dummy variable

        float x_0 = 0.0;  // mean value for level 0 dummy variable
        float x_1 = 0.0;  // mean value for level 1 dummy variable

        int n_0 = 0;  // number of level 0
        int n_1 = 0;  // number of level 1

        for(int index = 0; index < iArrayLen; index += iSpS)  // go through the samples at the centre of each bit  within the window
        {
            if(ptr_inArray[iOffset + index] < fThresh)  // if it is level 0
            {
            ++n_0;  // update total number of level 0
            x_0 += ptr_inArray[iOffset + index];  // update mean value
            x2_0 += POW2(ptr_inArray[iOffset + index]);  // update variance
            } 
            else  // if it is a level 1
            {
            ++n_1;  // update total number of level 0
            x_1 += ptr_inArray[iOffset + index];  // update mean value
            x2_1 += POW2(ptr_inArray[iOffset + index]);  // update variance
            }
        }

        float mean_0 = x_0/n_0;  // calculate mean value of level 0
        float mean_1 = x_1/n_1;  // calculate mean value of level 1

        float var_0 = x2_0/(n_0 - 1) - POW2(mean_0)*n_0/(n_0 - 1);  // calculate variance value of level 0
        float var_1 = x2_1/(n_1 - 1) - POW2(mean_1)*n_1/(n_1 - 1);  // calculate variance value of level 1

        #ifdef _DEBUG_MODE_
        std::cout << "mean_0 = " << mean_0 << std::endl;
        std::cout << "mean_1 = " << mean_1 << std::endl;
        std::cout << "var_0:1 = " << x2_0/(n_0 - 1) << std::endl;
        std::cout << "var_0:2 = " << POW2(mean_0)*n_0 << std::endl;
        std::cout << "var_1:1 = " << x2_1/(n_1 - 1) << std::endl;
        std::cout << "var_1:2 = " << POW2(mean_1)*n_1 << std::endl;
        #endif

        // update length array
        ptr_iLen[0] = n_0;
        ptr_iLen[1] = n_1;

        // update result array
        prt_fRes[0] = mean_0;
        prt_fRes[1] = mean_1;
        prt_fRes[2] = var_0;
        prt_fRes[3] = var_1;

        return;
    }



    template <class T> 
    void FillArray(T *ptr_array, T value, const int iArrayLen)  // fill the array with given value
    {
        for(int index = 0; index < iArrayLen; index++)  // go through the array elements
        {
            ptr_array[index] = value;  // update array element
        }
    }


    template <class T> 
    void InterpArray(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen, const int iInterRatio)  // interpolate input array
    {
        for(int index = 0; index < iArrayLen; ++index)  // go through input items
        {
            FillArray<T>((ptr_outArray + index*iInterRatio), ptr_inArray[index], iInterRatio);
        }
    }

    template <class T> 
    void CopyArrays(const T *ptr_srcArray, T *ptr_destArray, const int iArrayLen)  // copies 'iArrayLen' contents of 'ptr_srcArray' to 'ptr_destArray'
    {
        for(int index = 0; index < iArrayLen; index++)  // go through the array elements
        {
            ptr_destArray[index] = ptr_srcArray[index];  // update destination array element with source array element
        }   
    }


    template <class T>
    void Num2Bits(const T inArg, char *ptr_destArray, const int iSeqLen)  // converts input number to equivalent bit sequence
    {
        for(int index_b = 0; index_b < iSeqLen; ++index_b)  // go through the letter bits
        {
            ptr_destArray[index_b] = ( inArg & (1 << index_b) ) >> index_b;  // extract bit from the character
            
        }
    }


    template <class T> 
    void AddArrays(const T *ptr_array_1, const T *ptr_array_2, T *ptr_outArray, const int iArrayLen)  // copies 'iArrayLen' contents of 'ptr_array_1' + 'ptr_array_2' to 'ptr_destArray'
    {
        for(int index = 0; index < iArrayLen; index++)  // go through the array elements
        {
            ptr_outArray[index] = ptr_array_1[index] + ptr_array_2[index];  // update array element
        }
    }


    template <class T> 
    void SubtractArrays(const T *ptr_array_1, const T *ptr_array_2, T *ptr_outArray, const int iArrayLen)  // copies 'iArrayLen' contents of 'ptr_array_1' - 'ptr_array_2' to 'ptr_destArray'
    {
        for(int index = 0; index < iArrayLen; index++)  // go through the array elements
        {
            ptr_outArray[index] = ptr_array_1[index] - ptr_array_2[index];  // update array element
        }
    }


    template <class T> 
    void LinearMap(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen, const T fA, const T fB)  // apply linear map to input array
    {
        for(int index = 0; index < iArrayLen; index++)  // go through the array elements
        {
            ptr_outArray[index] = (T)(ptr_inArray[index]*fA + fB);  // update array element
        }
    }


    template <class T>
    int MatchBitSeq(const T *ptr_inArray, const T *ptr_inPattern, T *ptr_auxPattern, const int iArrayLen, const int iPatternLen)  // find index of first matching of pattern and the input array
    {
        bool bMemAlloc = false;  // memory flag
        if (ptr_auxPattern == nullptr)  // if no auxillary memory is provided
        {
            ptr_auxPattern = new T [iPatternLen];  // allocate the memory
            bMemAlloc = true;  // set the memory flag
        }

        int test;
        int index_Match = -1;
        int index_Min = -1;
        int Min_Val = INT_MAX;  // initial index of minimum value
        
        for(int index_T = 0; index_T < (iArrayLen - iPatternLen); ++index_T)  // go through available element in the input array
        {
            SubtractArrays(ptr_inPattern, (ptr_inArray + index_T), ptr_auxPattern, iPatternLen);  // calculate the difference

            test = ApplySum<int, char, int>(ptr_auxPattern, abs, iPatternLen);//0;  // initialise the test

            if(test == 0)  // if it is a match
            {
                index_Match = index_T;
                break;
            }

            if(test < Min_Val)  // if the difference is minimum
            {
                index_Min = index_Match;  // update the index of minimum
                Min_Val = test;  // update minimum value
            }
        }

        Min_Val = (Min_Val <= MIN_MATCH_VAL*(iArrayLen - iPatternLen)) ? -1 : Min_Val;  // make sure minimum match is met

        if (bMemAlloc == true)  // if no axillary memory is provided
        {
            delete[] ptr_auxPattern;  // release the memory
            ptr_auxPattern = nullptr;  // label the array as empty
        }

        return (index_Match == -1) ? index_Min : index_Match;  // return the matching index or index of minimum match
    }


    template <class T>
    void DecimatArray(const T *ptr_inArray, T *ptr_outArray, const int iArrayLen, const int iDecimatRatio)  // decimation of input array
    {
        int index_O = 0;  // ouput array index
        for(int index = 0; index < iArrayLen; index += iDecimatRatio)  // go through input items
        {
            ptr_outArray[index_O++] = ptr_inArray[index];  // update output array
            
        }
    }


    template <class T>
    T Bits2Num(const char *ptr_inArray, const int iSeqLen)  // converts input bit sequence to equivalent number
    {
        T res = 0;  // initialise the number

        for(int index_b = 0; index_b < iSeqLen; ++index_b)  // go through the letter bits
        {
            res += ptr_inArray[index_b] << index_b;  // convert bit to equivalent decimal number
            
        }
        
        return res;  // return the convertion result
    }


    template <class T, class U, class V>
    T ApplySum(const U *ptr_inArray, V (*f)(V), const int iArrayLen)  // apply given function to each element in the array and then sum them
    {
        T res = 0;  // initialise the number

        for(int index = 0; index < iArrayLen; ++index)  // go through the letter bits
        {
            res += (*f)(ptr_inArray[index]);  // apply the function and sum it up
            
        }
        
        return res;  // return the convertion result       
    }


    template <class T>
    T Sum(const T *ptr_inArray, const int iArrayLen)  // returns summation of array elements
    {
        T res = 0;  // initialise the number

        for(int index = 0; index < iArrayLen; ++index)  // go through the letter bits
        {
            res += ptr_inArray[index];  // apply the function and sum it up
            
        }
        
        return res;  // return the convertion result 
    }


    template <class T, class U>
    void Apply(const T *ptr_inArray, T *ptr_outArray, U (*f)(U), const int iArrayLen)  // returns summation of array elements
    {
        for(int index = 0; index < iArrayLen; ++index)  // go through the letter bits
        {
            ptr_outArray[index] = (*f)(ptr_inArray[index]);  // apply the function and sum it up
            
        }
    }


    template <class T>
    float WeightAverage(const T *ptr_inArray, const float *prt_fWeights, const int iCentreIndex, const int iArrayLen, const int iWeightLen)  // returns weighted average of input
    {
        int Index_L = 0;  // index of input array; left side
        int Index_R = 0;  // index of input array; right side
        float sum = ptr_inArray[iCentreIndex] * prt_fWeights[0];  // calculate central value
        float weight = prt_fWeights[0];  // update weight value
        #ifdef _DEBUG_MODE_
        std::cout << "----------------------------------------------------------" << std::endl;
        std::cout << "WeightAverage: Index " << 0 << " out of " << iWeightLen - 1 << std::endl;
        std::cout << "WeightAverage: Central index = " << iCentreIndex << std::endl;
        std::cout << "WeightAverage: Central weight = " << prt_fWeights[0] << std::endl;
        #endif

        for(int index = 1; index < iWeightLen; ++index)  // go through left and right sides elements
        {
            #ifdef _DEBUG_MODE_
            std::cout << "WeightAverage: Index " << index << " out of " << iWeightLen - 1 << std::endl;
            #endif
                    
            Index_L = iCentreIndex - index;  // update index of input array; left side
            if( (Index_L >= 0) )  // if it's a valid index
            {
                #ifdef _DEBUG_MODE_
                std::cout << "WeightAverage: Left index = " << Index_L << std::endl;
                std::cout << "WeightAverage: Left weight = " << prt_fWeights[index] << std::endl;
                #endif
                sum += ptr_inArray[Index_L] * prt_fWeights[index];  // update central value
                weight += prt_fWeights[index];  // update weight value
            }

            Index_R = iCentreIndex + index;  // update index of input array; right side
             if( (Index_R < iArrayLen) )  // if it's a no valid index
            {
                #ifdef _DEBUG_MODE_
                std::cout << "WeightAverage: Right index = " << Index_R << std::endl;
                std::cout << "WeightAverage: Right weight = " << prt_fWeights[index] << std::endl;
                #endif
                sum += ptr_inArray[Index_R] * prt_fWeights[index];  // update central value
                weight += prt_fWeights[index];  // update weight value
            }
        }

        return (weight != 0.0) ? sum/weight : 0.0;
    }


    // template definitions; this is done for C++ tempelate incompatibility with SWIG.
    template class Bank_Buff<char>;  // bank buffer class

    template void DisplayArray<char>(const char *ptr_inArray, const int iArrayLen, const int iOffset);  // display the array items - <char>
    template void DisplayArray<short>(const short *ptr_inArray, const int iArrayLen, const int iOffset);  // display the array items - <short>
    template void DisplayArray<int>(const int *ptr_inArray, const int iArrayLen, const int iOffset);  // display the array items - <char>
    template void DisplayArray<float>(const float *ptr_inArray, const int iArrayLen, const int iOffset);  // display the array items - <float>

    template float DC_Value<float>(const float *ptr_inArray, const int iArrayLen, const int iOffset);  // find DC value of the signal - <float>
    template float DC_Value<char>(const char *ptr_inArray, const int iArrayLen, const int iOffset);  // find DC value of the signal - <char>

    template int FindFirstEdge<float>(const float *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int edgeFindingRange);  // find the first edge index - <float>

    template void CalcMeanVar<float>(const float *ptr_inArray, const int iArrayLen, const int iOffset, const float fThresh, const int iSpS, int *ptr_iLen, float *prt_fRes);  // calculate mean and variance at levels 0 and 1 - <float>

    template void FillArray<float>(float *ptr_array, const float value, const int iArrayLen);  // fill the array with given value - <float>
    template void FillArray<char>(char *ptr_array, const char value, const int iArrayLen);  // fill the array with given value - <char>
    template void FillArray<short>(short *ptr_array, const short value, const int iArrayLen);  // fill the array with given value - <short>
    template void FillArray<int>(int *ptr_array, const int value, const int iArrayLen);  // fill the array with given value - <int>

    template void InterpArray<char>(const char *ptr_inArray, char *ptr_outArray, const int iArrayLen, const int iInterRatio);  // interpolate of input array - <char>

    template void CopyArrays<char>(const char *ptr_srcArray, char *ptr_destArray, const int iArrayLen);  // copies 'iArrayLen' contents of 'ptr_srcArray' to 'ptr_destArray' - <char>
    template void CopyArrays<float>(const float *ptr_srcArray, float *ptr_destArray, const int iArrayLen);  // copies 'iArrayLen' contents of 'ptr_srcArray' to 'ptr_destArray' - <float>

    template void Num2Bits<char>(const char inArg, char *ptr_destArray, const int iSeqLen);  // converts input number to equivalent bit sequence - <char>

    template int MatchBitSeq<char>(const char *ptr_inArray, const char *ptr_inPattern, char *ptr_auxPattern, const int iArrayLen, const int iPatternLen);  // find index of first matching of pattern and the input array - <char>

    template void SubtractArrays<char>(const char *ptr_array_1, const char *ptr_array_2, char *ptr_fOutArray, const int iArrayLen);  // copies 'iArrayLen' contents of 'ptr_array_1' - 'ptr_array_2' to 'ptr_destArray' - <char>

    template void DecimatArray<char>(const char *ptr_inArray, char *ptr_outArray, const int iArrayLen, const int iDecimatRatio);  // decimation of input array

    template char Bits2Num<char>(const char *ptr_inArray, const int iSeqLen);  // converts input bit sequence to equivalent number
    template short Bits2Num<short>(const char *ptr_inArray, const int iSeqLen);  // converts input bit sequence to equivalent number

    template int ApplySum<int, char, int>(const char *ptr_inArray, int (*f)(int), const int iArrayLen);  // apply given function to each element in the array and then sum them - <char, char>

    template float WeightAverage<float>(const float *ptr_inArray, const float *prt_fWeights, const int iCentreIndex, const int iArrayLen, const int iWeightLen);  // returns weighted average of input - <char>

    template void LinearMap<float>(const float *ptr_inArray, float *ptr_outArray, const int iArrayLen = 0, const float fA = 1, const float fB = 0);  // apply linear map to input array - <float>

  } /* namespace FSO_Comm */
} /* namespace gr */