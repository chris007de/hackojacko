package com.wearabled;

import android.graphics.Color;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by Patrick Rehm on 24.11.2014.
 */
public class HackoJackoProtocol {

    public static final byte OFFCOMMAND = 0x00;
    public static final byte COLCOMMAND = 0x02;
    public static final byte ONCOMMAND =  0x03;
    public static final byte PRESETCOMMAND = 0x01;

    private static final byte HEADERLEN = 0x08;

    public static void sendAllOffCommand() {
        sendSimpleCommand(OFFCOMMAND);
    }

    public static void sendAllOnCommand() {
        sendSimpleCommand(ONCOMMAND);
    }

    public static void sendColorCommand(int color, int noLeds) {
        int r = Color.red(color);
        int g = Color.green(color);
        int b = Color.blue(color);

        byte rByte = (byte) (r);
        byte gByte = (byte) (g);
        byte bByte = (byte) (b);

        byte[] colorBytes = new byte[]{rByte, gByte, bByte};

         /*byte[] colorBytes = new byte[noLeds * 3];
         for (int i = 0; i < noLeds * 3; i += 3)
         {
            colorBytes[i] = rByte;
            colorBytes[i + 1] = gByte;
            colorBytes[i + 2] = bByte;
         }*/

        byte[] header = constructHeader(COLCOMMAND, noLeds * 3);
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
