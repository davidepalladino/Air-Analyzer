package it.davidepalladino.airanalyzer.model;

import com.google.gson.annotations.SerializedName;

public class Measure {
    @SerializedName("DateAndTime")
    private String dateAndTime;

    @SerializedName("DayWeek")
    private String dayWeek;

    @SerializedName("Room")
    private int room;

    @SerializedName("Temperature")
    private float temperature;

    @SerializedName("Humidity")
    private float humidity;


    public Measure(String dateAndTime, String dayWeek, int room, float temperature, float humidity) {
        this.dateAndTime = dateAndTime;
        this.dayWeek = dayWeek;
        this.room = room;
        this.temperature = temperature;
        this.humidity = humidity;
    }

    public String getDateAndTime() {
        return dateAndTime;
    }

    public void setDateAndTime(String dateAndTime) {
        this.dateAndTime = dateAndTime;
    }

    public String getDayWeek() {
        return dayWeek;
    }

    public void setDayWeek(String dayWeek) {
        this.dayWeek = dayWeek;
    }

    public int getRoom() {
        return room;
    }

    public void setRoom(int room) {
        this.room = room;
    }

    public float getTemperature() {
        return temperature;
    }

    public void setTemperature(float temperature) {
        this.temperature = temperature;
    }

    public float getHumidity() {
        return humidity;
    }

    public void setHumidity(float humidity) {
        this.humidity = humidity;
    }

    @Override
    public String toString() {
        return "Measure{" +
                "dateAndTime='" + dateAndTime + '\'' +
                ", dayWeek='" + dayWeek + '\'' +
                ", room=" + room +
                ", temperature=" + temperature +
                ", humidity=" + humidity +
                '}';
    }

    public class MeasureDate {
        @SerializedName("Hour")
        private String hour;

        @SerializedName("Temperature")
        private float temperature;

        @SerializedName("Humidity")
        private float humidity;

        public MeasureDate(String hour, float temperature, float humidity) {
            this.hour = hour;
            this.temperature = temperature;
            this.humidity = humidity;
        }

        public String getHour() {
            return hour;
        }

        public void setHour(String hour) {
            this.hour = hour;
        }

        public float getTemperature() {
            return temperature;
        }

        public void setTemperature(float temperature) {
            this.temperature = temperature;
        }

        public float getHumidity() {
            return humidity;
        }

        public void setHumidity(float humidity) {
            this.humidity = humidity;
        }

        @Override
        public String toString() {
            return "MeasureDate{" +
                    "hour='" + hour + '\'' +
                    ", temperature=" + temperature +
                    ", humidity=" + humidity +
                    '}';
        }
    }
}