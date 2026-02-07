#ifndef _H_AEFX_SUITE_HELPER_TEMPLATE
#define _H_AEFX_SUITE_HELPER_TEMPLATE

#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <SPBasic.h>
#include <SPSuites.h>

/***********************************************************************************************************
	*** HACK to undefine MSWindows ***

	MSWindow is being defined on Mac Platforms from the file:
	.../dva/build/mb/macos/remote/dva/artifacts_manifest/photoshop-pie.pie/pie-macos-1.51.0/bin/AdobePSL/photoshop_sdk/config/photoshop/config/platform.hpp
	It appears that originally this code was commented out. But in new code (1.51) it is uncommented:
	```
	// REVISIT (sparent) : This also come from Switches.h... sometimes. Untangle
	//#ifndef MSWindows
	//  #define MSWindows (PHOTOSHOP_PLATFORM(MICROSOFT))
	//#endif
	```
	We cannot modify platform.hpp because it's coming from the Photoshop SDK. Instead will undef MSWindows where
	we include platform.hpp
*************************************************************************************************************/
#if PS_PLATFORM_APPLE
#ifdef MSWindows
#undef MSWindows
#endif
#endif

#ifdef __cplusplus

// Throws A_Err_MISSING_SUITE if acquisition fails and the second template argument, ALLOW_NO_SUITE, is set to false
template<typename SUITETYPE, bool ALLOW_NO_SUITE = false>
class AEFX_SuiteScoper
{
public:
    
    AEFX_SuiteScoper(
        SPBasicSuite *pica_basicP,
        const char *suite_name,
        int32_t suite_versionL)
    {
        i_suite_name = suite_name;
        i_suite_versionL = suite_versionL;
        i_basic_suiteP = pica_basicP;

        const void *suiteP = nullptr;
        SPErr err = i_basic_suiteP->AcquireSuite(i_suite_name, i_suite_versionL, &suiteP);

        if (err != kSPNoError)
        {
            if (ALLOW_NO_SUITE)
            {
                suiteP = NULL;
            }
            else
            {
                A_THROW(A_Err_MISSING_SUITE);
            }
        }

        i_suiteP = reinterpret_cast<const SUITETYPE*>(suiteP);
    }

    AEFX_SuiteScoper(
        const PF_InData *in_data,
        const char *suite_name,
        int32_t suite_versionL,
        PF_OutData    *out_dataP0 = 0,
        const char *err_stringZ0 = 0)
    {
        i_suite_name = suite_name;
		i_suite_versionL = suite_versionL;
		i_basic_suiteP = in_data->pica_basicP;

        const void *suiteP = nullptr;
		SPErr err = i_basic_suiteP->AcquireSuite(i_suite_name, i_suite_versionL, &suiteP);

		if (err != kSPNoError) {
			if (ALLOW_NO_SUITE) {
				suiteP = NULL;
			}
			else {
				if (out_dataP0) {
					const char	*error_stringPC = err_stringZ0 ? err_stringZ0 : "Not able to acquire AEFX Suite.";
					out_dataP0->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
					PF_SPRINTF(out_dataP0->return_msg, error_stringPC);
				}
				A_THROW(A_Err_MISSING_SUITE);
			}
		}

		i_suiteP = reinterpret_cast<const SUITETYPE*>(suiteP);
	}

	~AEFX_SuiteScoper()
	{
		if (i_suiteP) {
			i_basic_suiteP->ReleaseSuite(i_suite_name, i_suite_versionL);	// ignore error, nothing we can do in dtor
		}
	}

	const SUITETYPE* operator->() const { return i_suiteP; }
	const SUITETYPE* get() const { return i_suiteP; }

private:
	mutable const SUITETYPE		*i_suiteP;
	SPBasicSuite				*i_basic_suiteP;
	const char					*i_suite_name;
	int32_t						i_suite_versionL;
};


#endif		// __cplusplus

#endif		// _H
