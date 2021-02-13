package it.davidepalladino.airanalyzer.control;

import android.widget.EditText;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CheckField {
    //TODO Check expression password
    private static final String expressionPassword = "((?=.*\\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[\\W]).{8,20})";
    private static final String expressionEmail = "^[\\w\\.-]+@([\\w\\-]+\\.)+[A-Z]{2,4}$";

    public static boolean checkPassword(EditText editText) {
        return checkSyntax(editText, expressionPassword);
    }

    public static boolean checkEmail(EditText editText) {
        return checkSyntax(editText, expressionEmail);
    }

    private static boolean checkSyntax(EditText editText, String expression) {
        Pattern pattern = Pattern.compile(expression, Pattern.CASE_INSENSITIVE);
        Matcher matcher = pattern.matcher(editText.getText().toString());

        return matcher.matches();
    }
}
