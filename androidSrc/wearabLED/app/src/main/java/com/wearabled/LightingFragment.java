package com.wearabled;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.content.Context;
import android.widget.Toast;
import android.graphics.Color;

import java.nio.ByteBuffer;

import com.wearabled.HandleHelper;
import com.wearabled.ConnectionThread;
import com.larswerkman.holocolorpicker.ColorPicker;
import com.larswerkman.holocolorpicker.ValueBar;

import com.wearabled.R;

public class LightingFragment extends Fragment implements ColorPicker.OnColorChangedListener {

    ConnectionThread mConThread = null;
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
                byte[] lol = new byte[] {0x54, 0x55, 0x56, 0x57};
                //HandleHelper.getConnectionThread().write("1".getBytes());
                HandleHelper.getConnectionThread().write("TUVW".getBytes());
            }
        });

        Button darkButton = (Button) rootView.findViewById(R.id.lightOff);
        darkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HandleHelper.getConnectionThread().write("2\n".getBytes());
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

        Context context = getActivity().getApplicationContext();
        CharSequence text = "Color: " + String.valueOf(color);
        int duration = Toast.LENGTH_SHORT;

        int r = Color.red(color);
        int g = Color.green(color);
        int b = Color.blue(color);

        //char rR = char (r);
        //char rG = char (g);
        //char rB = char (b);

        byte[] rBytes = ByteBuffer.allocate(4).putInt(r).array();
        byte[] gBytes = ByteBuffer.allocate(4).putInt(g).array();
        byte[] bBytes = ByteBuffer.allocate(4).putInt(b).array();

        byte rByte = rBytes[3];
        byte gByte = gBytes[3];
        byte bByte = bBytes[3];

        byte[] lol = new byte[] {rByte, gByte, bByte, 0x10, 0x13};

        //HandleHelper.getConnectionThread().write((String.valueOf(rC) + String.valueOf(rG) + String.valueOf(rB) + "\n").getBytes());
        //HandleHelper.getConnectionThread().write(lol);
        Log.d("COLOR" , "R: " + String.valueOf(rByte));
        Log.d("COLOR" , "G: " + String.valueOf(gByte));
        Log.d("COLOR" , "B: " + String.valueOf(bByte));


    }
}
