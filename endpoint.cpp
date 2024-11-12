#include "endpoint.hpp"

odrive_endpoint::odrive_endpoint() {}

odrive_endpoint::~odrive_endpoint()
{
	if  (libusb_context_ != NULL)
	{
		libusb_exit(libusb_context_);
		libusb_context_ = NULL;
	}
}


int odrive_endpoint::init(uint64_t serialNumber)
{
	libusb_device **usb_device_list; // A pointer to pointer that shows the list of devices
	
	int ret = 1;			 // Assumes it does not work

	if (libusb_context_ == NULL)
    	{
        	int result = libusb_init(&libusb_context_);
        	if (result != LIBUSB_SUCCESS)
        	{
            	std::cerr << "Failed to initialize libusb: " << libusb_error_name(result) << std::endl;
            	return result;
        	}
    	}

	// libusb_get_device_list() retrieves list of USB devices connected

	ssize_t device_count = libusb_get_device_list(libusb_context_, &usb_device_list);

	if (device_count <= 0)
	{
		return device_count; // returns the device count as error
	}

	for (size_t i = 0; i < device_count; ++i)
	{
		libusb_device *device = usb_device_list[i];
		libusb_device_descriptor desc = {0};

		// retrieves the descriptor of the current device

		int result = libusb_get_device_descriptor(device, &desc);

		if (result != LIBUSB_SUCCESS)
		{
			std::cerr << "Error getting device descriptor" << std::endl;
			continue;
		}

		// checks for the Odrive USB ID

		if (desc.idVendor == ODRIVE_USB_VENDORID && desc.idProduct == ODRIVE_USB_PRODUCTID)
		{

			libusb_device_handle *device_handle;

			if (libusb_open(device, &device_handle) != LIBUSB_SUCCESS)
			{
				std::cerr << "Error opening USB device" << std::endl;
				continue;
			}

			// gets the config descriptor for the device
			
			struct libusb_config_descriptor *config;

			result = libusb_get_config_descriptor(device, 0 , &config);

			int ifNumber = 2;

			// detach kernel driver b/c it could prevent user management

			if ((libusb_kernel_driver_active(device_handle, ifNumber) != LIBUSB_SUCCESS) &&
			    (libusb_detach_kernel_driver(device_handle, ifNumber) != LIBUSB_SUCCESS))
			{
				std::cerr << "Driver error" << std::endl;
				libusb_close(device_handle);
				continue;
			}

			// claims the interface for access

			if ((result = libusb_claim_interface(device_handle, ifNumber)) != LIBUSB_SUCCESS)
			{
				std::cerr << "Error claiming device" << std::endl;
				libusb_close(device_handle);
			}
			else
			{
				bool attached_to_handle = false;
				unsigned char buf[128];

				// Retrieves the sn
				result = libusb_get_string_descriptor_ascii(device_handle, desc.iSerialNumber, buf, 127);

				if (result <= 0)
				{
					std::cerr << "Error getting data" << std::endl;
					result = libusb_release_interface(device_handle, ifNumber);
					libusb_close(device_handle);
					continue;
				}
				else
				{
					std::stringstream stream;
					stream << std::uppercase << std::hex << serialNumber;
					std::string sn(stream.str());

					if (sn.compare(0, strlen((const char *)buf), (const char *)buf) == 0)
					{
						std::cout << "Device 0x" << std::setw(16) << std::setfill('0')
							  << std::uppercase << std::hex << serialNumber << " found" << std::endl;
						odrive_handle_ = device_handle;
						attached_to_handle = true;
						ret = ODRIVE_OK;
						break;
					}
				}

				// cleans up if not attached

				if (!attached_to_handle)
				{
					result = libusb_release_interface(device_handle, ifNumber);
					libusb_close(device_handle);
				}
			}
		}
	}

	libusb_free_device_list(usb_device_list, 1);

	return ret;
}
