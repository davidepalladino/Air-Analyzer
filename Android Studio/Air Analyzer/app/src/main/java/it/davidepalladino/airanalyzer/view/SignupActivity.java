package it.davidepalladino.airanalyzer.view;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;

import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.API;
import it.davidepalladino.airanalyzer.control.TextWatcherField;
import it.davidepalladino.airanalyzer.model.api.Login;
import it.davidepalladino.airanalyzer.model.api.Signup;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import static it.davidepalladino.airanalyzer.control.CheckField.*;

public class SignupActivity extends AppCompatActivity implements AdapterView.OnItemSelectedListener, TextWatcherField.AuthTextWatcherCallback {
    private static final String NAMEFILE_LOGIN = "login";

    private EditText editTextUsername;
    private EditText editTextPassword;
    private EditText editTextEmail;
    private EditText editTextName;
    private EditText editTextSurname;
    private EditText editTextAnswer1;
    private EditText editTextAnswer2;
    private EditText editTextAnswer3;

    private TextView textViewUsername;
    private TextView textViewPassword;
    private TextView textViewEmail;
    private TextView textViewName;
    private TextView textViewSurname;
    private TextView textViewAnswer1;
    private TextView textViewAnswer2;
    private TextView textViewAnswer3;

    private Spinner spinnerQuestions1;
    private Spinner spinnerQuestions2;
    private Spinner spinnerQuestions3;

    private Button buttonContinue;

    private API api;

    private Intent intentFrom;

    private String questionSelected1;
    private String questionSelected2;
    private String questionSelected3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);

        intentFrom = getIntent();

        editTextUsername = (EditText) findViewById(R.id.editTextUsername);
        editTextPassword = (EditText) findViewById(R.id.editTextPassword);
        editTextEmail = (EditText) findViewById(R.id.editTextEmail);
        editTextName = (EditText) findViewById(R.id.editTextName);
        editTextSurname = (EditText) findViewById(R.id.editTextSurname);
        editTextAnswer1 = (EditText) findViewById(R.id.editTextAnswer1);
        editTextAnswer2 = (EditText) findViewById(R.id.editTextAnswer2);
        editTextAnswer3 = (EditText) findViewById(R.id.editTextAnswer3);

        textViewUsername = (TextView) findViewById(R.id.textViewUsername);
        textViewPassword = (TextView) findViewById(R.id.textViewPassword);
        textViewEmail = (TextView) findViewById(R.id.textViewEmail);
        textViewName = (TextView) findViewById(R.id.textViewName);
        textViewSurname = (TextView) findViewById(R.id.textViewSurname);
        textViewAnswer1 = (TextView) findViewById(R.id.textViewAnswer1);
        textViewAnswer2 = (TextView) findViewById(R.id.textViewAnswer2);
        textViewAnswer3 = (TextView) findViewById(R.id.textViewAnswer3);

        spinnerQuestions1 = (Spinner) findViewById(R.id.spinnerQuestions1);
        spinnerQuestions2 = (Spinner) findViewById(R.id.spinnerQuestions2);
        spinnerQuestions3 = (Spinner) findViewById(R.id.spinnerQuestions3);

        buttonContinue = (Button) findViewById(R.id.buttonContinue);

        editTextUsername.addTextChangedListener(new TextWatcherField(this, editTextUsername));
        editTextPassword.addTextChangedListener(new TextWatcherField(this, editTextPassword));
        editTextEmail.addTextChangedListener(new TextWatcherField(this, editTextEmail));
        editTextName.addTextChangedListener(new TextWatcherField(this, editTextName));
        editTextSurname.addTextChangedListener(new TextWatcherField(this, editTextSurname));
        editTextAnswer1.addTextChangedListener(new TextWatcherField(this, editTextAnswer1));
        editTextAnswer2.addTextChangedListener(new TextWatcherField(this, editTextAnswer2));
        editTextAnswer3.addTextChangedListener(new TextWatcherField(this, editTextAnswer3));

        ArrayAdapter<String> adapterQuestions1 = new ArrayAdapter<String>(this, R.layout.item_spinner, getResources().getStringArray(R.array.Questions1));
        adapterQuestions1.setDropDownViewResource(R.layout.item_spinner_dropdown);
        spinnerQuestions1.setAdapter(adapterQuestions1);

        ArrayAdapter<String> adapterQuestions2 = new ArrayAdapter<String>(this, R.layout.item_spinner, getResources().getStringArray(R.array.Questions2));
        adapterQuestions2.setDropDownViewResource(R.layout.item_spinner_dropdown);
        spinnerQuestions2.setAdapter(adapterQuestions2);

        ArrayAdapter<String> adapterQuestions3 = new ArrayAdapter<String>(this, R.layout.item_spinner, getResources().getStringArray(R.array.Questions3));
        adapterQuestions3.setDropDownViewResource(R.layout.item_spinner_dropdown);
        spinnerQuestions3.setAdapter(adapterQuestions3);

        spinnerQuestions1.setOnItemSelectedListener(this);
        spinnerQuestions2.setOnItemSelectedListener(this);
        spinnerQuestions3.setOnItemSelectedListener(this);

        buttonContinue.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean error = false;

                if (!checkAuthEditText(editTextUsername)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextPassword)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextEmail)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextName)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextSurname)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextAnswer1)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextAnswer2)) {
                    error = true;
                }

                if (!checkAuthEditText(editTextAnswer3)) {
                    error = true;
                }

                if (!error) {
                    Signup signup = new Signup(
                            "",
                            editTextUsername.getText().toString(),
                            editTextPassword.getText().toString(),
                            editTextEmail.getText().toString(),
                            editTextName.getText().toString(),
                            editTextSurname.getText().toString(),
                            questionSelected1,
                            questionSelected2,
                            questionSelected3,
                            editTextAnswer1.getText().toString(),
                            editTextAnswer2.getText().toString(),
                            editTextAnswer3.getText().toString()
                    );

                    executeSignup(signup);
                } else {
                    Toast.makeText(SignupActivity.this, getString(R.string.toastErrorSignup), Toast.LENGTH_LONG).show();
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
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        if (parent.getId() == R.id.spinnerQuestions1) {
            questionSelected1 = parent.getItemAtPosition(position).toString();
        }

        if (parent.getId() == R.id.spinnerQuestions2) {
            questionSelected2 = parent.getItemAtPosition(position).toString();
        }

        if (parent.getId() == R.id.spinnerQuestions3) {
            questionSelected3 = parent.getItemAtPosition(position).toString();
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {

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

            case R.id.editTextEmail:
                errorTextView = textViewEmail;
                errorMessage = getString(R.string.errorEmail);

                if (!checkEmail(editText) && !editText.getText().toString().isEmpty()) {
                    error = true;
                    errorMessage = getString(R.string.noticeEmail);
                }

                break;

            case R.id.editTextName:
                errorTextView = textViewName;
                errorMessage = getString(R.string.errorName);
                break;

            case R.id.editTextSurname:
                errorTextView = textViewSurname;
                errorMessage = getString(R.string.errorSurname);
                break;

            case R.id.editTextAnswer1:
                errorTextView = textViewAnswer1;
                errorMessage = getString(R.string.errorAnswer);
                break;

            case R.id.editTextAnswer2:
                errorTextView = textViewAnswer2;
                errorMessage = getString(R.string.errorAnswer);
                break;

            case R.id.editTextAnswer3:
                errorTextView = textViewAnswer3;
                errorMessage = getString(R.string.errorAnswer);
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

    private void executeSignup(Signup signup) {
        /* Verifying if the username is already exists or not. */
        if (!editTextUsername.getText().toString().isEmpty()) {
            Call<Signup.NoResponse> call = api.checkUsername(editTextUsername.getText().toString());
            call.enqueue(new Callback<Signup.NoResponse>() {
                @Override
                public void onResponse(Call<Signup.NoResponse> call, Response<Signup.NoResponse> response) {
                    if (response.code() == 422) {
                        return;
                    }

                    if (response.code() == 201) {
                        /* Using direct the TextView of the class because is a async method. */
                        textViewUsername.setVisibility(View.VISIBLE);
                        textViewUsername.setText(getString(R.string.existsUsername));
                    }
                }

                @Override
                public void onFailure(Call<Signup.NoResponse> call, Throwable t) {

                }
            });
        }

        /* Verifying if the email is already exists or not. */
        if (!editTextEmail.getText().toString().isEmpty()) {
            Call<Signup.NoResponse> call = api.checkEmail(editTextEmail.getText().toString());
            call.enqueue(new Callback<Signup.NoResponse>() {
                @Override
                public void onResponse(Call<Signup.NoResponse> call, Response<Signup.NoResponse> response) {
                    if (response.code() == 422) {
                        return;
                    }

                    if (response.code() == 201) {
                        /* Using direct the TextView of the class because is a async method. */
                        textViewEmail.setVisibility(View.VISIBLE);
                        textViewEmail.setText(getString(R.string.existsEmail));
                    }
                }

                @Override
                public void onFailure(Call<Signup.NoResponse> call, Throwable t) {

                }
            });
        }

        /* Trying to signup the new user. */
        Call<Signup.NoResponse> call = api.signup(signup);
        call.enqueue(new Callback<Signup.NoResponse>() {
            @Override
            public void onResponse(Call<Signup.NoResponse> call, Response<Signup.NoResponse> response) {
                if (response.code() == 422) {
                    Toast.makeText(SignupActivity.this, getString(R.string.toastErrorSignup), Toast.LENGTH_LONG).show();
                } else if (response.code() == 201) {
                    saveFileLogin(signup);

                    new AlertDialog.Builder(SignupActivity.this)
                            .setTitle(R.string.nice)
                            .setMessage(R.string.alertDialogSignInSuccessful)
                            .setNeutralButton(R.string.ok, new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {
                                    finish();
                                }
                            })
                            .setIcon(R.drawable.ic_baseline_done_24)
                            .show();
                } else if (response.code() == 500) {
                    Toast.makeText(SignupActivity.this, getString(R.string.toastErrorServer), Toast.LENGTH_LONG).show();
                } else if (response.code() == 403) {
                    Toast.makeText(SignupActivity.this, getString(R.string.toastErrorService), Toast.LENGTH_LONG).show();
                }
            }

            @Override
            public void onFailure(Call<Signup.NoResponse> call, Throwable t) {

            }
        });
    }

    private void saveFileLogin(Signup signup) {
        try {
            FileOutputStream loginFOS = openFileOutput(NAMEFILE_LOGIN, MODE_PRIVATE);

            ObjectOutputStream loginOOS = new ObjectOutputStream(loginFOS);
            loginOOS.writeObject(new Login(signup.getUsername(), signup.getPassword()));
            loginOOS.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}