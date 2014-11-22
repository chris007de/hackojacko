package com.wearabled;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;
import android.content.Context;

import java.io.IOException;
import java.util.UUID;


public class ConnectThread extends Thread {

    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;
    private final String mUUIDString = "00001101-0000-1000-8000-00805f9b34fb";
    private final Handler mHandler;
    private final Context mContext;


    public ConnectThread(BluetoothDevice device, Handler handler, Context context) {
        // Use a temporary object that is later assigned to mmSocket,
        // because mmSocket is final
        BluetoothSocket tmp = null;
        mmDevice = device;
        mHandler = handler;
        mContext = context;

        // Get a BluetoothSocket to connect with the given BluetoothDevice
        try {
            // MY_UUID is the app's UUID string, also used by the server code
            tmp = device.createRfcommSocketToServiceRecord(UUID.fromString(mUUIDString));
            mHandler.obtainMessage(
                    0,
                    mContext.getString(R.string.SocketCreationSuccess)).sendToTarget();
        } catch (IOException e) {
            mHandler.obtainMessage(
                    0,
                    mContext.getString(R.string.SocketCreationFailure)).sendToTarget();
        }
        mmSocket = tmp;
    }

    public void run() {
        try {
            mmSocket.connect();
        } catch (IOException connectException) {
            Log.d("ConnectionHandler", connectException.toString());
            try {
                mmSocket.close();
            } catch (IOException closeException) { }
            return;
        }
        mHandler.obtainMessage(
                0,
                mContext.getString(R.string.ConnectionSuccessful)).sendToTarget();
        manageConnectedSocket(mmSocket);
    }

    private void manageConnectedSocket(BluetoothSocket socket)
    {
        ConnectionThread conThread = new ConnectionThread(socket, mHandler);
        conThread.start();
    }

    /** Will cancel an in-progress connection, and close the socket */
    public void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) { }
    }
}