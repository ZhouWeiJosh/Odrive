#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#include <stdint.h>
#include <libusb-1.0/libusb.h>

extern libusb_context* libusb_context_;
extern libusb_device_handle* odrive_handle_;

int init(uint64_t serialNumber);

#endif
