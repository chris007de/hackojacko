package com.wearabled;

import android.graphics.Color;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by Patrick Rehm on 24.11.2014.
 */
public class HackoJackoProtocol {

    public static final byte OFFCOMMAND    = 0x00;
    public static final byte ONCOMMAND     = 0x01;
    public static final byte PRESETCOMMAND = 0x02;

    private static final byte HEADERLEN    = 0x08;

    public static void sendAllOffCommand() {
        sendSimpleCommand(OFFCOMMAND);
    }

    public static void sendAllOnCommand() {
        sendSimpleCommand(ONCOMMAND);
    }

    public static void sendColorCommand(int color) {
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
        Log.d("COLOR", "R: " + String.valueOf(rByte));
        Log.d("COLOR" , "G: " + String.valueOf(gByte));
        Log.d("COLOR" , "B: " + String.valueOf(bByte));

        /*
         * \TODO construct the actual message
         */
        sendMsg(colorBytes);
    }

    public static void activatePreset(byte presetId) {
        /*
         * \TODO Preset command....
         */
    }

    private static void sendMsg (byte [] msg) {
        ConnectionThread conThread = HandleHelper.getConnectionThread();
        if (null == conThread) {
            return;
        }
        HandleHelper.getConnectionThread().write(msg);
    }

    private static void sendSimpleCommand(byte commandId) {
        if (ONCOMMAND == commandId || OFFCOMMAND == commandId) {
            sendMsg(constructHeader(commandId, HEADERLEN));
        }
    }

    private static byte [] constructHeader(byte msgType, byte msgLen) {
        /*
         * \TODO fill in the correct header bytes :)
         */
        byte[] header = new byte[HEADERLEN];
        header[0] = 'A';
        header[1] = 'J';
        header[2] = 'A';
        header[3] = 'B';
        header[4] = msgType;
        header[5] = 0;
        header[6] = msgLen;
        header[7] = 0;
        return header;
    }
}
