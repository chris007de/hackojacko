package com.wearabled;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.graphics.Color;

import java.nio.ByteBuffer;
import com.larswerkman.holocolorpicker.ColorPicker;
import com.larswerkman.holocolorpicker.ValueBar;

/*
 * \TODO Add Toast for unconnected message sends
 * \TODO Encapsulate the HackoJackoProtocol in an extra class
 */
public class LightingFragment extends Fragment implements ColorPicker.OnColorChangedListener {
    public LightingFragment() {

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.fragment_lighting, container, false);

        Button lightButton = (Button) rootView.findViewById(R.id.makeLights);
        lightButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ConnectionThread conThread = HandleHelper.getConnectionThread();
                if (null == conThread) {
                    return;
                }
                HandleHelper.getConnectionThread().write("1\n".getBytes());
            }
        });

        Button darkButton = (Button) rootView.findViewById(R.id.lightOff);
        darkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                ConnectionThread conThread = HandleHelper.getConnectionThread();
                if (null == conThread) {
                    return;
                }
                HandleHelper.getConnectionThread().write("2\n".getBytes());
            }
        });

        Button blinkButton = (Button) rootView.findViewById(R.id.blink);
        blinkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                /*
                 * \Todo: Send Blink command
                 */
            }
        });

        Button runButton = (Button) rootView.findViewById(R.id.run);
        runButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                /*
                 * \Todo: Send Run command
                 */
            }
        });

        Button randomButton = (Button) rootView.findViewById(R.id.random);
        randomButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                /*
                 * \Todo: Send random command
                 */
            }
        });

        ColorPicker picker = (ColorPicker) rootView.findViewById(R.id.picker);
        ValueBar valueBar = (ValueBar) rootView.findViewById(R.id.valuebar);
        picker.addValueBar(valueBar);
        picker.getColor();
        picker.setOldCenterColor(picker.getColor());
        picker.setOnColorChangedListener(this);
        picker.setShowOldCenterColor(false);

        return  rootView;
    }

    @Override
    public void onColorChanged(int color) {
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
        Log.d("COLOR" , "R: " + String.valueOf(rByte));
        Log.d("COLOR" , "G: " + String.valueOf(gByte));
        Log.d("COLOR" , "B: " + String.valueOf(bByte));
    }
}
