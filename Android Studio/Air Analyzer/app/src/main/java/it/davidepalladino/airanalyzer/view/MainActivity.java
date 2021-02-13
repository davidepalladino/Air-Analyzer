package it.davidepalladino.airanalyzer.view;

import androidx.appcompat.app.AppCompatActivity;

import android.content.SharedPreferences;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.DataPointInterface;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.OnDataPointTapListener;
import com.jjoe64.graphview.series.Series;

import java.util.List;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.API;
import it.davidepalladino.airanalyzer.model.Measure;
import it.davidepalladino.airanalyzer.model.api.Login;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {
    private static final String NAMEFILE_LOGIN = "login";
    private static final String TOKEN = "token";

    private GraphView graph;
    private ListView listViewMeasures;

    private API api;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SharedPreferences login = getSharedPreferences(NAMEFILE_LOGIN, MODE_PRIVATE);
        String token = login.getString(TOKEN, "");

        listViewMeasures = findViewById(R.id.listViewMeasures);
        graph = (GraphView) findViewById(R.id.graph);

        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(API.BASE_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        api = retrofit.create(API.class);
        getToday(token);
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

    private void getToday(String token) {
        Call<List<Measure.MeasureDate>> call = api.getDate("Bearer " + token, "13", "02", "2021");
        call.enqueue(new Callback<List<Measure.MeasureDate>>() {
            @Override
            public void onResponse(Call<List<Measure.MeasureDate>> call, Response<List<Measure.MeasureDate>> response) {
                List<Measure.MeasureDate> listMeasures = response.body();

                DataPoint[] datapoint = new DataPoint[listMeasures.size()];
                String[] stringMeasures = new String[listMeasures.size()];

                for (int m = 0; m < listMeasures.size(); m++) {
                    datapoint[m] = new DataPoint(Integer.parseInt(listMeasures.get(m).getHour()), listMeasures.get(m).getHumidity());
                    stringMeasures[m] = listMeasures.get(m).toString();
                }

                LineGraphSeries<DataPoint> series = new LineGraphSeries<DataPoint>(datapoint);
                series.setDrawDataPoints(true);
                series.setDataPointsRadius(10);

                series.setOnDataPointTapListener(new OnDataPointTapListener() {
                    @Override
                    public void onTap(Series series, DataPointInterface dataPoint) {
                        Toast.makeText(MainActivity.this, String.format("%.2f", dataPoint.getY()) + "%", Toast.LENGTH_LONG).show();
                    }
                });

                graph.getViewport().setXAxisBoundsManual(true);
                graph.getViewport().setMinX(0);
                graph.getViewport().setMaxX(24);

                graph.getViewport().setMinY(0);
                graph.getViewport().setMaxY(100);

                // enable scaling and scrolling
                graph.getViewport().setScalable(true);
                graph.getViewport().setScalableY(false);



                graph.addSeries(series);
                listViewMeasures.setAdapter(new ArrayAdapter<String>(getApplicationContext(), android.R.layout.simple_list_item_1, stringMeasures));
            }

            @Override
            public void onFailure(Call<List<Measure.MeasureDate>> call, Throwable t) {
                Toast.makeText(getApplicationContext(), t.getMessage(), Toast.LENGTH_LONG).show();
            }
        });
    }
}