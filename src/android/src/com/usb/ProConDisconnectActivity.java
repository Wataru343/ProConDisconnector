package com.usb;
import java.io.*;
import java.net.*;
import java.util.*;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtActivity;


public class ProConDisconnectActivity extends QtActivity {
    private native void device_attached(int vid, int pid, String name);

    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static final int VID_NINTENDO = 0x057E;
    private static final int PID_PROCONTROLLER = 0x2009;
    private PendingIntent permission_intent_;
    private UsbManager manager_;
    private HashMap<String, SwitchController> connected_devices_;

    public ProConDisconnectActivity() {
        Log.d("Java", "initialized.");
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        manager_ = (UsbManager)getSystemService(Context.USB_SERVICE);

        registerReceiver(broadcast_receiver_, new IntentFilter(UsbManager.ACTION_USB_DEVICE_ATTACHED));
        registerReceiver(broadcast_receiver_, new IntentFilter(UsbManager.ACTION_USB_DEVICE_DETACHED));
        registerReceiver(broadcast_receiver_, new IntentFilter(ACTION_USB_PERMISSION));

        permission_intent_ = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);

        connected_devices_ = new HashMap<String, SwitchController>();


        final Handler handler = new Handler();

        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                search_device();
            }
        }, 1000);
    }

    private void search_device() {
        HashMap<String, UsbDevice> devices = manager_.getDeviceList();
        Iterator<UsbDevice> itr = devices.values().iterator();
        Log.d("Java", "aaaaaa.");
        while(itr.hasNext()) {
            UsbDevice device = itr.next();

            if (device.getVendorId() == VID_NINTENDO && device.getProductId () == PID_PROCONTROLLER) {
                manager_.requestPermission(device, permission_intent_);
            }
        }
    }

    private final BroadcastReceiver broadcast_receiver_ = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if(ACTION_USB_PERMISSION.equals(action)) {
                Log.d("Java", "ACTION_USB_PERMISSION");

                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if(device != null) {
                            UsbDeviceConnection connection = manager_.openDevice(device);
                            connection.claimInterface(device.getInterface(0), true);

                            connected_devices_.put(device.getDeviceName(), new SwitchController(device, connection));
                            device_attached(device.getDeviceId(), device.getProductId(), device.getDeviceName());
                            Log.d("Java", "device: " + device.getDeviceId() + " DeviceName: " + device.getDeviceName());
                        }
                    } else {
                        Log.d("Java", "permission denied for device " + device);
                    }
                }
            } else if(UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                Log.d("Java", "ACTION_USB_DEVICE_ATTACHED");

                synchronized(this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (device != null) {
                        manager_.requestPermission(device, permission_intent_);
                    }
                }
            } else if(UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                Log.d("Java", "ACTION_USB_DEVICE_DETACHED");

                synchronized(this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    Log.d("Java", "device: " + device.getDeviceId() + " DeviceName: " + device.getDeviceName());

                    if(connected_devices_.containsKey(device.getDeviceName()))
                        connected_devices_.remove(device.getDeviceName());
                }
            }
        }
    };


    public void write(String device_name) {
        synchronized(this) {
            if(connected_devices_.containsKey(device_name)){
                UsbDevice device = connected_devices_.get(device_name).getDevice();
                UsbDeviceConnection connection = connected_devices_.get(device_name).getConnection();

                for(int i = 0; i < device.getInterfaceCount(); i++) {
                    UsbInterface intf = device.getInterface(i);

                    UsbEndpoint endpoint = null;

                    for (int j = 0; j < intf.getEndpointCount(); j++) {
                        if (intf.getEndpoint(j).getDirection() == UsbConstants.USB_DIR_OUT) {
                            endpoint = intf.getEndpoint(j);
                            break;
                        }
                    }


                    byte[] bytes = {(byte) 0x80, (byte) 0x02};
                    int ret = connection.bulkTransfer(endpoint, bytes, bytes.length, 0);


                    byte[] bytes2 = {(byte) 0x80, (byte) 0x92, (byte) 0x00, (byte) 0x31, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x01, (byte) 0x05, (byte) 0x00, (byte) 0x01, (byte) 0x40, (byte) 0x40, (byte) 0x00, (byte) 0x01, (byte) 0x40, (byte) 0x40, (byte) 0x07, (byte) 0x00};
                    ret = connection.bulkTransfer(endpoint, bytes2, bytes2.length, 0);

                    Log.d("TAG", "command witten");
                }
            }
        }

    }
}