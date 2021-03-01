<?php

namespace App\Http\Controllers;

use DateTime;
use Illuminate\Http\Request;
use Illuminate\Support\Carbon;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\DB;

class AirAnalyzerController extends Controller {
    public function __construct() {
        $this->middleware('auth:api', ['except' => []]);
    }

    private function checkUser($user) {
        $checkDatabase = DB::table('uses')
            ->join('users', 'users.id', '=', 'uses.userId')
            ->join('services', 'services.id', '=', 'uses.serviceId')
            ->where('users.id', '=', $user->id)
            ->where('users.isActive', "=", 1)
            ->get();

        if ($checkDatabase->count() != 0) {
            return true;
        } else {
            return false;
        }
    }

    public function getActiveRooms(Request $request) {
        $user = Auth::user();

        if ($this->checkUser($user)) {
            $rooms = DB::table('airanalyzer_' . $user->id . '.Room')
                ->where('IsActive', 1)
                ->get();

            if ($rooms->count() != 0) {
                return response()->json($rooms);
            } else {
                return response()->json(null, 204);
            }
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function getInactiveRooms(Request $request) {
        $user = Auth::user();

        if ($this->checkUser($user)) {
            $rooms = DB::table('airanalyzer_' . $user->id . '.Room')
                ->where('IsActive', 0)
                ->get();

            if ($rooms->count() != 0) {
                return response()->json($rooms);
            } else {
                return response()->json(null, 204);
            }
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function renameRoom(Request $request) {
        $user = Auth::user();

        $this->validate($request, [
            'ID' => 'required|string',
            'Name' => 'required|string'
        ]);

        if ($this->checkUser($user)) {
            DB::table('airanalyzer_' . $user->id . '.Room')
                ->where('ID', $request->input('ID'))
                ->update(['Name' => $request->input('Name')]);
            return response()->json(null, 200);
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function addRoom(Request $request) {
        $user = Auth::user();

        $this->validate($request, [
            'ID' => 'required|string',
        ]);

        if ($this->checkUser($user)) {
            DB::table('airanalyzer_' . $user->id . '.Room')
                ->where('ID', $request->input('ID'))
                ->update(['IsActive' => 1]);
            return response()->json(null, 200);
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function removeRoom(Request $request) {
        $user = Auth::user();

        $this->validate($request, [
            'ID' => 'required|string',
        ]);

        if ($this->checkUser($user)) {
            DB::table('airanalyzer_' . $user->id . '.Room')
                ->where('ID', $request->input('ID'))
                ->update(['IsActive' => 0]);
            return response()->json(null, 200);
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function getMeasureDateLatest(Request $request) {
        $user = Auth::user();

        $this->validate($request, [
            'room' => 'required|string',
            'day' => 'required|string',
            'month' => 'required|string',
            'year' => 'required|string'
        ]);
        $datetime = DateTime::createFromFormat('Y-m-d',$request->input('year') . '-' . $request->input('month') . '-' . $request->input('day'));

        if ($this->checkUser($user)) {
            $measure = DB::table('airanalyzer_' . $user->id . '.Measure')
                ->whereDate('DateAndTime', date("Y-m-d", $datetime->getTimestamp()))
                ->where('Room', $request->input('room'))
                ->orderByDesc('DateAndTime')
                ->first();

            if ($measure != null) {
                return response()->json($measure);
            } else {
                return response()->json(null, 204);
            }
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }

    public function getMeasuresDateAverage(Request $request) {
        $user = Auth::user();

        $this->validate($request, [
            'room' => 'required|string',
            'day' => 'required|string',
            'month' => 'required|string',
            'year' => 'required|string'
        ]);
        $datetime = DateTime::createFromFormat('Y-m-d',$request->input('year') . '-' . $request->input('month') . '-' . $request->input('day'));

        if ($this->checkUser($user)) {
            $measures = DB::table('airanalyzer_' . $user->id . '.Measure')
                ->whereDate('DateAndTime', date("Y-m-d", $datetime->getTimestamp()))
                ->where('Room', $request->input('room'))
                ->select(DB::raw('HOUR(DateAndTime) as Hour, AVG(Humidity) as Humidity, AVG(Temperature) as Temperature'))
                ->groupBy('Hour')
                ->get();


            if ($measures->count() != null) {
                return response()->json($measures);
            } else {
                return response()->json(null, 204);
            }
        } else {
            return response()->json(['message' => 'Unauthorized'], 401);
        }
    }
}
