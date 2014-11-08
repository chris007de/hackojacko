package com.wearabled;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;

import java.io.IOException;
import java.util.ArrayList;


public class ConnectionThread extends Thread {
    private final BluetoothSocket mSocket;
    private final java.io.InputStream mInStream;
    private final java.io.OutputStream mOutStream;
    private final Handler mHandler;
    //private final Handler tempUpdateHandler;
    private final int STARTSYMBOL = 0x42;

    public ConnectionThread(BluetoothSocket socket, Handler handler) {
        mSocket = socket;
        mHandler = handler;
        //tempUpdateHandler = HandleHelper.getmTemperatureHandler();
        java.io.InputStream tmpIn = null;
        java.io.OutputStream tmpOut = null;


        // Get the input and output streams, using temp objects because
        // member streams are final
        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) { }

        mInStream = tmpIn;
        mOutStream = tmpOut;
    }

    public void run() {
        try {
            while (true) {
                int startSymbol = mInStream.read();
                /*
                 * Sync to the Frame length...
                 */
                if (startSymbol == STARTSYMBOL) {
                    int version = mInStream.read();
                    int senderId = mInStream.read();
                    int noOfSensors = mInStream.read();
                    ArrayList<Integer> temps = new ArrayList<Integer>();
                    for (int i = 0; i <  noOfSensors; i++) {
                        int curTempLow = mInStream.read();
                        int curTempHigh = mInStream.read();
                        int curTemp = (curTempHigh << 1) + curTempLow;
                        temps.add(i, curTemp);
                        String msg = String.valueOf(i) + ";" + String.valueOf(curTemp);
                        //tempUpdateHandler.obtainMessage(0, msg).sendToTarget();
                    }
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /* Call this from the main activity to send data to the remote device */
    public void write(byte[] bytes) {
        try {
            mOutStream.write(bytes);
        } catch (IOException e) { }
    }

    /* Call this from the main activity to shutdown the connection */
    public void cancel() {
        try {
            mSocket.close();
        } catch (IOException e) { }
    }
}
