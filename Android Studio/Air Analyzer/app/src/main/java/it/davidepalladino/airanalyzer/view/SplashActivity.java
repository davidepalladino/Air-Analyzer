package it.davidepalladino.airanalyzer.view;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.API;
import it.davidepalladino.airanalyzer.model.api.Login;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import static it.davidepalladino.airanalyzer.model.IntentConst.*;

public class SplashActivity extends AppCompatActivity {
    private static final String NAMEFILE_LOGIN = "login";
    private static final String TOKEN = "token";

    private API api;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);
    }

    @Override
    protected void onStart() {
        super.onStart();

        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(API.BASE_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        api = retrofit.create(API.class);

        Login login = new Login();
        readFileLogin(login);
    }

    private void executeLogin(Login login) {
        Call<Login.Response> call = api.login(login);
        call.enqueue(new Callback<Login.Response>() {
            @Override
            public void onResponse(Call<Login.Response> call, Response<Login.Response> response) {
                Intent intentTo;

                if (response.code() == 422) {
                    intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                } else if (response.code() == 204) {
                    intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                    intentTo.putExtra(MESSAGE_TOAST, getString(R.string.toastUserNotValidated));
                } else if (response.code() == 200) {
                    intentTo = new Intent(SplashActivity.this, MainActivity.class);

                    SharedPreferences login = getSharedPreferences(NAMEFILE_LOGIN, MODE_PRIVATE);
                    SharedPreferences.Editor loginEdit = login.edit();
                    loginEdit.putString(TOKEN, response.body().getToken()).apply();
                } else {
                    intentTo = new Intent(SplashActivity.this, LoginActivity.class);
                }

                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        startActivity(intentTo);
                        finish();
                    }
                }, 2000);
            }

            @Override
            public void onFailure(Call<Login.Response> call, Throwable t) {

            }
        });
    }

    private void readFileLogin(Login login) {
        try {
            FileInputStream loginFOS = openFileInput(NAMEFILE_LOGIN);

            ObjectInputStream loginOOS = new ObjectInputStream(loginFOS);
            login = (Login) loginOOS.readObject();
            loginOOS.close();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }

        executeLogin(login);
    }
}