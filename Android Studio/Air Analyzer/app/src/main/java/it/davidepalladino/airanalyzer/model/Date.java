package it.davidepalladino.airanalyzer.model;

import android.os.Parcel;
import android.os.Parcelable;

public class Date implements Parcelable {
    public String day;
    public String month;
    public String year;

    public Date(String day, String month, String year) {
        this.day = day;
        this.month = month;
        this.year = year;
    }

    protected Date(Parcel in) {
        day = in.readString();
        month = in.readString();
        year = in.readString();
    }

    public static final Creator<Date> CREATOR = new Creator<Date>() {
        @Override
        public Date createFromParcel(Parcel in) {
            return new Date(in);
        }

        @Override
        public Date[] newArray(int size) {
            return new Date[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(day);
        dest.writeString(month);
        dest.writeString(year);
    }

    @Override
    public String toString() {
        return "Date{" +
                "day='" + day + '\'' +
                ", month='" + month + '\'' +
                ", year='" + year + '\'' +
                '}';
    }
}
