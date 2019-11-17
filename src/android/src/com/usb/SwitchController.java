package com.usb;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;

public class SwitchController {
    private UsbDevice device_;
    private UsbDeviceConnection connection_;

    public SwitchController(UsbDevice device, UsbDeviceConnection connection) {
        device_ = device;
        connection_ = connection;
    }

    public UsbDevice getDevice() {
        return device_;
    }

    public UsbDeviceConnection getConnection() {
        return connection_;
    }
}