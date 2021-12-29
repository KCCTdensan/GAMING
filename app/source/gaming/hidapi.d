/*******************************************************
 HIDAPI - Multi-Platform library for
 communication with HID devices.

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009, All Rights Reserved.

 At the discretion of the user of this library,
 this software may be licensed under the terms of the
 GNU General Public License v3, a BSD-Style license, or the
 original HIDAPI license as outlined in the LICENSE.txt,
 LICENSE-gpl3.txt, LICENSE-bsd.txt, and LICENSE-orig.txt
 files located at the root of the source distribution.
 These files may also be found in the public source
 code repository located at:
        https://github.com/libusb/hidapi .
********************************************************/

module gaming.hidapi;

extern(C):

enum HID_API_VERSION_MAJOR = 0;
enum HID_API_VERSION_MINOR = 11;
enum HID_API_VERSION_PATCH = 0;
enum HID_API_VERSION_STR   = "v" ~ HID_API_VERSION_MAJOR.stringof
                           ~ "." ~ HID_API_VERSION_MINOR.stringof
                           ~ "." ~ HID_API_VERSION_PATCH.stringof;

struct hid_api_version {
  int major;
  int minor;
  int patch;
}

struct hid_device_;
alias hid_device = hid_device_;

struct hid_device_info {
  char *path;
  ushort vendor_id;
  ushort product_id;
  wchar* serial_number;
  ushort release_number;
  wchar* manufacturer_string;
  wchar* product_string;
  ushort usage_page;
  ushort usage;
  int interface_number;
  hid_device_info* next;
}

int   hid_init();
int   hid_exit();
hid_device_info* hid_enumerate      (ushort vendor_id, ushort product_id);
void  hid_free_enumeration          (hid_device_info* devs);
hid_device* hid_open                (ushort vendor_id, ushort product_id, const wchar* serial_number);
hid_device* hid_open_path           (const char* path);
int   hid_write                     (hid_device* dev, const ubyte* data, size_t length);
int   hid_read_timeout              (hid_device* dev, ubyte* data, size_t length, int milliseconds);
int   hid_read                      (hid_device* dev, ubyte* data, size_t length);
int   hid_set_nonblocking           (hid_device* dev, int nonblock);
int   hid_send_feature_report       (hid_device* dev, const ubyte* data, size_t length);
int   hid_get_feature_report        (hid_device* dev, ubyte* data, size_t length);
int   hid_get_input_report          (hid_device* dev, ubyte* data, size_t length);
void  hid_close                     (hid_device* dev);
int   hid_get_manufacturer_string   (hid_device* dev, wchar* string, size_t maxlen);
int   hid_get_product_string        (hid_device* dev, wchar* string, size_t maxlen);
int   hid_get_serial_number_string  (hid_device* dev, wchar* string, size_t maxlen);
int   hid_get_indexed_string        (hid_device* dev, int string_index, wchar* string, size_t maxlen);
const(wchar*) hid_error             (hid_device* dev);
const(hid_api_version*) hid_version();
const(char*) hid_version_str();
