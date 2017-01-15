/**@file osrfx2.c 
 */
//=============================================================================
// INCLUDES
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "osrfx2.h"

//=============================================================================
// DEFINITIONS
//=============================================================================
/**
 * Print the usage for this program.
 * 
 * @param argc Argument count
 * @param argv Argument data
 */
static void PrintUsage (int argc, char* argv[]);

/**
 * Prints the state of the switches on the OSRFX2 device.
 * 
 * @param device 
 */
void osrfx2_print_switches (libusb_device_handle* device);

/**
 * Prints the state of the switches by reading from endpoint 1.
 * 
 * @param device 
 */
void osrfx2_print_switches_intr (libusb_device_handle* device);


//=============================================================================
// IMPLEMENTATION
//=============================================================================
void osrfx2_set_led (libusb_device_handle* device, int level) {
    int status;
    int i;
    unsigned char data = (unsigned char)(level & 0xFF);

    status = libusb_control_transfer(
        device,
        (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT),
        OSRFX2_CMD_SET_BARGRAPH,
        0,
        0,
        &data,
        sizeof(data),
        1000
    );

    switch (status) {
    case LIBUSB_ERROR_TIMEOUT:
        {
            fprintf(stderr, "usb timeout\n");
            break;
        }
    case LIBUSB_ERROR_PIPE:
        {
            fprintf(stderr, "usb pipe error\n");
            break;
        }
    case LIBUSB_ERROR_OVERFLOW:
        {
            fprintf(stderr, "usb overflow\n");
            break;
        }
    case LIBUSB_ERROR_NO_DEVICE:
        {
            fprintf(stderr, "no usb device available\n");
            break;
        }
    default:
        {
            #if 0
            printf("Switches:\n");
            for (i=0; i<8; i++) {
                printf("  SW%d = %s\n", i+1, (data >> (7-i)) & 0x1 ? "OFF" : "ON");
            }
            #endif
            break;
        }
    }
}

void osrfx2_print_switches (libusb_device_handle* device) {
    int status;
    int i;
    unsigned char data;

    status = libusb_control_transfer(
        device,
        (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN),
        OSRFX2_CMD_READ_SWITCHES,
        0,
        0,
        &data,
        sizeof(data),
        1000
    );

    switch (status) {
    case LIBUSB_ERROR_TIMEOUT:
        {
            fprintf(stderr, "usb timeout\n");
            break;
        }
    case LIBUSB_ERROR_PIPE:
        {
            fprintf(stderr, "usb pipe error\n");
            break;
        }
    case LIBUSB_ERROR_OVERFLOW:
        {
            fprintf(stderr, "usb overflow\n");
            break;
        }
    case LIBUSB_ERROR_NO_DEVICE:
        {
            fprintf(stderr, "no usb device available\n");
            break;
        }
    default:
        {
            printf("Switches:\n");
            for (i=0; i<8; i++) {
                printf("  SW%d = %s\n", i+1, (data >> (7-i)) & 0x1 ? "OFF" : "ON");
            }
            break;
        }
    }
}

void osrfx2_print_switches_intr (libusb_device_handle* device) {
    int status;
    int transferred;
    int i;
    unsigned char data;

    status = libusb_interrupt_transfer(
        device,
        (1|LIBUSB_ENDPOINT_IN),
        &data,
        sizeof(data),
        &transferred,
        1000
    );
    
    switch (status) {
    case LIBUSB_ERROR_TIMEOUT:
        {
            fprintf(stderr, "usb timeout\n");
            break;
        }
    case LIBUSB_ERROR_PIPE:
        {
            fprintf(stderr, "usb pipe error\n");
            break;
        }
    case LIBUSB_ERROR_OVERFLOW:
        {
            fprintf(stderr, "usb overflow\n");
            break;
        }
    case LIBUSB_ERROR_NO_DEVICE:
        {
            fprintf(stderr, "no usb device available\n");
            break;
        }
    default:
        {
            if (transferred > 0) {
                printf("Switches:\n");
                for (i=0; i<8; i++) {
                    printf("  SW%d = %s\n", i+1, (data >> (7-i)) & 0x1 ? "OFF" : "ON");
                }
            } else {
                printf("Switches Unchanged\n");
            }
            break;
        }
    }
}

void PrintUsage (int argc, char* argv[]) {
    fprintf(
        stderr,
        "Usage: %s [-l led_level] [-s] [-v]\n",
        argv[0]
    );
}

int main (int argc, char* argv[]) {
    int retValue = 0;
    int status;
    int opt;
    int ledLevel = 0;
    bool haveLEDLevel = false;
    bool haveSwitchState = false;
    bool isUSBInitialized = false;
    libusb_device_handle* device;

    //Process CLI options
    while ((opt = getopt(argc, argv, "l:s")) != -1) {
        switch (opt) {
        case 'l':
            {
                haveLEDLevel = true;
                ledLevel = strtoul(optarg, NULL, 16);
                break;
            }
        case 's':
            {
                haveSwitchState = true;
                break;
            }
        default:
            {
                PrintUsage(argc, argv);
                goto Exit;
            }
        }
    }

    //Access device via libusb
    status = libusb_init(NULL);
    if (status < 0) {
        fprintf(stderr, "libusb_init failed: %s\n", libusb_error_name(status));
        goto Exit;
    } else {
        isUSBInitialized = true;
    }

    device = libusb_open_device_with_vid_pid(NULL, OSRFX2_VID, OSRFX2_PID);
    if (device == NULL) {
        fprintf(stderr, "Unable to open device %04x:%04x\n", OSRFX2_VID, OSRFX2_PID);
        goto Exit;
    }

    libusb_set_auto_detach_kernel_driver(device, 1);
    status = libusb_claim_interface(device, 0);
    if (status != LIBUSB_SUCCESS) {
        libusb_close(device);
        fprintf(stderr, "Unable to claim interface for device\n");
        goto Exit;
    }

    if (haveSwitchState) {
        printf("Reading switch state from EP1:\n");
        osrfx2_print_switches_intr(device);

        printf("Reading switch state via request:\n");
        osrfx2_print_switches(device);
    }

    if (haveLEDLevel) {
        osrfx2_set_led(device, ledLevel);
    }
    
    libusb_release_interface(device, 0);
    libusb_close(device);
        
Exit:
    if (isUSBInitialized) {
        libusb_exit(NULL);
    }
    return retValue;
}


//=============================================================================
