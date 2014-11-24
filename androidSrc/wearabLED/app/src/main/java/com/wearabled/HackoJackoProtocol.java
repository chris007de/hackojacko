package com.wearabled;

import android.graphics.Color;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by Patrick Rehm on 24.11.2014.
 */
public class HackoJackoProtocol {

    public static void sendAllOffCommand() {
        sendCommand(0x00);
    }

    public static void sendAllOnCommand() {
        sendCommand(0x02);
    }

    public static void sendColorCommand(int color) {

        ConnectionThread conThread = HandleHelper.getConnectionThread();
        if (null == conThread) {
            return;
        }
        int r = Color.red(color);
        int g = Color.green(color);
        int b = Color.blue(color);

        byte[] rBytes = ByteBuffer.allocate(4).putInt(r).array();
        byte[] gBytes = ByteBuffer.allocate(4).putInt(g).array();
        byte[] bBytes = ByteBuffer.allocate(4).putInt(b).array();

        byte rByte = rBytes[3];
        byte gByte = gBytes[3];
        byte bByte = bBytes[3];

        byte[] colorBytes = new byte[] {rByte, gByte, bByte, 0x10, 0x13};
        conThread.write(colorBytes);
        Log.d("COLOR", "R: " + String.valueOf(rByte));
        Log.d("COLOR" , "G: " + String.valueOf(gByte));
        Log.d("COLOR" , "B: " + String.valueOf(bByte));
    }

    public static void activatePreset(int presetId) {
        sendCommand(presetId);
    }

    private static void sendCommand(int commandId) {
        ConnectionThread conThread = HandleHelper.getConnectionThread();
        if (null == conThread) {
            return;
        }
        HandleHelper.getConnectionThread().write("2\n".getBytes());
    }
}
