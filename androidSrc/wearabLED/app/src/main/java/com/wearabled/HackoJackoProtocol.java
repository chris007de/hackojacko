package com.wearabled;

import android.graphics.Color;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by Patrick Rehm on 24.11.2014.
 */
public class HackoJackoProtocol {

    public static final byte T_PRESET   = 0x00;
    public static final byte P_ALL_OFF  = 0x00;
    public static final byte P_ALL_ON   = 0x01;
    public static final byte P_BLINK    = 0x02;
    public static final byte P_RUN      = 0x03;
    public static final byte P_RANDOM   = 0x04;

    public static final byte T_DIRECT = 0x01;

    public static final byte T_SPEED = 0x02;


    private static final byte HEADERLEN = 0x08;

    public static void sendColorCommand(int color) {

        byte rByte = (byte)((color >> 16) & 0xFF);
        byte gByte = (byte)((color >> 8) & 0xFF);
        byte bByte = (byte)((color) & 0xFF);

        byte[] colorBytes = new byte[]{rByte, gByte, bByte};

         /*byte[] colorBytes = new byte[noLeds * 3];
         for (int i = 0; i < noLeds * 3; i += 3)
         {
            colorBytes[i] = rByte;
            colorBytes[i + 1] = gByte;
            colorBytes[i + 2] = bByte;
         }*/

        byte[] header = constructHeader(T_DIRECT, 3);
        sendMsg(marshallPacket(header, colorBytes));
    }

    public static void sendSpeedCommand (byte speed) {
        byte[] speedByte = new byte[] {speed};
        byte[] header = constructHeader (T_SPEED, 1);
        sendMsg(marshallPacket(header, speedByte));
    }

    public static void activatePreset(byte presetId) {

        int msgLen = 0;
        byte[] body;

        switch(presetId) {
            case P_ALL_OFF:
            case P_ALL_ON:
            case P_BLINK:
            case P_RANDOM:
            case P_RUN:
                body = new byte[]{presetId};
                msgLen = 1;
                break;
            default:
                // unkown presetId
                return;
        }
        byte[] header = constructHeader(T_PRESET, msgLen);
        sendMsg(marshallPacket(header, body));
    }

    private static void sendMsg (byte [] msg) {
        ConnectionThread conThread = HandleHelper.getConnectionThread();
        if (null == conThread) {
            return;
        }
        HandleHelper.getConnectionThread().write(msg);
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
