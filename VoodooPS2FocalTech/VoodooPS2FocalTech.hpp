/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 *
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.2 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 *
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _APPLEPS2FOCALTECHTOUCHPAD_H
#define _APPLEPS2FOCALTECHTOUCHPAD_H

#include "VoodooPS2Controller/ApplePS2MouseDevice.h"
#include "Multitouch Support/VoodooPS2MultitouchInterface.hpp"
#include "LegacyIOHIPointing.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ApplePS2ALPSGlidePoint Class Declaration
//

#define FOCALTECH_MAX_FINGERS 5

#define kPacketLengthMax    6

#define LOGICAL_MAX_X       2432//2432
#define LOGICAL_MAX_Y       1664
#define PHYSCICAL_MAX_X     1216//1216
#define PHYSCICAL_MAX_Y     832//832

struct focaltech_finger_state {
    /* The touchpad has generated a touch event for the finger */
    bool active;
    
    /*
     * The touchpad has sent position data for the finger. The
     * flag is 0 when the finger is not active, and there is a
     * time between the first touch event for the finger and the
     * following absolute position packet for the finger where the
     * touchpad has declared the finger to be valid, but we do not
     * have any valid position yet.
     */
    bool valid;
    
    /*
     * Absolute position (from the bottom left corner) of the
     * finger.
     */
    unsigned int x;
    unsigned int y;
};

/*
 * Description of the current state of the touchpad hardware.
 */
struct focaltech_hw_state {
    /*
     * The touchpad tracks the positions of the fingers for us,
     * the array indices correspond to the finger indices returned
     * in the report packages.
     */
    struct focaltech_finger_state fingers[FOCALTECH_MAX_FINGERS];
    
    /*
     * Finger width 0-7 and 15 for a very big contact area.
     * 15 value stays until the finger is released.
     * Width is reported only in absolute packets.
     * Since hardware reports width only for last touching finger,
     * there is no need to store width for every specific finger,
     * so we keep only last value reported.
     */
    unsigned int width;
    
    /* True if the clickpad has been pressed. */
    bool pressed;
};

typedef struct FTE_BYTES
{
    UInt8 byte0;
    UInt8 byte1;
    UInt8 byte2;
} FTE_BYTES_t;

class EXPORT ApplePS2FocalTechTouchPad : public IOHIPointing
{
    typedef IOHIPointing super;
    OSDeclareDefaultStructors(ApplePS2FocalTechTouchPad);
    
private:
    ApplePS2MouseDevice * _device;
    RingBuffer<UInt8,kPacketLengthMax*32> _ringBuffer;
    UInt32                _packetByteCount;
    uint64_t              keytime;
    uint64_t              maxaftertyping;
    bool                  _interruptHandlerInstalled;
    bool                  _powerControlHandlerInstalled;
    unsigned int          left  : 1;
    unsigned int          right : 1;
    FTE_BYTES_t           bytes;
    UInt8                 _lastDeviceData[16];
    OSArray*              transducers;
    VoodooPS2MultitouchInterface* mt_interface;
    
    struct focaltech_hw_state State;
    
    void process_touch_packet(UInt8 *packet);
    void process_abs_packet(UInt8 *packet);
    void process_rel_packet(UInt8 *packet);
    inline unsigned int clamp(unsigned int x,unsigned int min,unsigned int max)
    {
        if(x<min)
            return min;
        if(x>max)
            return max;
        return x;
    }
    
    bool publish_multitouch_interface();
    void unpublish_multitouch_interface();
    bool init_multitouch_interface();
    void sendTouchDataToMultiTouchInterface();
    
protected:
    virtual void   doHardwareReset();
    virtual void   switchProtocol();
    virtual void   getProductID(FTE_BYTES_t *bytes);
    virtual void   packetReady();
    virtual void   parsePacket(UInt8 *packet);
    virtual void   setTouchPadEnable( bool enable );
    virtual void   setDevicePowerState(UInt32 whatToDo);
    virtual PS2InterruptResult interruptOccurred(UInt8 data);
    
protected:
public:
    bool init( OSDictionary * properties ) override;
    ApplePS2FocalTechTouchPad * probe( IOService * provider, SInt32 *    score ) override;
    
    bool start( IOService * provider ) override;
    void stop( IOService * provider ) override;
    
    UInt32 deviceType() override;
    UInt32 interfaceID() override;
    
    virtual IOReturn message(UInt32 type, IOService* provider, void* argument) override;
};

#endif /* _APPLEPS2FOCALTECHTOUCHPAD_H */
