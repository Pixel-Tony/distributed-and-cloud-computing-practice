#ifndef NST_RESOURCE_RC
#define NST_RESOURCE_RC

#ifndef APP_ICON_ICO
	#error "APP_ICON_ICO must be defined"
#else 
	MAINICON ICON APP_ICON_ICO 
#endif

#endif // NST_RESOURCE_RC