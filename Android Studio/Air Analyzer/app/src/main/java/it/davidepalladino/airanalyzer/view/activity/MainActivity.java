package it.davidepalladino.airanalyzer.view.activity;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.fragment.app.FragmentManager;

import android.app.DatePickerDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.DatePicker;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.ExtendedFloatingActionButton;
import com.google.android.material.tabs.TabLayout;

import java.util.ArrayList;
import java.util.Calendar;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.DatabaseService;
import it.davidepalladino.airanalyzer.control.Setting;
import it.davidepalladino.airanalyzer.model.Date;
import it.davidepalladino.airanalyzer.model.Room;
import it.davidepalladino.airanalyzer.view.dialog.RemoveRoomDialog;
import it.davidepalladino.airanalyzer.view.dialog.RenameRoomDialog;
import it.davidepalladino.airanalyzer.view.fragment.AddRoomFragment;
import it.davidepalladino.airanalyzer.view.fragment.RoomFragment;

import static it.davidepalladino.airanalyzer.control.DatabaseService.REQUEST_CODE;
import static it.davidepalladino.airanalyzer.control.Setting.TOKEN;

public class MainActivity extends AppCompatActivity implements DatePickerDialog.OnDateSetListener, TabLayout.OnTabSelectedListener {
    private static final String BROADCAST_REQUEST_CODE_MASTER = "MainActivity";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_GET_ROOM = "GetRoom";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_LOGIN = "Login";
    private static final String TAB_ADD = "Add";
    public static final String INTENT_ROOM = "ROOM";
    private static final int MAX_ATTEMPS = 3;

    private FragmentManager fragmentManager;
    private RoomFragment roomFragment;

    private TabLayout tabbarRoom;

    private Calendar calendar;

    private Setting setting;
    private Room roomSelected;
    private Date date;

    private int attempts = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbarMain = (Toolbar) findViewById(R.id.toolbarMain);
        setSupportActionBar(toolbarMain);

        tabbarRoom = (TabLayout) findViewById(R.id.tabbarRoom);
        tabbarRoom.setLongClickable(true);
        tabbarRoom.addOnTabSelectedListener(this);

        ExtendedFloatingActionButton floatingActionButtonCalendar = (ExtendedFloatingActionButton) findViewById(R.id.floatingActionButtonCalendar);
        floatingActionButtonCalendar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new DatePickerDialog(
                        MainActivity.this,
                        MainActivity.this,
                        calendar.get(Calendar.YEAR),
                        calendar.get(Calendar.MONTH),
                        calendar.get(Calendar.DAY_OF_MONTH)
                ).show();
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        setting = new Setting(MainActivity.this);

        roomSelected = new Room();

        calendar = Calendar.getInstance();
        date = new Date(
                String.valueOf(calendar.get(Calendar.DAY_OF_MONTH)),
                String.valueOf(calendar.get(Calendar.MONTH) + 1),
                String.valueOf(calendar.get(Calendar.YEAR))
        );
    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(broadcastReceiver, new IntentFilter(DatabaseService.BROADCAST));

        Intent intentDatabaseService = new Intent(MainActivity.this, DatabaseService.class);
        bindService(intentDatabaseService, serviceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onPause() {
        super.onPause();

        unbindService(serviceConnection);
        unregisterReceiver(broadcastReceiver);
    }

    @Override
    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
        calendar.set(Calendar.YEAR, year);
        calendar.set(Calendar.MONTH, month);
        calendar.set(Calendar.DAY_OF_MONTH, dayOfMonth);

        date.day = String.valueOf(dayOfMonth);
        date.month = String.valueOf(month + 1);
        date.year = String.valueOf(year);

        updateFragmentRoom(roomSelected.getId(), date);
    }

    @Override
    public boolean onOptionsItemSelected (MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menuItemRenameThisRoom:
                RenameRoomDialog renameRoomDialog = new RenameRoomDialog();
                renameRoomDialog.setRoom(roomSelected);
                renameRoomDialog.setToken(setting.readToken());
                renameRoomDialog.show(getSupportFragmentManager(), "");
            case R.id.menuItemRemoveThisRoom:
                RemoveRoomDialog removeRoomDialog = new RemoveRoomDialog();
                removeRoomDialog.setRoom(roomSelected);
                removeRoomDialog.setToken(setting.readToken());
                removeRoomDialog.show(getSupportFragmentManager(), "");
            case R.id.menuItemLogout:
                setting.saveToken("");
                setting.saveLogin(null);

                Intent intentTo = new Intent(MainActivity.this, LoginActivity.class);
                startActivity(intentTo);
                finish();
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_toolbar_main, menu);
        return true;
    }

    @Override
    public void onTabSelected(TabLayout.Tab tab) {
        if (((String) tab.getTag()).compareTo(TAB_ADD) != 0) {
            roomSelected.setId((String) tab.getTag().toString());
            roomSelected.setName(tab.getText().toString());

            updateFragmentRoom(roomSelected.getId(), date);
        } else {
            fragmentManager.beginTransaction().replace(R.id.frameRoom, new AddRoomFragment()).commit();
        }
    }

    @Override
    public void onTabUnselected(TabLayout.Tab tab) {
    }

    @Override
    public void onTabReselected(TabLayout.Tab tab) {
    }

    public void updateFragmentRoom(String roomID, Date date) {
        roomFragment = RoomFragment.newInstance(roomID, date);
        fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().replace(R.id.frameRoom, roomFragment).commit();
    }

    public void createTabbar(ArrayList<Room> listRooms) {
        tabbarRoom.removeAllTabs();

        for (int i = 0; i < listRooms.size(); i++) {
            TabLayout.Tab tab = tabbarRoom.newTab();
            if (listRooms.get(i).getName() == null) {
                tab.setText(listRooms.get(i).getId());
            } else {
                tab.setText(listRooms.get(i).getId() + " - " + listRooms.get(i).getName());
            }

            tab.setTag(listRooms.get(i).getId());
            tabbarRoom.addTab(tab);
        }

        TabLayout.Tab tab = tabbarRoom.newTab();
        tab.setIcon(R.drawable.ic_baseline_add_24);
        tab.setTag(TAB_ADD);
        tabbarRoom.addTab(tab);
    }

    public DatabaseService databaseService;
    public ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            DatabaseService.LocalBinder localBinder = (DatabaseService.LocalBinder) service;
            databaseService = localBinder.getService();

            databaseService.getRoom(setting.readToken(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_ROOM);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
        }
    };

    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context contextFrom, Intent intentFrom) {
            if (intentFrom != null) {
                if (intentFrom.hasExtra(DatabaseService.REQUEST_CODE) && intentFrom.hasExtra(DatabaseService.STATUS_CODE)) {
                    int statusCode = intentFrom.getIntExtra(DatabaseService.STATUS_CODE, 0);
                    switch (statusCode) {
                        case 200:
                            // GET ROOM BROADCAST
                            if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_ROOM) == 0) {
                                ArrayList<Room> listRooms = intentFrom.getParcelableArrayListExtra(INTENT_ROOM);

                                if (listRooms.isEmpty() && attempts <= MAX_ATTEMPS) {
                                    new Handler().postDelayed(new Runnable() {
                                        @Override
                                        public void run() {
                                            databaseService.getRoom(setting.readToken(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_ROOM);
                                            attempts++;
                                        }
                                    }, 1000);

                                    return;
                                }

                                createTabbar(listRooms);
                                attempts = 1;

                            // SET ROOM BROADCAST
                            // REMOVE ROOM BROADCAST
                            } else if (
                                    (intentFrom.getStringExtra(REQUEST_CODE).compareTo(RenameRoomDialog.BROADCAST_REQUEST_CODE_MASTER + RenameRoomDialog.BROADCAST_REQUEST_CODE_EXTENSION_SET_ROOM) == 0) ||
                                     (intentFrom.getStringExtra(REQUEST_CODE).compareTo(RemoveRoomDialog.BROADCAST_REQUEST_CODE_MASTER + RemoveRoomDialog.BROADCAST_REQUEST_CODE_EXTENSION_REMOVE_ROOM) == 0)
                            ) {
                                databaseService.getRoom(setting.readToken(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_ROOM);

                            // LOGIN BROADCAST
                            } else if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_LOGIN) == 0) {
                                setting.saveToken(intentFrom.getStringExtra(TOKEN));
                                attempts = 1;
                            }

                            break;
                        case 204:
                        case 401:
                            if (attempts <= MAX_ATTEMPS) {
                                new Handler().postDelayed(new Runnable() {
                                    @Override
                                    public void run() {
                                        databaseService.login(setting.readLogin(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_LOGIN);
                                        attempts++;
                                    }
                                }, 1000);
                            } else {
                                Intent intentTo = new Intent(MainActivity.this, LoginActivity.class);
                                startActivity(intentTo);
                                finish();
                            }

                            break;
                        case 404:
                        case 500:
                            Toast.makeText(MainActivity.this, getString(R.string.toastServerOffline), Toast.LENGTH_LONG).show();
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    };
}