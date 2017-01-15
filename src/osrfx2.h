/**@file osrfx2.h 
 */
#ifndef OSRFX2_H_
#define OSRFX2_H_
//=============================================================================
// INCLUDES
//=============================================================================


//=============================================================================
// DEFINITIONS
//=============================================================================
#define OSRFX2_VID      0x0547  //Cypress Semiconductor
#define OSRFX2_PID      0x1002  //Sample Device
#define OSRFX2_VERSION  0

#define OSRFX2_CMD_READ_7SEG_DISPLAY    0xD4
#define OSRFX2_CMD_READ_SWITCHES        0xD6
#define OSRFX2_CMD_READ_BARGRAPH        0xD7
#define OSRFX2_CMD_SET_BARGRAPH         0xD8
#define OSRFX2_CMD_IS_HIGHSPEED         0xD9
#define OSRFX2_CMD_SET_7SEG_DISPLAY     0xDB

//=============================================================================
#endif //OSRFX2_H_
