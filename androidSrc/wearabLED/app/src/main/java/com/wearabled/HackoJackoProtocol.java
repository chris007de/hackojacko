package com.wearabled;

import android.graphics.Color;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by Patrick Rehm on 24.11.2014.
 */
public class HackoJackoProtocol {

    public static final byte OFFCOMMAND = 0x00;
    public static final byte ONCOMMAND = 0x01;
    public static final byte PRESETCOMMAND = 0x02;

    private static final byte HEADERLEN = 0x08;

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

        byte[] colorBytes = new byte[]{rByte, gByte, bByte};
        Log.d("COLOR", "R: " + String.valueOf(r));
        Log.d("COLOR", "G: " + String.valueOf(g));
        Log.d("COLOR", "B: " + String.valueOf(b));

        byte[] header = constructHeader(PRESETCOMMAND, 3);
        sendMsg(marshallPacket(header, colorBytes));
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
            sendMsg(constructHeader(commandId, 0));
        }
    }

    private static byte [] constructHeader(byte msgType, int msgLen) {
        int totalLen = msgLen + HEADERLEN;
        byte[] lenBytes = ByteBuffer.allocate(4).putInt(totalLen).array();
        byte[] header = new byte[HEADERLEN];
        header[0] = 'A';
        header[1] = 'J';
        header[2] = 'A';
        header[3] = 'B';
        header[4] = msgType;
        header[5] = lenBytes[3];
        header[6] = lenBytes[2];
        header[7] = 0; // \TODO insert CRC check!
        return header;
    }

    private static byte[] marshallPacket(byte[] header, byte[] body) {
        int length = header.length + body.length;
        byte[] msg = new byte[length];
        System.arraycopy(header, 0, msg, 0, header.length);
        System.arraycopy(body, 0, msg, header.length, body.length);
        return msg;
    }
}
