#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#include <stdint.h>
#include <iostream>
#include <cstdint>		// Provides fixed-width integer types
#include <cstring>
#include <sstream>
#include <string>
#include <iomanip>
#include <libusb-1.0/libusb.h>

#define ODRIVE_OK	0
#define ODRIVE_ERROR	1

#define ODRIVE_USB_VENDORID	0x1209
#define ODRIVE_USB_PRODUCTID	0x0D32

class odrive_endpoint
{
	public:
		odrive_endpoint();
		~odrive_endpoint();

		int init(uint64_t serialNumber);

	private:
		libusb_context* libusb_context_ = NULL;
		libusb_device_handle* odrive_handle_ = NULL;
};

#endif
