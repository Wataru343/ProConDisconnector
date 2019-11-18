package com.usb;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;

public class SwitchController {
    private UsbDevice device_;
    private UsbDeviceConnection connection_;
    private  State state_;

    public enum State {
        NONE,
        ATTACHED,
        PERMITTED,
        UNPAIRED,
    }

    public SwitchController(UsbDevice device) {
        device_ = device;
        connection_ = null;
        state_ = State.NONE;
    }

    public UsbDevice getDevice() {
        return device_;
    }

    public UsbDeviceConnection getConnection() {
        return connection_;
    }

    public void setConnection(UsbDeviceConnection connection) {
        connection_ = connection;
    }

    public State getState() {
        return state_;
    }

    public void setState(State state) {
        state_ = state;
    }
}
