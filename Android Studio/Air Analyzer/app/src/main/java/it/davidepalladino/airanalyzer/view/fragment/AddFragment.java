package it.davidepalladino.airanalyzer.view.fragment;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.os.IBinder;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;

import java.util.ArrayList;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.DatabaseService;
import it.davidepalladino.airanalyzer.control.Setting;
import it.davidepalladino.airanalyzer.model.Room;
import it.davidepalladino.airanalyzer.view.activity.MainActivity;
import it.davidepalladino.airanalyzer.view.widget.Toast;

import static android.content.Context.BIND_AUTO_CREATE;
import static it.davidepalladino.airanalyzer.control.DatabaseService.REQUEST_CODE;
import static it.davidepalladino.airanalyzer.control.IntentConst.INTENT_ROOM;

public class AddFragment extends Fragment implements AdapterView.OnItemSelectedListener, View.OnClickListener {
    public static final String BROADCAST_REQUEST_CODE_MASTER = "AddFragment";
    public static final String BROADCAST_REQUEST_CODE_EXTENSION_GET_ACTIVE_ROOM = "GetActiveRooms";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_GET_INACTIVE_ROOMS = "GetInactiveRooms";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_ADD_ROOM = "AddRoom";

    private LinearLayout linearLayoutAddRoom;
    private Spinner spinnerRooms;
    private EditText editTextLocalIP;
    private Button buttonAddRoom;
    private Button buttonAddDevice;

    private Toast toast;
    private Setting setting;
    public Room roomSelected;

    public static AddFragment newInstance() {
        AddFragment fragment = new AddFragment();
        return fragment;
    }

    @Override
    public void onStart() {
        super.onStart();
        toast = new Toast(getActivity(), getLayoutInflater());
        setting = new Setting(getContext());
    }

    @Override
    public void onResume() {
        super.onResume();

        getActivity().registerReceiver(broadcastReceiver, new IntentFilter(DatabaseService.BROADCAST));

        Intent intentDatabaseService = new Intent(getActivity(), DatabaseService.class);
        getActivity().bindService(intentDatabaseService, serviceConnection, BIND_AUTO_CREATE);
    }

    @Override
    public void onPause() {
        super.onPause();

        getActivity().unbindService(serviceConnection);
        getActivity().unregisterReceiver(broadcastReceiver);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View layoutFragment = inflater.inflate(R.layout.fragment_add, container, false);

        linearLayoutAddRoom = layoutFragment.findViewById(R.id.linearLayoutAddRoom);

        spinnerRooms = layoutFragment.findViewById(R.id.spinnerRooms);
        spinnerRooms.setOnItemSelectedListener(this);

        editTextLocalIP = layoutFragment.findViewById(R.id.editTextLocalIP);

        buttonAddRoom = layoutFragment.findViewById(R.id.buttonAddRoom);
        buttonAddRoom.setOnClickListener(this);

        buttonAddDevice = layoutFragment.findViewById(R.id.buttonAddDevice);
        buttonAddDevice.setOnClickListener(this);

        return layoutFragment;
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        roomSelected = (Room) parent.getItemAtPosition(position);
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.buttonAddRoom:
                databaseService.addRoom(setting.readToken(), roomSelected, BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_ADD_ROOM);

                break;
            case R.id.buttonAddDevice:
                //TODO Implement the Socket Connection with a new Service.

                break;
        }
    }

    public DatabaseService databaseService;
    public ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            DatabaseService.LocalBinder localBinder = (DatabaseService.LocalBinder) service;
            databaseService = localBinder.getService();

            databaseService.getInactiveRooms(setting.readToken(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_INACTIVE_ROOMS);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
        }
    };

    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context contextFrom, Intent intentFrom) {
            if (intentFrom != null) {
                if (intentFrom.hasExtra(REQUEST_CODE) && intentFrom.hasExtra(DatabaseService.STATUS_CODE)) {
                    int statusCode = intentFrom.getIntExtra(DatabaseService.STATUS_CODE, 0);
                    switch (statusCode) {
                        case 200:
                            // GET INACTIVE ROOMS BROADCAST
                            if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_INACTIVE_ROOMS) == 0) {
                                linearLayoutAddRoom.setVisibility(View.VISIBLE);

                                ArrayList<Room> listRooms = intentFrom.getParcelableArrayListExtra(INTENT_ROOM);

                                ArrayAdapter<Room> arrayRooms = new ArrayAdapter<Room>(getContext(), android.R.layout.simple_spinner_item, listRooms);
                                arrayRooms.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

                                spinnerRooms.setAdapter(arrayRooms);

                            // ADD ROOM BROADCAST
                            } else if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_ADD_ROOM) == 0) {
                                databaseService.getActiveRooms(setting.readToken(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_ACTIVE_ROOM);
                            }

                            break;
                        case 204:
                            if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_INACTIVE_ROOMS) == 0) {
                                linearLayoutAddRoom.setVisibility(View.GONE);
                            }

                            break;
                        case 401:
                            break;
                        case 404:
                        case 500:
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    };
}