package it.davidepalladino.airanalyzer.view;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.API;
import it.davidepalladino.airanalyzer.control.TextWatcherField;
import it.davidepalladino.airanalyzer.model.api.Login;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import static it.davidepalladino.airanalyzer.control.CheckField.checkPassword;
import static it.davidepalladino.airanalyzer.model.IntentConst.*;

public class LoginActivity extends AppCompatActivity implements TextWatcherField.AuthTextWatcherCallback {
    private static final String NAMEFILE_LOGIN = "login";
    private static final String TOKEN = "token";

    private EditText editTextUsername;
    private EditText editTextPassword;

    private TextView textViewUsername;
    private TextView textViewPassword;

    private API api;

    private Login login;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        editTextUsername = (EditText) findViewById(R.id.editTextUsername);
        editTextPassword = (EditText) findViewById(R.id.editTextPassword);

        textViewUsername = (TextView) findViewById(R.id.textViewUsername);
        textViewPassword = (TextView) findViewById(R.id.textViewPassword);

        editTextUsername.addTextChangedListener(new TextWatcherField(this, editTextUsername));
        editTextPassword.addTextChangedListener(new TextWatcherField(this, editTextPassword));

        TextView textViewSignIn = (TextView) findViewById(R.id.textViewSignIn);
        textViewSignIn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intentToSigin = new Intent(LoginActivity.this, SignupActivity.class);
                startActivity(intentToSigin);
            }
        });

        Button buttonLogIn = (Button) findViewById(R.id.buttonLogIn);
        buttonLogIn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean error = false;

                if (!checkAuthEditText(editTextUsername)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextPassword)) {
                    error = true;
                }

                if (!error) {
                    Login login = new Login(
                            editTextUsername.getText().toString(),
                            editTextPassword.getText().toString()
                    );

                    executeLogin(login);
                } else {
                    Toast.makeText(LoginActivity.this, getString(R.string.toastErrorLogin), Toast.LENGTH_LONG).show();
                }
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(API.BASE_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        api = retrofit.create(API.class);

        readFileLogin(login);

        Intent intentFrom = getIntent();
        if (intentFrom != null && intentFrom.hasExtra(MESSAGE_TOAST)) {
            Toast.makeText(this, intentFrom.getStringExtra(MESSAGE_TOAST), Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public boolean checkAuthEditText(EditText editText) {
        TextView errorTextView = null;
        String errorMessage = "";

        boolean error = false;

        switch (editText.getId()) {
            case R.id.editTextUsername:
                errorTextView = textViewUsername;
                errorMessage = getString(R.string.errorUsername);

                break;

            case R.id.editTextPassword:
                errorTextView = textViewPassword;
                errorMessage = getString(R.string.errorPassowrd);

                if (!checkPassword(editText) && !editText.getText().toString().isEmpty()) {
                    error = true;
                    errorMessage = getString(R.string.noticePassword);
                }

                break;
        }

        if (editText.getText().toString().length() != 0 && !error) {
            errorTextView.setVisibility(View.GONE);

            return true;
        } else {
            errorTextView.setVisibility(View.VISIBLE);
            errorTextView.setText(errorMessage);

            return false;
        }
    }

    private void executeLogin(Login login) {
        Call<Login.Response> call = api.login(login);
        call.enqueue(new Callback<Login.Response>() {
            @Override
            public void onResponse(Call<Login.Response> call, Response<Login.Response> response) {
                if (response.code() == 422) {
                    Toast.makeText(LoginActivity.this, getString(R.string.toastErrorLogin), Toast.LENGTH_LONG).show();
                } else if (response.code() == 204) {
                    Toast.makeText(LoginActivity.this, getString(R.string.toastUserNotValidated), Toast.LENGTH_LONG).show();
                } else if (response.code() == 401) {
                    Toast.makeText(LoginActivity.this, getString(R.string.toastIncorrectUsernamePassword), Toast.LENGTH_LONG).show();
                } else if (response.code() == 200) {
                    saveFileLogin(login);

                    SharedPreferences login = getSharedPreferences(NAMEFILE_LOGIN, MODE_PRIVATE);
                    SharedPreferences.Editor loginEdit = login.edit();
                    loginEdit.putString(TOKEN, response.body().getToken()).apply();

                    Intent intentTo = new Intent(LoginActivity.this, MainActivity.class);
                    startActivity(intentTo);
                    finish();
                }

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

            editTextUsername.setText(login.getUsername());
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    private void saveFileLogin(Login login) {
        try {
            FileOutputStream loginFOS = openFileOutput(NAMEFILE_LOGIN, MODE_PRIVATE);

            ObjectOutputStream loginOOS = new ObjectOutputStream(loginFOS);
            loginOOS.writeObject(login);
            loginOOS.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}