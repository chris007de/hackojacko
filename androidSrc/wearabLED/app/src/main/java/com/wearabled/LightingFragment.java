package com.wearabled;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.wearabled.HandleHelper;
import com.wearabled.ConnectionThread;

import com.wearabled.R;

public class LightingFragment extends Fragment {

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
                HandleHelper.getConnectionThread().write("1\n".getBytes());
            }
        });

        Button darkButton = (Button) rootView.findViewById(R.id.lightOff);
        darkButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                HandleHelper.getConnectionThread().write("0\n".getBytes());
            }
        });
        return  rootView;
    }
}
