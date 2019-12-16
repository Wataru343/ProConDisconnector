package com.usb;
import java.io.*;
import java.net.*;
import java.util.*;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtActivity;


public class ProConDisconnectActivity extends QtActivity {
    private native void device_attached(int vid, int pid, String name);
    private native void device_detached(int vid, int pid, String name);

    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static final int VID_NINTENDO = 0x057E;
    private static final int PID_PROCONTROLLER = 0x2009;
    private PendingIntent permission_intent_;
    private UsbManager manager_;
    private HashMap<String, SwitchController> connected_devices_;
    private Vibrator vibrator_;
    private Timer timer_;
    private TimerTask timer_task_;
    private Handler handler_;

    public ProConDisconnectActivity() {
        Log.d("Java", "initialized.");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        manager_ = (UsbManager)getSystemService(Context.USB_SERVICE);

        registerReceiver(broadcast_receiver_, new IntentFilter(UsbManager.ACTION_USB_DEVICE_ATTACHED));
        registerReceiver(broadcast_receiver_, new IntentFilter(UsbManager.ACTION_USB_DEVICE_DETACHED));
        registerReceiver(broadcast_receiver_, new IntentFilter(ACTION_USB_PERMISSION));

        permission_intent_ = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);

        connected_devices_ = new HashMap<String, SwitchController>();

        vibrator_ = (Vibrator)getSystemService(VIBRATOR_SERVICE);

        handler_ = new Handler();
        timer_ = new Timer(false);
        timer_task_ = null;
    }

    private void search_device() {
        HashMap<String, UsbDevice> devices = manager_.getDeviceList();
        Iterator<UsbDevice> itr = devices.values().iterator();
        boolean found = false;

        Log.d("Java", "search device: " + String.valueOf(devices.size()));

        if(devices.size() > 0) {
            while (itr.hasNext()) {
                UsbDevice device = itr.next();

                if (device.getVendorId() == VID_NINTENDO && device.getProductId() == PID_PROCONTROLLER) {
                    if (!connected_devices_.containsKey(device.getDeviceName())) {
                        SwitchController controller = new SwitchController(device);
                        controller.setState(SwitchController.State.ATTACHED);

                        connected_devices_.put(device.getDeviceName(), controller);
                    }
                }
            }


            for (SwitchController value : connected_devices_.values()) {
                if (value.getState() == SwitchController.State.ATTACHED) {
                    manager_.requestPermission(value.getDevice(), permission_intent_);
                    found = true;
                }
            }
        } else {
            connected_devices_.clear();
        }

        if(!found)
            timer_.cancel();
    }

    private String all_device() {
        String result = "";
        for(String key : connected_devices_.keySet()) {
            result += key + ",";
        }

        if(result.length() > 0)
            result = result.substring(0, result.length() - 2);

        Log.d("Java", "all device: " + result);

        return result;
    }

    private Timer make_timer() {
        Timer timer = new Timer(false);
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                handler_.post(new Runnable() {
                    @Override
                    public void run() {
                        search_device();
                    }
                });
            }
        }, 0, 500);

        return timer;
    }

    private final BroadcastReceiver broadcast_receiver_ = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d("Java", "action: " + action);

            if(ACTION_USB_PERMISSION.equals(action)) {
                Log.d("Java", "ACTION_USB_PERMISSION");

                synchronized (this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if(device != null) {
                            if(connected_devices_.containsKey(device.getDeviceName())) {
                                SwitchController controller = connected_devices_.get(device.getDeviceName());

                                if(controller.getState() == SwitchController.State.ATTACHED) {
                                    UsbDeviceConnection connection = manager_.openDevice(controller.getDevice());
                                    connection.claimInterface(device.getInterface(0), true);

                                    controller.setConnection(connection);
                                    controller.setState(SwitchController.State.PERMITTED);
                                    device_attached(controller.getDevice().getDeviceId(), controller.getDevice().getProductId(), controller.getDevice().getDeviceName());

                                    Log.d("Java", "device: " + device.getDeviceId() + " DeviceName: " + device.getDeviceName());
                                } else {
                                    Log.d("Java", "device is already permitted: " + device);
                                }
                            }
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
                        SwitchController controller = new SwitchController(device);
                        controller.setState(SwitchController.State.ATTACHED);

                        connected_devices_.put(device.getDeviceName(), controller);

                        timer_ = make_timer();
                    }
                }
            } else if(UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                Log.d("Java", "ACTION_USB_DEVICE_DETACHED");

                synchronized(this) {
                    UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    Log.d("Java", "device: " + device.getDeviceId() + " DeviceName: " + device.getDeviceName());

                    if(connected_devices_.containsKey(device.getDeviceName())) {
                        SwitchController controller = connected_devices_.get(device.getDeviceName());
                        device_detached(controller.getDevice().getDeviceId(), controller.getDevice().getProductId(), controller.getDevice().getDeviceName());
                        connected_devices_.remove(device.getDeviceName());
                    }
                }
            }
        }
    };


    public int write(String device_name, byte[] data) {
        int ret = -1;
        synchronized (this) {
            if (connected_devices_.containsKey(device_name)) {
                SwitchController controller = connected_devices_.get(device_name);
                UsbDevice device = controller.getDevice();
                UsbDeviceConnection connection = controller.getConnection();

                for (int i = 0; i < device.getInterfaceCount(); i++) {
                    UsbInterface intf = device.getInterface(i);

                    UsbEndpoint endpoint= null;

                    for (int j = 0; j < intf.getEndpointCount(); j++) {
                        if (intf.getEndpoint(j).getDirection() == UsbConstants.USB_DIR_OUT) {
                            endpoint = intf.getEndpoint(j);
                            break;
                        }
                    }

                    ret = connection.bulkTransfer(endpoint, data, data.length, 0);
                    Log.d("Java", "command witten. Length: " + String.valueOf(ret));
                }
            }
        }

        return ret;
    }

    public int read(String device_name, byte[] dst) {
        int ret = -1;
        synchronized (this) {
            if (connected_devices_.containsKey(device_name)) {
                SwitchController controller = connected_devices_.get(device_name);
                UsbDevice device = controller.getDevice();
                UsbDeviceConnection connection = controller.getConnection();

                for (int i = 0; i < device.getInterfaceCount(); i++) {
                    UsbInterface intf = device.getInterface(i);

                    UsbEndpoint endpoint = null;

                    for (int j = 0; j < intf.getEndpointCount(); j++) {
                        if (intf.getEndpoint(j).getDirection() == UsbConstants.USB_DIR_IN) {
                            endpoint = intf.getEndpoint(j);
                            break;
                        }
                    }

                    UsbRequest request = new UsbRequest();
                    if (!request.initialize(connection, endpoint)) return ret;

                    ret = connection.bulkTransfer(endpoint, dst, dst.length, 1000);

                    request.close();


                    StringBuilder sb = new StringBuilder();
                    for (byte d : dst) {
                        sb.append(String.format("0x%02X ", d));
                    }
                    String str = sb.toString();

                    Log.d("Java", "data read.: " + str);
                }
            }
        }

        return ret;
    }

    public void vibrate() {
        if(vibrator_.hasVibrator()) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                long pattern[] = {200, 200, 200, 200};
                int amplitudes[] = {0, 255, 0, 255};
                vibrator_.vibrate(VibrationEffect.createWaveform(pattern, amplitudes,-1));
            } else {
                long pattern[] = {200, 200, 200, 200};
                vibrator_.vibrate(pattern, -1);
            }
        }
    }
}
