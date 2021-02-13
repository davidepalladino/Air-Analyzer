package it.davidepalladino.airanalyzer.control;
import java.util.List;

import it.davidepalladino.airanalyzer.model.Measure;
import it.davidepalladino.airanalyzer.model.api.Login;
import it.davidepalladino.airanalyzer.model.api.Signup;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.POST;
import retrofit2.http.Query;

public interface API {
    //String BASE_URL = "http://192.168.0.2:8000/api/";
    String BASE_URL = "http://airanalyzer.servehttp.com:50208/";

    @POST("api/login")
    Call<Login.Response> login(@Body Login login);

    @POST("api/signupAirAnalyzer")
    Call<Signup.NoResponse> signup(@Body Signup sigup);

    @GET("api/checkUsername")
    Call<Signup.NoResponse> checkUsername(@Query("username") String username);

    @GET("api/checkEmail")
    Call<Signup.NoResponse> checkEmail(@Query("email") String email);

    @GET("api/airanalyzer/getDate")
    Call<List<Measure.MeasureDate>> getDate(@Header("Authorization") String token, @Query("day") String day, @Query("month") String month, @Query("year") String year);
}