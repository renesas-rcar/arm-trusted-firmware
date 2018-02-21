#pragma once
#include <stdint.h>

#ifndef	__DDR_INIT_E3_
#define	__DDR_INIT_E3_

#define RCAR_E3_DDR_VERSION    "rev.0.03"

#ifdef ddr_qos_init_setting          // rev.0.03
   #define REFRESH_RATE  3900        // Average periodic refresh interval[ns]. Support 3900,7800
#else                                // rev.0.03
   #if RCAR_REF_INT == 0             // rev.0.03
      #define REFRESH_RATE  3900     // rev.0.03
   #else                             // rev.0.03
      #define REFRESH_RATE  3900     // rev.0.03
   #endif                            // rev.0.03
#endif                               // rev.0.03


//uint32_t init_ddr_e31866(void);    // rev.0.03
//uint32_t init_ddr_e31600(void);    // rev.0.03

extern int32_t InitDram(void);       // rev.0.03
#define INITDRAM_OK (0)              // rev.0.03
#define INITDRAM_NG (0xffffffff)     // rev.0.03
#define INITDRAM_ERR_I (0xffffffff)  // rev.0.03
#define INITDRAM_ERR_O (0xfffffffe)  // rev.0.03
#define INITDRAM_ERR_T (0xfffffff0)  // rev.0.03

#endif /* __DDR_INIT_E3_ */

