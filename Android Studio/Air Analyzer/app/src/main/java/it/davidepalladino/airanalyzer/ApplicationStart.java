package it.davidepalladino.airanalyzer;

import android.app.Application;
import android.content.Intent;

import it.davidepalladino.airanalyzer.control.DatabaseService;

public class ApplicationStart extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        Intent intentDatabaseService = new Intent(this, DatabaseService.class);
        startService(intentDatabaseService);
    }
}
