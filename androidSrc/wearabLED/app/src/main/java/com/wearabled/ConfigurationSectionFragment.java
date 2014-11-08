package com.wearabled;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Spinner;
import android.widget.TextView;
import android.content.SharedPreferences;
import android.widget.ProgressBar;
import android.util.Log;


import com.wearabled.R;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Set;

public class ConfigurationSectionFragment extends Fragment {

    public static final String CONFIG_FILE = "GrillThermConf";
    Button mConnectButton = null;
    BluetoothAdapter mBluetoothAdapter = null;
    ArrayList<String> mDeviceArray = null;
    ArrayAdapter<String> mDeviceAdapter = null;
    Spinner mSpinner = null;
    CheckBox mRememberChoice = null;
    TextView mStatusText = null;
    HashMap<String, BluetoothDevice> mDevicesByName = null;
    Handler mHandler = null;
    View rootView = null;
    ProgressBar mConfigProgress = null;
    public ConfigurationSectionFragment() {

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(
                R.layout.fragment_wearabled_config, container, false);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        mDevicesByName = new HashMap<String, BluetoothDevice>();
        mStatusText = (TextView) rootView.findViewById(R.id.statusTextView);
        mConfigProgress = (ProgressBar) rootView.findViewById((R.id.connectionProgress));

        setupSpinner(rootView);

        mHandler = new Handler(Looper.getMainLooper()) {

            @Override
            public void handleMessage(Message inputMessage) {
                // Gets the image task from the incoming Message object.
                String message = (String) inputMessage.obj; //Extract the string from the Message
                if (message.equals(getActivity().getString(R.string.ConnectionError))) {
                    enableGui();
                    resetGui();
                    mConfigProgress.setVisibility(View.INVISIBLE);
                }
                else if (message.equals(getActivity().getString(R.string.ConnectionSuccessful))) {
                    setGuiConnectedMode();
                }
                mStatusText.setText(message);
            }
        };

        mConnectButton = (Button) rootView.findViewById(R.id.connectButton);
        mConnectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                disableGui();
                mConfigProgress.setVisibility(View.VISIBLE);
                ConnectThread connectThread = new ConnectThread(
                        mDevicesByName.get(mSpinner.getSelectedItem().toString()),
                        mHandler,
                        getActivity());
                connectThread.start();
                SharedPreferences settings =
                        v.getContext().getSharedPreferences(CONFIG_FILE, 0);
                SharedPreferences.Editor editor = settings.edit();

                if (mRememberChoice.isChecked()) {
                    /*
                     * If the user wishes, we'll store the device...
                     */

                    editor.putString("prefDev",mSpinner.getSelectedItem().toString());
                    Log.d(
                            "CONNECTOR",
                            "Remembering BT Device: " + mSpinner.getSelectedItem().toString());
                } else {
                    editor.putString("prefDev", "none");
                    Log.d("CONNECTOR", "Not remembering devices anymore...");
                }
                editor.commit();

            }
        });

        return rootView;
    }

    private void disableGui()
    {
        mConnectButton.setClickable(false);
        mSpinner.setClickable(false);
        mRememberChoice.setClickable(false);
        mConnectButton.setAlpha(0.5f);
        mSpinner.setAlpha(0.5f);
        mRememberChoice.setAlpha(0.5f);
    }

    private void enableGui()
    {
        mConnectButton.setClickable(true);
        mSpinner.setClickable(true);
        mRememberChoice.setClickable(true);
        mConnectButton.setAlpha(1.0f);
        mSpinner.setAlpha(1.0f);
        mRememberChoice.setAlpha(1.0f);
    }

    private void resetGui()
    {
        mConnectButton.setText(getActivity().getString(R.string.connectButtonText));
    }

    private void setGuiConnectedMode()
    {
        mConfigProgress.setVisibility(View.INVISIBLE);
        /*
         * \TODO modularize the OnClickListeners for connecting and disconnecting
         * Until then, disconnecting is disabled...
        */
        /*
        mConnectButton.setClickable(true);
        mConnectButton.setAlpha(1.0f);
        mConnectButton.setText(getActivity().getString(R.string.disconnectButtonText));


        mConnectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                resetGui();
            }
        });*/
    }

    private void setupSpinner(View view)
    {
        mSpinner = (Spinner) view.findViewById(R.id.spinner);
        mDeviceArray = new ArrayList<String>();
        mDeviceAdapter =
                new ArrayAdapter<String>(
                        getActivity(),
                        android.R.layout.simple_spinner_item,
                        mDeviceArray);
        mDeviceAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpinner.setAdapter(mDeviceAdapter);
        mDeviceAdapter.notifyDataSetChanged();

        SharedPreferences settings =
                view.getContext().getSharedPreferences(CONFIG_FILE, 0);
        String prefDev = settings.getString("prefDev","none");
        Log.d("Connector" , "PrefDev: " + prefDev);

        mRememberChoice = (CheckBox) view.findViewById(R.id.rememberChoice);
        if (prefDev.equals("none")) {
            mRememberChoice.setChecked(false);
        } else {
            mRememberChoice.setChecked(true);
        }
        populateDeviceList();
        mSpinner.setSelection(getIndex(prefDev));
    }

    private void populateDeviceList()
    {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();

        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                // Add the name and address to an array adapter to show in a ListView
                //mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                mDeviceAdapter.add(device.getName());
                mDevicesByName.put(device.getName(), device);
            }
        }
    }

    private int getIndex(String myString){

        int index = 0;
        for (int i = 0; i < mSpinner.getCount(); i++ ){
            if (mSpinner.getItemAtPosition(i).equals(myString)){
                index = i;
            }
        }
        return index;
    }
}
