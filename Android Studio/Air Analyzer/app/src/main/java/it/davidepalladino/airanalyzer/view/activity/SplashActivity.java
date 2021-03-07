package it.davidepalladino.airanalyzer.view.activity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.controller.DatabaseService;
import it.davidepalladino.airanalyzer.controller.Setting;
import it.davidepalladino.airanalyzer.model.Login;

import static it.davidepalladino.airanalyzer.controller.DatabaseService.STATUS_CODE_SERVICE;
import static it.davidepalladino.airanalyzer.controller.Setting.NAMEPREFERENCE_TOKEN;
import static it.davidepalladino.airanalyzer.controller.IntentConst.*;

public class SplashActivity extends AppCompatActivity {
    private static final String BROADCAST_REQUEST_CODE_MASTER = "SplashActivity";

    private Setting setting;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);
    }

    @Override
    protected void onStart() {
        super.onStart();
        setting = new Setting(SplashActivity.this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(broadcastReceiver, new IntentFilter(INTENT_BROADCAST));

        Intent intentDatabaseService = new Intent(SplashActivity.this, DatabaseService.class);
        bindService(intentDatabaseService, serviceConnection, BIND_AUTO_CREATE);
    }

    @Override
    protected void onPause() {
        super.onPause();

        unbindService(serviceConnection);
        unregisterReceiver(broadcastReceiver);
    }

    public DatabaseService databaseService;
    public ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            DatabaseService.LocalBinder localBinder = (DatabaseService.LocalBinder) service;
            databaseService = localBinder.getService();

            Login login = setting.readLogin();
            if (login != null) {
                databaseService.login(login, BROADCAST_REQUEST_CODE_MASTER);
            } else {
                Intent intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                startActivity(intentTo);
                finish();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
        }
    };

    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context contextFrom, Intent intentFrom) {
            if (intentFrom != null) {
                if (intentFrom.hasExtra(DatabaseService.REQUEST_CODE_SERVICE) && intentFrom.hasExtra(STATUS_CODE_SERVICE)) {
                    if (intentFrom.getStringExtra(DatabaseService.REQUEST_CODE_SERVICE).compareTo(BROADCAST_REQUEST_CODE_MASTER) == 0) {
                        Intent intentTo = null;

                        int statusCode = intentFrom.getIntExtra(STATUS_CODE_SERVICE, 0);
                        switch (statusCode) {
                            case 200:
                                intentTo = new Intent(SplashActivity.this, MainActivity.class);
                                setting.saveToken(intentFrom.getStringExtra(NAMEPREFERENCE_TOKEN));

                                break;
                            case 204:
                                intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                                intentTo.putExtra(INTENT_MESSAGE_TOAST, getString(R.string.toastUserNotValidated));

                                break;
                            case 404:
                            case 500:
                                intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                                intentTo.putExtra(INTENT_MESSAGE_TOAST, getString(R.string.toastServerOffline));

                                break;
                            default:
                                intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                                break;
                        }

                        Intent finalIntentTo = intentTo;
                        new Handler().postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                if (finalIntentTo != null) {
                                    startActivity(finalIntentTo);
                                    finish();
                                }
                            }
                        }, 2000);
                    }
                }
            }
        }
    };
}