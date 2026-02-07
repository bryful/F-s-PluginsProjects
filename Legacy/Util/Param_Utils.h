#ifndef H_PARAM_UTILS
#define H_PARAM_UTILS

//	do not include DVA headers here
#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <AE_Macros.h>


// Returns DST pointer. DST must be of size SZ. SRC must be null terminated.
// Auto null terminates and truncates if necessary.
static inline char* PF_STRNNCPY(char* DST, const char* SRC, size_t SZ) {
	#ifdef AE_OS_WIN
		// Windows: use strncpy_s
		::strncpy_s((DST), (SZ), (SRC), _TRUNCATE);
	#else
		// Mac: use strlcpy
		::strlcpy((DST), (SRC), (SZ));
	#endif
	return DST;
}

#define        PF_ParamDef_IS_PUI_FLAG_SET(_defP, _puiFlag)        \
  (((_defP)->ui_flags & _puiFlag) != 0)
   
#define        PF_ParamDef_IS_PARAM_FLAG_SET(_defP, _paramFlag)    \
   (((_defP)->flags & _paramFlag) != 0)

#define         PF_ParamID_DFLT     0

// clang-format off
#define PF_ADD_COLOR(NAME, RED, GREEN, BLUE, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_COLOR; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.cd.value.red = (RED); \
		def.u.cd.value.green = (GREEN); \
		def.u.cd.value.blue = (BLUE); \
		def.u.cd.value.alpha = 255; \
		def.u.cd.dephault = def.u.cd.value; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_ARBITRARY2(NAME, WIDTH, HEIGHT, PARAM_FLAGS, PUI_FLAGS, DFLT, ID, REFCON)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ARBITRARY_DATA; \
		def.flags = (PARAM_FLAGS); \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.ui_width = (WIDTH);\
	    def.ui_height = (HEIGHT);\
		def.ui_flags = (PUI_FLAGS); \
		def.u.arb_d.value = NULL;\
		def.u.arb_d.pad = 0;\
		def.u.arb_d.dephault = (DFLT); \
		def.uu.id = def.u.arb_d.id = (ID); \
		def.u.arb_d.refconPV = REFCON; \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

#define PF_ADD_ARBITRARY(NAME, WIDTH, HEIGHT, PUI_FLAGS, DFLT, ID, REFCON)\
	PF_ADD_ARBITRARY2(NAME, WIDTH, HEIGHT, PF_ParamFlag_NONE, PUI_FLAGS, DFLT, ID, REFCON)

// clang-format off
#define PF_ADD_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_SLIDER; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.sd.value_str[0] = '\0'; \
		def.u.sd.value_desc[0] = '\0'; \
		def.u.sd.valid_min = (VALID_MIN); \
		def.u.sd.slider_min = (SLIDER_MIN); \
		def.u.sd.valid_max = (VALID_MAX); \
		def.u.sd.slider_max = (SLIDER_MAX); \
		def.u.sd.value = def.u.sd.dephault = (DFLT); \
		def.uu.id = (A_long)(ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// clang-format off
// this macro assumes you're passing in fixed values
#define PF_ADD_FIXED_PRECONVERTED(NAME, VALID_MIN_FIXED, VALID_MAX_FIXED, SLIDER_MIN_FIXED, SLIDER_MAX_FIXED, DFLT_FIXED, PREC, DISP, FLAGS, ID) \
	do {\
		PF_Err	priv_err        = PF_Err_NONE; \
		def.param_type          = PF_Param_FIX_SLIDER; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.fd.value_str[0]   = '\0'; \
		def.u.fd.value_desc[0]	= '\0'; \
		def.u.fd.valid_min      = VALID_MIN_FIXED; \
		def.u.fd.slider_min     = SLIDER_MIN_FIXED; \
		def.u.fd.valid_max      = VALID_MAX_FIXED; \
		def.u.fd.slider_max     = SLIDER_MAX_FIXED; \
		def.u.fd.value          = def.u.fd.dephault = DFLT_FIXED; \
		def.u.fd.precision		= static_cast<PF_Precision>(PREC); \
		def.u.fd.display_flags  |= (PF_ValueDisplayFlags)(DISP); \
		def.flags				|= (PF_ParamFlags)(FLAGS); \
		def.uu.id               = (A_long)(ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

    // this macro assumes you're passing in float values
#define PF_ADD_FIXED(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID) \
    PF_ADD_FIXED_PRECONVERTED(  \
        NAME,   \
        FLOAT2FIX(VALID_MIN), \
        FLOAT2FIX(VALID_MAX), \
        FLOAT2FIX(SLIDER_MIN), \
        FLOAT2FIX(SLIDER_MAX), \
        FLOAT2FIX(DFLT), \
        PREC, DISP, FLAGS, ID)

// clang-format on

// clang-format off
#define PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_FLOAT_SLIDER; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.fs_d.valid_min		= static_cast<PF_FpShort>(VALID_MIN); \
		def.u.fs_d.slider_min		= static_cast<PF_FpShort>(SLIDER_MIN); \
		def.u.fs_d.valid_max		= static_cast<PF_FpShort>(VALID_MAX); \
		def.u.fs_d.slider_max		= static_cast<PF_FpShort>(SLIDER_MAX); \
		def.u.fs_d.value			= static_cast<PF_FpShort>(DFLT); \
		def.u.fs_d.dephault			= static_cast<PF_FpShort>(def.u.fs_d.value); \
		def.u.fs_d.precision		= static_cast<PF_Precision>(PREC); \
		def.u.fs_d.display_flags	= (DISP); \
		def.u.fs_d.fs_flags			|= (WANT_PHASE) ? PF_FSliderFlag_WANT_PHASE : 0; \
		def.u.fs_d.curve_tolerance	= static_cast<PF_FpShort>(CURVE_TOLERANCE);\
		def.uu.id = (A_long)(ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// safer newer version
// clang-format off
#define PF_ADD_FLOAT_SLIDERX(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID)	\
	do {																										\
		AEFX_CLR_STRUCT(def);																					\
		def.flags = (FLAGS);																					\
		PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, AEFX_DEFAULT_CURVE_TOLERANCE, DFLT, PREC, DISP, false, ID);	\
	} while (0)
// clang-format on

// copied from Pr version of Param_Utils.h. It is used in some of Pr versions of AE effects
// clang-format off
#define PF_ADD_FLOAT_EXPONENTIAL_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, EXPONENT, ID) \
	do {																										\
		AEFX_CLR_STRUCT(def);																					\
        def.u.fs_d.useExponent		= true;\
        def.u.fs_d.exponent			= static_cast<PF_FpShort>(EXPONENT);\
		PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, ID);	\
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_CHECKBOX(NAME_A, NAME_B, DFLT, FLAGS, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_CHECKBOX; \
		PF_STRNNCPY(def.PF_DEF_NAME, NAME_A, sizeof(def.PF_DEF_NAME)); \
		def.u.bd.u.PF_DEF_NAMEPTR  = (NAME_B); \
		def.u.bd.value		= (DFLT); \
		def.u.bd.dephault	= (PF_Boolean)(def.u.bd.value); \
		def.flags			|= (FLAGS); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// safer newer version
// clang-format off
#define PF_ADD_CHECKBOXX(NAME, DFLT, FLAGS, ID)\
	do {\
		AEFX_CLR_STRUCT(def); \
		PF_ADD_CHECKBOX(NAME, "", DFLT, FLAGS, ID); \
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_BUTTON(PARAM_NAME, BUTTON_NAME, PUI_FLAGS, PARAM_FLAGS, ID)\
	do {\
		AEFX_CLR_STRUCT(def); \
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type		= PF_Param_BUTTON; \
		PF_STRNNCPY(def.PF_DEF_NAME, PARAM_NAME, sizeof(def.PF_DEF_NAME)); \
		def.u.button_d.u.PF_DEF_NAMESPTR  = (BUTTON_NAME); \
		def.flags			= (PARAM_FLAGS); \
		def.ui_flags		= (PUI_FLAGS); \
		def.uu.id			= (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_ANGLE(NAME, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ANGLE; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.ad.value = def.u.ad.dephault = FLOAT2FIX(DFLT); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on


// clang-format off
#define PF_ADD_NULL(NAME, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_NO_DATA; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on


// clang-format off
#define PF_ADD_POPUP(NAME, CHOICES, DFLT, STRING, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POPUP; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.pd.num_choices = static_cast<short>(CHOICES); \
		def.u.pd.dephault = (DFLT); \
		def.u.pd.value = def.u.pd.dephault; \
		def.u.pd.u.PF_DEF_NAMESPTR = (STRING); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on
	
// clang-format off
#define PF_ADD_LAYER(NAME, DFLT, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_LAYER; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.ld.dephault = (DFLT); \
		def.uu.id = ID;\
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)			
// clang-format on

#define PF_ADD_255_SLIDER(NAME, DFLT, ID)\
	PF_ADD_SLIDER( (NAME), 0, 255, 0, 255, (DFLT), (ID))

#define PF_ADD_PERCENT(NAME, DFLT, ID)\
	PF_ADD_FIXED( (NAME), 0, 100, 0, 100, (DFLT), 1, 1, 0, (ID))

// clang-format off
#define PF_ADD_POINT_PRECONVERTED(NAME, X_DFLT_FIXED, Y_DFLT_FIXED, RESTRICT_BOUNDS, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POINT; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.td.restrict_bounds = RESTRICT_BOUNDS;\
		def.u.td.x_value = def.u.td.x_dephault = X_DFLT_FIXED; \
		def.u.td.y_value = def.u.td.y_dephault = Y_DFLT_FIXED; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_POINT(NAME, X_DFLT, Y_DFLT, RESTRICT_BOUNDS, ID) \
    PF_ADD_POINT_PRECONVERTED(  \
        NAME,                   \
        FLOAT2FIX(X_DFLT),      \
        FLOAT2FIX(Y_DFLT),      \
        RESTRICT_BOUNDS,        \
        ID)
// clang-format on

// clang-format off
#define PF_ADD_POINT_3D(NAME, X_DFLT, Y_DFLT, Z_DFLT, ID) \
	do	{\
		AEFX_CLR_STRUCT(def); \
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POINT_3D; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.u.point3d_d.x_value = def.u.point3d_d.x_dephault = X_DFLT; \
		def.u.point3d_d.y_value = def.u.point3d_d.y_dephault = Y_DFLT; \
		def.u.point3d_d.z_value = def.u.point3d_d.z_dephault = Y_DFLT; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_TOPIC(NAME, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_START; \
		PF_STRNNCPY(def.PF_DEF_NAME, (NAME), sizeof(def.PF_DEF_NAME) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// clang-format off
#define PF_END_TOPIC(ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_END; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)	
// clang-format on

// clang-format off
#define PF_ADD_VERSIONED_FLAG(NAME) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_CHECKBOX; \
		def.PF_DEF_NAME[0] = 0; \
		def.u.bd.u.PF_DEF_NAMEPTR  = (NAME); \
		def.u.bd.value = true; \
		def.u.bd.dephault = false; \
		def.flags = PF_ParamFlag_USE_VALUE_FOR_OLD_PROJECTS; \
		def.ui_flags = PF_PUI_INVISIBLE; \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
// clang-format on

// newer safer version
// clang-format off
#define PF_ADD_TOPICX(NAME, FLAGS, ID) \
	do {\
		AEFX_CLR_STRUCT(def); \
		def.flags = (FLAGS); \
		PF_ADD_TOPIC(NAME, ID); \
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_POPUPX(NAME, NUM_CHOICES, DFLT, ITEMS_STRING, FLAGS, ID) \
	do {															\
		AEFX_CLR_STRUCT(def);										\
		def.flags = (FLAGS);										\
		PF_ADD_POPUP(NAME, NUM_CHOICES, DFLT, ITEMS_STRING, ID);	\
	} while (0)
// clang-format on

// clang-format off
#define PF_ADD_FLOAT_SLIDERX_DISABLED(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID)	\
	do {																										\
	AEFX_CLR_STRUCT(def);																					\
	def.flags = (FLAGS);\
	def.ui_flags = PF_PUI_DISABLED;\
	PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, 0, DFLT, PREC, DISP, 0, ID);	\
	} while (0)
// clang-format on

#ifdef __cplusplus
namespace fxparam_utility {

	template <typename T>
	inline int RoundF(T	x)
	{
		int	ret;

		if (x > 0) {
			ret = (int)(x + (T)0.5);
		} else {
			if ((int)(x + (T)0.5) == (x + (T)0.5)) {
				ret = (int)x;
			} else {
				ret = (int)(x - (T)0.5);
			}
		}

		return ret;
	}
};

inline PF_Err PF_AddPointControl(PF_InData *in_data,
								 const char *nameZ,
								 float x_defaultF,			// 0-1
								 float y_defaultF,		// 0-1
								 bool restrict_boundsB,
								 PF_ParamFlags param_flags,
								 PF_ParamUIFlags pui_flags,
								 A_long param_id)
{
    PF_ParamDef		def = {{0}};
	namespace du = fxparam_utility;

	def.flags = param_flags;
	def.ui_flags = pui_flags;

	PF_ADD_POINT(nameZ, du::RoundF(x_defaultF*100), du::RoundF(y_defaultF*100), restrict_boundsB, param_id);	// has error return in macro

	return PF_Err_NONE;
}
#endif // __cplusplus

#endif // H_PARAM_UTILS
