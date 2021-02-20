package it.davidepalladino.airanalyzer.view.fragment;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.os.IBinder;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.jjoe64.graphview.DefaultLabelFormatter;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.BarGraphSeries;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.DataPointInterface;
import com.jjoe64.graphview.series.OnDataPointTapListener;
import com.jjoe64.graphview.series.Series;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;

import it.davidepalladino.airanalyzer.R;
import it.davidepalladino.airanalyzer.control.DatabaseService;
import it.davidepalladino.airanalyzer.control.Setting;
import it.davidepalladino.airanalyzer.model.MeasureAverage;
import it.davidepalladino.airanalyzer.model.MeasureFull;
import it.davidepalladino.airanalyzer.model.Room;
import it.davidepalladino.airanalyzer.view.activity.LoginActivity;

import static android.content.Context.BIND_AUTO_CREATE;
import static android.graphics.Typeface.ITALIC;
import static android.graphics.Typeface.NORMAL;
import static it.davidepalladino.airanalyzer.control.DatabaseService.REQUEST_CODE;
import static it.davidepalladino.airanalyzer.control.IntentConst.INTENT_MEASURE;
import static it.davidepalladino.airanalyzer.control.Setting.TOKEN;

public class RoomFragment extends Fragment {
    private static final String BROADCAST_REQUEST_CODE_MASTER = "RoomFragment";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_LAST = "GetDateLast";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_AVERAGE = "GetDateAverage";
    private static final String BROADCAST_REQUEST_CODE_EXTENSION_LOGIN = "Login";
    public static final String BUNDLE_ROOM = "ROOM";
    public static final String BUNDLE_DATE_RAW = "DATE_RAW";
    private static final int MAX_ATTEMPTS = 3;

    private GraphView graphTemperature;
    private GraphView graphHumidity;

    private TextView textViewDate;
    private TextView textViewLatestTemperature;
    private TextView textViewLatestHumidity;
    private TextView textViewLatestTime;

    private Setting setting;
    private Room room;
    private Calendar calendarSelected;

    private MeasureFull lastMeasureDate;
    private ArrayList<MeasureAverage> listMeasuresDateAverage;

    private int attempts = 1;

    public static RoomFragment newInstance(Room room, Long dateRaw) {
        RoomFragment fragment = new RoomFragment();

        Bundle args = new Bundle();
        args.putParcelable(BUNDLE_ROOM, room);
        args.putLong(BUNDLE_DATE_RAW, dateRaw);
        fragment.setArguments(args);

        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            room = getArguments().getParcelable(BUNDLE_ROOM);
            calendarSelected = Calendar.getInstance();
            calendarSelected.setTimeInMillis(getArguments().getLong(BUNDLE_DATE_RAW));
        }
    }

    @Override
    public void onStart() {
        super.onStart();
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
        View layoutFragment = inflater.inflate(R.layout.fragment_room, container, false);

        graphTemperature = (GraphView) layoutFragment.findViewById(R.id.graphTemperature);
        graphHumidity = (GraphView) layoutFragment.findViewById(R.id.graphHumidity);

        textViewDate = (TextView) layoutFragment.findViewById(R.id.textViewDate);
        textViewLatestTemperature = (TextView) layoutFragment.findViewById(R.id.textViewLatestTemperature);
        textViewLatestHumidity = (TextView) layoutFragment.findViewById(R.id.textViewLatestHumidity);
        textViewLatestTime = (TextView) layoutFragment.findViewById(R.id.textViewLatestTime);

        textViewDate.setText(getFormattedDate(calendarSelected));

        return layoutFragment;
    }

    private String getFormattedDate(Calendar calendarSelected) {
        SimpleDateFormat formatter = new SimpleDateFormat(getString(R.string.format_date));
        String date = formatter.format(calendarSelected.getTime());
        return date;
    }

    private void generateBarGraph(ArrayList<MeasureAverage> listMeasures) {
        if (listMeasures.isEmpty() && attempts <= MAX_ATTEMPTS) {
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    databaseService.getMeasuresDateAverage(setting.readToken(), room.getId(), calendarSelected, BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_AVERAGE + room.getId());
                    attempts++;
                }
            }, 1000);

            return;
        } else if (attempts == MAX_ATTEMPTS + 1) {
            Toast.makeText(getContext(), getString(R.string.toastNoMeasuresOnDate) + "'" + room.getName() + "'", Toast.LENGTH_LONG).show();
            return;
        }

        attempts = 1;

        /* Temeperature */
        DataPoint[] dataPointsTemperature = new DataPoint[listMeasures.size()];
        for (int m = 0; m < listMeasures.size(); m++) {
            dataPointsTemperature[m] = new DataPoint(Integer.parseInt(listMeasures.get(m).getHour()), listMeasures.get(m).getTemperature());
        }
        BarGraphSeries<DataPoint> seriesTemperature = new BarGraphSeries<DataPoint>(dataPointsTemperature);
        seriesTemperature.setOnDataPointTapListener(new OnDataPointTapListener() {
            @Override
            public void onTap(Series series, DataPointInterface dataPoint) {
                Toast.makeText(getActivity(),
                        getString(R.string.time) +
                        String.format("%.0f:00", dataPoint.getX()) +
                        "\n" +
                        getString(R.string.temperature) +
                        ": " +
                        String.format("%.2f", dataPoint.getY()) +
                        " °C", Toast.LENGTH_LONG).show();
            }
        });
        drawBarGraph(graphTemperature, seriesTemperature, " °C");

        /* Humidity */
        DataPoint[] dataPointsHumidity = new DataPoint[listMeasures.size()];
        for (int m = 0; m < listMeasures.size(); m++) {
            dataPointsHumidity[m] = new DataPoint(Integer.parseInt(listMeasures.get(m).getHour()), listMeasures.get(m).getHumidity());
        }
        BarGraphSeries<DataPoint> seriesHumidity = new BarGraphSeries<DataPoint>(dataPointsHumidity);
        seriesHumidity.setOnDataPointTapListener(new OnDataPointTapListener() {
            @Override
            public void onTap(Series series, DataPointInterface dataPoint) {
                Toast.makeText(getActivity(),
                        getString(R.string.time) +
                        String.format("%.0f:00", dataPoint.getX()) +
                        "\n" +
                        getString(R.string.humidity) +
                        ": " +
                        String.format("%.2f", dataPoint.getY()) +
                        "%", Toast.LENGTH_LONG).show();
            }
        });
        drawBarGraph(graphHumidity, seriesHumidity, " %");
    }

    private void drawBarGraph(GraphView graph, BarGraphSeries<DataPoint> series, String yValue) {
        series.setSpacing(25);

        graph.getViewport().setScalable(true);
        graph.getViewport().setScalableY(false);

        graph.getViewport().setScrollable(true);
        graph.getViewport().setScalableY(false);

        graph.getViewport().setXAxisBoundsManual(true);
        graph.getViewport().setMinX(0);
        graph.getViewport().setMaxX(24);

        graph.getViewport().setYAxisBoundsManual(false);
        graph.getViewport().setMinY(0);
        graph.getViewport().setMaxY(100);

        graph.addSeries(series);

        graph.getGridLabelRenderer().setLabelFormatter(new DefaultLabelFormatter() {
            @Override
            public String formatLabel(double value, boolean isValueX) {
                if (isValueX) {
                    // X value
                    return super.formatLabel(value, isValueX) + ":00";
                } else {
                    // Y value
                    return super.formatLabel(value, isValueX) + yValue;
                }
            }
        });
    }

    public DatabaseService databaseService;
    public ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            DatabaseService.LocalBinder localBinder = (DatabaseService.LocalBinder) service;
            databaseService = localBinder.getService();

            databaseService.getMeasureDateLatest(setting.readToken(), room.getId(), calendarSelected, BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_LAST + room.getId());
            databaseService.getMeasuresDateAverage(setting.readToken(), room.getId(), calendarSelected, BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_AVERAGE + room.getId());
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
                            // GET DATE LAST BROADCAST
                            if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_LAST + room.getId()) == 0) {
                                lastMeasureDate = intentFrom.getParcelableExtra(INTENT_MEASURE);

                                if (lastMeasureDate != null) {
                                    textViewLatestTemperature.setTypeface(null, NORMAL);
                                    textViewLatestHumidity.setTypeface(null, NORMAL);

                                    textViewLatestTemperature.setText(
                                            String.valueOf(lastMeasureDate.getTemperature()) +
                                            " °C"
                                    );

                                    textViewLatestHumidity.setText(
                                            String.valueOf(lastMeasureDate.getHumidity()) +
                                            " %"
                                    );

                                    textViewLatestTime.setText(getString(R.string.textViewLatestMeasurementAt) + lastMeasureDate.getDateAndTime().substring(11, 16));
                                } else {
                                    textViewLatestTemperature.setTypeface(null, ITALIC);
                                    textViewLatestHumidity.setTypeface(null, ITALIC);

                                    textViewLatestTemperature.setText(getString(R.string.textViewNone));
                                    textViewLatestHumidity.setText(getString(R.string.textViewNone));
                                }

                            // GET DATE AVERAGE BROADCAST
                            } else if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_GET_DATE_AVERAGE + room.getId()) == 0) {
                                listMeasuresDateAverage = intentFrom.getParcelableArrayListExtra(INTENT_MEASURE);

                                generateBarGraph(listMeasuresDateAverage);

                            // LOGIN BROADCAST
                            } else if (intentFrom.getStringExtra(REQUEST_CODE).compareTo(BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_LOGIN) == 0) {
                                setting.saveToken(intentFrom.getStringExtra(TOKEN));
                                attempts = 1;
                            }

                            break;
                        case 204:
                        case 401:
                            if (attempts <= MAX_ATTEMPTS) {
                                new Handler().postDelayed(new Runnable() {
                                    @Override
                                    public void run() {
                                        databaseService.login(setting.readLogin(), BROADCAST_REQUEST_CODE_MASTER + BROADCAST_REQUEST_CODE_EXTENSION_LOGIN);
                                        attempts++;
                                    }
                                }, 1000);
                            } else {
                                Intent intentTo = new Intent(getActivity(), LoginActivity.class);
                                startActivity(intentTo);
                                getActivity().finish();
                            }

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