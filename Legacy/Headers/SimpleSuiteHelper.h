/*********************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
* Copyright 2025 Adobe
* All Rights Reserved.
*
* NOTICE: All information contained herein is, and remains
* the property of Adobe and its suppliers, if any. The intellectual
* and technical concepts contained herein are proprietary to Adobe
* and its suppliers and are protected by all applicable intellectual
* property laws, including trade secret and copyright laws.
* Dissemination of this information or reproduction of this material
* is strictly forbidden unless prior written permission is obtained
* from Adobe.
*********************************************************************/

#ifndef _H_SIMPLESUITEHELPER
#define _H_SIMPLESUITEHELPER

#include "A.h"
#include "SPBasic.h"
#include <cassert>

// Simple non-template suite helper class
template<typename SuiteType>
class SimpleSuiteHelper
{
public:
    SimpleSuiteHelper(const SPBasicSuite* basic_suiteP, const A_char* suite_name, int32_t suite_version);
    ~SimpleSuiteHelper();

    const SuiteType* operator->() const { return i_SuiteP; }
    SuiteType* get() const { return i_SuiteP; }

private:
    mutable SuiteType* i_SuiteP;
    const SPBasicSuite* const i_basic_suiteP;
    const A_char* i_suite_name;
    int32_t i_suite_version;
};

template<typename SuiteType>
SimpleSuiteHelper<SuiteType>::SimpleSuiteHelper(const SPBasicSuite* basic_suiteP, const A_char* suite_name, int32_t suite_version)
    : i_basic_suiteP(basic_suiteP), i_SuiteP(NULL), i_suite_name(suite_name), i_suite_version(suite_version)
{
    assert(basic_suiteP);
    assert(suite_name);
    
    const void* acquired_suite = NULL;
    A_Err err = i_basic_suiteP->AcquireSuite(i_suite_name, i_suite_version, &acquired_suite);
    if (err || !acquired_suite) {
        // Could throw or assert here, but for now just leave i_SuiteP as NULL
        assert(false);
    } else {
        i_SuiteP = reinterpret_cast<SuiteType*>(const_cast<void*>(acquired_suite));
    }
}

template<typename SuiteType>
SimpleSuiteHelper<SuiteType>::~SimpleSuiteHelper()
{
    if (i_SuiteP) {
        #ifdef DEBUG
            A_Err err = 
        #endif
        i_basic_suiteP->ReleaseSuite(i_suite_name, i_suite_version);
        
        #ifdef DEBUG
            assert(!err);
        #endif        
    }
}

#endif // _H_SIMPLESUITEHELPER 
