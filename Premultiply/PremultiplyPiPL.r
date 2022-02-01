#include "Fs_Target.h"

#include "AEConfig.h"
#include "AE_EffectVers.h"

#ifndef AE_OS_WIN
	#include "AE_General.r"
#endif

resource 'PiPL' (16000) {
	{	/* array properties: 12 elements */
		/* [1] */
		Kind {
			AEEffect
		},
		/* [2] */
		Name {
			/*AEのメニューに使われる */
			FS_NAME
		},
		/* [3] */
		Category {
			/*AEのメニューに使われる */
			FS_CATEGORY
		},
		
#ifdef AE_OS_WIN
	#ifdef AE_PROC_INTELx64
		CodeWin64X86 {"EntryPointFunc"},
	#else
		CodeWin32X86 {"EntryPointFunc"},
	#endif	
#else
	#ifdef AE_OS_MAC
		CodeMachOPowerPC {"EntryPointFunc"},
		CodeMacIntel32 {"EntryPointFunc"},
		CodeMacIntel64 {"EntryPointFunc"},
	#endif
#endif

		/* [6] */
		AE_PiPL_Version {
			2,
			0
		},
		/* [7] */
		AE_Effect_Spec_Version {
			PF_PLUG_IN_VERSION,
			PF_PLUG_IN_SUBVERS
		},
		/* [8] */
		AE_Effect_Version {
			FS_VERSION 
		},
		/* [9] */
		AE_Effect_Info_Flags {
			0
		},
		/* [10] */
		AE_Effect_Global_OutFlags {
			FS_OUT_FLAGS
		},
		AE_Effect_Global_OutFlags_2 {
			FS_OUT_FLAGS2
		},
		/* [11] */
		AE_Effect_Match_Name {
			/*プラグインの識別に使われる */
			FS_NAME
		},
		/* [12] */
		AE_Reserved_Info {
			0
		}
	}
};

