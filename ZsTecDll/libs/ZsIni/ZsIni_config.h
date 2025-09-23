#ifndef _ZS_INI_CONFIG_H_
#define _ZS_INI_CONFIG_H_

#define LPCTSTR const char*
#define LPTSTR char*

#ifdef _WIN32	//windows平台
	#ifdef _WIN64
		#ifdef NDEBUG
			#pragma comment(lib,".\\libs\\zsini\\x64r\\ZsIni.lib")
		#else
			#pragma comment(lib,".\\libs\\zsini\\x64d\\ZsIni.lib")
		#endif
	#else			//32位程序
		#ifdef NDEBUG
			#pragma comment(lib,".\\libs\\zsini\\x32r\\ZsIni.lib")
		#else
			#pragma comment(lib,".\\libs\\zsini\\x32d\\ZsIni.lib")
		#endif
	#endif
#endif

#endif//_ZS_INI_CONFIG_H_
