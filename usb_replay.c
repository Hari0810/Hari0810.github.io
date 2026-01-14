#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID  0x0483
#define PRODUCT_ID 0xA30E
#define ENDPOINT_OUT 0x02
#define TIMEOUT 1000

void bulk_write(libusb_device_handle *handle, const unsigned char *data, int length) {
    int transferred = 0;
    int res = libusb_bulk_transfer(handle, ENDPOINT_OUT, (unsigned char *)data, length, &transferred, TIMEOUT);
    if (res != 0 || transferred != length) {
        fprintf(stderr, "Error: Bulk write failed. Res: %d, Transferred: %d\n", res, transferred);
        exit(1);
    }
}

void replay(libusb_device_handle *handle) {
    // Example bulk writes (from your Python script)
    unsigned char packet1[] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x2C, 0x05, 0x82, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0xE8, 0xCB, 0x10, 0x4A};
    unsigned char packet2[] = {0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x82, 0x08, 0x00, 0x00, 0x00, 0xB8, 0x1E, 0x05, 0x3E,
                               0x00, 0x00, 0x00, 0x00, 0xC6, 0xE6, 0x10, 0x4A};
    unsigned char packet3[] = {0x00, 0x00, 0x00, 0x00, 0xC0, 0x02, 0x05, 0x80, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                               0x14, 0x9C, 0x79, 0x3D, 0x63, 0xF3, 0x10, 0x4A};

    // Write packets
    bulk_write(handle, packet1, sizeof(packet1));
    bulk_write(handle, packet2, sizeof(packet2));
    bulk_write(handle, packet3, sizeof(packet3));
}

libusb_device_handle *open_device(libusb_context *ctx) {
    libusb_device **list;
    libusb_device *found = NULL;
    libusb_device_handle *handle = NULL;
    ssize_t count = libusb_get_device_list(ctx, &list);

    if (count < 0) {
        fprintf(stderr, "Error: Failed to get USB device list\n");
        return NULL;
    }

    for (ssize_t i = 0; i < count; i++) {
        struct libusb_device_descriptor desc;
        int res = libusb_get_device_descriptor(list[i], &desc);
        if (res < 0) {
            fprintf(stderr, "Error: Failed to get device descriptor\n");
            continue;
        }

        if (desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID) {
            found = list[i];
            break;
        }
    }

    if (found) {
        int res = libusb_open(found, &handle);
        if (res < 0) {
            fprintf(stderr, "Error: Failed to open device\n");
            handle = NULL;
        }
    } else {
        fprintf(stderr, "Error: Device not found\n");
    }

    libusb_free_device_list(list, 1);
    return handle;
}

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    int res;

    res = libusb_init(&ctx);
    if (res < 0) {
        fprintf(stderr, "Error: Failed to initialize libusb\n");
        return 1;
    }

    handle = open_device(ctx);
    if (!handle) {
        libusb_exit(ctx);
        return 1;
    }

    res = libusb_claim_interface(handle, 0);
    if (res < 0) {
        fprintf(stderr, "Error: Failed to claim interface\n");
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    res = libusb_reset_device(handle);
    if (res < 0) {
        fprintf(stderr, "Error: Failed to reset device\n");
        libusb_release_interface(handle, 0);
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    printf("Sending packets to the device...\n");
    for (int i = 0; i < 1000000; i++) {
        replay(handle);
    }

    printf("Completed.\n");

    libusb_release_interface(handle, 0);
    libusb_close(handle);
    libusb_exit(ctx);

    return 0;
}
