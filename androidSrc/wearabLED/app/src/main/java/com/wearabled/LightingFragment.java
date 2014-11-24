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
                HackoJackoProtocol.sendAllOnCommand();
            }
        });

        Button darkButton = (Button) rootView.findViewById(R.id.lightOff);
        darkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.sendAllOffCommand();
            }
        });

        Button blinkButton = (Button) rootView.findViewById(R.id.blink);
        blinkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset((byte)0x03);
            }
        });

        Button runButton = (Button) rootView.findViewById(R.id.run);
        runButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset((byte)0x04);
            }
        });

        Button randomButton = (Button) rootView.findViewById(R.id.random);
        randomButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HackoJackoProtocol.activatePreset((byte)0x05);
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

        HackoJackoProtocol.sendColorCommand(color);

    }
}
